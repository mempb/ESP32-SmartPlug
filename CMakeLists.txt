cmake_minimum_required(VERSION 3.16.0)
list(APPEND EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/components/esp-homekit-sdk/components"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/esp-homekit-sdk/components/homekit"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/esp-homekit-sdk/examples/common")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ESP32-SmartPlug)