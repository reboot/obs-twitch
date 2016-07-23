#include <jansson.h>

#include "configuration.hpp"
#include "authentication.hpp"

Configuration::Configuration(Twitch *twitch, QWidget *parent) : QDialog(parent) {
    this->twitch = twitch;

    ui = new Ui::Configuration();
    ui->setupUi(this);

    setUser();
    setChannel();

    connect(twitch->getAPI(), SIGNAL(userUpdated()), this, SLOT(userUpdated()));
}

Configuration::~Configuration() {
    delete ui;
}

void Configuration::setUser() {
    json_t *user = twitch->getAPI()->getUser();
    if (user == NULL) {
        ui->user->clear();
        return;
    }

    json_t *displayName = json_object_get(user, "display_name");
    if (displayName == NULL) {
        ui->user->clear();
        return;
    }

    ui->user->setText(json_string_value(displayName));
}

void Configuration::setChannel() {
    const char *channel = twitch->getChannel();
    if (channel == NULL) {
        ui->channel->clear();
        return;
    }

    ui->channel->setText(channel);
}

void Configuration::userUpdated() {
    setUser();
}

void Configuration::authenticate() {
    (Authentication::create(twitch, this))->show();
}

void Configuration::channelModified() {
    QString text = ui->channel->text();

    if (!text.isEmpty())
        twitch->setChannel(text.toUtf8().constData());
    else
        twitch->setChannel(NULL);
}

void Configuration::accept() {
    QDialog::accept();

    twitch->saveConfig();
}

void Configuration::reject() {
    QDialog::reject();

    twitch->loadConfig();
}
