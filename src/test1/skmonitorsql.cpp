#include "skmonitorsql.h"
#include <QTextCodec>
#include <QVector>
#include <QString>
 
QSKMonitorSQL::QSKMonitorSQL(QString sFileName, QObject *parent) :
    QObject(parent)
{
    m_pdb = NULL;
 
 
    m_nSetMaxRecordNum = 0;
    m_bSetRecordFlag = false;
 
 
    //编码转换
    m_sDB = fn_UnicodeToUTF8(sFileName);//编码转换
 
 
    m_bStartBegin = false;            //初始化没有插入BEGIN命令
    m_bStartCommit = false;           //初始化没有插入COMMIT命令
}
 
 
/*********************************************************************************************
  Func: 连接到指定数据库
  input: null
  ouput:null
 ********************************************************************************************/
bool QSKMonitorSQL::fn_connectSQL()
{
    if( (sqlite3_open(m_sDB. toLatin1().data(), &m_pdb)) != 0 ) //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
    {
        m_bDBIsOk = false;
        return false;
    }
    m_bDBIsOk = true;
    return true;
};
 
 
/*********************************************************************************************
  Func: 关闭指定数据库
  input: null
  ouput:null
 ********************************************************************************************/
bool QSKMonitorSQL::fn_closeSQL()
{
    if (m_pdb!=NULL)
    {
        sqlite3_close(m_pdb);
    }
    return false;
}
 
 
/*********************************************************************************************
  Func: 运行BEGIN命令
  input: null
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_StartBegin()
{
    if (m_bStartBegin == false)
    {
        if (m_pdb != NULL)
        {
            QString stmp;
            stmp = "begin;";
            stmp = fn_UnicodeToUTF8(stmp);     //编码转换
            sqlite3_exec( m_pdb , stmp. toLatin1().data() , 0 , 0 , &m_pzErrMsg );
            m_bStartBegin = true;
            m_bStartCommit = false;
        }
    }
}
/*********************************************************************************************
  Func: 运行commit命令
  input: null
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_StartCommit()
{
    if (m_bStartCommit == true)            //提交，存入磁盘
    {
        if (m_pdb != NULL)
        {
            QString stmp;
            stmp = "commit;";
            stmp = fn_UnicodeToUTF8(stmp);     //编码转换
            sqlite3_exec( m_pdb , stmp. toLatin1().data() , 0 , 0 , &m_pzErrMsg );
            m_bStartBegin = false;
            m_bStartCommit = false;
        }
    }
}
 
 
/*********************************************************************************************
  Func: 初始化数据表
  input: 表名，表列名
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_InitTable(QString sTableName, QStringList sNameList, QStringList sType)
{
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "create table " + sTableName + "(logtime datetime";
 
 
        for (int i=0;i<sNameList.size();i++)
        {
            stmp = stmp + "," + sNameList.at(i) + " " + sType.at(i);
        }
 
 
        stmp = stmp + ")";
        stmp = stmp + ";";
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg);
 
 
        m_sInsertT1 = "insert into ";
        m_sInsertT1 = m_sInsertT1 + sTableName + " values(";        //记录插入语句部分str
        m_sTable = sTableName;                    //表的名字
        m_nColumn = sNameList.size() + 1;         //列总数=记录列数+时间
        m_sUpdateT1 = "update " + sTableName;     //记录修改语句
        m_sUpdateT1 = m_sUpdateT1 + " set ";
        m_sTitleList = sNameList;                 //列名称
    }
    fn_StartCommit();
}
 
 
/*********************************************************************************************
  Func: 初始化数据表
  input: 表名，表列名
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_InitTableForBase(QString sTableName, QStringList sNameList, QStringList sType)
{
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "create table " + sTableName + "(";
 
 
        for (int i=0;i<sNameList.size();i++)
        {
            if (i != 0)
            {
                stmp += ",";
            }
            stmp = stmp + sNameList.at(i) + " " + sType.at(i);
        }
 
 
        stmp = stmp + ")";
        stmp = stmp + ";";
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg);
 
 
        m_sInsertT1 = "insert into ";
        m_sInsertT1 = m_sInsertT1 + sTableName + " values(";        //记录插入语句部分str
        m_sTable = sTableName;                    //表的名字
        m_nColumn = sNameList.size() + 1;         //列总数=记录列数+时间
        m_sUpdateT1 = "update " + sTableName;     //记录修改语句
        m_sUpdateT1 = m_sUpdateT1 + " set ";
        m_sTitleList = sNameList;                 //列名称
    }
    fn_StartCommit();
}
 
 
/*********************************************************************************************
  Func: 增加数据到数据库
  input: null
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_InsertData(QString sTableName, QStringList sDataList, QDateTime dt)
{
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        if (m_bSetRecordFlag)//设置了最大行数
        {
            int nMaxRecordSize = fn_RecordSize(sTableName);//当前最大记录数
            if (nMaxRecordSize < 0) return;
 
 
            if (nMaxRecordSize >= m_nSetMaxRecordNum)//超出限制，删掉开始项
            {
                QString stmp;
                stmp = "delete from ";
                stmp = stmp + sTableName;
                stmp = stmp + " where rowid <= (select min(rowid) from ";
                stmp = stmp + sTableName + ");";
 
 
                stmp = fn_UnicodeToUTF8(stmp);//编码转换
                sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg);
            }
 
 
            QString stmp;
            stmp = "insert into ";
            stmp = stmp + sTableName + " values(";
            QString sdatetime = dt.toString("yyyy-MM-dd hh:mm:ss");
            stmp = stmp + "'";
            stmp = stmp + sdatetime;
            stmp = stmp + "'";
 
 
            for (int i=0;i<sDataList.size();i++)
            {
                stmp = stmp + ",'";
                stmp = stmp + sDataList.at(i) + "'";
            }
            stmp = stmp + ")";
            stmp = stmp + ";";
 
 
            stmp = fn_UnicodeToUTF8(stmp);//编码转换
            sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg );
        }
        else
        {
            QString stmp;
            stmp = "insert into ";
            stmp = stmp + sTableName + " values(";
            QString sdatetime = dt.toString("yyyy-MM-dd hh:mm:ss");
            stmp = stmp + "'";
            stmp = stmp + sdatetime;
            stmp = stmp + "'";
 
 
            for (int i=0;i<sDataList.size();i++)
            {
                stmp = stmp + ",'";
                stmp = stmp + sDataList.at(i) + "'";
            }
            stmp = stmp + ")";
            stmp = stmp + ";";
 
 
            stmp = fn_UnicodeToUTF8(stmp);//编码转换
            sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg );
        }
    }
    fn_StartCommit();
}
 
 
/*********************************************************************************************
  Func: 增加数据到数据库
  input: null
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_InsertDataForBase(QString sTableName, QStringList sDataList)
{
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        QString stmp;
 
 
        stmp.clear();
        stmp = "insert into ";
        stmp = stmp + sTableName + " values(";
 
 
        for (int i=0; i<sDataList.size(); i++)
        {
            if (i != 0)
            {
                stmp = stmp + ",";
            }
            stmp = stmp + "'";
            stmp = stmp + sDataList.at(i) + "'";
        }
 
 
        stmp = stmp + ")";
        stmp = stmp + ";";
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        sqlite3_exec(m_pdb ,stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg);
    }
    fn_StartCommit();
}
 
 
/*********************************************************************************************
  Func: 修改数据库数据
  input: null
  ouput:null
 ********************************************************************************************/
int QSKMonitorSQL::fn_UpdateData(QString sTableName, QStringList sDataList, int index)
{
    int  nRet = -1;
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        QString stmp;
        int isize;
        stmp = "update " + sTableName;     //修改语句
        stmp = stmp + " set ";
 
 
        isize = sDataList.size();
        for (int i=0;i<isize;i++)
        {
            stmp = stmp + m_sTitleList.at(i);
            stmp = stmp + " = ";
            stmp = stmp + "'";
            stmp = stmp + sDataList.at(i);
            stmp = stmp + "'";
            if (i != isize - 1)             //不是最后一个时
            {
                stmp = stmp + ",";
            }
        }
        stmp = stmp + " where rowid = (select MIN(rowid) + ";
        stmp = stmp + QString::number(index);
        stmp = stmp + " from ";
        stmp = stmp + m_sTable;
        stmp = stmp + ")";
        stmp = stmp + ";";
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        nRet = sqlite3_exec(m_pdb, stmp. toLatin1().data(), NULL, NULL, &m_pzErrMsg );
    }
    fn_StartCommit();
 
 
    return nRet;
}
 
 
/*********************************************************************************************
  Func: 删除数据
  input: null
  ouput:null
 ********************************************************************************************/
void QSKMonitorSQL::fn_DeleteData(QString sTableName)
{
    fn_StartBegin();
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "delete from ";
        stmp = stmp + sTableName;
        stmp = stmp + ";";
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        sqlite3_exec(m_pdb , stmp. toLatin1().data() , NULL , NULL , &m_pzErrMsg );
    }
    fn_StartCommit();
}
 
 
/*********************************************************************************************
  Func: 上电时获得最小的rowid序号
  input: null
  ouput:null
 ********************************************************************************************/
uint QSKMonitorSQL::fn_GetMinRowID(QString sTableName)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select min(rowid) from ";
        stmp = stmp + sTableName;
        stmp = stmp + ";";
        int nRow;
        int nColumn;
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        int result = sqlite3_get_table(m_pdb, stmp. toLatin1().data(), &m_ppazResult, &nRow, &nColumn, &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QString sData;
            sData.sprintf("%s", m_ppazResult[1]);
            int nMin;
            nMin = sData.toInt();
            sqlite3_free_table(m_ppazResult);          //释放
            return nMin;
        }
        else
        {
            sqlite3_free_table(m_ppazResult);          //释放
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
 
 
/*********************************************************************************************
  Func: 获得数据库记录大小
  input: null
  ouput:记录条数
 ********************************************************************************************/
int QSKMonitorSQL::fn_RecordSize(QString sTableName)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select count(*) from ";
        stmp = stmp + sTableName;
        int nRow;
        int nColumn;
 
 
        stmp = fn_UnicodeToUTF8(stmp);//编码转换
        int result = sqlite3_get_table(m_pdb , stmp. toLatin1().data(), &m_ppazResult , &nRow , &nColumn , &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QString sNum;
            sNum.sprintf("%s",m_ppazResult[1]);
            sqlite3_free_table(m_ppazResult);
            return sNum.toInt();
        }
        else
        {
            sqlite3_free_table(m_ppazResult);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
 
 
/*********************************************************************************************
  Func: 获取数据库全部数据
  input: pData-返回的查询数据
  ouput:返回查询结果总数
 ********************************************************************************************/
int QSKMonitorSQL::fn_QueryAllData(QString sTableName, QVector<QStringList> *pData)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select * from ";
        stmp = stmp + sTableName;
 
 
        //格式化查询语句
 
 
        int nRow;
        int nColumn;
        int result = sqlite3_get_table(m_pdb , stmp. toLatin1().data(), &m_ppazResult , &nRow , &nColumn ,
                                        &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QStringList listTmp;
            QString sData;
            for (int i=0;i<nRow;i++)
            {
                int iTmp;
                iTmp = (i + 1)*nColumn;             //返回数据中，前nColumn个是字段名字，偏移字段数目
                for (int j=0;j<nColumn;j++)
                {
                    sData.sprintf("%s",m_ppazResult[iTmp + j]);
                    listTmp.append(sData);
                }
                pData->append(listTmp);
                listTmp.clear();
            }
            sqlite3_free_table(m_ppazResult);          //释放
            return nRow;
        }
        else
        {
            sqlite3_free_table(m_ppazResult);          //释放
            return -1;
        }
    }
    else
    {
        return -1;
    }
}
 
 
/*********************************************************************************************
  Func: 按序号查询指定范围的数据
  input: iStart-开始rowid序号 iNumber-查询总数 pData-返回的查询数据
  ouput:返回查询结果总数
 ********************************************************************************************/
int QSKMonitorSQL::fn_QueryDataByIndex(QString sTableName, int iStart, int iNumber, QVector<QStringList> *pData)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select * from ";
        stmp = stmp + sTableName + " ";
        stmp = stmp + "where rowid >= ";
        stmp =  stmp + QString::number(iStart) + " limit ";
        stmp = stmp + QString::number(iNumber);
        int nRow;
        int nColumn;
        int result = sqlite3_get_table(m_pdb , stmp. toLatin1().data(), &m_ppazResult , &nRow , &nColumn ,
                                        &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QStringList listTmp;
            QString sData;
            for (int i=0;i<nRow;i++)
            {
                int iTmp;
                iTmp = (i + 1)*nColumn;             //返回数据中，前nColumn个是字段名字，偏移字段数目
                for (int j=0;j<nColumn;j++)
                {
                    sData.sprintf("%s",m_ppazResult[iTmp + j]);
                    listTmp.append(sData);
                }
                pData->append(listTmp);
                listTmp.clear();
            }
            sqlite3_free_table(m_ppazResult);          //释放
            return nRow;
        }
        else
        {
            sqlite3_free_table(m_ppazResult);          //释放
            return -1;
        }
    }
    else
    {
        return -1;
    }
}
 
 
/*********************************************************************************************
  Func: 按时间查询指定范围的数据
  input: dtStart-开始时间 dtEnd-结束时间 pData-返回的查询数据
  ouput:返回查询结果总数
 ********************************************************************************************/
int QSKMonitorSQL::fn_QueryDataByTime(QString sTableName, QDateTime dtStart, QDateTime dtEnd, QVector<QStringList> *pData)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select * from ";
        stmp = stmp + sTableName + " ";
        stmp = stmp + "where logtime >= ";
        stmp =  stmp + "'" + dtStart.toString("yyyy-MM-dd hh:mm:ss") + "'" + " and logtime <= ";
        stmp =  stmp  + "'" + dtEnd.toString("yyyy-MM-dd hh:mm:ss") + "'";
        int nRow;
        int nColumn;
        int result = sqlite3_get_table(m_pdb , stmp. toLatin1().data(), &m_ppazResult , &nRow , &nColumn ,
                                        &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QStringList listTmp;
            QString sData;
            for (int i=0;i<nRow;i++)
            {
                int iTmp;
                iTmp = (i + 1)*nColumn;             //返回数据中，前nColumn个是字段名字，偏移字段数目
                for (int j=0;j<nColumn;j++)
                {
                    sData.sprintf("%s",m_ppazResult[iTmp + j]);
                    listTmp.append(sData);
                }
                pData->append(listTmp);
                listTmp.clear();
            }
            sqlite3_free_table(m_ppazResult);          //释放
            return nRow;
        }
        else
        {
            sqlite3_free_table(m_ppazResult);          //释放z
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
 
 
/*********************************************************************************************
  Func: 按值查询指定范围的数据
  input: nType-比较条件 nFrontV-比较前值 nBackV-比较后值 pData-返回的查询数据
  ouput:返回查询结果总数
 ********************************************************************************************/
int QSKMonitorSQL::fn_QueryDataByValue(QString sTableName, int nType, QString sFrontV, QString sBackV, QVector<QStringList> *pData)
{
    return true;
}
 
 
/*********************************************************************************************
  Func: 有开始时间和结束时间算出这段时间有多少数据
  input: 开始时间 结束时间 返回行数 最小rowid
  ouput:找到数据返回1 没有找到返回0
  Select count(*) From MAIN.[hisdata] where hisdata.logtime >= '2012-05-24 15:53:48'
and hisdata.logtime >= '2012-05-24 15:55:48' ;
 ********************************************************************************************/
bool QSKMonitorSQL::fn_RecordSizeByTime(QString sTableName, QDateTime dtStart, QDateTime dtEnd, int *pRows, int *pMaxID,int *pMinID)
{
    if (m_pdb!=NULL)
    {
        QString stmp;
        stmp = "select count(*), MIN(rowid), MAX(rowid) from ";
        stmp = stmp + sTableName + " ";
        stmp = stmp + "where logtime >= ";
        stmp =  stmp + "'" + dtStart.toString("yyyy-MM-dd hh:mm:ss") + "'" + " and logtime <= ";
        stmp =  stmp  + "'" + dtEnd.toString("yyyy-MM-dd hh:mm:ss") + "'";
        int nRow;
        int nColumn;
        int result = sqlite3_get_table(m_pdb , stmp. toLatin1().data(), &m_ppazResult , &nRow , &nColumn ,
                                        &m_pzErrMsg );
        if (result == SQLITE_OK)
        {
            QString sData;
            int counts;
            sData.sprintf("%s",m_ppazResult[3]);           //获得返回条数
            counts = sData.toInt();                     //行数
            if (counts == 0)                            //没有数据
            {
                *pRows = counts;
                *pMinID = 0;
                *pMaxID = 0;
            }
            else
            {
                *pRows = counts;
                sData.sprintf("%s",m_ppazResult[4]);           //获得最小rowid
                *pMinID = sData.toInt();
                sData.sprintf("%s",m_ppazResult[5]);           //获得最大rowid
                *pMaxID = sData.toInt();
 
 
            }
        }
        sqlite3_free_table(m_ppazResult);          //释放
        return 1;
    }
    else
    {
        *pRows = 0;
        *pMinID = 0;
        *pMaxID = 0;
        return 0;
    }
}
 
 
/*********************************************************************************************
  Func: 设置最大行数
  input: nMaxRows：最大行数值
  ouput: 无
 ********************************************************************************************/
void QSKMonitorSQL::fn_SetMaxRowId(int nMaxRows)
{
    m_nSetMaxRecordNum = nMaxRows;
    m_bSetRecordFlag = true;
}
 
 
/*********************************************************************************************
  Func: unicode 到UTF8转换
  input: null
  ouput:null
 ********************************************************************************************/
QString QSKMonitorSQL::fn_UnicodeToUTF8(QString str)
{
    QString sName;
    QByteArray baT4;
    baT4.clear();
    baT4=QTextCodec::codecForName("UTF8")->fromUnicode(str);
    sName = (QString)baT4;
    return sName;
}