#include <QNetworkReply>
#include <QSignalMapper>

#include <jansson.h>

#include <util/bmem.h>

#include "api.hpp"

API::API() {
    this->networkAccessManager = new QNetworkAccessManager();

    this->accessToken = NULL;

    for (auto i : streams)
        json_decref(i.second);
    streams.clear();

    this->user = NULL;
}

API::~API() {
    if (user)
        json_decref(user);

    if (accessToken)
        bfree(accessToken);

    delete networkAccessManager;
}

void API::setAccessToken(const char *accessToken) {
    this->accessToken = bstrdup(accessToken);
}

json_t *API::getUser() {
    return user;
}

void API::updateUser() {
    if (accessToken == NULL)
        return;

    QByteArray authorization;
    authorization = "OAuth ";
    authorization.append(accessToken);

    QNetworkRequest networkRequest(QUrl("https://api.twitch.tv/kraken/user"));
    networkRequest.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    networkRequest.setRawHeader("Authorization", authorization);

    QNetworkReply *networkReply = networkAccessManager->get(networkRequest);
    connect(networkReply, SIGNAL(finished()), this, SLOT(finishedUserRequest()));

}

void API::finishedUserRequest() {
    QNetworkReply *networkReply = reinterpret_cast<QNetworkReply *>(QObject::sender());

    if (networkReply->error()) {
        blog(LOG_WARNING, "Twitch API failed");
        return;
    }

    QByteArray data = networkReply->readAll();
    blog(LOG_DEBUG, "Twitch API response: %s", data.data());

    if (user != NULL) {
        json_decref(user);
        user = NULL;
    }

    json_t *root;
    json_error_t json_error;

    root = json_loads(data.data(), 0, &json_error);
    if(!root)
    {
        blog(LOG_ERROR, "error: on line %d: %s\n", json_error.line, json_error.text);
        return;
    }

    user = root;
    emit userUpdated();
}

json_t *API::getStreams(const char *channel) {
    auto i = streams.find(channel);
    if (i == streams.end())
        return NULL;
    return i->second;
}

void API::updateStreams(const char *channel) {
    if (accessToken == NULL)
        return;

    QString url;
    url = "https://api.twitch.tv/kraken/streams/";
    url += QString(channel).toLower();
    url += "/";

    QByteArray authorization;
    authorization = "OAuth ";
    authorization.append(accessToken);

    QNetworkRequest networkRequest(url);
    networkRequest.setRawHeader("Accept", "application/vnd.twitchtv.v3+json");
    networkRequest.setRawHeader("Authorization", authorization);

    QNetworkReply *networkReply = networkAccessManager->get(networkRequest);

    QSignalMapper *signalMapper = new QSignalMapper();
    signalMapper->setMapping(networkReply, channel);

    connect(networkReply, SIGNAL(finished()), signalMapper, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(finishedStreamsRequest(const QString &)));
}

void API::finishedStreamsRequest(const QString _channel) {
    QSignalMapper *signalMapper = reinterpret_cast<QSignalMapper *>(QObject::sender());
    QNetworkReply *networkReply = reinterpret_cast<QNetworkReply *>(signalMapper->mapping(_channel));
    char *channel = bstrdup(_channel.toUtf8().constData());

    json_t *s = getStreams(channel);
    if (s != NULL)
        json_decref(s);

    try {
        if (networkReply->error()) {
            blog(LOG_WARNING, "Twitch API failed");
            throw 0;
        }

        QByteArray data = networkReply->readAll();
        blog(LOG_DEBUG, "Twitch API response: %s", data.data());

        json_t *root;
        json_error_t json_error;

        root = json_loads(data.data(), 0, &json_error);
        if(!root)
        {
            blog(LOG_ERROR, "error: on line %d: %s\n", json_error.line, json_error.text);
            throw 0;
        }

        streams[channel] = root;
    } catch (...) {
        streams.erase(channel);
    }

    emit streamsUpdated(channel);

    bfree(channel);
}
