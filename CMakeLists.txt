# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.11)
project(SSVM-EVMC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(Boost REQUIRED)
option(SSVM_EVMC_DISABLE_TOOLS "Disable SSVM-EVMC tools." OFF)
option(SSVM_DISABLE_HOST_FUNCTIONS "Disable all SSVM host functions. " ON)
option(SSVM_DISABLE_AOT_RUNTIME "Disable SSVM LLVM-based ahead of time compilation runtime." ON)

# Clone ssvm-core
if(NOT SSVM_CORE_PATH)
  include(FetchContent)
  FetchContent_Declare(
    ssvm
    GIT_REPOSITORY https://github.com/second-state/SSVM.git
    GIT_TAG hydai/0.6.3-evmc-tmp
  )

  FetchContent_GetProperties(ssvm)
  if (NOT ssvm_POPULATED)
      FetchContent_Populate(ssvm)
  endif ()
  add_subdirectory(${ssvm_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils/ssvm-core)
  set(SSVM_CORE_SRC_DIR ${ssvm_SOURCE_DIR})
else()
  get_filename_component(
    SSVM_CORE_PATH "${SSVM_CORE_PATH}"
    REALPATH
    BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  add_subdirectory(${SSVM_CORE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/utils/ssvm-core)
  set(SSVM_CORE_SRC_DIR ${SSVM_CORE_PATH})
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
if (NOT SSVM_EVMC_DISABLE_TOOLS)
  add_subdirectory(tools)
endif()
add_subdirectory(utils)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
