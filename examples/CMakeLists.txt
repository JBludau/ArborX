if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.16)
  project(ArborXExamples CXX)
  find_package(ArborX 1.3 REQUIRED)
  enable_testing()
endif()

add_subdirectory(access_traits)

add_subdirectory(callback)

add_subdirectory(simple_intersection)

add_subdirectory(molecular_dynamics)

find_package(Boost COMPONENTS program_options)
if(Boost_FOUND)
  add_subdirectory(viz)
  add_subdirectory(dbscan)
  add_subdirectory(raytracing)
  add_subdirectory(brute_force)
endif()
