import pyArborX as aX

import time
import os
import signal


def run():
 
    ## setup to run the smaller hacc problem
    algorithm = "dbscan" # dbscan or MinimumSpanningTree
    filename = "hacc_37M.arborx"
    binary = True
    max_num_points = -1
    eps = 0.042 
    cluster_min_size = 1 
    core_min_size = 2
    verify = False # currently not supported
    labels = "" # currently not supported
    print_dbscan_timers = True
    impl = "fdbscan"


    print(f"algorithm           : {algorithm}")
    if algorithm == "dbscan":
        print(f"eps                 : {eps}")
        print(f"cluster min size    : {cluster_min_size}")
        print(f"implementation      : {impl}")
        print(f"verify              : {verify}")
    print(f"minpts              : {core_min_size}")
    print(f"filename            : {filename} {binary} {max_num_points}")
    if not labels == "":
        print(f"filename [labels]   : {labels} binary")
    print(f"print timers        : {print_dbscan_timers}")

    if impl == "fdbscan":
        implementation = aX.DBSCAN_Implementation(0)

    elif impl == "fdbscan_densebox":
        implementation = aX.DBSCAN_Implementation(1)


    startReadIn = time.time()
    data = aX.Util_loadData(filename,binary,max_num_points)
    primitives = aX.Util_vec2view(data,"primitives")
    endReadIn=time.time()
    print(f"readin time {endReadIn-startReadIn}")

    execSpace = aX.ExecutionSpace()
    if algorithm == "dbscan":
        parameters = aX.DBSCAN_Parameters()
        parameters.setPrintTimers(print_dbscan_timers)
        parameters.setImplementation(implementation)
        startdbscan=time.time()
        labels = aX.dbscan(execSpace,primitives,eps,core_min_size,parameters)
        

        startCluster=time.time()
        cluster_indices = aX.intView1D("Testing::cluster_indices", 0)
        cluster_offset = aX.intView1D("Testing::cluster_offset", 0)

        aX.sortAndFilterClusters(execSpace, labels, cluster_indices, cluster_offset, cluster_min_size)
        endCluster=time.time()
        enddbscan=time.time()

        num_clusters = cluster_offset.size() - 1
        num_cluster_points = cluster_indices.size()

        print(f"-- postprocess      : {endCluster-startCluster}")
        print(f"total time          : {enddbscan-startdbscan}")

        n = primitives.size()
        print(f"#clusters           : {num_clusters}")
        print(f"#cluster points     : {num_cluster_points} [{100*num_cluster_points / n}]")
        print(f"#noise points       : {n-num_cluster_points} [{100*(n-num_cluster_points)/n}]")
    elif (algorithm == "mst"):

        startmst=time.time()
        mst = aX.Details_MinimumSpanningTree(execSpace,primitives,core_min_size)
        endmst=time.time()

        print(f"total time {endmst-startmst}")
    else:
        print("ERROR: algorithm used is not known")

    #deleting all arborx items (this has to be done, as the python garbage collector is currently not respecting the kokkos finalize
    #this is especially ugly as you can not call delete on something still inside a list, so it has to be popped, then assigned, then deleted, as del does not deal with temporaries


if __name__ == '__main__':
    aX.initialize()
    aX.printConfig()
    run()
    aX.finalize()
