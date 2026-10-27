# Install script for directory: C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/src/v3.2.2-facb5187b6.clean/config

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/pkgs/imath_x64-windows/debug")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Imath" TYPE FILE FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/ImathConfig.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/Imath.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath" TYPE FILE FILES
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/ImathConfig.cmake"
    "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/ImathConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath/ImathTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath/ImathTargets.cmake"
         "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/CMakeFiles/Export/337404c792ddf4581238d79dc7aa1ce1/ImathTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath/ImathTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath/ImathTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath" TYPE FILE FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/CMakeFiles/Export/337404c792ddf4581238d79dc7aa1ce1/ImathTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Imath" TYPE FILE FILES "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/CMakeFiles/Export/337404c792ddf4581238d79dc7aa1ce1/ImathTargets-debug.cmake")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/marcpp12/Documents/GitHub/Motor-Uni/build/vcpkg_installed/vcpkg/blds/imath/x64-windows-dbg/config/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
