add_library(Core STATIC)

# Sources
file(GLOB CORE_PRIVATE_SOURCES ${SOURCE_RUNTIME_DIRECTORY}/Core/Private/*.cpp)
file(GLOB CORE_PUBLIC_HEADERS ${SOURCE_RUNTIME_DIRECTORY}/Core/Public/*.h)

target_sources(Core PRIVATE ${CORE_PRIVATE_SOURCES})
target_sources(Core PUBLIC ${CORE_PUBLIC_HEADERS})

# Create the project filters
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Core/Private Private)
GROUPSOURCES(${SOURCE_RUNTIME_DIRECTORY}/Core/Public Public)

# Link libs
target_link_libraries(Core PUBLIC Graphics)

target_include_directories(Core PUBLIC ${SOURCE_THIRDPARTY_DIRECTORY}/glm)
target_include_directories(Core PUBLIC ${SOURCE_THIRDPARTY_DIRECTORY}/stb)

# Include self
target_include_directories(Core PRIVATE ${SOURCE_RUNTIME_DIRECTORY}/Core/Private)
target_include_directories(Core PUBLIC ${SOURCE_RUNTIME_DIRECTORY}/Core/Public)

if(${CMAKE_HOST_WIN32})
    target_compile_definitions(Core PUBLIC -DPPP_WINDOWS)
endif()

# Target properties
set_target_properties(Core PROPERTIES
                      FOLDER "Engine")