
#-------------------------------------------------------------------------------
# Checks
#-------------------------------------------------------------------------------

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR OR EXISTS "${CMAKE_BINARY_DIR}/CMakeLists.txt")
	message(SEND_ERROR "In-source builds are not allowed. Please create a separate build directory and run 'cmake /path/to/folder [options]' there.")
	message(FATAL_ERROR "You can remove the file \"CMakeCache.txt\" and directory \"CMakeFiles\" in ${CMAKE_SOURCE_DIR}.")
endif()

#-------------------------------------------------------------------------------
# CMake Policies
#-------------------------------------------------------------------------------

if(POLICY CMP0054)
	cmake_policy(SET CMP0054 NEW)
endif()

#-------------------------------------------------------------------------------
# Platform Detection
#-------------------------------------------------------------------------------

if(APPLE)
    set(MAC TRUE)
elseif(WIN32)
	set(WIN TRUE)
elseif(UNIX)
	set(LINUX TRUE)
endif()

#-------------------------------------------------------------------------------
# Global Settings
#-------------------------------------------------------------------------------

if(WIN)
    option(SMTG_USE_STATIC_CRT "Use static crt on Windows" OFF)
endif()

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")

if(LINUX)
	set(common_linker_flags "-Wl,--no-undefined")
	set(CMAKE_MODULE_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Module Library Linker Flags")
	set(CMAKE_SHARED_LINKER_FLAGS "${common_linker_flags}" CACHE STRING "Shared Library Linker Flags")
endif()

# Output directories
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(MAC)
    # macOS defaults
    if(NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
    	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8" CACHE STRING "macOS deployment target")
    endif()
    if(NOT DEFINED ENV{SDKROOT})
		execute_process(COMMAND xcrun --sdk macosx --show-sdk-path OUTPUT_VARIABLE CMAKE_OSX_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
    option(SMTG_BUILD_UNIVERSAL_BINARY "Build universal binary (32 & 64 bit)" OFF)
    if(SMTG_BUILD_UNIVERSAL_BINARY)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;i386" CACHE STRING "macOS universal binary")
		set(CMAKE_XCODE_ATTRIBUTE_ARCHS "$(ARCHS_STANDARD_32_64_BIT)")
        set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "$<$<CONFIG:Debug>:YES>$<$<CONFIG:Release>:NO>")
    endif()
endif()

# Generation Settings
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "Generate compile commands" FORCE)
set(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES ON)
set(CMAKE_SKIP_INSTALL_RULES ON)
set(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER Predefined)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_compile_options($<$<CONFIG:Debug>:-DDEVELOPMENT=1>)
add_compile_options($<$<CONFIG:Release>:-DRELEASE=1>)

if(WIN)
	add_compile_options(/MP)
	add_compile_options($<$<CONFIG:Debug>:/ZI>)
	if(SMTG_USE_STATIC_CRT)
		# /MTd = MultiThreaded Debug Runtime
		# /MT  = MultiThreaded Runtime
		add_compile_options($<$<CONFIG:Debug>:/MTd>)
		add_compile_options($<$<CONFIG:Release>:/MT>)
	else()
		# /MDd = MultiThreadedDLL Debug Runtime
		# /MD  = MultiThreadedDLL Runtime
		add_compile_options($<$<CONFIG:Debug>:/MDd>)
		add_compile_options($<$<CONFIG:Release>:/MD>)
	endif()
endif()

# Add colors to clang output when using Ninja
# See: https://github.com/ninja-build/ninja/wiki/FAQ
if (UNIX AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_GENERATOR STREQUAL "Ninja")
	add_compile_options(-fcolor-diagnostics)
endif()
