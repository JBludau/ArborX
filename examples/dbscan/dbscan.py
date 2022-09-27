import kokkos
import pykokkos as pk
import pyArborX as aX

import struct
from enum import Enum


def loadData(filename: str, binary: bool = True, max_num_points: int = -1):
    mode_print: str = "binary" if binary else "text"
    print(f"Reading in {filename} in {mode_print} mode...")

    if binary:
        with open(filename, "rb") as f:
            contents = f.read()
            info = struct.unpack("i" * ((8) // 4), contents[:8])
            data = struct.unpack("f" * ((len(contents)- 8) // 4), contents[8:])
    else:
        with open(filename, "rb") as f:
            info: List[str] = f.readline().strip().split(" ")
            data = f.readlines()

    num_points: int = int(info[0])
    dim: int = int(info[1])

    assert(dim == 2 or dim == 3)

    if max_num_points > 0 and max_num_points < num_points:
        num_points = max_num_points

    v = pk.View([num_points, dim], pk.float)
    if not binary:
        for index, line in enumerate(data):
            if index >= num_points:
                break
            point_data: List[float] = [float(f) for f in line.strip().split(" ")]
            for d in range(dim):
                v[index][d] = point_data[d]
    else:
        for index, data in enumerate(data):
            point_id: int = index // dim
            if point_id >= num_points:
                break
            dimension: int = index % dim
            v[point_id][dimension] = data

    print("done")
    print(f"Read in {num_points} {dim}D points")

    return v

####################################################################
class Implementation(Enum):
    FDBSCAN = "FDBSCAN"
    FDBSCAN_DenseBox = "FDBSCAN_DenseBox"

class Parameters:
    def __init__(self):
        self._print_timers: bool = False
        self._implementation = Implementation.FDBSCAN_DenseBox

    def setPrintTimers(self, print_timers: bool):
        self._print_timers = print_timers
        return self

    def setImplementation(self, impl: Implementation):
        self._implementation = impl
        return self

@pk.workunit
def iota(i: int, v: pk.View1D[int], value: int):
    v[i] = value + i

@pk.workunit
def finalize_labels(i: int, cluster_sizes: pk.View1D[int], labels: pk.View1D[int]):
    vstat: int = labels[i]
    old: int = vstat
    next_label: int = labels[vstat]

    while vstat > next_label:
        vstat = next_label
        next_label = labels[vstat]

    if vstat != old:
        labels[i] = vstat

    pk.atomic_increment(cluster_sizes, [labels[i]])

@pk.workunit
def mark_noise(i: int, cluster_sizes: pk.View1D[int], labels: pk.View1D[int]):
    if cluster_sizes[labels[i]] == 1:
        labels[i] = -1

class PrimitivesWithRadius:
    def __init__(self, primitives, eps):
        self.primitives = primitives
        self.eps = eps


def dbscan(exec_space, primitives, eps: float, core_min_size: int, parameters = Parameters()):
    assert(eps > 0)
    assert(core_min_size >= 2)

    is_special_case: bool = (core_min_size == 2)
    n: int = primitives.extent(0)

    num_neigh = pk.View([0], dtype=int)
    labels = pk.View([n], dtype=int)

    pk.parallel_for(n, iota, v=labels, value=0)

    if parameters._implementation is Implementation.FDBSCAN:
        bvh = aX.BVH(exec_space, primitives.array)
        predicates = aX.PrimitivesWithRadius(primitives.array, eps)

        if is_special_case:
            core_points = aX.CCSCorePoints()
            callback = aX.FDBSCANCallback(labels.array, core_points)
            bvh.query(exec_space, predicates, callback)

    cluster_sizes = pk.View([n], dtype=int)
    print(type(cluster_sizes), cluster_sizes.dtype, cluster_sizes.space, cluster_sizes.layout)
    print(type(labels), labels.dtype, labels.space, labels.layout)
    pk.parallel_for(pk.RangePolicy(pk.Cuda,0,n), finalize_labels, cluster_sizes=cluster_sizes, labels=labels)
    pk.parallel_for(pk.RangePolicy(pk.Cuda,0,n), mark_noise, cluster_sizes=cluster_sizes, labels=labels)
    #  pk.parallel_for(pk.RangePolicy(pk.Cuda, 0, n), finalize_labels, cluster_sizes=cluster_sizes, labels=labels)
    #  pk.parallel_for(pk.RangePolicy(pk.Cuda, 0, n), mark_noise, cluster_sizes=cluster_sizes, labels=labels)

    return labels

#################################################################################
@pk.workunit
def exclusiveScanWorkunit(i: int, update: pk.Acc[int], final_pass: bool, _in: pk.View1D[int], _out: pk.View1D[int]):
    in_i: int = _in[i]
    if final_pass:
        _out[i] = update
    update += in_i


def exclusivePrefixSumTwoViews(src: pk.View1D[int], dst: pk.View1D[int]):
    n: int = src.extent(0)
    assert(n == dst.extent(0))

    pk.parallel_scan(pk.RangePolicy(pk.Cuda,0,n), exclusiveScanWorkunit, _in=src, _out=dst)


def exclusivePrefixSum(v: pk.View1D[int]) -> None:
    exclusivePrefixSumTwoViews(v, v)


@pk.workunit
def computeClusterSizes(i: int, labels: pk.View1D[int], cluster_sizes: pk.View1D[int]):
    if labels[i] < 0:
        return

    pk.atomic_fetch_add(cluster_sizes, [labels[i]], 1)

@pk.workunit
def computeClusterOffsetWithFilter(
    i: int, update: pk.Acc[int], final_pass: bool, cluster_sizes: pk.View1D[int],
    cluster_offset: pk.View1D[int], map_cluster_to_offset_position: pk.View1D[int],
    cluster_min_size: int
):
    is_cluster_too_small: bool = cluster_sizes[i] < cluster_min_size
    if not is_cluster_too_small:
        if final_pass:
            cluster_offset[update] = cluster_sizes[i]
            map_cluster_to_offset_position[i] = update
        update += 1
    else:
        if final_pass:
            map_cluster_to_offset_position[i] = -1

@pk.workunit
def computeClusterIndices(
    i: int, labels: pk.View1D[int], map_cluster_to_offset_position: pk.View1D[int],
    cluster_starts: pk.View1D[int], cluster_indices: pk.View1D[int]
):
    if labels[i] < 0:
        return

    offset_pos: int = map_cluster_to_offset_position[labels[i]]
    if offset_pos != -1:
        position: int = pk.atomic_fetch_add(cluster_starts, [offset_pos], 1)
        cluster_indices[position] = i

def sortAndFilterClusters(
    exec_space: pk.ExecutionSpace, labels: pk.View1D[int],
    cluster_indices: pk.View1D[int], cluster_offset: pk.View1D[int],
    cluster_min_size: int = 1
) -> None:
    assert(cluster_min_size >= 1)

    n: int = labels.extent(0)
    cluster_sizes: pk.View1D[int] = pk.View([n], int)
    pk.parallel_for(n, computeClusterSizes, labels=labels, cluster_sizes=cluster_sizes)

    map_cluster_to_offset_position: pk.View1D[int] = cluster_sizes
    cluster_offset.resize(0, n + 1)

    num_clusters: int = pk.parallel_scan(
        n, computeClusterOffsetWithFilter, cluster_sizes=cluster_sizes,
        cluster_offset=cluster_offset, map_cluster_to_offset_position=map_cluster_to_offset_position,
        cluster_min_size=cluster_min_size)

    cluster_offset.resize(0, num_clusters + 1)
    exclusivePrefixSum(cluster_offset)

    # instead of clone()
    cluster_starts = pk.View([cluster_offset.extent(0)], int)
    cluster_starts[:] = cluster_offset[:]

    cluster_indices.resize(0, cluster_offset[-1])
    pk.parallel_for(
        n, computeClusterIndices,
        labels=labels, map_cluster_to_offset_position=map_cluster_to_offset_position,
        cluster_starts=cluster_starts, cluster_indices=cluster_indices)
