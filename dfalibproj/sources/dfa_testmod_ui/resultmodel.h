#ifndef RESULTMODEL_H
#define RESULTMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QAbstractTableModel>

struct Result{
    QString str;
    int gqd;
    int hrp;
    int trp;
    int imt;
};

class ResultModel : public QAbstractTableModel{
    Q_OBJECT
public:

    enum ResultModelRoles
    {
        StringRole = Qt::UserRole + 1,
        GqgRole,
        HrpRole,
        ImtRole,
        TrpRole
    };
    ResultModel(QObject* p=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void addResult(const Result new_result);
    void clear();

private:
    QList<Result> m_result_list;

};

#endif // RESULTMODEL_H
