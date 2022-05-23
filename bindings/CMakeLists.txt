project(pyArborX LANGUAGES CXX)

add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

# toplevel dir for python library

include_directories(".")

set(PY_MODULES Point Box Experimental BVH)

foreach(PY_MODULE IN LISTS PY_MODULES)
  add_subdirectory(./${PY_MODULE})
  list(APPEND PY_IMPORT "from .${PY_MODULE} import * \n")
endforeach()

          
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/pyArborX)
file(WRITE ${PROJECT_BINARY_DIR}/pyArborX/__init__.py ${PY_IMPORT})

# PYBIND11_ADD_MODULE(pyArborX pyArborX.cpp)
# set_target_properties(pyArborX PROPERTIES OUTPUT_NAME pyArborX)
# target_link_libraries(pyArborX PRIVATE ArborX::ArborX)
