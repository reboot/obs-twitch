#include <QWebEngineView>
#include <QUrlQuery>

#include <qt-wrappers.hpp>

#include "config.h"
#include "authentication.hpp"

namespace Implementation {

    class Authentication : public ::Authentication {

    public:
        Authentication();

    public:
        Authentication(Twitch *twitch, QWidget *parent = 0);

        ~Authentication();

    private:
        Twitch *twitch;

        Ui::Authentication *ui;

        void urlChanged(const QUrl &url);

    };

    Authentication::Authentication(Twitch *twitch, QWidget *parent) : ::Authentication(parent) {
        this->twitch = twitch;

        ui = new Ui::Authentication();
        ui->setupUi(this);

        QUrlQuery urlQuery;
        urlQuery.addQueryItem("response_type", "token");
        urlQuery.addQueryItem("client_id", TWITCH_CLIENT_ID);
        urlQuery.addQueryItem("redirect_uri", "https://api.twitch.tv/kraken/");
        urlQuery.addQueryItem("scope", "user_read");
        urlQuery.addQueryItem("force_verify", "true");

        QUrl url("https://api.twitch.tv/kraken/oauth2/authorize");
        url.setQuery(urlQuery);

        ui->webView->setUrl(url);
    }

    Authentication::~Authentication() {
        delete ui;
    }

    void Authentication::urlChanged(const QUrl &url) {
        QUrlQuery fragment(url.fragment());

        QString accessToken = fragment.queryItemValue("access_token");
        if (accessToken.isEmpty())
            return;

        twitch->setAccessToken(QT_TO_UTF8(accessToken));

        destroy(true, true);
    }

}

Authentication::Authentication(QWidget *parent) : QDialog(parent) {
}

Authentication *Authentication::create(Twitch *twitch, QWidget *parent) {
    return new Implementation::Authentication(twitch, parent);
}
