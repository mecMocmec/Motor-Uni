# Install script for directory: C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/pkgs/openexr_x64-windows")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-rel/src/lib/OpenEXRUtil/OpenEXRUtil-3_4.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-rel/bin/OpenEXRUtil-3_4.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfCheckFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfDeepImage.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfDeepImageChannel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfDeepImageIO.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfDeepImageLevel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfFlatImage.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfFlatImageChannel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfFlatImageIO.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfFlatImageLevel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImage.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImageChannel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImageChannelRenaming.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImageDataWindow.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImageIO.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfImageLevel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfSampleCountChannel.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXRUtil/ImfUtilExport.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-rel/src/lib/OpenEXRUtil/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
