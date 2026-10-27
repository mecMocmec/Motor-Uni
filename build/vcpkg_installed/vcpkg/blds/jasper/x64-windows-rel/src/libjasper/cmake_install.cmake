# Install script for directory: C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/pkgs/jasper_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/x64-windows-rel/src/libjasper/jasper.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/x64-windows-rel/src/libjasper/jasper.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jasper" TYPE FILE FILES
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_cm.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_compiler.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/x64-windows-rel/src/libjasper/include/jasper/jas_config.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_debug.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_dll.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_fix.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_getopt.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_icc.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_image.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_init.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_log.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_malloc.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_math.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jasper.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_seq.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_stream.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_string.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_thread.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_tmr.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_tvp.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_types.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/src/sion-4.2.9-f66eb5598c.clean/src/libjasper/include/jasper/jas_version.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/x64-windows-rel/src/libjasper/include/jasper/jas_export_cmake.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/jasper/x64-windows-rel/src/libjasper/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
