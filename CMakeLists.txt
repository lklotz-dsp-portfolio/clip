cmake_minimum_required(VERSION 3.10)
project(dsp_a1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

add_executable(callback callback.c)

target_include_directories(callback PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
target_link_directories(callback PRIVATE ${PORTAUDIO_LIBRARY_DIRS})
target_link_libraries(callback PRIVATE ${PORTAUDIO_LIBRARIES})
