cmake_minimum_required(VERSION 3.9)
project(Multihash VERSION 0.0.0
				  DESCRIPTION "C++ multihash library"
				  LANGUAGES CXX
)

# ————————————————————————————————————
# download and build test dependencies
# ————————————————————————————————————
configure_file(
	${CMAKE_HOME_DIRECTORY}/cmake/TestsDependencies.cmake.in
	googletest-download/CMakeLists.txt
)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if (result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

add_subdirectory(
	${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL
)

# —————————————————————————
# create target for library
# —————————————————————————
add_library(multihashlib src/multihash.cpp)

target_include_directories(multihashlib
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(multihashlib PRIVATE cxx_std_17)
target_compile_options(multihashlib PRIVATE -Werror)

# —————————————————————————————————————————
# create target for test and enable testing
# —————————————————————————————————————————
add_executable(multihashtest tests/multihash_test.cpp)

target_link_libraries(multihashtest multihashlib gtest_main)
target_compile_features(multihashtest PRIVATE cxx_std_17)
target_compile_options(multihashtest PRIVATE -Werror)

enable_testing()
add_test(multihashlib_test multihashtest)

# —————————————————————————
# installation instructions
# —————————————————————————
set_target_properties(multihashlib PROPERTIES EXPORT_NAME Multihash)
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Multihash)

install(TARGETS multihashlib
	EXPORT multihashlib_targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT multihashlib_targets
	FILE
		MultihashTargets.cmake
	NAMESPACE
		Multihash::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# ———————————————————————————————
# create file ConfigVersion.cmake
# ———————————————————————————————
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MultihashConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	${CMAKE_CURRENT_LIST_DIR}/cmake/MultihashConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MultihashConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# ————————————————————————————————————
# install the config and configversion
# ————————————————————————————————————
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/MultihashConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/MultihashConfigVersion.cmake
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# ——————————————————————————
# export from the build tree
# ——————————————————————————
export(
	EXPORT multihashlib_targets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/MultihashTargets.cmake
	NAMESPACE Multihash::
)

# ————————————————
# register package
# ————————————————
export(PACKAGE Multihash)

