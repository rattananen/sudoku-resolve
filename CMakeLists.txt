
cmake_minimum_required (VERSION 3.28)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so" ".dll")
set(CMAKE_CXX_STANDARD 20)

project ("sudoku-resolve")


file(GLOB sodoku_files 
   "src/sudoku/common.cpp"
   "src/sudoku/text.cpp"
   "src/sudoku/alg.cpp"
  ) 

add_executable (sudoku-resolve "src/main_sudoku_cmd.cpp" ${sodoku_files})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(sudoku-resolve PRIVATE -static)
endif()