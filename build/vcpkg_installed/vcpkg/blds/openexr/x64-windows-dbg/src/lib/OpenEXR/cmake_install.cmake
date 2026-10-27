# Install script for directory: C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/pkgs/openexr_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-dbg/src/lib/OpenEXR/OpenEXR-3_4_d.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-dbg/bin/OpenEXR-3_4_d.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfAcesFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfArray.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfBoxAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfBytesAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfCRgbaFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfChannelList.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfChannelListAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfChromaticities.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfChromaticitiesAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfCompositeDeepScanLine.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfCompression.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfCompressionAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfCompressor.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfContext.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfContextInit.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfConvert.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepCompositing.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepFrameBuffer.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepImageState.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepImageStateAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepScanLineInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepScanLineInputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepScanLineOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepScanLineOutputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepTiledInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepTiledInputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepTiledOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDeepTiledOutputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfDoubleAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfEnvmap.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfEnvmapAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfExport.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfFloatAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfFloatVectorAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfForward.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfFrameBuffer.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfFramesPerSecond.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfGenericInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfGenericOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfHeader.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfHTCompressor.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfHuf.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfIDManifest.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfIDManifestAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfIO.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfInputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfInt64.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfIntAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfKeyCode.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfKeyCodeAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfLineOrder.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfLineOrderAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfLut.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfMatrixAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfMisc.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfMultiPartInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfMultiPartOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfMultiView.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfName.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfNamespace.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfOpaqueAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfOutputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfPartHelper.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfPartType.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfPixelType.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfPreviewImage.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfPreviewImageAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfRational.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfRationalAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfRgba.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfRgbaFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfRgbaYca.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfStandardAttributes.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfStdIO.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfStringAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfStringVectorAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTestFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfThreading.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTileDescription.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTileDescriptionAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTiledInputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTiledInputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTiledOutputFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTiledOutputPart.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTiledRgbaFile.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTimeCode.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfTimeCodeAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfVecAttribute.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfVersion.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfWav.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/src/v3.4.15-df9bf5d7db.clean/src/lib/OpenEXR/ImfXdr.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/openexr/x64-windows-dbg/src/lib/OpenEXR/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
