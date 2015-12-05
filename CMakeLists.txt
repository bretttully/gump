cmake_minimum_required(VERSION 2.8)

# ====================================================================
# use clang instead of gcc? (this must come before declaring the project)
option(USE_CLANG "Build application with clang" OFF)
if(USE_CLANG)
    SET (CMAKE_C_COMPILER   "/usr/bin/clang")
    SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
    SET (CMAKE_AR      "/usr/bin/llvm-ar")
    SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
    SET (CMAKE_NM      "/usr/bin/llvm-nm")
    SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
    SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
else(USE_CLANG)
    SET (CMAKE_C_COMPILER   "/usr/bin/gcc")
    SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
    SET (CMAKE_AR      "/usr/bin/ar")
    SET (CMAKE_LINKER  "/usr/bin/ld")
    SET (CMAKE_NM      "/usr/bin/nm")
    SET (CMAKE_OBJDUMP "/usr/bin/objdump")
    SET (CMAKE_RANLIB  "/usr/bin/ranlib")
endif(USE_CLANG)

# ====================================================================
# We have found that `ninja` is much faster than `make`
set(CMAKE_GENERATOR "Ninja")

# ====================================================================
# The project must be defined after selecting the compiler
project(gump)

# ====================================================================
# default build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to RelWithDebInfo")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

SET (CMAKE_C_FLAGS                "-Wall -std=c99")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -ftree-vectorize")

SET (CMAKE_CXX_FLAGS                "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ftree-vectorize")
# -----------------------------------------------------

# ====================================================================
# default compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimplicit-function-declaration")

# enable c++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# this is required by CGAL
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math")

# VTK uses some deprecated functions, use this flag to hide them
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

# Clang doesn't like missing braces -- gcc doesn't include them
# in -Wall anymore, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=25137
if(USE_CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
endif(USE_CLANG)

# ====================================================================
# find the external packages
find_package(PkgConfig REQUIRED)

pkg_check_modules(LOG4CXX REQUIRED liblog4cxx)
include_directories(${LOG4CXX_INCLUDE_DIRS})
link_directories(${LOG4CXX_LIBRARY_DIRS})

#set(VTK_DIR "/usr/local/lib/vtk")
find_package(VTK 5.10 REQUIRED QUIET)
include_directories(${VTK_INCLUDE_DIRS})
link_directories(${VTK_LIBRARY_DIRS})

# Update the root location for boost
set(BOOST_ROOT "/usr")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARY_DIRS})

# ----
# for the threaded testing
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# ====================================================================
# set up the default directories
set(base_dir ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${base_dir})

include_directories(${base_dir}) # used in the testing directory

set(src_dir ${base_dir}/src)
include_directories(${src_dir})

set(test_dir ${base_dir}/test)
include_directories(${test_dir})

set(lib_dir ${base_dir}/lib)
link_directories(${lib_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${lib_dir})

set(bin_dir ${base_dir}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

set(build_dir ${base_dir}/_build)
set(CMAKE_BINARY_DIR ${build_dir})

# ====================================================================
# set up the libraries
set(GTEST_LIBS gtest ${CMAKE_THREAD_LIBS_INIT})
set(LOG4CXX_LIBS log4cxx aprutil-1 expat apr-1)
set(BOOST_LIBS boost_filesystem boost_system boost_thread boost_iostreams)
#message(STATUS "VTK libs: ${VTK_LIBRARIES}")
set(VTK_LIBS ${VTK_LIBRARIES})

# ====================================================================
# add the src and testing directories
add_subdirectory(src)
add_subdirectory(test)

message(STATUS "-- -- -- User set options -- -- -- --")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Use Clang: ${USE_CLANG}")
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")

