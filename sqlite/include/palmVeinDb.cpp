/******************************************************************
* File Name��palmVeinDb.cpp
* File Identification��palmVein Database
* summary��realize palmVein Database Function
* Version��1.0
* Author:lilong
* Finish Data��2021-12-1
*******************************************************************/
#include "palmVeinDb.h"
#include "baseSqlite.h"
#include <mutex>

palmVeinDb *palmVeinDb::m_dbInstance = nullptr;
const std::string palmVeinDb::m_dbName("./palmVein.db");

const std::string g_pFeaTable("CameraCaseTable");
const std::string g_pParaTable("paraTable");

memberInfo_t featureTableNameMap[] = {
    {offsetof(featureTable, camera_id), "camera_id", TYPE_INTEGER, false, 0},
    {offsetof(featureTable, event_url), "event_url", TYPE_BLOB, false, 0},
    {offsetof(featureTable, cavans_json), "cavans_json", TYPE_TEXT, true, 0},
    {offsetof(featureTable, event_report_time), "event_report_time", TYPE_TEXT, false, 0},
    {0, "", 0, 0},
};

memberInfo_t paraTableNameMap[] = {
    {offsetof(paraTable, iPort), "iPort", TYPE_INTEGER, false, 0},
    {offsetof(paraTable, ip), "ip", TYPE_TEXT, false, 0},
    {0, "", 0, 0, 0},
};

static const palmTableInfo g_palmVeinTableInfo[] = {
    {featureTableNameMap, g_pFeaTable, true},
//    {paraTableNameMap, g_pParaTable, true},
    {NULL, "", 0},
};




bool palmVeinDb::openDb()
{
        return openDataBase(m_dbName);
}

bool palmVeinDb::initDataBase()
{

        if (!m_dbInstance->openDb())
        {
                return false;
        }
        const palmTableInfo *pPalmVeinTableInfo = g_palmVeinTableInfo;
        if (pPalmVeinTableInfo == NULL)
        {
                std::cout << "no table need create" <<std::endl;
                return false;
        }
        do
        {
                if (pPalmVeinTableInfo->fTable && (!m_dbInstance->isTableInDB(pPalmVeinTableInfo->tableName)))
                {
                        if (!m_dbInstance->createTb(*pPalmVeinTableInfo))
                        {
                                std::cout << "table " << pPalmVeinTableInfo->tableName << " create failed!!" <<std::endl;
                                return false;
                        }
                }
                pPalmVeinTableInfo++;
        } while (!pPalmVeinTableInfo->tableName.empty());

        return true;
}

bool palmVeinDb::createTb(const palmTableInfo &palmVeinTableInfo)
{
        bool fRet = 0;
        tableParam_t tableParam;
        memset(&tableParam, 0, sizeof(tableParam));
        for (int i = 0; i < TABLE_MEM_NUM_MAX; i++)
        {
                if (palmVeinTableInfo.tableInfo[i].pColName.empty())
                {
                        break;
                }
                tableParam.tColumInfoTable[i].pColName = palmVeinTableInfo.tableInfo[i].pColName;
                tableParam.tColumInfoTable[i].bType = palmVeinTableInfo.tableInfo[i].bType;
                tableParam.tColumInfoTable[i].bPrimary = palmVeinTableInfo.tableInfo[i].bPrimary;
                tableParam.tColumInfoTable[i].fUnique = palmVeinTableInfo.tableInfo[i].fUnique;
        }

        fRet = createTable(palmVeinTableInfo.tableName, tableParam);

        return fRet;
}

bool palmVeinDb::insertOneRecord(const std::string &tableName, void *dataStruct)
{
        bool fRet = false;
        insertParam_t insertInfo;
        const char *pColName = NULL;
        const memberInfo_t *pMember = NULL;

        if (!m_dbInstance->isTableInDB(tableName))
        {
                return false;
        }

        if (tableName.compare(g_pFeaTable) == 0)
        {
                featureTable *pFeaSt = (featureTable *)dataStruct;//将传参转换为标题列的
                const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->path));

                memset(&insertInfo, 0, sizeof(insertInfo));

                pMember = m_dbInstance->getTableInfo(0, static_cast<int>(&((featureTable *)0)->id));
                insertInfo.tRowValueInfo[0].pColName = pMember->pColName;
                insertInfo.tRowValueInfo[0].uRowValue.iValue = pFeaSt->id;
                std::cout<<"pFeaSt->id"<< pFeaSt->id<<std::endl;
                pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->path));
                insertInfo.tRowValueInfo[1].pColName = pMember->pColName;
                strcpy(insertInfo.tRowValueInfo[1].uRowValue.bText, pFeaSt->path);

                pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->name));
                insertInfo.tRowValueInfo[2].pColName = pMember->pColName;
                strcpy(insertInfo.tRowValueInfo[2].uRowValue.bText, pFeaSt->name);

                pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->feature));
                insertInfo.tRowValueInfo[3].pColName = pMember->pColName;
                insertInfo.tRowValueInfo[3].uRowValue.tBlob.blen = sizeof(pFeaSt->feature);
                memcpy(insertInfo.tRowValueInfo[3].uRowValue.tBlob.bBlob, pFeaSt->feature, sizeof(pFeaSt->feature));
        }
        m_dbInstance->insertRecord(tableName, &insertInfo);

        return true;
}

const memberInfo_t *palmVeinDb::getTableInfo(int iTableNum, int iMemberOffsetInStruct)
{
        int iIndex = 0;
        const memberInfo_t *pPtr = NULL;
        const palmTableInfo *pMemberInfo = NULL;

        if (iTableNum >= 0 && iTableNum < sizeof(g_palmVeinTableInfo) / sizeof(g_palmVeinTableInfo[0]))
        {
                pMemberInfo = &g_palmVeinTableInfo[iTableNum];
                for (iIndex = 0; !pMemberInfo->tableInfo->pColName.empty(); iIndex++)
                {
                        if (pMemberInfo->tableInfo[iIndex].dwOffset == iMemberOffsetInStruct)
                        {
                                pPtr = &pMemberInfo->tableInfo[iIndex];
                                break;
                        }
                }
        }
        return pPtr;
}
//获取表标题的信息

bool palmVeinDb::isExisInTable(const std::string &tableName, const memberInfo_t *pMember, const queryInfo &queryData)
{
    unDataStruct data;
    switch (queryData.sqliteType)
    {
    case SQLITE_TEXT:
        memset(data.bText, 0, sizeof(data.bText));
        memcpy(data.bText, queryData.strValue.c_str(), queryData.strValue.size());
        break;
    case SQLITE_INTEGER:
        data.iValue = queryData.iValue;
        break;
    default:
        break;
    }

    bool fRet = dataIsExist(g_pFeaTable, pMember->pColName, pMember->bType, data);
    return fRet;
}

void palmVeinDb::synTableData(const std::string &pTableName, std::vector<selectResult_t> &tableData)
{
    m_dbInstance->getAllData(pTableName, tableData);
}

int palmVeinDb::getFeatureRecordNum()
{
    return getDBRecordNum(g_pFeaTable);
}

bool palmVeinDb::clearTableData(const std::string &tableName)
{
    return m_dbInstance->deleteAllRecord(tableName);
}
/*
 * function��if palmVeinDb not eixst create palmVeinDb else open palmVeinDb
 * inout parameter��no
 * output parameter��no
 * return ��successed:true failed:false
 */
bool palmVeinDb::getInstance()
{
    static std::once_flag oc;
    std::call_once(oc, [&]{ m_dbInstance = new palmVeinDb(); }); // ensure funtion only runninng one
    if (m_dbInstance == nullptr)
    {
        return false;
    }
    return true;
}

bool palmVeinDb::isExistInPvTb(const queryInfo &queryData)
{
    bool fRet = false;
    switch (queryData.nameType)
    {
    case NAME:
        if (1)
        {
            const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->name));
            fRet = m_dbInstance->isExisInTable(g_pFeaTable, pMember, queryData);
            std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  name = " << queryData.strValue << " pMember->pColName =" << pMember->pColName <<std::endl;
            if (fRet)
            {
                std::cout << "warning!!! name " << queryData.strValue << " exist" <<std::endl;
            }
        }
        break;
    case PATH:
        if (1)
        {
            const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->path));
            fRet = m_dbInstance->isExisInTable(g_pFeaTable, pMember, queryData);
            std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  name = " << queryData.strValue << " pMember->pColName" << pMember->pColName <<std::endl;
            if (fRet)
            {
                std::cout << "warning!!! path " << queryData.strValue << " exist" <<std::endl;
            }
        }
        break;
    case ID:
        if (1)
        {
            const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->id));
            fRet = m_dbInstance->isExisInTable(g_pFeaTable, pMember, queryData);
            std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  intValue = " << queryData.iValue << " pMember->pColName = " << pMember->pColName <<std::endl;
            if (fRet)
            {
                std::cout << "warning!!! id " << queryData.iValue << " exist" <<std::endl;
            }
        }
        break;
    default:
        break;
    }

    return fRet;
}

int palmVeinDb::getMavIdValue(const std::string &name)
{
    return m_dbInstance->getMaxId(name);
}


bool palmVeinDb::PVDeleteRecordByValue(const std::string &tableName, const memberInfo_t *pMember, const deleteInfo &deleteData)
{
	unDataStruct data;
	switch (deleteData.sqliteType)
	{
	case SQLITE_TEXT:
		memset(data.bText, 0, sizeof(data.bText));
		memcpy(data.bText, deleteData.strValue.c_str(), deleteData.strValue.size());
		break;
	case SQLITE_INTEGER:
		data.iValue = deleteData.iValue;
		break;
	default:
		break;
	}

	bool fRet = deleteRecordByValue(g_pFeaTable, pMember->pColName, pMember->bType, data);
	return fRet;
}


bool palmVeinDb::deleteRecordByValuePvTb(const deleteInfo &queryData)
{
	bool fRet = false;
	switch (queryData.nameType)
	{
	case NAME:
		if (1)
		{
			const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->name));
			fRet = m_dbInstance->PVDeleteRecordByValue(g_pFeaTable, pMember, queryData);
			std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  name = " << queryData.strValue << " pMember->pColName =" << pMember->pColName <<std::endl;
			if (!fRet)
			{
				std::cout << "delete name " << queryData.strValue << " success" <<std::endl;
			}
		}
		break;
 	case PATH:
 		if (1)
 		{
 			const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->path));
 			fRet = m_dbInstance->PVDeleteRecordByValue(g_pFeaTable, pMember, queryData);
 			std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  name = " << queryData.strValue << " pMember->pColName" << pMember->pColName <<std::endl;
 			if (!fRet)
 			{
 				std::cout << "delete path " << queryData.strValue << " failed" <<std::endl;
			}
 		}
 		break;
	case ID:
		if (1)


		{
			const memberInfo_t *pMember = m_dbInstance->getTableInfo(0, (int)(&((featureTable *)0)->id));
			fRet = m_dbInstance->PVDeleteRecordByValue(g_pFeaTable, pMember, queryData);
			std::cout << "fRet = " << fRet << "   g_pFeaTable = " << g_pFeaTable << "  intValue = " << queryData.iValue << " pMember->pColName" << pMember->pColName <<std::endl;
			if (!fRet)
			{
				std::cout << "delete id " << queryData.iValue << " failed" <<std::endl;
			}
		}
		break;
	default:
		break;
	}

	return fRet;
}