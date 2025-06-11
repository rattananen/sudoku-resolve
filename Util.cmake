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

function(Add_copy_asset for)
    set(resource_dir "${CMAKE_SOURCE_DIR}/assets/")
    set(resource_dest_dir "${CMAKE_CURRENT_BINARY_DIR}/") 
    add_custom_command(
         TARGET ${for} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource_dir} ${resource_dest_dir} 
         COMMENT "copy_directory resources folder from ${resource_dir} => ${resource_dest_dir}"
     )
endfunction()