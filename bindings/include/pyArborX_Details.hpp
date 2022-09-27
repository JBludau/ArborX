#ifndef PYARBORX_DETAILS_HPP
#define PYARBORX_DETAILS_HPP

#include <ArborX_DBSCAN.hpp>
#include <ArborX_DetailsFDBSCAN.hpp>
#include <ArborX_MinimumSpanningTree.hpp>

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

using CorePoints = ArborX::Details::CCSCorePoints;
using FDBSCANCallback =
    ArborX::Details::FDBSCANCallback<MemorySpace, CorePoints>;
using PrimitivesWithRadius = ArborX::Details::PrimitivesWithRadius<Primitives>;

void generateDetailsWrapper(py::module &m);

#endif
