#ifndef RESULTMODEL_H
#define RESULTMODEL_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

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
        GqdRole,
        HrpRole,
        ImtRole,
        TrpRole
    };
    ResultModel(QObject* p=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void addResult(const Result new_result);
    Q_INVOKABLE virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void clear();
    QList<Result> getData();
    static bool cmp_str_less(const Result &a, const Result &b);
    static bool cmp_str_greater(const Result &a, const Result &b);
    static bool cmp_gqd_less(const Result &a, const Result &b);
    static bool cmp_gqd_greater(const Result &a, const Result &b);
    static bool cmp_hrp_less(const Result &a, const Result &b);
    static bool cmp_hrp_greater(const Result &a, const Result &b);
    static bool cmp_imt_less(const Result &a, const Result &b);
    static bool cmp_imt_greater(const Result &a, const Result &b);
    static bool cmp_trp_less(const Result &a, const Result &b);
    static bool cmp_trp_greater(const Result &a, const Result &b);

private:
    QList<Result> m_result_list;

};

#endif // RESULTMODEL_H
