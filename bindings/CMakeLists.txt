project(pyArborX)

add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

include_directories(".")

add_subdirectory(./Point)
add_subdirectory(./Box)
add_subdirectory(./BVH)

# PYBIND11_ADD_MODULE(pyArborX pyArborX.cpp)
# set_target_properties(pyArborX PROPERTIES OUTPUT_NAME pyArborX)
# target_link_libraries(pyArborX PRIVATE ArborX::ArborX)
