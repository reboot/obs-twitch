#pragma once

#include <QObject>
#include <QMainWindow>
#include <QNetworkAccessManager>

#include <obs-data.h>

#include "api.hpp"

class Twitch : public QObject {

    Q_OBJECT

public:
    Twitch();

    ~Twitch();

    QMainWindow *getMainWindow();

    void setAccessToken(const char *accessToken);

    API *getAPI();

    const char *getAccessToken();

    void setChannel(const char *channel);

    const char *getChannel();

    void loadConfig();

    void saveConfig();

private:
    obs_data_t *config;

    API *api;

};
