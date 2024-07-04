# Add executable
add_library(Engine STATIC)

# Sources
file(GLOB ENGINE_PRIVATE_SOURCES ${SOURCE_RUNTIME_DIRECTORY}/Engine/Private/*.cpp)
file(GLOB ENGINE_PUBLIC_HEADERS ${SOURCE_RUNTIME_DIRECTORY}/Engine/Public/*.h)

target_sources(Engine PRIVATE ${ENGINE_PRIVATE_SOURCES})
target_sources(Engine PUBLIC ${ENGINE_PUBLIC_HEADERS})

# Create the project filters
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Engine/Private Private)
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Engine/Public Public)

#### -------------- TODO update correct directories

target_link_libraries(Engine PUBLIC Graphics)
target_link_libraries(Engine PUBLIC Core)
target_link_libraries(Engine PUBLIC ${Vulkan_LIBRARIES})

# Include self
target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(Engine PRIVATE ${SOURCE_RUNTIME_DIRECTORY}/Engine/Private)
target_include_directories(Engine PUBLIC ${SOURCE_RUNTIME_DIRECTORY}/Engine/Public)

# Compile definitions
target_compile_definitions(Engine PUBLIC -DRTS_VULKAN)

if(${CMAKE_HOST_WIN32})
    target_compile_definitions(Engine PUBLIC -DPPP_WINDOWS)
endif()

# Target properties
set_target_properties(Engine PROPERTIES
                      FOLDER "Engine") 