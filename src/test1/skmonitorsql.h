#ifndef SKMONITORSQL_H
#define SKMONITORSQL_H
 
// 来源于网络 https://blog.csdn.net/lengyuezuixue/article/details/80863886
 
#include <QObject>
#include <QDateTime>
#include "sqlite3.h"
#include <QStringList>
#include <QString>
 
class QSKMonitorSQL : public QObject
{
    Q_OBJECT
public:
    explicit QSKMonitorSQL(QString sFileName, QObject *parent = 0);
    bool fn_connectSQL();
    bool fn_closeSQL();
    void fn_InitTable(QString sTableName, QStringList sNameList, QStringList sType);
    void fn_InitTableForBase(QString sTableName, QStringList sNameList, QStringList sType);
 
 
    void fn_InsertData(QString sTableName, QStringList sDataList, QDateTime dt);
    void fn_InsertDataForBase(QString sTableName, QStringList sDataList);
 
 
    int fn_UpdateData(QString sTableName, QStringList sDataList,int index);
    void fn_DeleteData(QString sTableName);
 
 
    int fn_QueryDataByIndex(QString sTableName, int iStart,int iNumber,QVector<QStringList> *pData);
    int fn_QueryDataByTime(QString sTableName, QDateTime dtStart,QDateTime dtEnd,QVector<QStringList> *pData);
    int fn_QueryDataByValue(QString sTableName, int nType, QString sFrontV, QString sBackV, QVector<QStringList> *pData);
    int fn_QueryAllData(QString sTableName, QVector<QStringList> *pData);
 
 
    int fn_RecordSize(QString sTableName);
    uint fn_GetMinRowID(QString sTableName);
    bool fn_RecordSizeByTime(QString sTableName, QDateTime dtStart, QDateTime dtEnd, int *pRows, int *pMaxID,int *pMinID);
 
 
    QString fn_UnicodeToUTF8(QString str);
 
 
    void fn_StartBegin();
    void fn_StartCommit();
 
 
    bool m_bDBIsOk;
 
 
    int m_nSetMaxRecordNum;
    bool m_bSetRecordFlag;
    void fn_SetMaxRowId(int nMaxRows);
 
 
public:
    bool m_bStartBegin;
    bool m_bStartCommit;
 
 
signals:
 
 
public slots:
 
 
private:
    QString m_sDB;
    QString m_sInsertT1;
    QString m_sUpdateT1;
    QString m_sTable;
    QStringList m_sTitleList;  //列名
    int m_nColumn;
    sqlite3 *m_pdb;            //数据库
    char *m_pzErrMsg;          //出错信息
    char  *m_perrmsg;          //出错信息
    char  **m_ppazResult;
 
 
};
 
 
#endif // SKMONITORSQL_H