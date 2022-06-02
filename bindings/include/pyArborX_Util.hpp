#ifndef PYARBORX_UTIL_HPP
#define PYARBORX_UTIL_HPP

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;
Predicates generateWithinQueries_device(ExecutionSpace execution_space,
                                        Primitives query_pts_view,
                                        int n_queries, float radius);

void generateUtilWrapper(py::module &m);

#endif
