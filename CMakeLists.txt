cmake_minimum_required (VERSION 3.13)

project (cli_tools VERSION 1.0.1 DESCRIPTION "cli_tools library")
include (GNUInstallDirs)
add_library (cli_tools SHARED src/CommandLine.cpp)
set_target_properties(cli_tools PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER src/CommandLine.hpp)

configure_file (cli_tools.pc.in cli_tools.pc @ONLY)
target_include_directories (cli_tools PRIVATE .)
install(TARGETS cli_tools
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/cli_tools.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
