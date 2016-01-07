cmake_minimum_required(VERSION 2.6)
project(game_engine)

set(CMAKE_USE_RELATIVE_PATHS TRUE)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Set an output directory for our binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    # Compiler options for GCC and Clang on Linus or OSX

    # Use C++ 11
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # Set warning levels
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

    # Set debug and release flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # Compiler options for MSVC on Windows
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Box2D REQUIRED)

message(STATUS "SDL2_LIBRARY...... ${SDL2_LIBRARY}")
message(STATUS "SDL2_INCLUDE_DIR.. ${SDL2_INCLUDE_DIR}")
message(STATUS "BOX2D_LIBRARY..... ${BOX2D_LIBRARY}")
message(STATUS "BOX2D_INCLUDE_DIR. ${BOX2D_INCLUDE_DIR}")

# Include directories with relevant items
add_subdirectory("${CMAKE_SOURCE_DIR}/source")
