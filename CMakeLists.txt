cmake_minimum_required(VERSION 2.8.12)

project(obs-twitch)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

set(OBS_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/libobs"
    "${CMAKE_SOURCE_DIR}/UI"
    "${CMAKE_SOURCE_DIR}/UI/obs-frontend-api"
    ${OBS_JANSSON_INCLUDE_DIRS}
)
set(OBS_LIBRARY
    libobs
    )
set(OBS_FRONTEND_API_LIBRARY
    obs-frontend-api
    )

include_directories(
    ${OBS_INCLUDE_DIRS}
    )

set(obs-twitch_SOURCES
    module.cpp
    twitch.cpp
    api.cpp
    configuration.cpp
    authentication.cpp
    viewers.cpp
)
set(obs-twitch_HEADERS
    twitch.hpp
    api.hpp
    configuration.hpp
    authentication.hpp
    viewers.hpp
)
qt5_wrap_ui(obs-twitch_UI_HEADERS
    configuration.ui
    authentication.ui
)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config.h")

add_library(obs-twitch MODULE
    ${obs-twitch_SOURCES}
    ${obs-twitch_HEADERS}
    ${obs-twitch_UI_HEADERS}
)
target_link_libraries(obs-twitch
    ${OBS_LIBRARY}
    ${OBS_FRONTEND_API_LIBRARY}
    Qt5::Widgets
    Qt5::WebEngineWidgets
    ${OBS_JANSSON_IMPORT}
)

install_obs_plugin_with_data(obs-twitch data)
