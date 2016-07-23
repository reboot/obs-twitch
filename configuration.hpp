#pragma once

#include <QDialog>

#include "twitch.hpp"

#include "ui_configuration.h"

class Configuration : public QDialog {

    Q_OBJECT

public:
    Configuration(Twitch *twitch, QWidget *parent = 0);

    ~Configuration();

private slots:
    void userUpdated();

    void authenticate();

    void channelModified();

    void accept();

    void reject();

private:
    Twitch *twitch;

    Ui::Configuration *ui;

    void setUser();

    void setChannel();

};
