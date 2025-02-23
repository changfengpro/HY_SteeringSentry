# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_gamepad_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED gamepad_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(gamepad_FOUND FALSE)
  elseif(NOT gamepad_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(gamepad_FOUND FALSE)
  endif()
  return()
endif()
set(_gamepad_CONFIG_INCLUDED TRUE)

# output package information
if(NOT gamepad_FIND_QUIETLY)
  message(STATUS "Found gamepad: 0.0.0 (${gamepad_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'gamepad' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${gamepad_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(gamepad_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${gamepad_DIR}/${_extra}")
endforeach()
