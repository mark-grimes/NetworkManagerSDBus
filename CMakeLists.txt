#
# CMake file to build LibNM.
# Mark Grimes (mark.grimes@rymapt.com)
# 06/Oct/2017
# copyright 2018 Rymapt Ltd
# Licence to be decided
#

project( LibNM )

cmake_minimum_required(VERSION 2.8)

if( NOT MSVC ) # Microsoft Visual Studio is C++11 by default and doesn't recognise this flag
	add_definitions( "-std=c++17" )
endif()


# Create the file that has the version information and git hash
execute_process( COMMAND git describe --dirty --always --tags WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE LibNM_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND git log -1 --pretty=format:%H WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE LibNM_GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE )
add_custom_target( ${PROJECT_NAME}_CreateVersionFile ALL ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" ) # Make sure the git hash is always checked (not just at configure time)
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${PROJECT_BINARY_DIR}/src/version.cpp" )


include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
if( NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
	set( ${PROJECT_NAME}_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE )
endif()
aux_source_directory( "${CMAKE_CURRENT_SOURCE_DIR}/src" library_sources )

option( BUILD_WITH_ASIO "Build the ASIO event loop" ON )
message( STATUS "BUILD_WITH_ASIO: ${BUILD_WITH_ASIO}" )
if( BUILD_WITH_ASIO )
	add_definitions( "-DLIBNM_ASIO_AVAILABLE" )
	aux_source_directory( "${CMAKE_CURRENT_SOURCE_DIR}/src/asio" library_sources )
endif()

add_library( ${PROJECT_NAME} STATIC ${library_sources} "${PROJECT_BINARY_DIR}/src/version.cpp" )
# Find the sd-bus library
find_package( PkgConfig REQUIRED )
pkg_check_modules( SDBus REQUIRED libsystemd )
target_link_libraries( ${PROJECT_NAME} ${SDBus_LIBRARIES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${SDBus_INCLUDE_DIRS} )

## Uncomment this if you get "undefined reference to `pthread_create'" when using std::thread
#if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
#	target_link_libraries( ${PROJECT_NAME} "pthread" )
#endif()

install( TARGETS ${PROJECT_NAME} DESTINATION lib )
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include )

option( BUILD_UNITTESTS "Build unit tests" ON )
message( STATUS "BUILD_UNITTESTS: ${BUILD_UNITTESTS}" )
if( BUILD_UNITTESTS )
	# Fix the test configuration file to have the correct paths
	configure_file( "${PROJECT_SOURCE_DIR}/tests/src/testinputs.cpp.in" "${PROJECT_BINARY_DIR}/tests/src/testinputs.cpp" @ONLY )
	include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/tests/include" )
	aux_source_directory( "${CMAKE_CURRENT_SOURCE_DIR}/tests/src" unittests_sources )
	if( NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
		list( REMOVE_ITEM unittests_sources "${CMAKE_CURRENT_SOURCE_DIR}/tests/src/main.cpp" )
		set( ${PROJECT_NAME}_TESTS_SOURCE ${unittests_sources} PARENT_SCOPE )
	else()
		add_executable( ${PROJECT_NAME}Tests ${unittests_sources} "${PROJECT_BINARY_DIR}/tests/src/testinputs.cpp" )
		target_link_libraries( ${PROJECT_NAME}Tests ${PROJECT_NAME} )
	endif()
endif()
