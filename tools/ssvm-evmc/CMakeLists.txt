# SPDX-License-Identifier: Apache-2.0

add_library(ssvm-evmc
  SHARED
  evmccreate.cpp
)

target_link_libraries(ssvm-evmc
  PRIVATE
  wasmedgeVM
  ssvmEVMCHostModuleEEI
)
