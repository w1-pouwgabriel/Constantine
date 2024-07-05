# Add library
add_library(Graphics STATIC)

# Sources
file(GLOB GRAPHICS_PRIVATE_SOURCES ${SOURCE_RUNTIME_DIRECTORY}/Graphics/Private/*.cpp)
file(GLOB GRAPHICS_PUBLIC_HEADERS ${SOURCE_RUNTIME_DIRECTORY}/Graphics/Public/*.h)

target_sources(Graphics PRIVATE ${GRAPHICS_PRIVATE_SOURCES})
target_sources(Graphics PUBLIC ${GRAPHICS_PUBLIC_HEADERS})

# Create the project filters
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Graphics/Private Private)
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Graphics/Public Public)

# Add a macro for debugging
# All current compile options: Debug, Release, RelWithDebInfo, and MinSizeRel
target_compile_definitions(Graphics PUBLIC -DENABLE_VALIDATION_LAYER)

# Private includes
MESSAGE(STATUS "Vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
MESSAGE(STATUS "Vulkan LIBRARIES: ${Vulkan_LIBRARIES}")

target_include_directories(Graphics PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(Graphics PUBLIC ${SOURCE_THIRDPARTY_DIRECTORY}/glm)
target_include_directories(Graphics PUBLIC ${SOURCE_THIRDPARTY_DIRECTORY}/glfw/include)
#target_include_directories(Graphics PUBLIC ${SOURCE_THIRDPARTY_DIRECTORY}/ImGui)

target_link_libraries(Graphics PUBLIC ${Vulkan_LIBRARIES})
target_link_libraries(Graphics PUBLIC glfw)
target_link_libraries(Graphics PUBLIC Core)

# Include self
target_include_directories(Graphics PRIVATE ${SOURCE_RUNTIME_DIRECTORY}/Graphics/Private)
target_include_directories(Graphics PUBLIC ${SOURCE_RUNTIME_DIRECTORY}/Graphics/Public)

if(${CMAKE_HOST_WIN32})
    target_compile_definitions(Graphics PUBLIC -DPPP_WINDOWS)
endif()

# Target properties
set_target_properties(Graphics PROPERTIES
                      FOLDER "Engine")
