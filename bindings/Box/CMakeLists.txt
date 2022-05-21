add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(Box pyArborX_Box.cpp)
set_target_properties(Box PROPERTIES OUTPUT_NAME ${PROJECT_SOURCE_DIR}/pyArborX/Box)
target_link_libraries(Box PRIVATE ArborX::ArborX)
