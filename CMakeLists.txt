project(obs-twitch)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Widgets REQUIRED)

include_directories(SYSTEM
	"${CMAKE_SOURCE_DIR}/libobs"
	"${CMAKE_SOURCE_DIR}/obs"
)

set(obs-twitch_SOURCES
	twitch.c
        viewers.cpp
)
set(obs-twitch_HEADERS
	config.h
	viewers.hpp
)

add_library(obs-twitch MODULE
	${obs-twitch_SOURCES}
	${obs-twitch_HEADERS}
)
target_link_libraries(obs-twitch
	libobs
        Qt5::Widgets
)

install_obs_plugin_with_data(obs-twitch data)
