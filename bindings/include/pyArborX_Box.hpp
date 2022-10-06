#ifndef PYARBORX_BOX_HPP
#define PYARBORX_BOX_HPP

#include <ArborX_Box.hpp>

#include <pybind11/pybind11.h>
#include <string.h>

namespace py = pybind11;

namespace pyArborXHelper
{
std::string pyPrintBox(ArborX::Box const &b);
}

struct Boxholder{
  ArborX::Box box;
};


void generateBoxWrapper(py::module &m);
void generateBoxHolderWrapper(py::module &m);
void generateBoxHolderFuncs(py::module &m);

#endif
