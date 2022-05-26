#ifndef PYARBORX_POINTVIEW_CPP
#define PYARBORX_POINTVIEW_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "pyArborX_PointView_Util.hpp"


namespace py = pybind11;


PYBIND11_MODULE(PointView, m) {

    m.attr("__name__") = "pyArborX.PointView";
    py::class_<PointView>(m,"PointView")
      .def(py::init<>())
      .def(py::init([](std::string label,size_t size){return new PointView{Kokkos::view_alloc(
                                                                           ExecutionSpace{},
                                                                           Kokkos::WithoutInitializing,
                                                                           label
                                                                           ),size};}))

      .def("__getitem__",
          [](PointView& p,long unsigned int idx){ return p(idx);}
          )

      .def("__setitem__",
          [](PointView& p,long unsigned int idx, ArborX::Point val){p(idx)=val;}
          );

    m.def("create_mirror_view",&create_mirror_view<PointView>,py::arg("source"));
    m.def("deep_copy",&deep_copy<PointView,PointView>,py::arg("dest"),py::arg("src"));
    // m.def("allocWithoutInitialize",&allocWithoutInitialize<PointView>,py::arg("name"),py::arg("size"),py::return_value_policy::move);
          
}


#endif

