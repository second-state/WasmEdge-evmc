# SPDX-License-Identifier: Apache-2.0

add_executable(ssvmEVMCTest
  evmcTest.cpp
  example_host.cpp
)

target_link_libraries(ssvmEVMCTest
  PRIVATE
  ssvmCommon
  ssvmEVMCUtilEVMCLoader
  utilGoogleTest
  ${CMAKE_DL_LIBS}
)

target_include_directories(ssvmEVMCTest
  PRIVATE
  ${SSVM_CORE_SRC_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
)
