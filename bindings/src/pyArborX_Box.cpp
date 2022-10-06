#ifndef PYARBORX_BOX_CPP
#define PYARBORX_BOX_CPP


#include <iostream>
#include "pyArborX_Box.hpp"

#include "pyArborX_Point.hpp"

namespace py = pybind11;

namespace pyArborXHelper
{
std::string pyPrintBox(ArborX::Box const &b)
{
  return "<pyArborX::Box with min Corner \n" + pyPrintPoint(b.minCorner()) +
         "and max Corner \n" + pyPrintPoint(b.maxCorner()) + ">";
}
} // end of namespace pyArborXHelper

void generateBoxWrapper(py::module &m)
{

  py::class_<ArborX::Box>(m, "Box")
      .def(py::init<>())
      .def(py::init<ArborX::Point, ArborX::Point>(), py::arg("min_corner"),
           py::arg("max_corner"))

      .def("minCorner",
           py::overload_cast<>(&ArborX::Box::minCorner, py::const_),
           py::return_value_policy::reference)
      .def("minCorner", py::overload_cast<>(&ArborX::Box::minCorner),
           py::return_value_policy::reference)
      .def("maxCorner",
           py::overload_cast<>(&ArborX::Box::maxCorner, py::const_),
           py::return_value_policy::reference)
      .def("maxCorner", py::overload_cast<>(&ArborX::Box::maxCorner),
           py::return_value_policy::reference)

      .def("__iadd__",
           py::overload_cast<ArborX::Box const &>(&ArborX::Box::operator+=),
           py::arg("ArborX::Box"), py::return_value_policy::reference)

      .def("__repr__",
           py::overload_cast<ArborX::Box const &>(&pyArborXHelper::pyPrintBox));
}

void generateBoxHolderWrapper(py::module &m)
{
  py::class_<Boxholder>(m,"Boxholder_bound")
    .def(py::init<>());
}

namespace pybind11 { namespace detail {
    template <> struct type_caster<Boxholder> {
    public:
        PYBIND11_TYPE_CASTER(Boxholder, const_name("Boxholder"));

        /**
         * Conversion part 1 (Python->C++): convert a PyObject into a inty
         * instance or return false upon failure. The second argument
         * indicates whether implicit conversions should be applied.
         */
        bool load(handle src, bool) {
            /* Extract PyObject from handle */
            PyObject *source = src.ptr();
            /* Try converting into a Python integer value */
            auto thing = getattr(source,"box");
            /* Now try to convert into a C++ int */
            value.box = thing.cast<ArborX::Box>();
            // Py_DECREF(tmp);
            /* Ensure return code was OK (to avoid out-of-range errors etc) */
            return !(PyErr_Occurred());
        }

        /**
         * Conversion part 2 (C++ -> Python): convert an inty instance into
         * a Python object. The second and third arguments are used to
         * indicate the return value policy and parent object (for
         * ``return_value_policy::reference_internal``) and are generally
         * ignored by implicit casters.
         */
        static handle cast(Boxholder src, return_value_policy /* policy */, handle /* parent */) {
          std::cout << "inside cast" << std::endl;
            py::object tmp = py::cast(src);
            return tmp;
        }
    };
}} // namespace pybind11::detail

ArborX::Box boxing (ArborX::Box box)
{
  std::cout << "got box " << pyArborXHelper::pyPrintBox(box) << std::endl;
  box = ArborX::Box(ArborX::Point(3,2,1),ArborX::Point(3,2,1));
  std::cout << "returning box " << pyArborXHelper::pyPrintBox(box) << std::endl; 
  return box;
}

Boxholder boxing_bh (Boxholder bh)
{
  std::cout << "got box " << pyArborXHelper::pyPrintBox(bh.box) << std::endl;
  bh.box = ArborX::Box(ArborX::Point(3,2,1),ArborX::Point(3,2,1));
  std::cout << "returning box " << pyArborXHelper::pyPrintBox(bh.box) << std::endl; 
  return bh;
}

py::object boxing_proxy (py::object obj)
{
  ArborX::Box box = getattr(obj,"box").cast<ArborX::Box>();
  box = boxing(box);
  setattr(obj,"box",py::cast(box));
  return obj;
}

py::object boxing_proxy_boxholder (py::object obj)
{
  Boxholder bh = obj.cast<Boxholder>();
  std::cout << "pre func call" << std::endl;
  bh = boxing_bh(bh);
  std::cout << "after assign" << std::endl;
  obj=bh;
  std::cout << "after cast" << std::endl;;
  return obj;
}

void generateBoxHolderFuncs (py::module &m)
{
  m.def("boxing",&boxing);
  m.def("boxing_proxy",&boxing_proxy);
  m.def("boxing_proxy_boxholder",&boxing_proxy_boxholder);
}

#endif
