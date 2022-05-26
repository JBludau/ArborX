add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(PointView pyArborX_Views.cpp)
set_target_properties(PointView PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/Views/PointView)
target_link_libraries(PointView PRIVATE ArborX::ArborX)
# TODO: consolidate cmake and preprocessor generation
PYBIND11_ADD_MODULE(IntersectView pyArborX_Views.cpp)
set_target_properties(IntersectView PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/Views/IntersectView)
target_link_libraries(IntersectView PRIVATE ArborX::ArborX)
