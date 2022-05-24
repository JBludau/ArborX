add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(PointView pyArborX_PointView.cpp)
set_target_properties(PointView PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/PointView)
target_link_libraries(PointView PRIVATE ArborX::ArborX)
