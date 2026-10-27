# Install script for directory: C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/pkgs/imath_x64-windows")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-rel/src/Imath/Imath-3_2.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-rel/src/Imath/Imath-3_2.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Imath" TYPE FILE FILES
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/half.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/halfFunction.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/halfLimits.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathBox.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathBoxAlgo.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathColor.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathColorAlgo.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathEuler.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathExport.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathForward.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathFrame.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathFrustum.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathFrustumTest.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathFun.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathGL.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathGLU.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathInt64.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathInterval.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathLine.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathLineAlgo.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathMath.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathMatrix.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathMatrixAlgo.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathNamespace.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathPlane.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathPlatform.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathQuat.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathRandom.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathRoots.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathShear.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathSphere.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathTypeTraits.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathVec.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/src/Imath/ImathVecAlgo.h"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-rel/config/ImathConfig.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-rel/src/Imath/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
