if(ENIGMA_BUILD_32_BIT)
    set(APP_INFO_FOLDER x86)
else()
    set(APP_INFO_FOLDER x64)
endif()

configure_file (
    "${APP_INFO_FOLDER}/app_info.in.xml"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/app_info.xml"
)