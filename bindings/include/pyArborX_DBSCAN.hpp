#ifndef PYARBORX_DBSCAN_HPP
#define PYARBORX_DBSCAN_HPP

#include <ArborX_DBSCAN.hpp>

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

template <typename ExecutionSpace, typename LabelsView,
          typename ClusterIndicesView, typename ClusterOffsetView>
void sortAndFilterClusters(ExecutionSpace const &exec_space,
                           LabelsView const &labels,
                           ClusterIndicesView &cluster_indices,
                           ClusterOffsetView &cluster_offset,
                           int cluster_min_size = 1);

void generateDBSCANWrapper(py::module &m);

#endif
