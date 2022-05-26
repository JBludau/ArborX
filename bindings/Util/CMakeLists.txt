add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(Util pyArborX_Util.cpp)
set_target_properties(Util PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/Util)
target_link_libraries(Util PRIVATE ArborX::ArborX)
