# SPDX-License-Identifier: Apache-2.0

add_executable(wasmedgeEVMCTest
  evmcTest.cpp
  example_host.cpp
)

target_link_libraries(wasmedgeEVMCTest
  PRIVATE
  wasmedgeCommon
  wasmedgeEVMCUtilEVMCLoader
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_DL_LIBS}
)

target_include_directories(wasmedgeEVMCTest
  PRIVATE
  ${WASMEDGE_CORE_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
)
