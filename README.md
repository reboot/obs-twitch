Twitch Dashboard Integration for OBS Studio
===========================================

The purpose of this plugin is to integrate some functionality of the
Twitch dashboard into OBS Studio.

Current Features
----------------

* Show current viewer count in the OBS status bar

Planned Features
----------------

- Configurable channel name and API key
- Show and edit title of broadcast
- Show and edit game

Build
-----

To build the plugin you have to integrate it into the build of OBS
Studio. Check out the repository into a subdirectory of the plugins
folder in the OBS Studio source and add it to the ` CMakeLists.txt` .
Before you start the build edit the `config.h` and change the Twitch
channel to your own.
