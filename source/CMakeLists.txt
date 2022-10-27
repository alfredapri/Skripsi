cmake_minimum_required(VERSION 3.0.0)
project(KiriTool VERSION 0.1.0 LANGUAGES C)

find_package(CURL CONFIG REQUIRED)

add_executable(KiriTool main.c)

target_include_directories(KiriTool PUBLIC "C:/vcpkg/installed/x64-windows")
target_link_libraries(KiriTool PUBLIC CURL::libcurl)