#ifndef PYARBORX_UTIL_CPP
#define PYARBORX_UTIL_CPP

#include <iostream>

#include "config.hpp"
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
// using ScopeGuard = Kokkos::ScopeGuard;

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


PYBIND11_MODULE(Util, m) {

    m.attr("__name__") = "pyArborX.Util";

    m.def("generateWithinQueries_device",&generateWithinQueries_device,py::arg("ExecutionSpace"),py::arg("Primitives"),py::arg("size"),py::arg("radius"));

    py::class_<Kokkos::ScopeGuard>(m, "ScopeGuard")
        .def(py::init(
            [](int argc, py::list &argv)
            {
              char *argv_c[argc];
              // for(unsigned i=0;i<argc;++i)
              // {
              // argv_c[i]= new char[py::string]
              // }
              return new Kokkos::ScopeGuard(argc, argv_c);
            }))
        .def(py::init(
            []()
            {
              Kokkos::InitArguments init_args;
              init_args.num_threads = 10;
              init_args.num_numa = 1;
              init_args.device_id = 0;
              init_args.ndevices = 1;
              std::cout << "ScopeGuard creation ...";
              // auto sg = make_shared<Kokkos::ScopeGuard>(init_args);
              auto sg = new Kokkos::ScopeGuard(init_args);
              std::cout << "finished" << std::endl;
              return sg;
            }));
    m.def("initialize", []() { Kokkos::initialize(); });
    m.def("printConfig", []() { Kokkos::print_configuration(std::cout); });
    m.def("finalize",&Kokkos::finalize);

}


#endif

