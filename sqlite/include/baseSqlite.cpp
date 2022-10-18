#include "stdio.h"
#include "stdlib.h"
#include "sqlite3.h"
#include "baseSqlite.h"
#include "string.h"
#include <iostream>
//using namespace std;
#include <string>
using std::string;

const char *pSqlCreateInter1 = "create table %Q ( ";
const char *pSqlCreateInter2 = " %s \"%s\" INTEGER default %d, ";
const char *pSqlCreateInter3 = " %s \"%s\" INTEGER default %d);";
const char *pSqlAlterInter = "ALTER TABLE \"%s\" ADD COLUMN \"%s\" INTEGER DEFAULT %d;";
const char *pSqlInsertInter = "INSERT INTO %Q (\"%s\") values(?);";
const char *pSqlUpdateInter1 = "UPDATE %Q SET \"%s\"=%d where \"%s\"==%d";
const char *pSqlUpdateInter2 = "and \"%s\"==%d";
const char *pSqlUpdateInter3 = "%s ;";

typedef struct sqlCmd_s
{
	char bDataType;
	const char *pSqlCreate[3];
	const char *pSqlAlter[3];
	const char *pSqlInsert[3];
	const char *pSqlUpdate[3];
} SqlCmdInfo_t;

SqlCmdInfo_t g_SqlCmdInfoTable[] = {
    {SQLITE_INTEGER, {pSqlCreateInter1, pSqlCreateInter2, pSqlCreateInter3}, {pSqlAlterInter, NULL, NULL}, {pSqlUpdateInter1, pSqlUpdateInter2, pSqlUpdateInter3}},
    {SQLITE_FLOAT, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},
    {SQLITE_TEXT, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},
    {SQLITE_BLOB, {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},
};

baseSqlite::baseSqlite(void)
{
	m_pDb = NULL;
	m_pAllData = NULL;
	m_iRecordNum = 0;
}

baseSqlite::~baseSqlite()
{
	if (m_pDb != NULL)
		sqlite3_close_v2(m_pDb);
	if (m_pAllData != NULL)
	{
		delete[] m_pAllData;
	}
}

bool baseSqlite::openDataBase(const std::string &dbName)
{
	int iRet = 0;
	char *pStr = NULL;

	iRet = sqlite3_open_v2(dbName.c_str(), &m_pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE| SQLITE_OPEN_FULLMUTEX, NULL);

	if (iRet != SQLITE_OK)
	{
		pStr = sqlite3_mprintf("%s", sqlite3_errmsg(m_pDb));
		if (pStr != NULL)
		{
			std::cout << pStr <<std::endl;
			sqlite3_free(pStr);
		}
		sqlite3_close_v2(m_pDb);
		m_pDb = NULL;
		return false;
	}
	return true;
}

int baseSqlite::closeDataBase(void)
{
	if (m_pDb != NULL)
	{
		sqlite3_close_v2(m_pDb);
		m_pDb = NULL;
		std::cout << "baseSqlite::CloseDataBase success!!!" << std::endl;
	}
	else

		std::cout << "baseSqlite::CloseDataBase error m_pDb=" << m_pDb << std::endl;
	{
		;
	}
	if (m_pAllData != NULL)
	{
		delete[] m_pAllData;
		m_pAllData = NULL;
	}
	return 0;
}

int baseSqlite::caculateInfoNum(cell_t *pInfoTable, int iMaxCellNum)
{
	int iNum = 0, i = 0;

	for (i = 0; i < iMaxCellNum; i++)
	{
		if (pInfoTable[i].pColName.empty())
		{
			break;
		}
		iNum++;
	}

	return iNum;
}


const char *baseSqlite::getTypeString(int iType)
{
	const char *pbRet = NULL;
	switch (iType)
	{
	case SQLITE_INTEGER:
		pbRet = "INTEGER";
		break;
	case SQLITE_FLOAT:
		pbRet = "FLOAT";
		break;
	case SQLITE_TEXT:
		pbRet = "TEXT";
		break;
	case SQLITE_BLOB:
		pbRet = "BLOB";
		break;
	default:
		pbRet = "TEXT";
		break;
	}

	return pbRet;
}

char* baseSqlite::getSqlStr(const std::string &pTableName, tableParam_t& pInfo)
{
	char *pSql = NULL;
	char *pSqlTmp = NULL;
	pSqlTmp = pSql = sqlite3_mprintf("create table %Q ( ", pTableName.c_str());
	for (int i = 0; i < pInfo.bColumNum; i++)
	{
		if (pSqlTmp != NULL && !pInfo.tColumInfoTable[i].pColName.empty())
		{

			switch (pInfo.tColumInfoTable[i].bType)
			{

			case SQLITE_INTEGER:
				if (pInfo.tColumInfoTable[i].bPrimary == 0)
				{
					if (pInfo.tColumInfoTable[i].uRowValue.iValue != 0)
					{
						pSql = sqlite3_mprintf("%s \"%s\" INTEGER DEFAULT %d, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str(),
							pInfo.tColumInfoTable[i].uRowValue.iValue);
					}
					else
					{
						pSql = sqlite3_mprintf("%s \"%s\" INTEGER DEFAULT NULL, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
					}
				}
				else
				{
					//std::cout<<" INTEGER PRIMARY KEY, name = "<<pInfo.tColumInfoTable[i].pColName<<std::endl;
					pSql = sqlite3_mprintf("%s \"%s\" INTEGER PRIMARY KEY, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
				}
				break;
			case SQLITE_FLOAT:
				if (pInfo.tColumInfoTable[i].uRowValue.flValue != 0)
				{ 
					pSql = sqlite3_mprintf("%s \"%s\" FLOAT DEFAULT %f, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str(),
						pInfo.tColumInfoTable[i].uRowValue.flValue);
				}
				else
				{
					pSql = sqlite3_mprintf("%s \"%s\" FLOAT DEFAULT NULL, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
				}
				break;
			case SQLITE_TEXT:
				if (strlen(pInfo.tColumInfoTable[i].uRowValue.bText) != 0)
				{
					pSql = sqlite3_mprintf("%s \"%s\" TEXT DEFAULT %s, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str(),
						pInfo.tColumInfoTable[i].uRowValue.bText);
				}
				else
				{
					pSql = sqlite3_mprintf("%s \"%s\" TEXT DEFAULT NULL, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
				}
				break;
			case SQLITE_BLOB:
				if (pInfo.tColumInfoTable[i].uRowValue.tBlob.bBlob[0] != 0)
				{
					pSql = sqlite3_mprintf("%s \"%s\" BLOB DEFAULT X\'%d\', ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str(),
						pInfo.tColumInfoTable[i].uRowValue.tBlob.bBlob[0]);
				}
				else
				{
					pSql = sqlite3_mprintf("%s \"%s\" BLOB DEFAULT NULL, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
				}
				break;
			default:
				if (pInfo.tColumInfoTable[i].uRowValue.tBlob.bBlob[0] != 0)
				{
					pSql = sqlite3_mprintf("%s \"%s\" BLOB DEFAULT X\'%d\', ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str(),
						pInfo.tColumInfoTable[i].uRowValue.tBlob.bBlob[0]);
				}
				else
				{
					pSql = sqlite3_mprintf("%s \"%s\" BLOB DEFAULT NULL, ", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
				}
				break;
			}
			sqlite3_free(pSqlTmp);
			pSqlTmp = pSql;
		}
		else
		{

			break;
		}
	}
	if (*(pSql + strlen(pSql) - 2) == ',')//不明白
	{
		memcpy(pSql + strlen(pSql) - 2, ");", 2);
	}

	//获取到了sql完整命令数据

	for (int i = 0; i < pInfo.bColumNum; i++)
	{
		pSqlTmp = pSql;
		if (pSql != NULL && !pInfo.tColumInfoTable[i].pColName.empty())
		{
			if (pInfo.tColumInfoTable[i].fUnique)
			{
				if (*(pSqlTmp + strlen(pSqlTmp) - 1) == ';')
				{
					//*(pSqlTmp + strlen(pSqlTmp) - 1) = 0;
					//*(pSqlTmp + strlen(pSqlTmp) - 1) = 0;
					memset((pSqlTmp + strlen(pSqlTmp) - 2), 0, 2);
				}
				pSql = sqlite3_mprintf("%s,unique(%s)", pSqlTmp, pInfo.tColumInfoTable[i].pColName.c_str());
			}
			pSqlTmp = pSql;
		}
	}
	if (*(pSql + strlen(pSql) - 1) != ';')
	{
		pSqlTmp = pSql;
		pSql = sqlite3_mprintf("%s);", pSqlTmp);
		sqlite3_free(pSqlTmp);
	}
	return pSql;
}
//获取到了sql完整命令数据

bool baseSqlite::createTable(const std::string &pTableName, tableParam_t& pInfo)
{
	char *pSql = NULL; 
	char *pSqlTmp = NULL;
	sqlite3_stmt *pStmt = NULL;
	int iMaxNum = sizeof(pInfo.tColumInfoTable) / sizeof(pInfo.tColumInfoTable[0]);
	int i = 0;

	pInfo.bColumNum = caculateInfoNum(pInfo.tColumInfoTable, iMaxNum); 

	if (pInfo.bColumNum > iMaxNum || pInfo.bColumNum == 0)
	{
		std::cout <<"table " << pTableName<<" bColumNum num error!!" <<std::endl;
		return false;
	}
	pSql = getSqlStr(pTableName, pInfo);
	std::cout <<"pSql  =  " << pSql<<std::endl;
	if (pSql != NULL)
	{
		if (m_pDb != NULL)
		{
			sqlite3_prepare_v2(m_pDb, pSql, strlen(pSql) + 1, &pStmt, NULL);
		}
		else
		{
			std::cout << "m_pDb=" << m_pDb << "   ERROR" << std::endl;
			closeDataBase();
		}
		
		sqlite3_free(pSql);

		if (sqlite3_step(pStmt) == SQLITE_DONE)
		{
			std::cout << "ok" <<std::endl;
		}

		else
		{
			std::cout << "fault" <<std::endl;
		}
		sqlite3_finalize(pStmt);
		return true;
	}
	return false;
}

int baseSqlite::alterTable(const std::string &pTableName, tableParam_t &pInfo)
{
	char *pSql = NULL;
	sqlite3_stmt *pStmt = NULL;
	int iRet = 0, iIndex = 0;
	int iMaxNum = sizeof(pInfo.tColumInfoTable) / sizeof(pInfo.tColumInfoTable[0]);


	pInfo.bColumNum = caculateInfoNum(pInfo.tColumInfoTable, iMaxNum);

	for (iIndex = 0; iIndex < pInfo.bColumNum; iIndex++)
	{

		if (!pInfo.tColumInfoTable[iIndex].pColName.empty())
		{
			switch (pInfo.tColumInfoTable[iIndex].bType)
			{
			case SQLITE_INTEGER:
				if (pInfo.tColumInfoTable[iIndex].bPrimary == 0)
				{
					if (pInfo.tColumInfoTable[iIndex].uRowValue.iValue != 0)
					{
						pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" INTEGER DEFAULT %d;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str(),
								       pInfo.tColumInfoTable[iIndex].uRowValue.iValue);
					}
					else
					{
						pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" INTEGER DEFAULT NULL;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
					}
				}
				else
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" INTEGER PRIMARY KEY ;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
				}
				break;
			case SQLITE_FLOAT:
				if (pInfo.tColumInfoTable[iIndex].uRowValue.iValue != 0)
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" FLOAT DEFAULT %f;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str(),
							       pInfo.tColumInfoTable[iIndex].uRowValue.flValue);
				}
				else
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" FLOAT DEFAULT NULL;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
				}
				break;
			case SQLITE_TEXT:
				if (strlen(pInfo.tColumInfoTable[iIndex].uRowValue.bText) != 0)
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" TEXT DEFAULT %s;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str(),
							       pInfo.tColumInfoTable[iIndex].uRowValue.bText);
				}
				else
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" TEXT DEFAULT NULL;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
				}
				break;
			case SQLITE_BLOB:
				if (pInfo.tColumInfoTable[iIndex].uRowValue.tBlob.bBlob[0] != 0)
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" BLOB DEFAULT X\'%d\';", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str(),
							       pInfo.tColumInfoTable[iIndex].uRowValue.tBlob.bBlob[0]);
				}
				else
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" BLOB DEFAULT NULL;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
				}
				break;
			default:
				if (pInfo.tColumInfoTable[iIndex].uRowValue.tBlob.bBlob[0] != 0)
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" BLOB DEFAULT X\'%d\';", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str(),
							       pInfo.tColumInfoTable[iIndex].uRowValue.tBlob.bBlob[0]);
				}
				else
				{
					pSql = sqlite3_mprintf("ALTER TABLE \"%s\" ADD COLUMN \"%s\" BLOB DEFAULT NULL;", pTableName.c_str(), pInfo.tColumInfoTable[iIndex].pColName.c_str());
				}
				break;
			}

			if (pSql != NULL)
			{
				iRet = 0;
				iRet = sqlite3_prepare_v2(m_pDb, pSql, strlen(pSql) + 1, &pStmt, NULL);
				if (iRet == SQLITE_OK)
				{
					if (sqlite3_step(pStmt) == SQLITE_DONE)
					{
					}
					else
					{

						std::cout << sqlite3_errmsg(m_pDb) << std::endl;
					}

					sqlite3_finalize(pStmt);
				}
				else
				{
					std::cout << sqlite3_errmsg(m_pDb) << std::endl;
				}
				sqlite3_free(pSql);
			}
			else
				iRet = -2;
		}
	}

	return 0;

ERROR_1:

	return iRet;
}
//添加列

int baseSqlite::copyOneTableFromSrcDbToDstDb(std::string &pDstDbName, std::string &pTableDstName, std::string &pSrcDbName, std::string &pTableSrcName)
{
	int iRet = -1;
	char *pStr = NULL;
	sqlite3 *pDstDb = NULL;
	char *pErr = NULL;

	iRet = sqlite3_open_v2(pDstDbName.c_str(), &pDstDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

	if (iRet != SQLITE_OK)
	{
		std::cout << sqlite3_errmsg(m_pDb) << std::endl;
		goto end_1;
	}

	pStr = sqlite3_mprintf("ATTACH %s AS DbSrc;", pSrcDbName.c_str());
	if (pStr == NULL)
	{

		goto end_1;
	}

	iRet = sqlite3_exec(pDstDb, pStr, NULL, NULL, &pErr);

	sqlite3_free(pStr);

	if (pErr != NULL)
	{

		sqlite3_free(pErr);
		goto end_1;
	}

	pStr = sqlite3_mprintf("INSERT INTO %s SELECT * FROM DbSrc.%s;", pTableDstName.c_str(), pTableSrcName.c_str());
	if (pStr == NULL)
	{

		goto end_1;
	}

	iRet = sqlite3_exec(pDstDb, pStr, NULL, NULL, &pErr);

	sqlite3_free(pStr);

	if (pErr != NULL)
	{

		sqlite3_free(pErr);
		goto end_1;
	}

	if (iRet != SQLITE_OK)
	{

		goto end_1;
	}

	iRet = 0;

end_1:

	sqlite3_close(pDstDb);

	return iRet;
}


int baseSqlite::addColumToTable(const string &pTableName, tableParam_t &pInfo)
{
	int iRet = 0;
	if (isTableInDB(pTableName) == true) 
	{
		iRet = alterTable(pTableName, pInfo);
	}
	else
	{
		iRet = createTable(pTableName, pInfo);
	}
	return iRet;
}

int baseSqlite::insertRecord(const string &pTableName, insertParam_t *pInfo)
{
	char *pSql = NULL;
	char *pSqlCol = NULL;
	char *pSqlVal = NULL;
	char *pSqlValOld = NULL;
	char *pSqlColOld = NULL;

	sqlite3_stmt *pStmt = NULL;
	int iIndex = 0;
	int iRet = 0;
	int iType = 0;

	int iMaxNum = sizeof(pInfo->tRowValueInfo) / sizeof(pInfo->tRowValueInfo[0]);

	pInfo->bRowValueNum = caculateInfoNum(pInfo->tRowValueInfo, iMaxNum);

	if (pInfo->bRowValueNum > iMaxNum || pInfo->bRowValueNum == 0)
	{
		iRet = -1;
		goto ERROR_1;
	}

	if (m_pDb != NULL)
	{

		if (pInfo->bRowValueNum == 1)
		{
			pSqlCol = sqlite3_mprintf("(\"%s\" )", pInfo->tRowValueInfo[iIndex].pColName.c_str());
			pSqlVal = sqlite3_mprintf("values(? )");
		}
		else
		{
			pSqlCol = sqlite3_mprintf("(\"%s\" ", pInfo->tRowValueInfo[iIndex].pColName.c_str());
			pSqlVal = sqlite3_mprintf("values(? ");
		}

		if (pSqlVal == NULL || pSqlCol == NULL)
		{
			sqlite3_free(pSqlCol);
			sqlite3_free(pSqlVal);
			goto ERROR_1;
		}

		for (iIndex = 1; iIndex < pInfo->bRowValueNum; iIndex++)
		{
			pSqlColOld = pSqlCol;
			pSqlValOld = pSqlVal;

			if (iIndex < pInfo->bRowValueNum - 1)
			{
				pSqlCol = sqlite3_mprintf(" %s, \"%s\"", pSqlColOld, pInfo->tRowValueInfo[iIndex].pColName.c_str());
				pSqlVal = sqlite3_mprintf(" %s, ?", pSqlValOld);
			}
			else
			{
				pSqlCol = sqlite3_mprintf(" %s, \"%s\" )", pSqlColOld, pInfo->tRowValueInfo[iIndex].pColName.c_str());
				pSqlVal = sqlite3_mprintf(" %s,  ?)", pSqlValOld);
			}
			sqlite3_free(pSqlColOld);
			sqlite3_free(pSqlValOld);
		}

		pSql = sqlite3_mprintf("INSERT INTO %Q %s %s;", pTableName.c_str(), pSqlCol, pSqlVal);

		sqlite3_free(pSqlVal);
		sqlite3_free(pSqlCol);

		if (pSql != NULL)
		{
			iRet = sqlite3_prepare_v2(m_pDb, pSql, strlen(pSql), &pStmt, NULL);

			if (iRet == SQLITE_OK)
			{

				for (iIndex = 0; iIndex < pInfo->bRowValueNum; iIndex++)
				{

					iType = getColumnType(pTableName, pInfo->tRowValueInfo[iIndex].pColName);

					switch (iType)
					{
					case SQLITE_INTEGER:
						sqlite3_bind_int64(pStmt, iIndex + 1, pInfo->tRowValueInfo[iIndex].uRowValue.iValue);
						break;
					case SQLITE_FLOAT:
						sqlite3_bind_double(pStmt, iIndex + 1, pInfo->tRowValueInfo[iIndex].uRowValue.flValue);
						break;
					case SQLITE_TEXT:
						sqlite3_bind_text(pStmt, iIndex + 1, pInfo->tRowValueInfo[iIndex].uRowValue.bText, -1, SQLITE_STATIC);
						break;
					case SQLITE_BLOB:
						sqlite3_bind_blob(pStmt, iIndex + 1, pInfo->tRowValueInfo[iIndex].uRowValue.tBlob.bBlob,
								  pInfo->tRowValueInfo[iIndex].uRowValue.tBlob.blen, SQLITE_STATIC);
						break;
					default:
						sqlite3_bind_int64(pStmt, iIndex + 1, pInfo->tRowValueInfo[iIndex].uRowValue.iValue);
						break;
					}
				}

				if (sqlite3_step(pStmt) == SQLITE_DONE)
				{

					std::cout << "insert into sqlite3_step success!! " << std::endl;
				}
				else
				{
					std::cout << sqlite3_errmsg(m_pDb) << std::endl;
				}
				sqlite3_finalize(pStmt);
			}
			else
			{
				std::cout << sqlite3_errmsg(m_pDb) << std::endl;
			}

			sqlite3_free(pSql);
		}
	}

	return 0;

ERROR_1:
	return iRet;
}


int baseSqlite::deleteTable(const string &pTableName)
{
	char *pSql = NULL, *pErr = NULL;
	int iRet = 0;
	pSql = sqlite3_mprintf("DROP TABLE %Q", pTableName.c_str());

	if (m_pDb != NULL && pSql != NULL)
	{
		iRet = sqlite3_exec(m_pDb, pSql, NULL, NULL, &pErr);
		if (pErr != NULL)
		{

			sqlite3_free(pErr);
			iRet = -1;
		}
	}
	else
	{

		iRet = -2;
	}

	if (pSql != NULL)
	{
		sqlite3_free(pSql);
	}

	return iRet;
}
int baseSqlite::updateRecordMethod2(string &pTableName, updateParam_t *pInfo)
{

	return 0;
}


int baseSqlite::updateRecordMethod1(string &pTableName, updateParam_t *pInfo)
{
	char *pSql = NULL, *pSqlTmp = NULL, *pSqlWhere = NULL;
	sqlite3_stmt *pStmt = NULL;

	int iRet = 0;
	int i = 0;

	char bType = 0;
	int iMaxNum = sizeof(pInfo->tColumInfoTable) / sizeof(pInfo->tColumInfoTable[0]);

	if (pInfo == NULL)
	{

		iRet = -1;
		goto ERROR_1;
	}

	pInfo->bColumNum = caculateInfoNum(pInfo->tColumInfoTable, iMaxNum);

	if (pInfo->bColumNum > iMaxNum || pInfo->bColumNum == 0)
	{

		iRet = -2;
		goto ERROR_1;
	}

	pInfo->bRowValueNum = caculateInfoNum(pInfo->tRowValueInfo, iMaxNum);
	iMaxNum = sizeof(pInfo->tRowValueInfo) / sizeof(pInfo->tRowValueInfo[0]);
	if (pInfo->bRowValueNum > iMaxNum || pInfo->bRowValueNum == 0)
	{

		iRet = -3;
		goto ERROR_1;
	}

	bType = getColumnType(pTableName, pInfo->tColumInfoTable[i].pColName);

	switch (bType)
	{
	case SQLITE_INTEGER:
		pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%d ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
		break;
	case SQLITE_FLOAT:
		pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%f ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.flValue);
		break;
	case SQLITE_TEXT:
		pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==\'%s\' ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.bText);
		break;
	case SQLITE_BLOB:
		pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%s ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.tBlob.bBlob);

		pSqlTmp = pSql = NULL;
		break;
	default:
		pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%d ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
		break;
	}

	//pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%d ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);

	for (i = 1; i < pInfo->bColumNum; i++)
	{
		if (pSqlTmp != NULL && !pInfo->tColumInfoTable[i].pColName.empty())
		{
			bType = getColumnType(pTableName, pInfo->tColumInfoTable[i].pColName);
			switch (bType)
			{
			case SQLITE_INTEGER:
				pSqlTmp = pSql = sqlite3_mprintf("and \"%s\"==%d ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
				break;
			case SQLITE_FLOAT:
				pSqlTmp = pSql = sqlite3_mprintf("and \"%s\"==%f ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.flValue);
				break;
			case SQLITE_TEXT:
				pSqlTmp = pSql = sqlite3_mprintf("and \"%s\"==\'%s\' ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.bText);
				break;
			case SQLITE_BLOB:
				pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%s ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.tBlob.bBlob);

				pSqlTmp = pSql = NULL;
				break;
			default:
				pSqlTmp = pSql = sqlite3_mprintf("and \"%s\"==%d ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
				break;
			}
			//sqlite3_free(pSqlTmp);
			pSqlTmp = pSql;
		}
		else
		{

			break;
		}
	}

	if (pSqlTmp != NULL)
	{
		pSql = sqlite3_mprintf("%s ;", pSqlTmp);
		sqlite3_free(pSqlTmp);
	}
	else
	{

		std::cout << "error" << std::endl;
	}

	pSqlWhere = pSql;

	if (pSql != NULL)
	{

		for (i = 0; i < pInfo->bRowValueNum; i++)
		{

			bType = getColumnType(pTableName, pInfo->tRowValueInfo[i].pColName);
			switch (bType)
			{
			case SQLITE_INTEGER:
				pSql = sqlite3_mprintf("UPDATE %Q SET \"%s\"=%d %s", pTableName.c_str(), pInfo->tRowValueInfo[i].pColName, pInfo->tRowValueInfo[i].uRowValue.iValue, pSqlWhere);
				break;
			case SQLITE_FLOAT:
				pSql = sqlite3_mprintf("UPDATE %Q SET \"%s\"=%f %s", pTableName.c_str(), pInfo->tRowValueInfo[i].pColName, pInfo->tRowValueInfo[i].uRowValue.flValue, pSqlWhere);
				break;
			case SQLITE_TEXT:
				pSql = sqlite3_mprintf("UPDATE %Q SET \"%s\"=\'%s\' %s", pTableName.c_str(), pInfo->tRowValueInfo[i].pColName, pInfo->tRowValueInfo[i].uRowValue.bText, pSqlWhere);
				break;
			case SQLITE_BLOB:
				pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"==%s ", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.tBlob.bBlob);

				pSql = NULL;
				break;
			default:
				pSql = sqlite3_mprintf("UPDATE %Q SET \"%s\"=%d %s", pTableName.c_str(), pInfo->tRowValueInfo[i].pColName, pInfo->tRowValueInfo[i].uRowValue.iValue, pSqlWhere);
				break;
			}

			if (pSql != NULL)
			{

				iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
				if (iRet == SQLITE_OK)
				{
					if (sqlite3_step(pStmt) == SQLITE_DONE)
					{

						std::cout << "execute succeed" << std::endl;
						
					}
					else
					{

						std::cout << sqlite3_errmsg(m_pDb) << std::endl;
					}
					sqlite3_finalize(pStmt);
				}
				else
				{
					std::cout << sqlite3_errmsg(m_pDb) <<std::endl;
				}

				sqlite3_free(pSql);
			}
		}

		sqlite3_free(pSqlWhere);
	}

	return 0;

ERROR_1:

	return iRet;
}

bool baseSqlite::deleteAllRecord(const std::string &pTableName)
{
	char *pSql = NULL;
	sqlite3_stmt *pStmt = NULL;
	bool fRet = false;

	pSql = sqlite3_mprintf("delete from %Q;", pTableName.c_str());

	if (pSql != NULL)
	{
		int iRet = 0;
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			if (sqlite3_step(pStmt) != SQLITE_DONE)
			{
				std::cout << sqlite3_errmsg(m_pDb) << std::endl;
			}
			else
				fRet = true;

			sqlite3_finalize(pStmt);
		}
		else
		{
			std::cout << sqlite3_errmsg(m_pDb) << std::endl;
		}
		sqlite3_free(pSql);
	}

	return fRet;
}


int baseSqlite::deleteOldestRecord(std::string &pTableName)
{
	char *pSql = NULL;
	sqlite3_stmt *pStmt = NULL;

	pSql = sqlite3_mprintf("delete from %Q where rowid in (select  max(rowid) from %Q);", pTableName.c_str(), pTableName.c_str());

	if (pSql != NULL)
	{
		int iRet = 0;
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			if (sqlite3_step(pStmt) != SQLITE_DONE)
			{
				std::cout << sqlite3_errmsg(m_pDb) <<std::endl;
			}

			sqlite3_finalize(pStmt);
		}
		else
		{
			std::cout << sqlite3_errmsg(m_pDb) <<std::endl;
		}
		sqlite3_free(pSql);
	}

	return 0;
}

typedef struct stringType2NumMap_s
{
	int iType;
	const string pbTypeName1;
	const string pbTypeName2;
} stringType2NumMap_t;

stringType2NumMap_t TypeToNumTable[] = {
    {SQLITE_INTEGER, "INTEGER", "integer"},
    {SQLITE_FLOAT, "FLOAT", "float"},
    {SQLITE_TEXT, "TEXT", "text"},
    {SQLITE_BLOB, "BLOB", "blob"},
};


int baseSqlite::convertTypeStringToNum(const std::string &pbTypeName)
{
	int iRet = SQLITE_NULL;
	int i = 0;
	for (i = 0; i < sizeof(TypeToNumTable) / sizeof(TypeToNumTable[0]); i++)
	{
		//if (strcmp(TypeToNumTable[i].pbTypeName1, pbTypeName) == 0 || strcmp(TypeToNumTable[i].pbTypeName2, pbTypeName) == 0)
		if (TypeToNumTable[i].pbTypeName1.compare(pbTypeName)==0 || TypeToNumTable[i].pbTypeName2.compare(pbTypeName) == 0)
		{
			iRet = TypeToNumTable[i].iType;
			break;
		}
	}

	if (i >= sizeof(TypeToNumTable) / sizeof(TypeToNumTable[0]))
	{

		std::cout << " not math type \n"
		     <<std::endl;
	}

	return iRet;
}
//将输入的字符串转变为sqlite的那4种类型的枚举

int baseSqlite::getColumnType(const std::string &pTableName, const std::string &pColumName)
{
	char *pSql = NULL;
	int iColNum = 0;
	int iRet = 0;
	const char *pTabColName = NULL;
	const char *pbTypeName = NULL;

	sqlite3_stmt *pStmt = NULL;

	if (pTableName.empty() || pColumName.empty())
	{
		iRet = -1;
		goto ERROR_1;
	}

	pSql = sqlite3_mprintf("SELECT %s from %Q;", pColumName.c_str(), pTableName.c_str());

	if (pSql != NULL)
	{

		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);
			iColNum = sqlite3_column_count(pStmt);
			pbTypeName = sqlite3_column_decltype(pStmt, 0);

			iRet = convertTypeStringToNum(pbTypeName);

			sqlite3_finalize(pStmt);
		}

		sqlite3_free(pSql);
	}

	return iRet;
ERROR_1:

	return iRet;
}
//获取该表中该列的数据类型

bool baseSqlite::isTableInDB(const std::string &tableName)
{
	char *pSql = NULL;
	int iColNum = 0;
	int iRet = 0;
	bool fRet = false;

	sqlite3_stmt *pStmt = NULL;

	if (tableName.empty())
	{
		return false;
	}

	pSql = sqlite3_mprintf("SELECT name from sqlite_master where name==\'%s\';", tableName.c_str());

	if (pSql != NULL)
	{
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);

			if (iRet == SQLITE_ROW)
			{
				//std::cout << "table " << pTableName.c_str() << " is exist" << std::endl;
				fRet = true;
			}

		}

		sqlite3_finalize(pStmt);
		sqlite3_free(pSql);
	}

	return fRet;
}
//知道在数据库里有没有表

bool baseSqlite::isColumnNameInTable(const std::string &pTableName, const std::string &pColumName)
{
	char *pSql = NULL;
	int iColNum = 0;
	int iRet = 0;
	const char *pTabColName = NULL;
	bool fRet = false;

	sqlite3_stmt *pStmt = NULL;

	if (pTableName.empty() || pColumName.empty())
	{
		iRet = -1;
		goto ERROR_1;
	}

	pSql = sqlite3_mprintf("SELECT * from %Q;", pTableName.c_str());

	if (pSql != NULL)
	{

		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);
			iColNum = sqlite3_column_count(pStmt);

			for (int i = 0; i < iColNum; i++)
			{
				pTabColName = sqlite3_column_name(pStmt, i);
				if (strcmp(pTabColName, pColumName.c_str()) == 0)
				{
					fRet = true;
					break;
				}
			}

			sqlite3_finalize(pStmt);
		}

		sqlite3_free(pSql);
	}

	return fRet;
ERROR_1:

	return fRet;
}
//查询在表中是否有这一列

bool baseSqlite::getColumnNumInTable(std::string &pTableName, int *piColumNum)
{
	char *pSql = NULL;
	int iColNum = 0;
	int iRet = 0;
	const char *pTabColName = NULL;
	bool fRet = false;

	sqlite3_stmt *pStmt = NULL;

	if (pTableName.empty())
	{

		iRet = -1;
		goto ERROR_1;
	}

	pSql = sqlite3_mprintf("SELECT * from %Q;", pTableName.c_str());

	if (pSql != NULL)
	{

		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);
			iColNum = sqlite3_column_count(pStmt);
			*piColumNum = iColNum;
			sqlite3_finalize(pStmt);
		}
		sqlite3_free(pSql);
	}

	return fRet;
ERROR_1:

	return fRet;
}
//获取表里列的数量

int baseSqlite::selectOneRecordData(const std::string &pTableName, selectParam_t *pInfo)
{
	/*
	char *pSql=NULL, *pSqlTmp=NULL, *pSqlWhere=NULL;
	int iColNum = 0;
	int iRet = 0;
	int i=0, iType=0;
	char *pbData;

	int iMaxNum = sizeof(pInfo->tColumInfoTable)/sizeof(pInfo->tColumInfoTable[0]);

	if (pInfo == NULL)
	{
	
		iRet = -1;
		goto ERROR_1;
       	}


	pInfo->bColumNum = caculateInfoNum(pInfo->tColumInfoTable, iMaxNum);

	if (pInfo->bColumNum > iMaxNum || pInfo->bColumNum == 0)
	{
	
		iRet = -2;
		goto ERROR_1;
	}


	iType = getColumnType(pTableName, pInfo->tColumInfoTable[i].pColName);
	switch (iType)
	{
		case SQLITE_INTEGER:
			pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"=%d", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
			break;
		case SQLITE_FLOAT:
			pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"=%f", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.flValue);
			break;
		case SQLITE_TEXT:
			pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"=%s", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.bText);
			break;
		case SQLITE_BLOB:
			pSqlTmp = pSql = sqlite3_mprintf("where \"%s\"=%s", pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.tBlob.bBlob[0]);
			
			break;
		case SQLITE_NULL:
			
			pInfoResult->tRowValueInfo[i].uRowValue.iValue = -1;
			break;
		default:
			
			break;
	}

	
	

	for(i=1; i<pInfo->bColumNum; i++)
	{
		if (pSqlTmp != NULL && pInfo->tColumInfoTable[i].pColName != NULL)
		{
			iType = getColumnType(pTableName, pInfo->tColumInfoTable[i].pColName);
			switch (iType)
			{
				case SQLITE_INTEGER:
					pSql = sqlite3_mprintf("%s and \"%s\"=%d", pSqlTmp, pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.iValue);
					break;
				case SQLITE_FLOAT:
					pSql = sqlite3_mprintf("%s and \"%s\"=%f", pSqlTmp, pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.flValue);
					break;
				case SQLITE_TEXT:
					pSql = sqlite3_mprintf("%s and \"%s\"=%s", pSqlTmp, pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.bText);
					break;
				case SQLITE_BLOB:
					pSql = sqlite3_mprintf("%s and \"%s\"=%d", pSqlTmp, pInfo->tColumInfoTable[i].pColName, pInfo->tColumInfoTable[i].uRowValue.tBlob.bBlob[0]);
				
					break;
				case SQLITE_NULL:
				
					pInfoResult->tRowValueInfo[i].uRowValue.iValue = -1;
					break;
				default:
					
					break;
			}
			sqlite3_free(pSqlTmp);
			pSqlTmp = pSql;
		}
		else
		{

			break;
		}

	}

	if (pSqlTmp != NULL)
	{
		pSql = sqlite3_mprintf("%s", pSqlTmp);
		sqlite3_free(pSqlTmp);
	}
	else
	{
		cout << "error" std::endl;
	}

	pSqlWhere = pSql;

	if (pSqlWhere != NULL)
	{
		sqlite3_stmt *pStmt = NULL;

		iMaxNum = sizeof(pInfoResult->tRowValueInfo)/sizeof(pInfoResult->tRowValueInfo[0]);

		pInfoResult->bRowValueNum = caculateInfoNum((cell_t*)pInfoResult->tRowValueInfo, iMaxNum);

		for (i=0; i<pInfoResult->bRowValueNum; i++)
		{

			if (isColumnNameInTable(pTableName, pInfoResult->tRowValueInfo[i].pColName)==true)
			{

				pSql = sqlite3_mprintf("SELECT \"%s\" from %Q %s ;", pInfoResult->tRowValueInfo[i].pColName, pTableName, pSqlWhere);
				

				if (pSql != NULL)
				{
					iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
					if (iRet == SQLITE_OK)
					{
						iRet = sqlite3_step(pStmt);
						

						if(iRet == SQLITE_ROW)
						{
							iType = sqlite3_column_type(pStmt, 0);
							switch (iType)
							{
								case SQLITE_INTEGER:
									pInfoResult->tRowValueInfo[i].uRowValue.iValue = sqlite3_column_int64(pStmt, 0);
									break;
								case SQLITE_FLOAT:
									pInfoResult->tRowValueInfo[i].uRowValue.flValue = sqlite3_column_double(pStmt, 0);
									break;
								case SQLITE_TEXT:
									pbData = (char *)sqlite3_column_text(pStmt, 0);
									memcpy(pInfoResult->tRowValueInfo[i].uRowValue.bText, pbData,strlen(pbData));
									break;
								case SQLITE_BLOB:
									iRet = sqlite3_column_bytes(pStmt, 0);
									pbData = (char *)sqlite3_column_blob(pStmt, 0);
									memcpy(pInfoResult->tRowValueInfo[i].uRowValue.tBlob.bBlob, pbData, iRet);
									pInfoResult->tRowValueInfo[i].uRowValue.tBlob.blen = iRet;
									break;
								case SQLITE_NULL:
									
									pInfoResult->tRowValueInfo[i].uRowValue.iValue = -1;
									break;
								default:
								
									break;
							}
						}
						else
						{
							pInfoResult->tRowValueInfo[i].uRowValue.iValue = -1;
						}
						


						sqlite3_finalize(pStmt);
					}
					else
					{
						cout << "error" std::endl;
					}




					sqlite3_free(pSql);
				}

			}
			else
			{
				pInfoResult->tRowValueInfo[i].uRowValue.iValue = -1;
			}

		}

		sqlite3_free(pSqlWhere);
	}

	return 0;

ERROR_1:

	return iRet;*/
	return 0;
}


int baseSqlite::selectAllRecordData(const std::string &pTableName, std::vector<selectResult_t> &tableData, selectParam_t *pInfo, bool fAllData)
{
	char *pSql = NULL, *pSqlTmp = NULL, *pSqlWhere = NULL;
	int iColNum = 0;
	int iRet = 0;
	int i = 0, iType = 0;
	int k = 0;
	char *pbData;
	int iRecordNum = 0;
	sqlite3_stmt *pStmt = 0;
	iRecordNum = getDBRecordNum(pTableName);
	selectResult_t tempInfo;
	memset(&tempInfo, 0, sizeof(selectResult_t));
	if (iRecordNum > 0)
	{
		if (m_pAllData == NULL)
		{
			m_pAllData = new selectResult_t[iRecordNum];
			memset(m_pAllData, 0, sizeof(m_pAllData[0]) * iRecordNum);
		}
		else
		{
			delete[] m_pAllData;
			m_pAllData = new selectResult_t[iRecordNum];
			memset(m_pAllData, 0, sizeof(m_pAllData[0]) * iRecordNum);
		}
	}
	else
	{
		return -1;
	}
	if (pInfo == NULL && fAllData == false)
	{

		iRet = -1;
		//goto ERROR_1;
	}
	if (fAllData == true)
	{

		pSql = sqlite3_mprintf("SELECT * from %Q;", pTableName.c_str());

		if (pSql != NULL)
		{

			iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
			if (iRet == SQLITE_OK)
			{
				iRet = sqlite3_step(pStmt);
				int iNcols = sqlite3_column_count(pStmt);

				while (iRet == SQLITE_ROW)
				{
					memset(&tempInfo, 0, sizeof(selectResult_t));
					for (int i = 0; i < iNcols; i++)
					{
						const char *name = sqlite3_column_name(pStmt, i);

						memcpy(tempInfo.tRowValueInfo[i].pColName, name, strlen(name));
						iType = sqlite3_column_type(pStmt, i);
						switch (iType)
						{
						case SQLITE_INTEGER:
							tempInfo.tRowValueInfo[i].uRowValue.iValue = sqlite3_column_int64(pStmt, i);
							break;
						case SQLITE_FLOAT:
							tempInfo.tRowValueInfo[i].uRowValue.flValue = sqlite3_column_double(pStmt, i);
							break;
						case SQLITE_TEXT:
							pbData = (char *)sqlite3_column_text(pStmt, i);
							strcpy(tempInfo.tRowValueInfo[i].uRowValue.bText, pbData);
							//strcpy_s(tempInfo.tRowValueInfo[i].uRowValue.bText, pbData);
							break;
						case SQLITE_BLOB:
							iRet = sqlite3_column_bytes(pStmt, i);
							pbData = (char *)sqlite3_column_blob(pStmt, i);
							memcpy(tempInfo.tRowValueInfo[i].uRowValue.tBlob.bBlob, pbData, iRet);
							tempInfo.tRowValueInfo[i].uRowValue.tBlob.blen = iRet;
							break;
						case SQLITE_NULL:

							tempInfo.tRowValueInfo[i].uRowValue.iValue = -1;
							break;
						default:

							break;
						}
						tempInfo.tRowValueInfo[i].bType = iType;
					}
					tempInfo.bRowValueNum = iNcols;
					tableData.push_back(tempInfo);

					//memcpy(&m_pAllData[k], &tempInfo, sizeof(m_pAllData[k]));
					m_iRecordNum++;
					iRet = sqlite3_step(pStmt);
					k++;
				}
				sqlite3_finalize(pStmt);
			}
			sqlite3_free(pSql);
		}
	}
	//pInfoResult = m_pAllData;
	return iRet;

}

//获取表中数据

int baseSqlite::updateRecord(int iMethod, string &pTableName, updateParam_t *pInfo)
{
	int iRet = 0;
	switch (iMethod)
	{
	case 0:
		iRet = updateRecordMethod1(pTableName, pInfo);
		break;
	case 1:
		iRet = updateRecordMethod2(pTableName, pInfo); //����չ
		break;
	default:

		iRet = -1;
		break;
	}

	return iRet;
}
//修改表中数据

int baseSqlite::selectRecordData(int iMethod, const std::string &pTableName, std::vector<selectResult_t> &tableData, bool fAllData, selectParam_t *pInfo)
{
	int iRet = 0;
	switch (iMethod)
	{
	case GET_ONE_RECORD:
		iRet = selectOneRecordData(pTableName, pInfo);
		break;
	case GET_ALL_RECORD:
		iRet = selectAllRecordData(pTableName, tableData, pInfo, fAllData);
		break;
	default:

		iRet = -1;
		break;
	}

	return iRet;
}

//选择表中数据

int baseSqlite::getAllData(const std::string &pTableName, std::vector<selectResult_t> &tableData)
{
	int iRet = 0;
	m_iRecordNum = 0;

	iRet = selectRecordData(GET_ALL_RECORD, pTableName, tableData, true);
	return iRet;
}
//获取选择表中数据

int baseSqlite::getDBRecordNum(const std::string &pTableName)
{
	char *pSql = NULL, *pSqlTmp = NULL, *pSqlWhere = NULL;
	int iRet = 0;
	int iDataNum = 0;
	sqlite3_stmt *pStmt = 0;
	pSql = sqlite3_mprintf("SELECT * from %Q;", pTableName.c_str());
	if (pSql != NULL)
	{
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			while (sqlite3_step(pStmt) == SQLITE_ROW)
			{
				iDataNum++;
			}
		}
		sqlite3_finalize(pStmt);
	}
	return iDataNum;
}
//获取表中记录的个数

bool baseSqlite::dataIsExist(const std::string &pTableName, const std::string &pColumName, const int iType, const unDataStruct &dataStr)
{
	char *pSql = NULL;
	char *pbData = NULL;
	const char *pbSour = dataStr.bText;
	int iRet = 0;
	int iGetType;
	sqlite3_stmt *pStmt = 0;
	if (getDBRecordNum(pTableName) <= 0)
	{
		return false;
	}

	switch (iType)
	{
	case SQLITE_INTEGER:
		pSql = sqlite3_mprintf("SELECT * from %s where %s  like %d%;", pTableName.c_str(), pColumName.c_str(), dataStr.iValue);
		break;
	case SQLITE_FLOAT:

		break;
	case SQLITE_TEXT:
		pSql = sqlite3_mprintf("SELECT * from %s where %s  like %Q%;", pTableName.c_str(), pColumName.c_str(), pbSour);
		break;
	case SQLITE_BLOB:

		break;
	case SQLITE_NULL:
		break;
	default:

		break;
	}

	if (pSql != NULL)
	{
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);
			int iNcols = sqlite3_column_count(pStmt);

			while (iRet == SQLITE_ROW)
			{
				for (int i = 0; i < iNcols; i++)
				{
					const char *name = sqlite3_column_name(pStmt, i);

					if (strcmp(pColumName.c_str(), name) == 0)
					{
						iGetType = sqlite3_column_type(pStmt, i);
						switch (iGetType)
						{
						case SQLITE_INTEGER:
							if (dataStr.iValue == sqlite3_column_int64(pStmt, i))
							{
								sqlite3_finalize(pStmt);
								sqlite3_free(pSql);
								return true;
							}
							break;
						case SQLITE_FLOAT:

							break;
						case SQLITE_TEXT:
							pbData = (char *)sqlite3_column_text(pStmt, i);
							if (strcmp(dataStr.bText, pbData) == 0)
							{
								sqlite3_finalize(pStmt);
								sqlite3_free(pSql);
								return true;
							}
							break;
						case SQLITE_BLOB:

							break;
						case SQLITE_NULL:

							break;
						default:

							break;
						}
					}
				}
				iRet = sqlite3_step(pStmt);
			}
			sqlite3_finalize(pStmt);
		}
		sqlite3_free(pSql);
	}
	return false;
}

//查找某个数据是否存在

int baseSqlite::getMaxId(const std::string &pTableName)
{
	char *pSql = NULL;
	char *pbData = NULL;
	const char *pbSour = "id";
	int iRet = 0;
	int iReco = 0;
	sqlite3_stmt *pStmt = 0;
	if ((iReco=getDBRecordNum(pTableName) )<= 0)
	{
		return 0;
	}
	pSql = sqlite3_mprintf("select max(%s) from %s;", pbSour, pTableName.c_str());

	std::cout<<"getDBRecordNum = "<< iReco<<"pTableName = "<<pTableName<<" pSql = "<<pSql<<std::endl;
	if (pSql != NULL)
	{
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{	
			iRet = sqlite3_step(pStmt);
			int iNcols = sqlite3_column_count(pStmt);
			for (int i = 0; i < iNcols; i++)
			{
				const char *name = sqlite3_column_name(pStmt, i);
				std::cout<<"pbSour ="<< pbSour<<"name = "<<name <<std::endl;
				//if (strcmp(pbSour, name) == 0)
				// if (strcmp("max(id)", name) == 0)
				// {	
					iRet = 	sqlite3_column_int64(pStmt, i);
					sqlite3_finalize(pStmt);
					sqlite3_free(pSql);
					
					std::cout<<"get id = "<<iRet<<std::endl;
					return iRet;
				//}	
			}
			sqlite3_finalize(pStmt);
		}
		sqlite3_free(pSql);
	}
	return iRet;
}
//获得最大的id值

bool baseSqlite::deleteRecordByValue(const std::string &pTableName, const std::string &pColumName, const int iType, const unDataStruct &dataStr)
{
	char *pSql = NULL;
	char *pbData = NULL;
	const char *pbSour = dataStr.bText;
	int iRet = 0;
	int iGetType;
	sqlite3_stmt *pStmt = 0;
	bool fRet = false;
	if (getDBRecordNum(pTableName) <= 0)
	{
		return false;
	}
	//DELETE FROM COMPANY WHERE ID = 7;
	switch (iType)
	{
	case SQLITE_INTEGER:
		pSql = sqlite3_mprintf("DELETE  from %s where %s=%d%;", pTableName.c_str(), pColumName.c_str(), dataStr.iValue);
		break;
	case SQLITE_FLOAT:

		break;
	case SQLITE_TEXT:
		pSql = sqlite3_mprintf("DELETE  from %s where %s=%Q%;", pTableName.c_str(), pColumName.c_str(), pbSour);
		break;
	case SQLITE_BLOB:

		break;
	case SQLITE_NULL:
		break;
	default:

		break;
	}

	if (pSql != NULL)
	{
		iRet = sqlite3_prepare_v2(m_pDb, pSql, -1, &pStmt, NULL);
		if (iRet == SQLITE_OK)
		{
			iRet = sqlite3_step(pStmt);
			int iNcols = sqlite3_column_count(pStmt);

// 			while (iRet == SQLITE_ROW)
// 			{
// 				iRet = sqlite3_step(pStmt);
// 				cout << "delete success 777777777777777" std::endl;
// 			}
			if (iRet == SQLITE_DONE)
			{
				fRet = true;
			}
			sqlite3_finalize(pStmt);
		}
		sqlite3_free(pSql);
	}
	return fRet;
}
//删除符合条件的那一行