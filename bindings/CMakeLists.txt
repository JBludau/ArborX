add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(pyArborX_st-jude ArborX_st-jude.cpp)
target_link_libraries(pyArborX_st-jude PRIVATE ArborX::ArborX)
