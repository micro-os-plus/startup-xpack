#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus)
# Copyright (c) 2021 Liviu Ionescu
#
# This Source Code Form is subject to the terms of the MIT License.
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.19/
# https://cmake.org/cmake/help/v3.19/manual/cmake-packages.7.html#package-configuration-file
cmake_minimum_required(VERSION 3.19)

# Use targets as include markers (variables are not scope independent).
if(TARGET micro-os-plus-startup-included)
  return()
else()
  add_custom_target(micro-os-plus-startup-included)
endif()

if(NOT TARGET micro-os-plus-build-helper-included)
  message(FATAL_ERROR "Include the mandatory build-helper (xpacks/micro-os-plus-build-helper/cmake/xpack-helper.cmake)")
endif()

message(STATUS "Processing xPack ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}...")

# -----------------------------------------------------------------------------
# Dependencies.

find_package(micro-os-plus-version REQUIRED)
find_package(micro-os-plus-diag-trace REQUIRED)

# -----------------------------------------------------------------------------
# The current folder.

get_filename_component(xpack_current_folder ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# -----------------------------------------------------------------------------

if (NOT TARGET micro-os-plus-startup-static)

  add_library(micro-os-plus-startup-static STATIC EXCLUDE_FROM_ALL)

  # ---------------------------------------------------------------------------

  xpack_glob_recurse_cxx(source_files "${xpack_current_folder}/src")
  xpack_display_relative_paths("${source_files}" "${xpack_current_folder}")

  target_sources(
    micro-os-plus-startup-static

    PRIVATE
      ${source_files}
  )

  target_include_directories(
    micro-os-plus-startup-static

    PUBLIC
      ${xpack_current_folder}/include
  )

  target_link_libraries(
    micro-os-plus-startup-static

    PUBLIC
      micro-os-plus::version
      micro-os-plus::diag-trace
      micro-os-plus::architecture
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::startup ALIAS micro-os-plus-startup-static)
  message(STATUS "=> micro-os-plus::startup")

endif()

# -----------------------------------------------------------------------------
