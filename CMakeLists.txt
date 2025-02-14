# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(Constatine VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify project directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# Include directories
include_directories(${INCLUDE_DIR})

# Gather source files, including tiny_gltf.cc
file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
)

# Add the executable
add_executable(Constatine ${SRC_FILES})

# Add GLFW include directory
include_directories(${INCLUDE_DIR}/GLFW)

# Add the GLFW library (assuming the DLL is in the lib folder)
# You can link dynamically by setting the library location
link_directories(${LIB_DIR})

# Link some libraries
target_link_libraries(Constatine ${LIB_DIR}/glfw3.dll)
target_link_libraries(Constatine opengl32)

# Static linking (e.g., glfw3.a or glfw3.lib)
target_link_libraries(Constatine ${LIB_DIR}/libtinygltf.a)

# Set compiler explicitly (if needed)
if(MINGW)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

# Find all DLL files in the lib directory
file(GLOB DLL_FILES "${CMAKE_SOURCE_DIR}/lib/*.dll")

# Copy each DLL file to the build directory
foreach(DLL ${DLL_FILES})
    add_custom_command(TARGET Constatine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL} ${CMAKE_BINARY_DIR}
    )
endforeach()

# Copy all files from lib directory to the build directory
add_custom_command(TARGET Constatine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)