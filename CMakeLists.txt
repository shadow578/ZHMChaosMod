cmake_minimum_required(VERSION 3.15)

project(ChaosMod CXX)

# Find latest version at https://github.com/OrfeasZ/ZHMModSDK/releases
# Set ZHMMODSDK_DIR variable to a local directory to use a local copy of the ZHMModSDK.
set(ZHMMODSDK_VER "v4.0.0-rc.2")
include(cmake/setup-zhmmodsdk.cmake)

# Set C++ standard to C++23.
set(CMAKE_CXX_STANDARD 23)

# Create the ChaosMod mod library.
file(GLOB_RECURSE SRC_FILES
        CONFIGURE_DEPENDS
        src/*.cpp
        src/*.c
        src/*.hpp
        src/*.h
)

add_library(ChaosMod SHARED
    ${SRC_FILES}
)

target_include_directories(ChaosMod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set UTF-8 flag.
set_target_properties(ChaosMod PROPERTIES COMPILE_FLAGS "/utf-8")

# Add SDK and DX headers dependencies.
find_package(directx-headers CONFIG REQUIRED)

target_link_libraries(ChaosMod
    ZHMModSDK
    Microsoft::DirectX-Guids
    Microsoft::DirectX-Headers
)

install(TARGETS ChaosMod
    RUNTIME DESTINATION bin
)

# Install the mod to the game folder when the `GAME_INSTALL_PATH` variable is set.
zhmmodsdk_install(ChaosMod)
