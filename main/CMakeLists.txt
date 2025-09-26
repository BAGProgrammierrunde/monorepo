# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
cmake_minimum_required(VERSION 3.5)

set(COMPONENT_DIRS "$ENV{IDF_PATH}/components" "./componentLinking")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)
