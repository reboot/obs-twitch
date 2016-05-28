#include <QLabel>

class Viewers : public QLabel {

    Q_OBJECT

public:
    Viewers();

    void setViewers(int viewers);

public slots:
    void startAPIRequest();

private:
    void finishedAPIRequest(const QString &text, const QString &error);
    void scheduleAPIRequest();

};
