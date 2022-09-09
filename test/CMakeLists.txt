include(FetchContent)

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


FetchContent_Declare(
  apr
  URL https://dlcdn.apache.org//apr/apr-1.7.0.tar.gz
  URL_HASH SHA256=48e9dbf45ae3fdc7b491259ffb6ccf7d63049ffacbc1c0977cced095e4c2d5a2
)
add_custom_target(apr_configure COMMAND )
FetchContent_MakeAvailable(apr)
include_directories("${apr_SOURCE_DIR}/include")


# Just add whatever is in those folders.
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/test/*.c"
        )

# Build the target from the list of all sources
add_executable(test-json-packer ${all_SRCS})

target_link_libraries(test-json-packer PRIVATE unity yajl)

add_test(NAME test-json-packer
         COMMAND test-json-packer)