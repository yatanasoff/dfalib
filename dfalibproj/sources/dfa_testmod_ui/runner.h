#ifndef RUNNER_H
#define RUNNER_H

#include <QObject>
#include "dfalib/algorithms.h"
#include <QtDebug>

class Runner : public QObject
{
    Q_OBJECT
public:
    explicit Runner(QObject *parent = nullptr);

signals:
    void sendText(QString text);
    void sendProgress(double val, double limit);
    void sendResults(QString results);
public slots:
    void doDFA(bool gqd, bool imt, bool trp, bool hrp, int min_len);
};

#endif // RUNNER_H
