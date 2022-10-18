#include "palmVeinDbApi.h"
#include "palmVeinDb.h"

bool palmVeinDbInit()
{
	bool fRet = false;

	fRet = palmVeinDb::getInstance();
	if (!fRet)
	{
		std::cout << "new palmVeinDb failed" << std::endl;
		return fRet;
	}
	fRet = palmVeinDb::initDataBase();
	if (!fRet)
	{
		std::cout << "palmVeinDb init failed" <<std::endl;
	}

	return fRet;
}

bool insertOneRecord(int tableCode, void *dataStruct)
{
	bool fRet = false;
	if (tableCode < 0)
	{
		return fRet;
	}
	const std::string tableName = getTableNameByCode(tableCode);
	if (tableName.empty())
	{
		return fRet;
	}
	fRet = palmVeinDb::insertOneRecord(tableName, dataStruct);

	return fRet;
}



bool synData(int tableCode, std::vector<selectResult_t> &tableData)
{
	bool fRet = false;
	if (tableCode < 0)
	{
		return fRet;
	}

	const std::string tableName = getTableNameByCode(tableCode);
	if (tableName.empty())
	{
		return fRet;
	}
	palmVeinDb::synTableData(tableName, tableData);
	return true;
}

std::string getTableNameByCode(int tableCode)
{
     std::string tableName;
	if (tableCode < 0)
	{
		return NULL;
	}
	switch (tableCode)
	{
	case FEATURE_TABLE:
		tableName = g_pFeaTable;
		break;
	default:
		break;
	}
	return tableName;
}

bool getFeatureTableData(std::vector<featureTable> &featureData)
{
	std::vector<selectResult_t> tableAllData;
	tableAllData.clear();
	featureData.clear();
	synData(FEATURE_TABLE, tableAllData);
	featureTable pFeaStGet;
	for (auto iter = tableAllData.begin(); iter != tableAllData.end(); iter++)
	{
		memset(&pFeaStGet, 0, sizeof(pFeaStGet));
		for (int i = 0; i < iter->bRowValueNum; i++)
		{
			if(strcmp(iter->tRowValueInfo[i].pColName,"index")==0)
			{	
				pFeaStGet.index = iter->tRowValueInfo[i].uRowValue.iValue;	
			}
			else if(strcmp(iter->tRowValueInfo[i].pColName,"id")==0)
			{
				pFeaStGet.id = iter->tRowValueInfo[i].uRowValue.iValue;	
			}
			else if(strcmp(iter->tRowValueInfo[i].pColName,"path")==0)
			{
				strcpy(pFeaStGet.path, iter->tRowValueInfo[i].uRowValue.bText);
			}
			else if(strcmp(iter->tRowValueInfo[i].pColName,"name")==0)
			{
				strcpy(pFeaStGet.name, iter->tRowValueInfo[i].uRowValue.bText);
			}
			else if(strcmp(iter->tRowValueInfo[i].pColName,"feature")==0)
			{
				memcpy(pFeaStGet.feature, iter->tRowValueInfo[i].uRowValue.tBlob.bBlob, iter->tRowValueInfo[i].uRowValue.tBlob.blen);
			}
		}
		featureData.push_back(pFeaStGet);
	}
	return true;
}


bool isExistInPalmVeinTable(const int iIntValue,const int iType)
{   
    queryInfo querySt;
    querySt.iValue = iIntValue;
    querySt.nameType = iType;
    querySt.sqliteType = SQLITE_INTEGER;
	return palmVeinDb::isExistInPvTb(querySt);
}

bool isExistInPalmVeinTable(const std::string sStr,const int iType)
{   
    queryInfo querySt;
    querySt.strValue = sStr;
    querySt.nameType = iType;
    querySt.sqliteType = SQLITE_TEXT;
	return palmVeinDb::isExistInPvTb(querySt);
}

bool isExistInPalmVeinTable(const char *pStr,const int iType)
{   
    queryInfo querySt;
    querySt.strValue = pStr;
    querySt.nameType = iType;
    querySt.sqliteType = SQLITE_TEXT;
	return palmVeinDb::isExistInPvTb(querySt);
}


bool deleteTbAllData(int tableCode)
{
	bool fRet = false;
	if (tableCode < 0)
	{
		return fRet;
	}

	const std::string tableName = getTableNameByCode(tableCode);
	if (tableName.empty())
	{
		return fRet;
	}
	palmVeinDb::clearTableData(tableName);
	return true;
}
int getMaxIdValue(int tableCode)
{
	int iRet = 0;
	if (tableCode < 0)
	{
		return iRet;
	}

	const std::string tableName = getTableNameByCode(tableCode);
	if (tableName.empty())
	{
		return iRet;
	}
	iRet = palmVeinDb::getMavIdValue(tableName);
	return iRet;
}

bool deleteRecordInPalmVeinTable(const deleteInfo &deleteData)
{
	return palmVeinDb::deleteRecordByValuePvTb(deleteData);
}