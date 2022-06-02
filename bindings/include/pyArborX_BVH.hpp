#ifndef PYARBORX_BVH_HPP
#define PYARBORX_BVH_HPP

#include <ArborX_LinearBVH.hpp>

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void generateBVHWrapper(py::module &m);

#endif
