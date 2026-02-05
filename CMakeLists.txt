# The following five lines of boilerplate have to be in your project's CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# The (official) ESP IDF component variant (baaaad if I'm honest)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(dino_game)

# The modified variant to use C++'s include features as they were intended and not interfered with by ESP IDF!
#set(COMPONENT_DIRS "$ENV{IDF_PATH}/components" "./componentLinking")
#include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#project(main)