
cmake_minimum_required (VERSION 3.28)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so" ".dll")
set(CMAKE_CXX_STANDARD 20)

project ("sudoku-resolve")

include("Util.cmake")

find_package(Tesseract CONFIG REQUIRED)

find_package(Leptonica CONFIG REQUIRED)


file(GLOB sodoku_files 
   "src/sudoku/common.cpp"
   "src/sudoku/text.cpp"
   "src/sudoku/alg.cpp"
   "src/sudoku/time.cpp"
  ) 

add_executable (sudoku_resolve "src/main_sudoku_cmd.cpp" ${sodoku_files})

add_executable (sudoku_test "src/main_sudoku_test.cpp" ${sodoku_files})
target_link_libraries(sudoku_test PRIVATE Tesseract::libtesseract leptonica)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(sudoku_resolve PRIVATE -static)
    target_link_options(sudoku_test PRIVATE -static)
    Add_dist(sudoku_resolve)
endif()

