#include "authenticationmanager.h"

AuthenticationManager::AuthenticationManager(QSettings *settimgs, QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager();
    m_auth_server = "dafnaapi.broutonlab.com";
    m_auth_server_port = 9598;
    m_settings =  settimgs;
}

void AuthenticationManager::doLogin(QString username, QString password)
{
    QString url = QString("http://%1:%2/login").arg(m_auth_server,QString::number(m_auth_server_port));
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonDocument  json;
    QJsonObject obj;
    obj["login"] = username;
    obj["password"] = password;
    obj["uuid"] = m_settings->value("uuid").toString();
    obj["machineid"] = QString::fromStdString(machineid::machineHash());
    json.setObject(obj);
    auto reply  = m_manager->post(req, json.toJson());
    connect(reply, &QNetworkReply::finished, this, &AuthenticationManager::loginDataReceived);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &AuthenticationManager::handleError);
}


void AuthenticationManager::startLogin()
{
    if(!m_settings->contains("uuid")){
        QString uuid =  QUuid::createUuid().toString().replace('{',"").replace('}',"");
        m_settings->setValue("uuid",uuid);
    }

    if(m_settings->contains("security_token")){
        if(validateSecurityToken()){
            emit validCredentials();
        }else{
            emit notValidCredentials(false);
        }
    }else{
        emit notValidCredentials(false);
    }

}

void AuthenticationManager::loginDataReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    reply->deleteLater();
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray body = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(body);
        int userId = doc["userid"].toInt(-1);
        QString jwt = doc["jwt"].toString();
        m_settings->setValue("security_token",jwt);
        m_settings->setValue("userid",userId);
        emit validCredentials();
    }else{
        emit notValidCredentials(true);
    }

}

bool AuthenticationManager::validateSecurityToken()
{
    QString url = QString("http://%1:%2/verify").arg(m_auth_server,QString::number(m_auth_server_port)),
            m_securityToken = m_settings->value("security_token").toString();
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    req.setRawHeader("Authorization", m_settings->value("security_token").toString().toUtf8());
    QJsonDocument  json;
    QJsonObject obj;
    obj["userid"] = m_settings->value("userid").toString();
    json.setObject(obj);
    auto reply  = m_manager->post(req, json.toJson());
    connect(reply, &QNetworkReply::finished, this, &AuthenticationManager::loginDataReceived);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &AuthenticationManager::handleError);
    return true;
}

void AuthenticationManager::handleError()
{
    emit notValidCredentials(true);
}
