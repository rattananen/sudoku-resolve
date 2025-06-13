
cmake_minimum_required (VERSION 3.28)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so" ".dll")
set(CMAKE_CXX_STANDARD 20)

project ("sudoku-resolve")

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

include("Util.cmake")

 if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
 endif()

file(GLOB sodoku_files 
   "src/sudoku/common.cpp"
   "src/sudoku/text.cpp"
   "src/sudoku/alg.cpp"
   "src/sudoku/time.cpp"
  ) 

add_executable (sudoku_resolve "src/main_sudoku_cmd.cpp" ${sodoku_files})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(sudoku_resolve PRIVATE -static)
    Add_dist(sudoku_resolve)
endif()


if(JKK_ENABLE_OCR)

find_package(Tesseract CONFIG REQUIRED)

set(OpenCV_STATIC ON)
find_package(OpenCV COMPONENTS png highgui)


add_executable (sudoku_ocr "src/main_sudoku_ocr.cpp" ${sodoku_files} "src/sudoku/ocr.cpp")
target_link_libraries(sudoku_ocr PRIVATE Tesseract::libtesseract ${OpenCV_LIBS})


add_executable (sudoku_test "src/main_sudoku_test.cpp" ${sodoku_files})
target_link_libraries(sudoku_test PRIVATE Tesseract::libtesseract ${OpenCV_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   target_link_options(sudoku_test PRIVATE -static)
   target_link_options(sudoku_ocr PRIVATE  -static)
endif()

endif()

