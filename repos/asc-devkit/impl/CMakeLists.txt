# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
add_compile_options(-Werror)

add_subdirectory(adv_api)
add_subdirectory(basic_api)
add_subdirectory(c_api)
add_subdirectory(tensor_api)
add_subdirectory(utils)

if(NOT BUILD_OPEN_PROJECT)
  install_package(
    PACKAGE
    tiling_api
    TARGETS
    tiling_api
    tiling_api_headers
    DIRECTORY
    ${ASCENDC_DIR}/include
    DESTINATION
    ${INSTALL_INCLUDE_DIR}/asc
    DIRECTORY
    ${ASCENDC_DIR}/impl
    DESTINATION
    ${INSTALL_INCLUDE_DIR}/asc
    FILES
    ${ASCENDC_DIR}/include/adv_api/tiling_api.h
    DESTINATION
    ${INSTALL_INCLUDE_DIR}/asc/include/adv_api)

  configure_file(${ASCENDC_DIR}/impl/adv_api/cmake/tiling_directory.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/tiling_directory.cmake @ONLY)
  install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/tiling_directory.cmake COMPONENT asc-devkit)
endif()
