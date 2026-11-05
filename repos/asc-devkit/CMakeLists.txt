# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)
include(cmake/fetch_cann_cmake.cmake)
project(asc-devkit)

init_cann_project(PREPEND_MODULE_PATH)
add_cann_target_options()
option(BUILD_OPEN_PROJECT "Build open asc-devkit lib project" ON)

set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "")

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(STATUS "compile project asc-devkit with library")
  option(BUILD_WITH_INSTALLED_DEPENDENCY_CANN_PKG "Build asc-devkit with cann pkg" ON)
else()
  message(STATUS "compile project asc-devkit with src")
  option(BUILD_WITH_INSTALLED_DEPENDENCY_CANN_PKG "Build asc-devkit with cann source" OFF)
endif()

set(ASCENDC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ASCENDC_INCLUDE_DIR ${ASCENDC_DIR}/include)
set(ASCENDC_IMPL_DIR ${ASCENDC_DIR}/impl)

if(BUILD_OPEN_PROJECT)
  include(cmake/config.cmake)
  include(cmake/func.cmake)
  set(INSTALL_LIBRARY_DIR ${CMAKE_SYSTEM_PROCESSOR}-linux)
  include(cmake/dependencies.cmake)
  add_cann_target_options()
endif()

add_subdirectory(impl)
add_subdirectory(tools)
add_subdirectory(cmake)


include(version.cmake)
if(BUILD_OPEN_PROJECT)
  check_cann_pkg_build_deps("asc-devkit")
endif()
add_cann_version_info_targets()

if(ENABLE_TEST)
  add_subdirectory(tests)
endif()

if(PACKAGE_OPEN_PROJECT)
  cmake_minimum_required(VERSION 3.16)
  project(asc-devkit VERSION 1.0.0)
  include(CMakePrintHelpers)
  include(cmake/package.cmake)
  include(cmake/version_info.cmake)
endif()
