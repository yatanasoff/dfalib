#include "resultmodel.h"

ResultModel::ResultModel(QObject* p):QAbstractTableModel (p)
{

}

int ResultModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_result_list.size();
}

int ResultModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant ResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){return QVariant();}

       switch(role)
       {
       case StringRole: return m_result_list.at(index.row()).str;
       case GqgRole: return m_result_list.at(index.row()).gqd;
       case ImtRole: return m_result_list.at(index.row()).imt;
       case HrpRole: return m_result_list.at(index.row()).hrp;
       case TrpRole: return m_result_list.at(index.row()).trp;
       }


       return QVariant();
}

void ResultModel::addResult(const Result new_result)
{
    beginInsertRows(QModelIndex(),rowCount(), rowCount());
    m_result_list.append(new_result);
    endInsertRows();
}

void ResultModel::clear()
{
    beginResetModel();
    m_result_list.clear();
    endResetModel();
}

QHash<int, QByteArray> ResultModel::roleNames() const
{
    QHash<int, QByteArray> roleNameMap;
    roleNameMap[StringRole] = "string";
    roleNameMap[GqgRole] = "gqd";
    roleNameMap[ImtRole] = "imt";
    roleNameMap[TrpRole] = "trp";
    roleNameMap[HrpRole] = "hrp";

    return roleNameMap;
}
