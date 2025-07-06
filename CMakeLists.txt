cmake_minimum_required(VERSION 3.15)

set(namespace "stz")
project("stz"
    VERSION 0.0.1
    DESCRIPTION "Arena based standard library"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_USER_MAKE_RULES_OVERRIDE "cmake/ClangOverrides.txt")
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        stzd
        SDL2
        SDL2_image
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/arena.cpp
        src/buf.cpp
        src/cmd.cpp
        src/file.cpp
        src/gui.cpp
        src/index.cpp
        src/map.cpp
        src/str.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# totally optional listing of include directories
get_property(inclds
    TARGET ${PROJECT_NAME}
    PROPERTY INCLUDE_DIRECTORIES
)
message(STATUS "Listing include directories for ${PROJECT_NAME}...")
# "dumb" listing with "raw" generator expressions on configuration
foreach(pth ${inclds})
    message(STATUS "  ${pth}")
endforeach()
# actually evaluated generator expressions printed to file on generation
string(JOIN "\n" includeDirectories ${inclds})
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/include-directories.txt"
    CONTENT ${includeDirectories}
)

# without it public headers won't get installed
set(public_headers
    include/arena.h
    include/buf.h
    include/slice.h
    include/cmd.h
    include/file.h
    include/gui.h
    include/index.h
    include/log.h
    include/map.h
    include/range.h
    include/str.h
    include/types.h
)

# not for MSVC
if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -pedantic
            -fno-exceptions
    )
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")
include(Installing)

# --------------------- Tests ---------------------
add_subdirectory(tests)

# --------------------- Examples ---------------------
add_subdirectory(examples)
