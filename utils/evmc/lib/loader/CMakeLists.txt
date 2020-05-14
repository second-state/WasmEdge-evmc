# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018-2019 The EVMC Authors.
# Licensed under the Apache License, Version 2.0.

add_library(ssvmEVMCUtilEVMCLoader
  STATIC
  loader.c
)

set_target_properties(ssvmEVMCUtilEVMCLoader
  PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
)

target_link_libraries(ssvmEVMCUtilEVMCLoader
  INTERFACE
  ${CMAKE_DL_LIBS}
)

target_include_directories(ssvmEVMCUtilEVMCLoader
  PUBLIC
  ${PROJECT_SOURCE_DIR}/utils/evmc/include
)
