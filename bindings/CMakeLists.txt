add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(pyArborX pyArborX.cpp)
target_link_libraries(pyArborX PRIVATE ArborX::ArborX)
