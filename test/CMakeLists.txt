include(FetchContent)

add_compile_options(-DUNITY_INCLUDE_DOUBLE)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)
include_directories("${unity_SOURCE_DIR}/src")

FetchContent_Declare(
  yajl
  GIT_REPOSITORY https://github.com/lloyd/yajl.git
  GIT_TAG        2.1.0  
)
FetchContent_MakeAvailable(yajl)
include_directories("${yajl_SOURCE_DIR}/src/api")

# Just add whatever is in those folders.
file(GLOB test_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/test/*.c"
        )

# Build the target from the list of all sources
add_executable(test-json-packer ${test_SRCS})

target_link_libraries(test-json-packer PRIVATE unity yajl tlvlib logclib)

add_test(NAME test-json-packer
         COMMAND test-json-packer)
