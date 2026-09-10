cmake_minimum_required(VERSION 4.4)
project(can_reader CXX)

include(CTest)
include(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install prefix" FORCE)
endif()
set(${PROJECT_NAME}_CAN_IFACE "can1" CACHE STRING
        "CAN interface name for system daemon")
set(CAN_IFACE "${${PROJECT_NAME}_CAN_IFACE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD  17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


 # Include Conan toolchain if it exists
 if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
     include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
 endif()

 # Find packages
find_package(CLI11 REQUIRED)
find_package(spdlog REQUIRED)
add_subdirectory(src)

# --- install rules ---
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Locate systemd's unit dir (falls back to the conventional path)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_get_variable(SYSTEMD_UNIT_DIR systemd systemdsystemunitdir)
endif()
if(NOT SYSTEMD_UNIT_DIR)
    message(LOG, "PkgConfig not found use default one")
    set(SYSTEMD_UNIT_DIR "${CMAKE_INSTALL_FULL_LIBDIR}/systemd/system")
endif()
configure_file(systemd/${PROJECT_NAME}.service.in ${PROJECT_NAME}.service @ONLY)


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.service
        DESTINATION ${SYSTEMD_UNIT_DIR})
install(FILES systemd/90-${PROJECT_NAME}.preset
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/system-preset)
