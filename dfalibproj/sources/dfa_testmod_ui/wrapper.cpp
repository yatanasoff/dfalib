#include "wrapper.h"

Wrapper::Wrapper(QSettings *settings, QObject *parent) : QObject(parent)
{
    m_settings = settings;
    m_result_model =  new ResultModel(this);
    m_result_model_calc = new ResultModel(this);

    m_runner  =  new Runner(Runner::RUNNER_TYPE_OPTIONSVIEW);
    m_runner_calc = new Runner(Runner::RUNNER_TYPE_CALCSVIEW);

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

