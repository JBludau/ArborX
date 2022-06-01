add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

foreach(PY_VIEW IN LISTS PY_VIEWS)
  set(VIEW_NAME ${PY_VIEW})
  set(VIEW_TYPE ${PY_VIEW}Type)
  set(VIEW_BASE_TYPE ${PY_VIEW}BaseType)

  configure_file(pyArborX_Views.cpp.in pyArborX_${PY_VIEW}.cpp)

  PYBIND11_ADD_MODULE(${PY_VIEW} pyArborX_${PY_VIEW}.cpp)
  set_target_properties(${PY_VIEW} PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/Views/${PY_VIEW})
  # target_include_directories(${PY_VIEW} PRIVATE ${PROJECT_SOURCE_DIR}/Views)
  target_link_libraries(${PY_VIEW} PRIVATE ArborX::ArborX)
endforeach()
