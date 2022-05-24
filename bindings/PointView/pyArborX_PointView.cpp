#ifndef PYARBORX_POINTVIEW_CPP
#define PYARBORX_POINTVIEW_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "config.hpp"

using PointView = Kokkos::View<ArborX::Point *,MemorySpace>;

namespace py = pybind11;

template<typename ViewType>
ViewType allocWithoutInitialize (std::string name ,unsigned long int size)
{
  return ViewType(Kokkos::view_alloc(
                                    ExecutionSpace{},
                                    Kokkos::WithoutInitializing,
                                    name)
                 ,size);
}

PYBIND11_MODULE(PointView, m) {

    m.attr("__name__") = "pyArborX.PointView";
    py::class_<PointView>(m,"PointView")
      .def(py::init<>())

      .def("__getitem__",
          [](PointView& p,long unsigned int idx){ return p(idx);}
          )

      .def("__setitem__",
          [](PointView& p,long unsigned int idx, ArborX::Point val){p(idx)=val;}
          );

    m.def("allocWithoutInitialize",&allocWithoutInitialize<PointView>,py::arg("name"),py::arg("size"));
          
}


#endif

//TODO: think again about if the interface should copy the values ... this would be a deviation of the interface of the original library
