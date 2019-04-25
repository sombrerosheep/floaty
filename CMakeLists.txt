cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(floaty C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(floaty_description "Prototype with SDL")
set(floaty_source_dir "${CMAKE_SOURCE_DIR}/src")
set(floaty_include_dir "${floaty_source_dir}/include")

# Build Configuration
# Define Debug if no value was passed
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Build type not specified: Using Debug by default.")
endif(NOT CMAKE_BUILD_TYPE)

if (NOT CMAKE_BUILD_TYPE MATCHES "^Debug$|^Release$")
  message(FATAL_ERROR
    "Unrecognized build type: ${CMAKE_BUILD_TYPE}"
    "Supported build types are \"Debug\" and \"Release\"" # TODO: place supported types into array and work on that vs hardcode
  )
endif()

# Check for 64 or 32 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_64 TRUE)
  set(PROCESSOR_ARCH "x64")
else()
  set(ARCH_64 FALSE)
  set(PROCESSOR_ARCH "x86")
endif()

# Dependencies
# SDL
# requires an environment variable called SDL_SHARED pointing to the location
# of the SDL shared libs downloaded from https://www.libsdl.org/
# This project does dynamic link
# TODO: Use findSDL2 cmake; several options/scripts are available
set(SDL2_INCLUDE_DIR "$ENV{SDL_SHARED}/debug/include/")
set(SDL2_LIBRARIES
  "$ENV{SDL_SHARED}/${CMAKE_BUILD_TYPE}/lib/${PROCESSOR_ARCH}/SDL2main.lib"
  "$ENV{SDL_SHARED}/${CMAKE_BUILD_TYPE}/lib/${PROCESSOR_ARCH}/SDL2.lib"
)

##### Info output #####
message(STATUS "")
message(STATUS "Building ${CMAKE_PROJECT_NAME} Version ${PROJECT_VERSION}")
message(STATUS "${floaty_description}")
message(STATUS "")
message(STATUS "Platform:   ${CMAKE_SYSTEM}")
message(STATUS "64-bit:     ${ARCH_64}")
message(STATUS "Compiler:   ${CMAKE_CXX_COMPILER}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS " CFLAGS:        ${CMAKE_CXX_FLAGS}")
message(STATUS " EXTRA_CFLAGS:  ${EXTRA_CXXFLAGS}")
message(STATUS " EXTRA_LDFLAGS: ${EXTRA_LDFLAGS}")
message(STATUS " EXTRA_LIBS:    ${EXTRA_LIBS}")
message(STATUS "")
message(STATUS "Project Variables:")
message(STATUS "  Source Dir:           ${floaty_source_dir}")
message(STATUS "  Include Dir:          ${floaty_include_dir}")
message(STATUS "")
message(STATUS "Dependency Report:")
message(STATUS "  SDL2 Include Dir:  ${SDL_INCLUDE_DIR}")
message(STATUS "  SDL2 Libraries:")
foreach(lib ${SDL2_LIBRARIES})
  message(STATUS "    ${lib}")
endforeach()
message(STATUS "")

#### Build ####
file(GLOB floaty_source "${floaty_source_dir}/*.c")
add_executable(floaty ${floaty_source})
target_include_directories(floaty PUBLIC ${floaty_include_dir} ${SDL2_INCLUDE_DIR})
target_link_libraries(floaty ${SDL2_LIBRARIES})

# Copy sdl shared lib to our build dest folder
# This prevents us from having these in path
# TODO: Verify Linux does not need this
if (WIN32)
  configure_file("$ENV{SDL_SHARED}/${CMAKE_BUILD_TYPE}/lib/${PROCESSOR_ARCH}/SDL2.dll" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/SDL2.dll" COPYONLY)
endif(WIN32)
