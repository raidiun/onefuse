cmake_minimum_required(VERSION 3.12)

project(onefuse)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

message("${CMAKE_MODULE_PATH}")

find_package(FUSE REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")

include_directories("${FUSE_INCLUDE_DIR}")

file(GLOB onefuse_support_SRCS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/LibOneFuse/*.cs"
    )

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/libonefuse.a"
    COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}/src/LibOneFuse"
            && dotnet publish /p:NativeLib=Static /p:SelfContained=true -r linux-x64 -c release
            && cp "${CMAKE_CURRENT_SOURCE_DIR}/src/LibOneFuse/bin/release/net5.0/linux-x64/publish/LibOneFuse.a" "${CMAKE_CURRENT_BINARY_DIR}/libonefuse.a"
    DEPENDS ${onefuse_support_SRCS}
    )
add_custom_target(onefuse_support_target ALL
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/libonefuse.a")
add_library(onefuse_support STATIC IMPORTED)
add_dependencies(onefuse_support onefuse_support_target)
set_target_properties(onefuse_support
    PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/libonefuse.a"
    )

add_executable(onefuse src/main.cpp)
target_link_libraries(onefuse ${FUSE_LIBRARIES} onefuse_support)
