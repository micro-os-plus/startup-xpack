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

if(micro-os-plus-startup-included)
  return()
endif()

set(micro-os-plus-startup-included TRUE)

message(STATUS "Including micro-os-plus-startup...")

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

  target_sources(
    micro-os-plus-startup-static

    PRIVATE
      ${xpack_current_folder}/src/startup.cpp
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
      micro-os-plus::diag-trace-static
      micro-os-plus::architecture
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::startup-static ALIAS micro-os-plus-startup-static)
  message(STATUS "micro-os-plus::startup-static")

endif()

# -----------------------------------------------------------------------------
