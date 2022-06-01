add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(ExecutionSpace pyArborX_ExecutionSpace.cpp)
set_target_properties(ExecutionSpace PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/ExecutionSpace)
target_link_libraries(ExecutionSpace PRIVATE ArborX::ArborX)
