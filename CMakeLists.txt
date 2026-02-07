cmake_minimum_required(VERSION 3.15)

project(ChaosMod CXX)

# Find latest version at https://github.com/OrfeasZ/ZHMModSDK/releases
# Set ZHMMODSDK_DIR variable to a local directory to use a local copy of the ZHMModSDK.
set(ZHMMODSDK_VER "v4.0.0-rc.3")
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
    src/BuildInfo.cpp
)

target_include_directories(ChaosMod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Effects
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Unlockers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Voting
)

# Set UTF-8 flag.
set_target_properties(ChaosMod PROPERTIES COMPILE_FLAGS "/utf-8")

# Add SDK and DX headers dependencies.
find_package(directx-headers CONFIG REQUIRED)
find_package(ixwebsocket CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(ChaosMod
    ZHMModSDK
    Microsoft::DirectX-Guids
    Microsoft::DirectX-Headers
    ixwebsocket::ixwebsocket
    nlohmann_json::nlohmann_json
    bcrypt
)

install(TARGETS ChaosMod
    RUNTIME DESTINATION bin
)

# Install the mod to the game folder when the `GAME_INSTALL_PATH` variable is set.
zhmmodsdk_install(ChaosMod)

# Write build info file before build.
set(BUILD_INFO_HEADER ${CMAKE_SOURCE_DIR}/src/BuildInfo.cpp)
add_custom_command(
    OUTPUT ${BUILD_INFO_HEADER}
    COMMAND ${CMAKE_COMMAND}
        -DOUTPUT_FILE=${BUILD_INFO_HEADER}
        -DZHMMODSDK_VER=${ZHMMODSDK_VER}
        -P ${CMAKE_SOURCE_DIR}/cmake/gen-build-info.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS
        ${CMAKE_SOURCE_DIR}/.git/HEAD
        ${CMAKE_SOURCE_DIR}/.git/index
    COMMENT "Generating Build Info."
)

add_custom_target(mod_build_info DEPENDS ${BUILD_INFO_HEADER})
add_dependencies(ChaosMod mod_build_info)

# Deploy SMF companion mod on build if SMF was found in the game folder.
if (EXISTS "${GAME_INSTALL_PATH}/SMF")
  add_custom_target(run_deploy ALL
    COMMAND "${GAME_INSTALL_PATH}/SMF/Deploy.exe"
    WORKING_DIRECTORY "${GAME_INSTALL_PATH}/SMF"
    COMMENT "Deploy SMF companion mod."
    VERBATIM
  )
else ()
  message(WARNING "SMF not found, companion mod will not be deployed automatically.")
endif ()

