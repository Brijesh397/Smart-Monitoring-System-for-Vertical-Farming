# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_edge_device_ai)

idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-unused-variable" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-maybe-uninitialized" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=format=" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-unused-but-set-parameter" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=nonnull-compare" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=stringop-truncation" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=uninitialized" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-stringop-truncation" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-deprecated-declarations" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-fno-if-conversion" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-implicit-fallthrough" APPEND)