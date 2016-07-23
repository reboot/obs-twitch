#pragma once

#include <QDialog>

#include "twitch.hpp"

#include "ui_authentication.h"

class Authentication : public QDialog {

    Q_OBJECT

public:
    Authentication(QWidget *parent);

    static Authentication *create(Twitch *twitch, QWidget *parent = 0);

public slots:
    virtual void urlChanged(const QUrl &) = 0;

};
