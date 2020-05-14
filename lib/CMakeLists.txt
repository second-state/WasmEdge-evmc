# SPDX-License-Identifier: Apache-2.0

add_library(ssvmEVMCHostModuleEEI
  eeienv.cpp
  eeifunc.cpp
  eeimodule.cpp
)

target_link_libraries(ssvmEVMCHostModuleEEI
  PRIVATE
  ssvmEVMCUtilKeccak
)

target_include_directories(ssvmEVMCHostModuleEEI
  PUBLIC
  ${Boost_INCLUDE_DIR}
  ${SSVM_CORE_SRC_DIR}/include
  ${PROJECT_SOURCE_DIR}/utils/keccak
  ${PROJECT_SOURCE_DIR}/utils/evmc/include
)
