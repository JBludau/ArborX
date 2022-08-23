#ifndef PYARBORX_UTIL_HPP
#define PYARBORX_UTIL_HPP

#include "config.hpp"
#include <ArborX_Version.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
Predicates generateWithinQueries_device(ExecutionSpace execution_space,
                                        Primitives query_pts_view,
                                        int n_queries, float radius);

PointViewType vec2view (std::vector<ArborX::Point> const & in, std::string const & label = "");

std::vector<ArborX::Point> loadData(std::string const &filename,
                                    bool binary = true, int max_num_points = -1);

void generateUtilWrapper(py::module &m);

#endif
