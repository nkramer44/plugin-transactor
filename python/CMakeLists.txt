cmake_minimum_required(VERSION 3.22)
project(plugin_transactor)
set(CMAKE_CXX_STANDARD 20)

find_package(SOCI REQUIRED)
find_package(pybind11 REQUIRED)


add_subdirectory(../external/rippled subproject/rippled EXCLUDE_FROM_ALL)
target_link_libraries(xrpl_core PUBLIC soci::soci)

add_library(lib)
target_sources(lib PRIVATE 
  src/lib.cpp
)
target_link_libraries(lib PUBLIC
  xrpl_core
)

target_include_directories(lib PRIVATE)

pybind11_add_module(plugin_transactor src/main.cpp)
target_compile_definitions(plugin_transactor
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_link_libraries(plugin_transactor PUBLIC lib)
