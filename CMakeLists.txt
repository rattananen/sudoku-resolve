﻿cmake_minimum_required (VERSION 3.28)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so" ".dll")
set(CMAKE_CXX_STANDARD 20)

project ("sudoku-resolve")

include("Util.cmake")

 if(CMAKE_BUILD_TYPE STREQUAL "Release")
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
  
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
   #set(BUILD_SHARED_LIBS OFF)
 endif()

file(GLOB sodoku_files 
   "src/sudoku/common.cpp"
   "src/sudoku/text.cpp"
   "src/sudoku/alg.cpp"
   "src/sudoku/time.cpp"
  ) 

add_executable (sudoku_resolve "src/main_sudoku_cmd.cpp" ${sodoku_files})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    Add_dist(sudoku_resolve)
endif()


if(JKK_ENABLE_OCR)
    #set(VCPKG_TARGET_TRIPLET "x86-windows-static")
    find_package(Tesseract CONFIG REQUIRED)
    find_package(OpenCV CONFIG REQUIRED)

    add_executable (sudoku_ocr "src/main_sudoku_ocr.cpp" ${sodoku_files} "src/sudoku/ocr.cpp")
    target_link_libraries(sudoku_ocr PUBLIC Tesseract::libtesseract ${OpenCV_LIBS})

    add_executable (sudoku_test "src/main_sudoku_test.cpp" ${sodoku_files})
    target_link_libraries(sudoku_test PRIVATE Tesseract::libtesseract ${OpenCV_LIBS})

endif()

