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
    case GqdRole: return m_result_list.at(index.row()).gqd;
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

QList<Result> ResultModel::getData()
{
    return m_result_list;
}

void ResultModel::sort(int column, Qt::SortOrder order)
{
   beginResetModel();
   if(column==0){
       if(order==Qt::SortOrder::AscendingOrder){
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_str_less);
       }else{
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_str_greater);
       }
   }else if(column==1){
       if(order==Qt::SortOrder::AscendingOrder){
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_gqd_less);
       }else{
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_gqd_greater);
       }
   }else if(column==3){
       if(order==Qt::SortOrder::AscendingOrder){
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_hrp_less);
       }else{
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_hrp_greater);
       }
   }else if (column==2) {
       if(order==Qt::SortOrder::AscendingOrder){
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_imt_less);
       }else{
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_imt_greater);
       }
   }else if (column==4) {
       if(order==Qt::SortOrder::AscendingOrder){
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_trp_less);
       }else{
           std::sort(m_result_list.begin(), m_result_list.end(),cmp_trp_greater);
       }
    }



    endResetModel();
}

bool ResultModel::cmp_str_less(const Result &a, const Result &b){return a.str < b.str;}
bool ResultModel::cmp_str_greater(const Result &a, const Result &b){return a.str > b.str;}
bool ResultModel::cmp_gqd_less(const Result &a, const Result &b){return a.gqd < b.gqd; }
bool ResultModel::cmp_gqd_greater(const Result &a, const Result &b){return  a.gqd > b.gqd;}
bool ResultModel::cmp_hrp_less(const Result &a, const Result &b){return a.hrp < b.hrp;}
bool ResultModel::cmp_hrp_greater(const Result &a, const Result &b){return a.hrp > b.hrp;}
bool ResultModel::cmp_imt_less(const Result &a, const Result &b){return a.imt < b.imt;}
bool ResultModel::cmp_imt_greater(const Result &a, const Result &b){return a.imt > b.imt;}
bool ResultModel::cmp_trp_less(const Result &a, const Result &b){return a.trp < b.trp;}
bool ResultModel::cmp_trp_greater(const Result &a, const Result &b){return a.trp > b.trp;}


QHash<int, QByteArray> ResultModel::roleNames() const
{
    QHash<int, QByteArray> roleNameMap;
    roleNameMap[StringRole] = "string";
    roleNameMap[GqdRole] = "gqd";
    roleNameMap[ImtRole] = "imt";
    roleNameMap[TrpRole] = "trp";
    roleNameMap[HrpRole] = "hrp";

    return roleNameMap;
}
