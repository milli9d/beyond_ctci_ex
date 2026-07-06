cmake_minimum_required(VERSION 3.28)

project(STL_Implementation
    VERSION 1.0.0
    DESCRIPTION "Modern C++26 STL Implementation"
    LANGUAGES C CXX
)

# Force GCC 14 for C++26 support
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
    find_program(GCC14_CXX g++-14)
    find_program(GCC14_C gcc-14)

    if(GCC14_CXX AND GCC14_C)
        set(CMAKE_CXX_COMPILER ${GCC14_CXX})
        set(CMAKE_C_COMPILER ${GCC14_C})
        message(STATUS "Switched to GCC 14 for C++26 support")
    endif()
endif()

# Standards and build type
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Build speed optimizations
set(CMAKE_CXX_COMPILER_LAUNCHER ccache) # Use ccache if available
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For better IDE support
set(CMAKE_COLOR_DIAGNOSTICS ON) # Faster colored output

# Output directories - organize builds into bin/ and lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For multi-config generators (Visual Studio, Xcode)
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# C++26 GNU EXTENSIONS + PEDANTIC CHECKS - Optimized for safety and speed
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -std=gnu++2c # C++26 with GNU extensions!
        -Wall # Essential warnings
        -Wextra # Extra warnings
        -Wpedantic # ISO compliance warnings
        -Werror # Treat warnings as errors
        -Weffc++ # Scott Meyers' Effective C++ warnings

        # Build speed optimizations:
        -fdiagnostics-color=always # Faster terminal output
        -fno-show-column # Reduce diagnostic overhead
    )

    # Debug flags - maximize debuggability over build/run speed
    add_compile_options($<$<CONFIG:Debug>:-g3>) # Full debug info, incl. macro definitions
    add_compile_options($<$<CONFIG:Debug>:-O0>) # No optimization - variables/steps match source exactly
    add_compile_options($<$<CONFIG:Debug>:-fno-inline>) # Keep real call frames for backtraces
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>) # Keep sanitizers!
    add_compile_options($<$<CONFIG:Debug>:-fno-sanitize-recover=address,undefined>) # Abort at first error, not later
    add_compile_options($<$<CONFIG:Debug>:-fno-omit-frame-pointer>)
    add_compile_options($<$<CONFIG:Debug>:-D_GLIBCXX_DEBUG>) # STL debug mode

    # Release flags
    add_compile_options($<$<CONFIG:Release>:-O3>)
    add_compile_options($<$<CONFIG:Release>:-DNDEBUG>)
    add_compile_options($<$<CONFIG:Release>:-flto>)
    add_compile_options($<$<CONFIG:Release>:-march=native>)

    add_link_options(
        $<$<CONFIG:Debug>:-fsanitize=address,undefined> # Keep sanitizers!
        $<$<CONFIG:Release>:-flto>
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX /permissive-)
endif()

# Auto-discover subdirectories
function(add_subdirs_recursive dir)
    file(GLOB children RELATIVE ${dir} ${dir}/*)

    foreach(child ${children})
        if(IS_DIRECTORY ${dir}/${child} AND EXISTS ${dir}/${child}/CMakeLists.txt)
            add_subdirectory(${child})
        endif()
    endforeach()
endfunction()

# Make root-level headers (e.g. common.hpp) visible to every target,
# including those in subdirectories added below.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ==============================================================================
# PROFILING MACROS - Clean, reusable profiling system
# ==============================================================================
include(cmake/Profiling.cmake)

# Auto-add each C/C++ file in the root directory as its own executable
file(GLOB ROOT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

foreach(SOURCE ${ROOT_SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE})
endforeach()

# Now process subdirectories (they can use the profiling macros)
add_subdirs_recursive(${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

# Individual testbenches will use add_full_profiling(target_name) in their own CMakeLists.txt

# Convenience target to profile all testbenches
add_custom_target(flame_all
    DEPENDS flame_logging_tb flame_forward_list_tb
    COMMENT "Profile all testbenches and generate flamegraphs"
) # Simple summary
message(STATUS "=== STL Implementation: ${CMAKE_BUILD_TYPE} ===")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Standard: C++26 with GNU extensions (-std=gnu++2c)")
message(STATUS "Warnings: PEDANTIC (-Wall -Wextra -Wpedantic -Werror -Weffc++)")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug: Optimized (-g2 -Og)")
    message(STATUS "Sanitizers: AddressSanitizer, UBSanitizer")
    message(STATUS "STL Debug: _GLIBCXX_DEBUG enabled")
endif()

message(STATUS "========================================")