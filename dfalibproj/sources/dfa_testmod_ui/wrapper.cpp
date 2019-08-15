#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent)
{
    m_result_model =  new ResultModel(this);
    runner  =  new Runner();
    runner->moveToThread(&runnerThread);
    connect(&runnerThread, &QThread::finished, runner, &QObject::deleteLater);
    connect(this, &Wrapper::startRunner, runner, &Runner::doDFA );
    connect(runner,&Runner::sendText, this, &Wrapper::receiveText);
    connect(runner,&Runner::sendProgress, this, &Wrapper::receiveProgress);
    connect(runner,&Runner::sendResults, this, &Wrapper::receiveResults);
    runnerThread.start();
}

void Wrapper::receiveText(QString text)
{
    emit textReceived(text);
}

void Wrapper::receiveProgress(double val, double limit)
{
    double progress = val / limit*100;
    emit updateProgress(progress);
}

void Wrapper::receiveResults(QString results){
    QStringList result_list =  results.split('\n');
    for(QString result_str : result_list){
        if(!result_str.isEmpty()){
            Result r;
            r.str = result_str.split(':')[0];
            r.gqd = result_str.split(':')[1].split(',')[0].toInt();
            r.imt = result_str.split(':')[1].split(',')[1].toInt();
            r.hrp = result_str.split(':')[1].split(',')[2].toInt();
            r.trp = result_str.split(':')[1].split(',')[3].toInt();
            m_result_model->addResult(r);
        }
    }


}

void Wrapper::doDFA(bool gqd, bool imt, bool trp, bool hrp, int min_len)
{
    m_result_model->clear();
    emit startRunner(gqd, imt, trp, hrp, min_len);
}

ResultModel *Wrapper::getModel()
{
    return m_result_model;
}
