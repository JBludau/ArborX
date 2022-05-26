#ifndef PYARBORX_VIEW_CPP
#define PYARBORX_VIEW_CPP

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "pyArborX_Views_Util.hpp"


namespace py = pybind11;
	
#define CREATE_VIEW(moduleName, viewType, dataType)\
\
PYBIND11_MODULE(moduleName, m) { \
                                 \
    m.attr("__name__") = std::string("pyArborX.Views.")+std::string(#moduleName); \
    py::class_<viewType>(m,#moduleName) \
      .def(py::init<>()) \
      .def(py::init([](std::string label,size_t size){ \
            return new viewType{Kokkos::view_alloc( \
                                                     ExecutionSpace{}, \
                                                     Kokkos::WithoutInitializing, \
                                                     label \
                                                     ),size};})) \
 \
    .def("__getitem__", \
        [](viewType& p,long unsigned int idx){ return p(idx);} \
        ) \
\
    .def("__setitem__", \
        [](viewType& p,long unsigned int idx, dataType val){p(idx)=val;} \
        ); \
\
  m.def("create_mirror_view",&create_mirror_view<viewType>,py::arg("source")); \
  m.def("deep_copy",&deep_copy<viewType,viewType>,py::arg("dest"),py::arg("src")); \
        \
} 


CREATE_VIEW(PointView,Primitives,PrimitivesBaseType);
CREATE_VIEW(IntersectView,Predicates,PredicatesBaseType);

#endif

