#ifndef PYARBORX_UTIL_CPP
#define PYARBORX_UTIL_CPP

#include "pyArborX_Util.hpp"

namespace py = pybind11;

Predicates generateWithinQueries_device(ExecutionSpace execution_space,
                                        Primitives query_pts_view,
                                        int n_queries, float radius)
{
  Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>
      within_queries("within_queries", n_queries);
  Kokkos::parallel_for(
      "Setup queries",
      Kokkos::RangePolicy<ExecutionSpace>(execution_space, 0, n_queries),
      KOKKOS_LAMBDA(int i) {
        within_queries(i) =
            ArborX::intersects(ArborX::Sphere{query_pts_view(i), radius});
      });

  return within_queries;
}

void generateUtilWrapper(py::module &m)
{

  m.def("generateWithinQueries_device", &generateWithinQueries_device,
        py::arg("ExecutionSpace"), py::arg("Primitives"), py::arg("size"),
        py::arg("radius"));
}

#endif
