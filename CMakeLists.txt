cmake_minimum_required(VERSION 3.2)
project(ALPIDEDecoder)

# by default build optimized code with debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
  # set(CMAKE_BUILD_TYPE ASAN)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(ROOT)

message(STATUS "In Project Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "ROOT Include Directory: ${ROOT_INCLUDE_DIRS}")

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR} )
file(COPY ${PROJECT_SOURCE_DIR}/rootlogon.C DESTINATION ${CMAKE_BINARY_DIR} )

message(STATUS "found inc ${headers}")
message(STATUS "found src ${sources}")

add_library(AD SHARED ${sources})

include_directories(include)

target_link_libraries( AD PUBLIC ROOT::Tree )

add_subdirectory(test)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Data)