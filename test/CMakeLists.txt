include(FetchContent)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)

include_directories("${unity_SOURCE_DIR}/src")

# After the following call, the CMake targets defined by googletest and
# Catch2 will be defined and available to the rest of the build
FetchContent_MakeAvailable(unity)

# Just add whatever is in those folders.
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/test/*.c"
        )

# Build the target from the list of all sources
add_executable(test-json-packer ${all_SRCS})

target_link_libraries(test-json-packer PRIVATE unity)