# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.11)
project(WasmEdge-EVMC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(Boost REQUIRED)
option(BUILD_AOT_RUNTIME "Enable WasmEdge LLVM-based ahead of time compilation runtime." OFF)

# Clone WasmEdge-core
if(NOT WASMEDGE_CORE_PATH)
  include(FetchContent)
  FetchContent_Declare(
    WasmEdge
    GIT_REPOSITORY https://github.com/WasmEdge/WasmEdge.git
    GIT_TAG 0.8.0
    )

  FetchContent_GetProperties(WasmEdge)
  if (NOT WasmEdge_POPULATED)
    FetchContent_Populate(WasmEdge)
  endif ()
  add_subdirectory(${wasmedge_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/wasmedge-core)
  set(WasmEdge_CORE_SRC_DIR ${wasmedge_SOURCE_DIR})
else()
  get_filename_component(
    WasmEdge_CORE_PATH "${WasmEdge_CORE_PATH}"
    REALPATH
    BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  add_subdirectory(${wasmedge_CORE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/utils/wasmedge-core)
  set(WasmEdge_CORE_SRC_DIR ${wasmedge_CORE_PATH})
endif()

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(utils)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
