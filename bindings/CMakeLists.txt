project(pyArborX LANGUAGES CXX)

find_package(ArborX REQUIRED)
find_package(pybind11 REQUIRED)

#choose modules that should be included in the build
set(PY_MODULES Point Box ExecutionSpace Experimental BVH Util)
set(PY_VIEWS PointView IntersectView intView1D)

#generate files for Viewtypes
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/generated_files)
foreach(PY_VIEW IN LISTS PY_VIEWS)
  set(VIEW_NAME ${PY_VIEW})
  set(VIEW_TYPE ${PY_VIEW}Type)
  set(VIEW_BASE_TYPE ${PY_VIEW}BaseType)
  set(VIEW_INCLUDE "\n#include \"pyArborX_${PY_VIEW}.hpp\"")

  configure_file(include/pyArborX_Views.hpp.in generated_files/pyArborX_${PY_VIEW}.hpp)
  configure_file(src/pyArborX_Views.cpp.in generated_files/pyArborX_${PY_VIEW}.cpp)
endforeach()

# get source files for selected modules
foreach(PY_MODULE IN LISTS PY_MODULES)
  list(APPEND PY_SRC ${PROJECT_SOURCE_DIR}/src/pyArborX_${PY_MODULE}.cpp)
  list(APPEND MODULE_INCLUDES "\n#include \"pyArborX_${PY_MODULE}.hpp\"" )
  list(APPEND MODULE_GENERATORS "\n generate${PY_MODULE}Wrapper(m)")
endforeach()

# get source files for generated modules (views)
foreach(PY_VIEW IN LISTS PY_VIEWS)
  list(APPEND PY_SRC ${PROJECT_BINARY_DIR}/generated_files/pyArborX_${PY_VIEW}.cpp)
  list(APPEND MODULE_INCLUDES "\n#include \"pyArborX_${PY_VIEW}.hpp\"" )
  list(APPEND MODULE_GENERATORS "\n generate${PY_VIEW}Wrapper(m)")
endforeach()

string(REPLACE ";" " " MODULE_INCLUDES "${MODULE_INCLUDES}")

configure_file(pyArborX.cpp.in generated_files/pyArborX.cpp)

#copy setup.py for development install to binary dir
file(COPY ${PROJECT_SOURCE_DIR}/setup.py DESTINATION ${PROJECT_BINARY_DIR}/setup.py)

PYBIND11_ADD_MODULE(pyArborX generated_files/pyArborX.cpp ${PY_SRC})
target_include_directories(pyArborX PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(pyArborX PUBLIC ${PROJECT_BINARY_DIR}/generated_files)
target_link_libraries(pyArborX PUBLIC ArborX::ArborX)
