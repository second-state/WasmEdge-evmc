# SPDX-License-Identifier: Apache-2.0


add_library(soll_runtime_test SHARED
  soll_runtime_test.cpp
  example_host.cpp
)

target_link_libraries(soll_runtime_test
  PRIVATE
  wasmedgeEVMCUtilEVMCLoader
  ${CMAKE_DL_LIBS}
)

target_include_directories(soll_runtime_test
  PRIVATE
  ${WasmEdge_CORE_SRC_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
)
