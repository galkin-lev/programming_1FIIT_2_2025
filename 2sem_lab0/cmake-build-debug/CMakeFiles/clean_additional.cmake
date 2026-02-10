# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/lab0_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/lab0_autogen.dir/ParseCache.txt"
  "lab0_autogen"
  )
endif()
