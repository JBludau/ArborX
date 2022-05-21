#ifndef PYARBORX_BOX_UTIL_HPP
#define PYARBORX_BOX_UTIL_HPP

#include <string.h>
#include <ArborX_Box.hpp>
#include "Point/pyArborX_Point_Util.hpp"

namespace pyArborXHelper {
std::string pyPrint (ArborX::Box const & b)
{
  return "<pyArborX::Box with min Corner \n" 
            + pyPrint(b.minCorner()) +
            "and max Corner \n"
            + pyPrint(b.maxCorner()) +
            ">";
}
} // end of namespace pyArborXHelper
#endif
