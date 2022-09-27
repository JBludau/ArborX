#ifndef PYARBORX_BVH_HPP
#define PYARBORX_BVH_HPP

#include "pyArborX_Details.hpp"
#include <ArborX_LinearBVH.hpp>

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

using TraversalPolicy = ArborX::Experimental::TraversalPolicy;

void generateBVHWrapper(py::module &m);

#endif
