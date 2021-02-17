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

function(target_sources_micro_os_plus_startup target)

  get_filename_component(xpack_current_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_sources(
    ${target}

    PRIVATE
      ${xpack_current_folder}/src/startup.cpp
  )

endfunction()

# -----------------------------------------------------------------------------

function(target_include_directories_micro_os_plus_startup target)

  get_filename_component(xpack_current_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_include_directories(
    ${target}

    PUBLIC
      ${xpack_current_folder}/include
  )

endfunction()

# -----------------------------------------------------------------------------

function(target_compile_definitions_micro_os_plus_startup target)

  # None

endfunction()

# =============================================================================

function(add_libraries_micro_os_plus_startup)

  # ---------------------------------------------------------------------------

  find_package(micro-os-plus-version)
  find_package(micro-os-plus-diag-trace)

  # ---------------------------------------------------------------------------

  if (NOT TARGET micro-os-plus-startup-objects)

    add_library(micro-os-plus-startup-objects OBJECT EXCLUDE_FROM_ALL)

    target_sources_micro_os_plus_startup(micro-os-plus-startup-objects)
    target_include_directories_micro_os_plus_startup(micro-os-plus-startup-objects)
    target_compile_definitions_micro_os_plus_startup(micro-os-plus-startup-objects)

    add_library(micro-os-plus::startup ALIAS micro-os-plus-startup-objects)
    message(STATUS "micro-os-plus::startup")

    target_link_libraries(
      micro-os-plus-startup-objects

      PUBLIC
        micro-os-plus::version
        micro-os-plus::diag-trace
        micro-os-plus::architecture
    )

  endif()

  # ---------------------------------------------------------------------------

if(true)
  if (NOT TARGET micro-os-plus-startup-static)

    add_library(micro-os-plus-startup-static STATIC EXCLUDE_FROM_ALL)

    target_sources_micro_os_plus_startup(micro-os-plus-startup-static)
    target_include_directories_micro_os_plus_startup(micro-os-plus-startup-static)
    target_compile_definitions_micro_os_plus_startup(micro-os-plus-startup-static)

    add_library(micro-os-plus::startup-static ALIAS micro-os-plus-startup-static)

    target_link_libraries(
      micro-os-plus-startup-static
      
      PUBLIC
        micro-os-plus::version
        micro-os-plus::diag-trace-static
        micro-os-plus::architecture
    )

  endif()
endif()

  # ---------------------------------------------------------------------------

endfunction()

# -----------------------------------------------------------------------------
