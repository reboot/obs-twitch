#pragma once

#include <QLabel>

#include <util/config-file.h>

#include "twitch.hpp"

class Viewers : public QLabel {

    Q_OBJECT

public:
    Viewers(Twitch *twitch);

    void setViewers(int viewers);

private:
    Twitch *twitch;

    config_t *config;

    void scheduleStreamsRequest();

private slots:
    void startStreamsRequest();

    void streamsUpdated(const char *channel);

};
