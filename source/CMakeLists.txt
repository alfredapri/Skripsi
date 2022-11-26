set(PROJECT_NAME kiritool)
set(SOURCES main.c includes/cJSON/cJSON.c includes/cJSON/cJSON.h)

cmake_minimum_required(VERSION 3.15.0)
project(${PROJECT_NAME} VERSION 1.2.6 LANGUAGES C)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(CURL REQUIRED)

# libcurl
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)
# cJSON
target_include_directories(${PROJECT_NAME} PRIVATE includes)

# UNIX specific commands
if (UNIX)
    include(GNUInstallDirs)

    file(ARCHIVE_CREATE OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/build/kiritool.1.gz
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/additionals/linux/kiritool.1
        FORMAT gnutar
        COMPRESSION GZip
    )
    set(MANFILES build/kiritool.1.gz)

    if (DEFINED CMAKE_INSTALL_MANDIR)
        install(FILES ${MANFILES} DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
    else()
        message(FATAL_ERROR "CMAKE_INSTALL_MANDIR is not defined. \n  ")
    endif()
endif()