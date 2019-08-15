#ifndef RUNNER_H
#define RUNNER_H


#include <QObject>
#include "dfalib/algorithms.h"
#include <QtDebug>
#include <QThread>

class Runner : public QThread
{
    Q_OBJECT
    void run() override;
public:
    enum type{
        RUNNER_TYPE_OPTIONSVIEW,
        RUNNER_TYPE_CALCSVIEW
    };
    explicit Runner(int type=RUNNER_TYPE_OPTIONSVIEW);
    int getType();
signals:
    void sendText(QString text);
    void sendProgress(double val, double limit);
    void sendResults(QString results);

public slots:
    void doDFA(bool gqd, bool imt, bool trp, bool hrp, int length);
    void parseStrings(QStringList strings);
    void stop();

private:
    bool gqd, imt, trp, hrp;
    int length;
    bool running, running_calc;
    int type;
    QStringList strings;
};

#endif // RUNNER_H
