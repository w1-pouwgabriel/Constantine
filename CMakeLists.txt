cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "Constantine")

project(${PROJECT_NAME})

#Define the project that you want to launch
#  The name must be the same as the folder its placed in
set(CONTSTANTINE_PROJECT_NAME "TestProject")

# Use C++ 17 as a standard
set(CMAKE_CXX_STANDARD 17)

# Enable folder structure in the IDE
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
	# Add a macro for debugging
	# All current compile options: Debug, Release, RelWithDebInfo, and MinSizeRel
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_compile_definitions(ENABLE_VALIDATION_LAYER=1)
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_compile_definitions(ENABLE_VALIDATION_LAYER=0)
	endif()

	# Remove the always defined _WINDOWS flag
	# CPP
	string (REPLACE "/D_WINDOWS" "" CMAKE_CUSTOM_CPP_FLAGS ${CMAKE_CXX_FLAGS})
	string (REPLACE "/DWIN32" "" CMAKE_CUSTOM_CPP_FLAGS ${CMAKE_CUSTOM_CPP_FLAGS}) 
	SET(CMAKE_CXX_FLAGS "${CMAKE_CUSTOM_CPP_FLAGS}" CACHE STRING "" FORCE)
	# C
	string (REPLACE "/D_WINDOWS" "" CMAKE_CUSTOM_C_FLAGS ${CMAKE_C_FLAGS})
	string (REPLACE "/DWIN32" "" CMAKE_CUSTOM_C_FLAGS ${CMAKE_CUSTOM_C_FLAGS})
	SET(CMAKE_C_FLAGS "${CMAKE_CUSTOM_C_FLAGS}" CACHE STRING "" FORCE)
else()
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_compile_definitions(ENABLE_VALIDATION_LAYER=1)
	elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	add_compile_definitions(ENABLE_VALIDATION_LAYER=0)
	endif()
endif()

# Debug postfix
SET (CMAKE_DEBUG_POSTFIX _d)

# Some easy access to folders within this repository
SET(SOURCE_THIRDPARTY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Engine/ThirdParty)
SET(SOURCE_RUNTIME_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Engine/Runtime)
SET(PROJECT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Projects)

MESSAGE(STATUS ${SOURCE_THIRDPARTY_DIRECTORY})
MESSAGE(STATUS ${SOURCE_RUNTIME_DIRECTORY})
MESSAGE(STATUS ${PROJECT_DIRECTORY})

add_subdirectory(${SOURCE_THIRDPARTY_DIRECTORY})
add_subdirectory(${SOURCE_RUNTIME_DIRECTORY})
add_subdirectory(${PROJECT_DIRECTORY})