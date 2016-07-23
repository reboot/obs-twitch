#pragma once

#include <map>

#include <QObject>
#include <QNetworkAccessManager>

struct json_t;

class API : public QObject {

    Q_OBJECT

public:
    API();

    ~API();

    void setAccessToken(const char *accessToken);

    json_t *getUser();

    void updateUser();

    json_t *getStreams(const char *channel);

    void updateStreams(const char *channel);

signals:
    void userUpdated();

    void streamsUpdated(const char *channel);

private slots:
    void finishedUserRequest();

    void finishedStreamsRequest(QString channel);

private:
    char *accessToken;

    QNetworkAccessManager *networkAccessManager;

    json_t *user;

    std::map<std::string, json_t *> streams;

};
