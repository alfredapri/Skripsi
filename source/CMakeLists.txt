cmake_minimum_required(VERSION 3.0.0)
project(kiritool VERSION 0.1.0 LANGUAGES C)

find_package(CURL CONFIG REQUIRED)

add_executable(kiritool main.c)

target_include_directories(kiritool PUBLIC "C:/vcpkg/installed/x64-windows")
target_link_libraries(kiritool PUBLIC CURL::libcurl)