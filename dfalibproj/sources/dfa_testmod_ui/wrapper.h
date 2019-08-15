#ifndef WRAPPER_H
#define WRAPPER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "runner.h"
#include "resultmodel.h"

class Wrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ResultModel* m_result_model READ getModel NOTIFY modelChanged)
public:
    explicit Wrapper(QObject *parent = nullptr);

signals:
    void startRunner(bool gqd, bool imt, bool trp, bool hrp,int min_len);
    void textReceived(QString text);
    void updateProgress(double val);
    void modelChanged();
public slots:
    void receiveText(QString text);
    void receiveProgress(double val, double limit);
    void receiveResults(QString results);
    void doDFA(bool gqd, bool imt, bool trp, bool hrp,int min_len);
    ResultModel *getModel();
private:
    QThread runnerThread;
    Runner *runner;
    ResultModel* m_result_model;
};

#endif // WRAPPER_H
