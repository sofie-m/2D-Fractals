# Install script for directory: C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/453-skeleton")
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
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/Debug/fmtd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/Release/fmt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/MinSizeRel/fmt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/RelWithDebInfo/fmt.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fmt" TYPE FILE FILES
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/args.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/base.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/chrono.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/color.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/compile.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/core.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/format.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/format-inl.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/os.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/ostream.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/printf.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/ranges.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/std.h"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/thirdparty/fmt-11.0.2/include/fmt/xchar.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/fmt-config.cmake"
    "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/fmt-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake"
         "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/CMakeFiles/Export/b834597d9b1628ff12ae4314c3a2e4b8/fmt-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/fmt.pc")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/sofie/cppProjects/CPSC-453-Assignment-1-Boilerplate/build/thirdparty/fmt-11.0.2/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
