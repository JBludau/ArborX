add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(Point pyArborX_Point.cpp)
set_target_properties(Point PROPERTIES OUTPUT_NAME ${PROJECT_SOURCE_DIR}/pyArborX/Point)
target_link_libraries(Point PRIVATE ArborX::ArborX)
