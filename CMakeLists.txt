# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.21...3.24)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  json-packer
  VERSION 1.0
  LANGUAGES C)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)
#set(CMAKE_C_STANDARD 11)

# Find packages go here.

# This is a "default" library, and will match the *** variable setting.
# Other common choices are STATIC, SHARED, and MODULE
# Including header files here helps IDEs but is not required.
# Output libname matches target name, with the usual extensions on your system
#add_library(MyLibExample simple_lib.cpp simple_lib.hpp)

# Link each target with other targets or add options, etc.

# Enables .c files to add headers in the include folder.
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

# Build Log.c lib
#================================================================
file(GLOB logclib_SRCS
        ${PROJECT_SOURCE_DIR}/lib/log.c/src/*.h
        ${PROJECT_SOURCE_DIR}/lib/log.c/src/*.c
        )
add_library(logclib SHARED
  ${logclib_SRCS}
)
target_include_directories(logclib PRIVATE ${PROJECT_SOURCE_DIR}/lib/log.c/)
#================================================================

# Build yajl lib
#================================================================
include(FetchContent)
FetchContent_Declare(
  yajl
  GIT_REPOSITORY https://github.com/lloyd/yajl.git
  GIT_TAG        2.1.0  
)
FetchContent_MakeAvailable(yajl)
#================================================================

# Build json-packer executable
#================================================================
# Just add whatever is in those folders.
include_directories("${yajl_BINARY_DIR}/yajl-2.1.0/include/")
include_directories("${PROJECT_SOURCE_DIR}/lib/log.c/src/")

file(GLOB main_SRCS
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.c"
)
# Build the target from the list of all sources
add_executable(json-packer ${main_SRCS})
# Make sure you link your targets with this command. It can also link libraries and even flags, so linking a target that does not exist will not give a configure-time error.
target_link_libraries(json-packer PRIVATE yajl logclib)
#================================================================


enable_testing()
add_subdirectory(test)
