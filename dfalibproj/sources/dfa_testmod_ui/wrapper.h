#ifndef WRAPPER_H
#define WRAPPER_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include "runner.h"
#include "resultmodel.h"
#include "authenticationmanager.h"


class Wrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ResultModel* m_result_model READ getModel NOTIFY modelChanged )
    Q_PROPERTY(ResultModel* m_result_model_calc READ getModelCalc NOTIFY modelCalcChanged )
public:
    explicit Wrapper(
            QSettings *settings,
            QObject *parent = nullptr);

signals:
    void textReceived(QString text);
    void updateProgress(double val);
    void clearUI();
    void textReceivedtoCalc(QString text);
    void updateCalcProgress(double val);
    void clearCalcUI();
    void modelChanged();
    void modelCalcChanged();

public slots:
    void receiveText(QString text);
    void receiveProgress(double val, double limit);
    void receiveResults(QString results);
    void doDFA(bool gqd=0, bool imt=0, bool trp=0, bool hrp=0,int length=0);
    void stopDFA();
    void stopParse();
    void doParseString(QString strings);
    ResultModel *getModel();
    ResultModel *getModelCalc();
    void saveToCSV(int type);

private:
    QSettings *m_settings;
    Runner *m_runner;
    ResultModel* m_result_model;
    Runner* m_runner_calc;
    ResultModel* m_result_model_calc;
};

#endif // WRAPPER_H
