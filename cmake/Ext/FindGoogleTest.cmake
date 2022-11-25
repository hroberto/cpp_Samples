
message("==> Find Project GOOGLE TEST")

SET(gtest_build_tests  OFF CACHE BOOL "...")
SET(gtest_build_samples  OFF CACHE BOOL "...")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable( googletest )

target_link_libraries( ${TARGET} ${googletest})
