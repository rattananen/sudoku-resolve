
cmake_minimum_required (VERSION 3.28)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ".so" ".dll")
set(CMAKE_CXX_STANDARD 20)

project ("sudoku-resolve")

function(Add_dist for)
    set(tname "$<TARGET_FILE_NAME:${for}>")
    set(dir "${CMAKE_SOURCE_DIR}/dist")
    string(TOLOWER "${for}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" dname)
    set(dest_dir "${dir}/${dname}")
    set(dest_path "${dest_dir}/${tname}")
    set(checksum "${dest_dir}/sha1sum.txt")

    add_custom_command(
        TARGET ${for} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${tname}" ${dest_path}
        COMMAND ${CMAKE_COMMAND} -E sha1sum "${tname}" > ${checksum}
     )
endfunction()

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

