if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    if(ENIGMA_BUILD_32_BIT)
        set(APP_INFO_FOLDER x86)
    else()
        set(APP_INFO_FOLDER x64)
    endif()

    configure_file(
        "Windows/${APP_INFO_FOLDER}/app_info.in.xml"
        "app_info.xml"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    configure_file(
        "Linux/app_info.in.xml"
        "app_info.xml"
    )
endif()

configure_file(
    "${CMAKE_SYSTEM_NAME}/job_optima_AV.xml"
    "job_optima_AV.xml"
    COPYONLY)
configure_file(
    "${CMAKE_SYSTEM_NAME}/job_optima_Krah.xml"
    "job_optima_Krah.xml"
    COPYONLY)
