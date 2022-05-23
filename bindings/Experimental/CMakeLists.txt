add_subdirectory(/pybind11 /root/lib/pybind11)
find_package(ArborX REQUIRED)

PYBIND11_ADD_MODULE(Experimental pyArborX_Experimental.cpp)
set_target_properties(Experimental PROPERTIES OUTPUT_NAME ${PROJECT_BINARY_DIR}/pyArborX/Experimental)
target_link_libraries(Experimental PRIVATE ArborX::ArborX)
