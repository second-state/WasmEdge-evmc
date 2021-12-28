# SPDX-License-Identifier: Apache-2.0

add_library(wasmedge-evmc
  SHARED
  evmccreate.cpp
)

target_link_libraries(wasmedge-evmc
  PRIVATE
  wasmedgeVM
  wasmedgeEVMCHostModuleEEI
)
