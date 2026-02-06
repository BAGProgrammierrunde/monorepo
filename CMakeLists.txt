# The following five lines of boilerplate have to be in your project's CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(DINO_GAME_ELSE_NEW_ENGINE true)
set(PROJECT_NAME "Empty")

if (DINO_GAME_ELSE_NEW_ENGINE)
    # The (official) ESP IDF component variant (baaaad if I'm honest)
    set(PROJECT_NAME "dino_game")
else()
    # The modified variant to use C++'s include features as they were intended and not interfered with by ESP IDF!
    set(COMPONENT_DIRS "$ENV{IDF_PATH}/components" "./componentLinking")
    set(PROJECT_NAME "main")
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(PROJECT_NAME)