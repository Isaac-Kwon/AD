# in exe directory

set(TEST_EXE
  test1_readfile
  test2_thrsana
  test3_decodeone
)

# for now put the executables in the build root
# for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(exe ${TEST_EXE})
  add_executable(${exe} ${exe}.cpp)
  target_link_libraries(${exe} PUBLIC AD ROOT::Core ROOT::Hist ROOT::Gpad)
endforeach()

message(STATUS "Target Executable: ${TEST_EXE}")