# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgeEVMCHostModuleEEI
  eeienv.cpp
  eeifunc.cpp
  eeimodule.cpp
)

target_link_libraries(wasmedgeEVMCHostModuleEEI
  PRIVATE
  wasmedgeEVMCUtilKeccak
)

target_include_directories(wasmedgeEVMCHostModuleEEI
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${WASMEDGE_CORE_SRC_DIR}/include
  ${WASMEDGE_CORE_SRC_DIR}/thirdparty
  ${PROJECT_SOURCE_DIR}/utils/keccak
  ${PROJECT_SOURCE_DIR}/utils/evmc/include
)
