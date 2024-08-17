

function (add_elevated_manifest TARGET)
    if (NOT WIN32)
        return()
    endif()
    
    # Path to output the manifest file relative to the build directory
    set(MANIFEST_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.exe.manifest")

    # Embed the manifest into the executable
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/elevated.manifest ${MANIFEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR} mt.exe -manifest ${MANIFEST_FILE} -outputresource:$<TARGET_FILE:${TARGET}>
    )
endfunction()
