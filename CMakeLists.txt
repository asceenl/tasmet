# CMakeList.txt for TaSMET
cmake_minimum_required (VERSION 2.8.5)
project(TaSMET)
set(PACKAGE_VERSION 0.1)
message("Running Cmake for TaSMET version ${PACKAGE_VERSION}")

# Set the Python version to link against. If this is not set, Cmake tries to find it automatically.
# set(TaSMET_PY_VERSION "2.7")
set(TaSMET_PY_VERSION "3.5m")

# Tracer name (name of the variable)
add_definitions(-DTRACER=1)
# add_definitions(-DTRACER_IN_COMMON)

add_definitions(-DTASMET_FLOAT=64)
add_definitions(-DTASMET_DEBUG=1)
add_definitions(-DARMA_DONT_USE_WRAPPER)
#====================================================
# Compiler settings ************************************************
#====================================================

# Always required make flags in case of both compilers
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pipe -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable  ")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors -pipe -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable  ")
#========Qt5 Stuff ==================================
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Protobuf REQUIRED)
# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#==================================================
# Optimized code flags *******************************************
#==================================================

#Debug mode
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX)FLAGS} -g -ggdb")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native")
# set(CMAKE_CLANG "${CMAKE_GCC} -march=native -mtune=native -fopenmp")


# To disable bound checking on std::vector, and to disable assertions
# add_definitions(-DNDEBUG)

# To increase speed on Armadillo
# add_definitions(-DARMA_NO_DEBUG)


# Disable traces
# add_definitions(-DTRACER=0)

# Pre-allocation size for matrices. Very important setting to tune the
# code in case we know that we are going to run with certain sizes of matrices and vectors. For Nf=6, set this to (2*Nf+1)^2=169
# For Nf=1: 9
# For Nf=2: 25
# For Nf=3: 49
# For Nf=4: 81
# For Nf=5: 121
# For Nf=6: 169
# For Nf=7: 225
# For Nf=8: 289
# For Nf=9: 361
# For Nf=10: 441
# For Nf=11: 529
# For Nf=12: 625
# Watch out! Setting prealloc too high can give too much overhead for smaller Nf's
# add_definitions(-DARMA_MAT_PREALLOC=625)


# ##########################
# Finding the presence of the prerequisites
# ##########################


# For importing find directives for Cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_tools)

# ##########################
# Python #####################
# ##########################

# Find the site_packages directory of python
execute_process(COMMAND python${TaSMET_PY_MAJOR_VERSION} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

# Armadillo
find_package(Armadillo REQUIRED)
add_definitions(-DARMA_USE_SUPERLU -DARMA_USE_CXX11)

# ==================== Compile the code in common and src

# This is the common code (gas and solid libs, etc)
# link_directories(common)

include_directories(
  src
  src/solver
  src/material
  src/sys
  )

# Add the code subdirectory
add_subdirectory(src)
add_subdirectory(testing)

add_executable(tasmet src/main.cpp src/gui/tasmet_resources.qrc)
add_executable(tasmet_solvemodel src/tasmet_solvemodel.cpp)
target_link_libraries(tasmet tasmet_gui tasmet_src messages Qt5::Widgets muparser openblas)
target_link_libraries(tasmet_solvemodel tasmet_src messages openblas muparser)
