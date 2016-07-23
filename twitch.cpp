#include <QMenuBar>
#include <QStatusBar>

#include <jansson.h>

#include <obs-module.h>
#include <obs-app.hpp>
#include <obs-frontend-api/obs-frontend-api.h>

#include "twitch.hpp"
#include "configuration.hpp"
#include "viewers.hpp"

void configurationTriggered(void *private_data) {
    Twitch *twitch = (Twitch *) private_data;

    (new Configuration(twitch, twitch->getMainWindow()))->show();
}

Twitch::Twitch() {
    api = new API();

    loadConfig();

    OBSMainWindow *mainWindow = reinterpret_cast<OBSMainWindow*>(getMainWindow());

    obs_frontend_add_tools_menu_item("Twitch", configurationTriggered, this);

    Viewers *viewers = new Viewers(this);
    QStatusBar *statusBar = mainWindow->statusBar();
    statusBar->addPermanentWidget(viewers);
}

Twitch::~Twitch() {
    delete api;

    obs_data_release(config);
}

QMainWindow *Twitch::getMainWindow() {
    OBSApp *app = App();
    return app->GetMainWindow();
}

void Twitch::setAccessToken(const char *accessToken) {
    obs_data_set_string(config, "accessToken", accessToken);

    api->setAccessToken(accessToken);
    api->updateUser();
}

const char *Twitch::getAccessToken() {
    if (!obs_data_has_user_value(config, "accessToken"))
        return NULL;
    return obs_data_get_string(config, "accessToken");
}

void Twitch::setChannel(const char *channel) {
    if (channel != NULL)
        obs_data_set_string(config, "channel", channel);
    else
        obs_data_erase(config, "channel");
}

const char *Twitch::getChannel() {
    if (!obs_data_has_user_value(config, "channel"))
        return NULL;
    return obs_data_get_string(config, "channel");
}

void Twitch::loadConfig() {
    char *config_dir = obs_module_config_path(NULL);
    if (config_dir == NULL) {
        blog(LOG_WARNING, "Could not load configuration file path");

        return;
    }
    os_mkdirs(config_dir);
    bfree(config_dir);

    char *configFile = obs_module_config_path("config.json");
    config = obs_data_create_from_json_file_safe(configFile, "bak");
    if (config == NULL) {
        blog(LOG_WARNING, "Could not load configuration file");

        config = obs_data_create();
        saveConfig();
    }
    bfree(configFile);

    api->setAccessToken(getAccessToken());
    api->updateUser();
}

void Twitch::saveConfig() {
    char *configFile = obs_module_config_path("config.json");
    obs_data_save_json_safe(config, configFile, "tmp", "bak");
    bfree(configFile);
}

API *Twitch::getAPI() {
    return api;
}
