# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.11)
project(WASMEDGE-EVMC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(Boost REQUIRED)
option(WASMEDGE_EVMC_BUILD_TESTS "Enable WasmEdge EVMC tests." ON)
option(WASMEDGE_BUILD_AOT_RUNTIME "Enable WasmEdge LLVM-based ahead of time compilation runtime." OFF)
option(WASMEDGE_BUILD_TOOLS "Generate wasmedge and wasmedgec tools." OFF)

# Clone wasmedge-core
if(NOT WASMEDGE_CORE_DIR)
  include(FetchContent)
  FetchContent_Declare(
    wasmedge
    GIT_REPOSITORY https://github.com/WasmEdge/WasmEdge.git
    GIT_TAG 0.9.0
  )

  FetchContent_GetProperties(wasmedge)
  if(NOT wasmedge_POPULATED)
    message(STATUS "Fetching WasmEdge repository")
    FetchContent_Populate(wasmedge)
    message(STATUS "Fetching WasmEdge repository - done")
  endif()
  add_subdirectory(${wasmedge_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/WasmEdge-core)
  set(WASMEDGE_CORE_DIR ${wasmedge_SOURCE_DIR})
  set(WASMEDGE_CORE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/utils/WasmEdge-core)
else()
  get_filename_component(
    WASMEDGE_CORE_DIR "${WASMEDGE_CORE_DIR}"
    REALPATH
    BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  if(NOT WASMEDGE_CORE_BIN_DIR)
    add_subdirectory(${WASMEDGE_CORE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/WasmEdge-core)
    set(WASMEDGE_CORE_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/utils/WasmEdge-core)
  else()
    get_filename_component(
      WASMEDGE_CORE_BIN_DIR "${WASMEDGE_CORE_BIN_DIR}"
      REALPATH
      BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
endif()

message(STATUS "Set WasmEdge-core source path: ${WASMEDGE_CORE_DIR}")
message(STATUS "Set WasmEdge-core binary path: ${WASMEDGE_CORE_BIN_DIR}")

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(BEFORE
  ${WASMEDGE_CORE_BIN_DIR}/include
  ${WASMEDGE_CORE_SRC_DIR}/include
)

include_directories(BEFORE
  ${spdlog_SOURCE_DIR}/include
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(utils)

if(WASMEDGE_EVMC_BUILD_TESTS)
  add_subdirectory(test)
endif()
