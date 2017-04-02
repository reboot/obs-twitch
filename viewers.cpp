#include <QCoreApplication>
#include <QStatusBar>
#include <QLabel>
#include <QTimer>

#include <string.h>
#include <jansson.h>

#include "util/base.h"

#include "obs-app.hpp"
#include "remote-text.hpp"
#include "qt-wrappers.hpp"

#include "config.h"
#include "viewers.hpp"

Viewers::Viewers() : QLabel() {
    setAlignment(Qt::AlignRight);
    setAlignment(Qt::AlignVCenter);
    setIndent(20);

    setViewers(-1);

    scheduleAPIRequest();
}

void Viewers::scheduleAPIRequest() {
    QTimer::singleShot(60000, this, SLOT(startAPIRequest()));
}

void Viewers::startAPIRequest() {
    blog(LOG_DEBUG, "Starting Twitch API request");

    std::string url;
    url = "https://api.twitch.tv/kraken/streams?channel=";
    url += TWITCH_CHANNEL;

    RemoteTextThread *thread = new RemoteTextThread(url);
    connect(thread, &RemoteTextThread::Result, this, &Viewers::finishedAPIRequest);
    thread->start();
}

void Viewers::finishedAPIRequest(const QString &text, const QString &error) {
    if (text.isEmpty()) {
        blog(LOG_WARNING, "Twitch API failed: %s", QT_TO_UTF8(error));
        scheduleAPIRequest();
        return;
    }

    blog(LOG_DEBUG, "Twitch API response: %s", QT_TO_UTF8(text));

    json_t *root;
    json_error_t json_error;

    root = json_loads(QT_TO_UTF8(text), 0, &json_error);
    if(!root)
    {
        blog(LOG_ERROR, "error: on line %d: %s\n", json_error.line, json_error.text);
        scheduleAPIRequest();
        return;
    }

    json_t *value;
    int viewers = -1;

    value = json_object_get(root, "streams");
    if (value != NULL)
        value = json_array_get(value, 0);
    if (value != NULL)
        value = json_object_get(value, "viewers");
    if (value != NULL)
        viewers = json_integer_value(value);
    blog(LOG_INFO, "Viewers: %d", viewers);
    setViewers(viewers);

    json_decref(root);

    scheduleAPIRequest();

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

extern "C" void viewers_init(void)
{
    OBSApp *app = App();
    QMainWindow *win = app->GetMainWindow();
    QStatusBar *statusBar = win->statusBar();

    Viewers *viewers = new Viewers();
    statusBar->addPermanentWidget(viewers);
}
