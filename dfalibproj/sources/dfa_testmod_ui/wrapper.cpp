#include "wrapper.h"

Wrapper::Wrapper(QSettings *settings,QObject *parent) : QObject(parent)
{
    m_settings = settings;
    m_result_model =  new ResultModel(this);
    m_result_model_calc = new ResultModel(this);

    m_runner  =  new Runner(Runner::RUNNER_TYPE_OPTIONSVIEW);
    m_runner_calc = new Runner(Runner::RUNNER_TYPE_CALCSVIEW);

    m_manager = new QNetworkAccessManager();
    m_auth_server = "0.0.0.0";
    m_auth_server_port = 9598;

    connect(m_runner,&Runner::sendText, this, &Wrapper::receiveText);
    connect(m_runner,&Runner::sendProgress, this, &Wrapper::receiveProgress);
    connect(m_runner,&Runner::sendResults, this, &Wrapper::receiveResults);

    connect(m_runner_calc,&Runner::sendText, this, &Wrapper::receiveText);
    connect(m_runner_calc,&Runner::sendProgress, this, &Wrapper::receiveProgress);
    connect(m_runner_calc,&Runner::sendResults, this, &Wrapper::receiveResults);
}

void Wrapper::receiveText(QString text)
{
    Runner* sender = dynamic_cast<Runner*>(QObject::sender());
    if (sender->getType()==Runner::RUNNER_TYPE_OPTIONSVIEW)
    {
        emit textReceived(text);
    }
    else if(sender->getType()==Runner::RUNNER_TYPE_CALCSVIEW)
    {
        emit textReceivedtoCalc(text);
    }

}

void Wrapper::receiveProgress(double val, double limit)
{
    double progress = val / limit*100;
    Runner* sender = dynamic_cast<Runner*>(QObject::sender());
    if (sender->getType()==Runner::RUNNER_TYPE_OPTIONSVIEW)
    {
        emit updateProgress(progress);
    }
    else if(sender->getType()==Runner::RUNNER_TYPE_CALCSVIEW)
    {
        emit updateCalcProgress(progress);
    }
}

void Wrapper::receiveResults(QString results){
    Runner* sender = dynamic_cast<Runner*>(QObject::sender());
    ResultModel* model;
    if (sender->getType()==Runner::RUNNER_TYPE_OPTIONSVIEW)
    {
        model = this->m_result_model;
    }
    else if(sender->getType()==Runner::RUNNER_TYPE_CALCSVIEW)
    {
        model = this->m_result_model_calc;
    }
    QStringList result_list =  results.split('\n');
    for(QString result_str : result_list){
        if(!result_str.isEmpty()){
            Result r;
            r.str = result_str.split(':')[0];
            r.gqd = result_str.split(':')[1].split(',')[0].toInt();
            r.imt = result_str.split(':')[1].split(',')[1].toInt();
            r.hrp = result_str.split(':')[1].split(',')[2].toInt();
            r.trp = result_str.split(':')[1].split(',')[3].toInt();
            model->addResult(r);
        }
    }
}

void Wrapper::doDFA(bool gqd, bool imt, bool trp, bool hrp, int length)
{
    m_result_model->clear();
    emit  clearUI();
    m_runner->doDFA(gqd, imt, trp, hrp, length);
}

void Wrapper::stopDFA()
{
    m_runner->stop();
}

void Wrapper::stopParse()
{
    m_runner_calc->stop();
}

void Wrapper::doParseString(QString strings)
{
    m_result_model_calc->clear();
    emit clearCalcUI();
    m_runner_calc->parseStrings(strings.split('\n'));
}

ResultModel *Wrapper::getModel()
{
    return m_result_model;
}

ResultModel *Wrapper::getModelCalc()
{
    return m_result_model_calc;
}

void Wrapper::saveToCSV(int type)
{
    QString filename =  QFileDialog::getSaveFileName(nullptr,"Save to CSV", QDir::homePath(),"CSV Files (*.csv)" );
    //    filename+=".csv";
    ResultModel *model;
    if(type==Runner::RUNNER_TYPE_OPTIONSVIEW){
        model = m_result_model;
    }else{
        model =  m_result_model_calc;
    }
    QFile f(filename);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream data(&f);
        QList<Result> results =  model->getData();
        data << "string, gqd, imt, hrp, trp" << endl;
        for(Result r : results){
            data << r.str << "," << r.gqd << "," << r.imt << "," << r.hrp << "," << r.trp << endl;
        }
    }
}

void Wrapper::doLogin(QString username, QString password)
{
    QString url = QString("http://%1:%2/login").arg(m_auth_server,QString::number(m_auth_server_port)),
            uuid = "",
            machineId = "";
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QJsonDocument  json;
    QJsonObject obj;
    obj["login"] = username;
    obj["password"] = password;
    obj["uuid"] = m_uuid;
    obj["machineid"] = QString::fromStdString(machineid::machineHash());
    json.setObject(obj);
    auto reply  = m_manager->post(req, json.toJson());
    connect(reply, &QNetworkReply::finished, this, &Wrapper::loginDataReceived);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Wrapper::handleError);

}

bool Wrapper::checkCredentials()
{
    QString authKey = m_settings->value("authKey").toString();
    return !authKey.isNull() && !authKey.isEmpty();
}

void Wrapper::startLogin()
{

    if(!m_settings->contains("uuid")){
        m_uuid =  QUuid::createUuid().toString().replace('{',"").replace('}',"");
        m_settings->setValue("uuid",m_uuid);
    }else{
        m_uuid  = m_settings->value("uuid").toString();
    }

    if(m_settings->contains("security_token")){
        m_security_token = m_settings->value("security_token").toString();
        if(verifySecurityToken(m_security_token)){
            emit validCredentials();
        }
    }else{
        emit loginNeeded();
    }


}

void Wrapper::loginDataReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    reply->deleteLater();
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray body = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(body);
        int userId = doc["userid"].toInt(-1);
        QString jwt = doc["jwt"].toString();
        if(verifySecurityToken(jwt)){
            qDebug()<<"ok";
            m_settings->setValue("security_token",jwt);
            emit validCredentials();
        }
    }
}

bool Wrapper::verifySecurityToken(QString token)
{
    auto decoded_token = jwt::decode(token.toStdString());
    //      debug data in jwt payload
          for(auto& e : decoded_token.get_payload_claims())
              std::cout << e.first << " = " << e.second.to_json() << std::endl;

    //    check if expired
//        if(decoded_token.has_expires_at()){
//            if(decoded_token.get_expires_at().time_since_epoch() <
//                    std::chrono::system_clock::now().time_since_epoch()){
    //            return false;
    //        }else{
    //            return true;
    //        }
//        }

    //  nothing to verify for the moment
    return true;
}

void Wrapper::handleError()
{
//    qDebug() << "error" << endl;
    emit notValidCredentials();
}


