#ifndef PYARBORX_POINT_UTIL_HPP
#define PYARBORX_POINT_UTIL_HPP

#include <string.h>
#include <ArborX_Point.hpp>

namespace pyArborXHelper {
std::string pyPrint (ArborX::Point const & p)
{
  return "<pyArborX::Point with " + std::to_string(p[0]) +" "+ std::to_string(p[1]) +" "+ std::to_string(p[2]) +">";
}
} // end of namespace pyArborXHelper
#endif
