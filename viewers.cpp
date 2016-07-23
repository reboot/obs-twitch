#include <QLabel>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <string.h>
#include <jansson.h>

#include "util/base.h"

#include "obs-app.hpp"
#include "remote-text.hpp"
#include "qt-wrappers.hpp"

#include "viewers.hpp"
#include "configuration.hpp"

Viewers::Viewers(Twitch *twitch) : QLabel() {
    this->twitch = twitch;

    setAlignment(Qt::AlignRight);
    setAlignment(Qt::AlignVCenter);
    setIndent(20);

    setViewers(-1);

    scheduleStreamsRequest();

    API *api = twitch->getAPI();
    connect(api, SIGNAL(streamsUpdated(const char *)), this, SLOT(streamsUpdated(const char *)));
}

void Viewers::scheduleStreamsRequest() {
    QTimer::singleShot(60000, this, SLOT(startStreamsRequest()));
}

void Viewers::startStreamsRequest() {
    blog(LOG_DEBUG, "Starting Twitch API request");

    const char *channel = twitch->getChannel();
    if (channel == NULL) {
        json_t *user = twitch->getAPI()->getUser();
        if (user == NULL) {
            scheduleStreamsRequest();
            return;
        }

        json_t *name = json_object_get(user, "name");
        if (name == NULL) {
            scheduleStreamsRequest();
            return;
        }

        channel = json_string_value(name);
    }

    API *api = twitch->getAPI();
    api->updateStreams(channel);
}

void Viewers::streamsUpdated(const char *channel) {
    int viewers = -1;

    API *api = twitch->getAPI();
    json_t *root = api->getStreams(channel);
    if(root)
    {
        json_t *value;

        value = json_object_get(root, "stream");
        if (value != NULL)
            value = json_object_get(value, "viewers");
        if (value != NULL)
            viewers = json_integer_value(value);
    }

    blog(LOG_INFO, "Viewers: %d", viewers);
    setViewers(viewers);

    scheduleStreamsRequest();

    return;
}

void Viewers::setViewers(int viewers) {
    QString text;

    text = "Viewers: ";
    if (viewers >= 0)
        text += QString::number(viewers);
    else
        text += "-";
    setText(text);
}
