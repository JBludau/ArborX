add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(BVH pyArborX_BVH.cpp)
set_target_properties(BVH PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/BVH)
target_link_libraries(BVH PRIVATE ArborX::ArborX)
