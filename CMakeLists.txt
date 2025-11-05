cmake_minimum_required(VERSION 3.29)
project(bleacho VERSION 0.0.0 LANGUAGES C CXX)
string(TIMESTAMP CURRENT_DATETIME "%Y%m%d_%H%M%S" UTC)
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpmcache")
include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OPENSSL_ROOT_DIR "C:/msys64/mingw64") # Change this to your liking, for me it's just this for now
#set(OPENSSL_USE_STATIC_LIBS TRUE)

execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_VERSION "${CURRENT_DATETIME}_${GIT_BRANCH}_${GIT_COMMIT_HASH}_debug")
else()
    set(BUILD_VERSION "${CURRENT_DATETIME}_${GIT_BRANCH}_${GIT_COMMIT_HASH}")
endif()

find_package(OpenSSL REQUIRED)

CPMAddPackage(
        NAME cpp-httplib
        GITHUB_REPOSITORY yhirose/cpp-httplib
        VERSION 0.27.0
)

add_compile_definitions(BUILD_VERSION="${BUILD_VERSION}")
message(STATUS "Build Version: ${BUILD_VERSION}")

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h") # This is bad for a library, but this isn't a library

set (INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${_dir})
endforeach()

list (REMOVE_DUPLICATES INCLUDE_DIRS)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "${PROJECT_NAME} can only be built with GCC or MinGW.")
endif()

if(MINGW)
    # Use static runtime to ship the dll bullshit in the executable
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -static -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -static -static-libgcc -static-libstdc++")
endif()

add_executable(bleacho ${SOURCES}) # I don't know if I should also include ${HEADERS} because of the inline stuff I am going to do probably
target_link_libraries(bleacho PRIVATE
        httplib::httplib
        OpenSSL::SSL
        OpenSSL::Crypto
)
target_compile_definitions(bleacho PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Applying size optimizations for ${PROJECT_NAME}")
    target_compile_options(bleacho PRIVATE
            -O3
            # -O1 → basic optimization
            # -O2 → good general optimization
            # -O3 → aggressive optimization (may increase code size in some cases)
            # -Os → optimize for size
            -ffunction-sections
            -fdata-sections
    )
else()
    message(STATUS "Debug build: optimizations disabled")
endif()