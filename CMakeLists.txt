cmake_minimum_required(VERSION 3.10)

# Export compile commands (useful for LSP tools)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_OSX_ARCHITECTURES "arm64")

project(shiba_inu VERSION 0.5
  DESCRIPTION "Gambling counting shiba inu"
  LANGUAGES CXX)

include_directories(include)

include_directories(
  src/utils
  src/gameplay
  src/graphics
)

file(GLOB SOURCES 
  src/*.cpp
  src/gameplay/*.cpp
  src/utils/*.cpp
  src/graphics/*.cpp
)

# Assuming 'assets' is a directory in your project root.
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(shiba_inu ${SOURCES})

# set the include and library paths manually because cmake can't find this.
# set(raylib_INCLUDE_DIR /home/linuxbrew/.linuxbrew/opt/raylib/include)
# set(raylib_LIBRARY /home/linuxbrew/.linuxbrew/opt/raylib/lib/libraylib.a)

set(raylib_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/raylib/include)
set(raylib_LIBRARY ${CMAKE_SOURCE_DIR}/third_party/raylib/libraylib.a)

# Include raylib headers
include_directories(${raylib_INCLUDE_DIR})

# Link raylib library after the executable target is created
target_link_libraries(shiba_inu PRIVATE ${raylib_LIBRARY})

## Codelldb
# Enable Debug Symbols
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Building in Debug mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")  # Add the -g flag for debugging symbols
endif()

# Add optimization flag for release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Building in Release mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
