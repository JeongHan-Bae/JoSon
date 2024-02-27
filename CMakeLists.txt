cmake_minimum_required(VERSION 3.24)
project(JoSon)

# Add your source files
set(SOURCE_FILES src/Doc.cpp src/Viso.cpp src/Joson.cpp)

# Create a dynamic library from the source files
add_library(JoSon SHARED ${SOURCE_FILES})

# Specify the include directories
target_include_directories(JoSon PUBLIC /include/JoSon)

set(DLL_DIR ${CMAKE_SOURCE_DIR}/lib)

set(DLL_FILES libJoSon.dll)

set(DEST_DIR ${CMAKE_BINARY_DIR})

foreach (DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET JoSon POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL_DIR}/${DLL_FILE}"
            "${DEST_DIR}")
endforeach ()
