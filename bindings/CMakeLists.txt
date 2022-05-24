project(pyArborX LANGUAGES CXX)

add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

# set the template options in config.hpp to the desired configuration
include_directories(".")

# definition of submodules included in build
set(PY_MODULES Point Box Experimental BVH PointView)

#generator for top level module file to be loaded in python
foreach(PY_MODULE IN LISTS PY_MODULES)
  add_subdirectory(./${PY_MODULE})
  list(APPEND PY_IMPORT "from .${PY_MODULE} import * \n")
endforeach()
         
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/pyArborX)
file(WRITE ${PROJECT_BINARY_DIR}/pyArborX/__init__.py ${PY_IMPORT})
