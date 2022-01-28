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

# This file is intended to be consumed by applications with:
#
# `add_subdirectory("xpacks/micro-os-plus-startup")`
#
# The result is a library that can be added to the linker with:
#
# `target_link_libraries(your-target PUBLIC micro-os-plus::startup)`
#
# To avoid poluting this file with test related details, there is a separate
# CMake configuration in the `tests` folder.

# -----------------------------------------------------------------------------
## Preamble ##

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required(VERSION 3.20)

project(
  micro-os-plus-startup
  DESCRIPTION "µOS++ startup code for bare-metal platforms"
)

# PRIVATE: build definitions, used internally
# INTERFACE: usage definitions, passed up to targets linking to it
# PUBLIC: both

if(COMMAND xpack_get_package_name_and_version)
  xpack_get_package_name_and_version("${CMAKE_CURRENT_SOURCE_DIR}/package.json")
  message(VERBOSE "Processing xPack ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}...")
endif()

# -----------------------------------------------------------------------------

add_library(micro-os-plus-startup-static STATIC EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------------

target_sources(
  micro-os-plus-startup-static

  PRIVATE
    "src/startup.cpp"
)

message(VERBOSE "+ sources/src/startup.cpp")

target_include_directories(
  micro-os-plus-startup-static

  PUBLIC
    "include"
)

message(VERBOSE "+ sources/include")

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
message(VERBOSE "> micro-os-plus::startup -> micro-os-plus-startup-static")

# -----------------------------------------------------------------------------
