cmake_minimum_required(VERSION 3.20)
project(Videohub
        VERSION 0.1
        DESCRIPTION "Blackmagic videohub integration"
        LANGUAGES CXX)

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    include(CTest)
    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
add_compile_options("$<$<CONFIG:RELWITHDEBINFO>:-DDEBUG>")

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)

option(COVERALLS "Generate coveralls data" OFF)

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

set(CMAKE_CXX_STANDARD 17)

#### Add External Dependencies ####
add_subdirectory(extern/spdlog)


add_subdirectory("extern/sfml")
set(SFML_INCS "extern/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/extern/sfml/lib")


find_package(OpenGL REQUIRED)

#add_subdirectory("extern/imgui")

#set(IMGUI_DIR "/extern/imgui")
#set(ImGui_INCLUDE_DIR "/extern/imgui")
#set(IMGUI_SFML_FIND_SFML OFF)
#set(IMGUI_SFML_IMGUI_DEMO ON)

#add_subdirectory("extern/imgui-sfml")


#set(IMGUI_INCS "extern/imgui")
#link_directories("${CMAKE_BINARY_DIR}/extern/imgui")
#set(IMGUI_SFML_INCS "extern/imgui-sfml")
#link_directories("${CMAKE_BINARY_DIR}/extern/imgui-sfml")


####

# The compiled library code is here
#add_subdirectory(src)

# Final application is here
#add_subdirectory(apps)

# Testing only available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        AND BUILD_TESTING)
    message("Testing enabled")
    add_subdirectory(tests)
endif()


file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(Videohub ${SOURCES})
target_include_directories(Videohub PRIVATE ${SFML_INCS} ${IMGUI_COMBINED_INCS}) # ${IMGUI_INCS} ${IMGUI_SFML_INCS})
target_link_libraries(Videohub PRIVATE  sfml-window sfml-graphics OpenGL::GL)

#add_custom_command(TARGET Videohub POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Videohub> ${OUTPUT_DIRECTORY}
#        COMMAND_EXPAND_LISTS
#        )

add_custom_command(TARGET Videohub POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory  "${PROJECT_SOURCE_DIR}/resources/" "${OUTPUT_DIRECTORY}/resources/"
        COMMAND_EXPAND_LISTS
        )