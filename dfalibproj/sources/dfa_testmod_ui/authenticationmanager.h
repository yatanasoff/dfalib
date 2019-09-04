#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>
#include <QUuid>
#include "machineid.h"
#include "jwt/jwt.h"

class AuthenticationManager: public QObject
{
    Q_OBJECT
public:
    explicit AuthenticationManager(QSettings *settimgs, QObject *parent = nullptr);
public slots:
    void doLogin(QString username, QString password);
    void startLogin();

private slots:
    bool validateSecurityToken();
    void loginDataReceived();
    void handleError();
signals:
    void notValidCredentials();
    void validCredentials();

private:
    QString m_auth_server;
    int m_auth_server_port;
    QNetworkAccessManager *m_manager;
    QSettings *m_settings;
};

#endif // AUTHENTICATIONMANAGER_H
