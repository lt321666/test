/*********************************************************************************************************
* �ļ����ƣ�DataObject.h
* ժ    Ҫ�����ļ���Ҫ����ʵ�����ݿ��������
* ��ǰ�汾��1.0
* ���ڣ�2021-08-13
*********************************************************************************************************/

#ifndef _BASESQLITE_H_
#define _BASESQLITE_H_

#include "sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
//using namespace std;

#define TYPE_INTEGER SQLITE_INTEGER
#define TYPE_FLOAT SQLITE_FLOAT
#define TYPE_TEXT SQLITE_TEXT
#define TYPE_BLOB SQLITE_BLOB

#define DB_TYPE_INTEGER SQLITE_INTEGER
#define DB_TYPE_FLOAT SQLITE_FLOAT
#define DB_TYPE_TEXT SQLITE_TEXT
#define DB_TYPE_BLOB SQLITE_BLOB

#define GET_ONE_RECORD 0
#define GET_ALL_RECORD 1
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef struct
{
	char bBlob[2048];
	int blen;
} bBlob_t;

typedef union
{
	int iValue;
	double flValue;
	char bText[200];
	bBlob_t tBlob;
} value_u;

typedef struct
{
	std::string pColName;  //col name
	value_u uRowValue;
	BYTE bType;         //data type
	bool fUnique;       //is only one
	BYTE bPrimary;      // origin value
} cell_t;

typedef struct
{
	char pColName[100];
	value_u uRowValue;
	BYTE bType;
	bool fUnique;
	BYTE bPrimary;
} cellQuery_t;

typedef struct
{
	cell_t tColumInfoTable[50];//命令字段
	BYTE bColumNum;
} tableParam_t;

typedef struct
{
	cell_t tColumInfoTable[10];
	BYTE bColumNum;

	cell_t tRowValueInfo[50];
	BYTE bRowValueNum;

} updateParam_t;

typedef struct
{
	cell_t tRowValueInfo[50];
	BYTE bRowValueNum;
} insertParam_t;

typedef struct
{
	cell_t tColumInfoTable[10];
	BYTE bColumNum;
} selectParam_t;

typedef struct
{
	cellQuery_t tRowValueInfo[50];
	BYTE bRowValueNum;
} selectResult_t;

typedef struct
{
	std::string pColName;
	BYTE bType;
	bool fUnique;
	BYTE bPrimary;
} memberInfo_t;

typedef struct
{
	int a;
} bSt;

typedef union
{
	int iValue;
	double flValue;
	char bText[300];
	bBlob_t tBlob;
} unDataStruct;

class baseSqlite
{

public:
	baseSqlite(void);
	~baseSqlite();

	int getAllData(const std::string &pTableName, std::vector<selectResult_t> &tableData);
	int selectRecordData(int iMethod, const std::string &pTableName, std::vector<selectResult_t> &tableData, bool fAllData = false, selectParam_t *pInfo = NULL);
	int selectAllRecordData(const std::string &pTableName, std::vector<selectResult_t> &tableData, selectParam_t *pInfo, bool fAllData = false);

	bool openDataBase(const std::string &dbName);
	bool createTable(const std::string &pTableName, tableParam_t& pInfo);

	int closeDataBase(void);

	int addColumToTable(const std::string &pTableName, tableParam_t &pInfo);
	int alterTable(const std::string &pTableName, tableParam_t &pInfo);
	bool isColumnNameInTable(const std::string &pTableName, const std::string &pColumName);
	bool getColumnNumInTable(std::string &pTableName, int *piColumNum);

	bool isTableInDB(const std::string &tableName);

	
	int deleteTable(const std::string &pTableName);

	int copyOneTableFromSrcDbToDstDb(std::string &pDstDbName, std::string &pTableDstName, std::string &pSrcDbName, std::string &pTableDbName);

	int insertRecord(const std::string &pTableName, insertParam_t *pInfo);
	int deleteOldestRecord(std::string &pTableName);
	bool deleteAllRecord(const std::string &pTableName);
	int updateRecord(int iMethod, std::string &pTableName, updateParam_t *pInfo);

	int selectOneRecordData(const std::string &pTableName, selectParam_t *pInfoParam);

	int getDBRecordNum(const std::string &pTableName);

	bool dataIsExist(const std::string &pTableName, const std::string &pColumName, const int iType, const unDataStruct &dataStr);
	bool deleteRecrdByValue(const std::string &pTableName, const std::string &pColumName, const int iType, const unDataStruct &dataStr);
    bool deleteRecordByValue(const std::string &pTableName, const std::string &pColumName, const int iType, const unDataStruct &dataStr);
	int getMaxId(const std::string &pTableName);
private:
	int updateRecordMethod1(std::string &pTableName, updateParam_t *pInfo);
	int updateRecordMethod2(std::string &pTableName, updateParam_t *pInfo);

	int convertTypeStringToNum(const std::string &pbTypeName);

	const char *getTypeString(int iType);
	int caculateInfoNum(cell_t *pInfoTable, int iMaxCellNum);

	int getColumnType(const std::string &pTableName, const std::string &pColumName);
	char* getSqlStr(const std::string &pTableName, tableParam_t& pInfo);

	char m_FileName[100];

protected:
	sqlite3 *m_pDb;

private:
	int m_iRecordNum;

public:
	selectResult_t *m_pAllData;
};
#endif
