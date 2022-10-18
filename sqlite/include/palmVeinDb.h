#ifndef __PALMVEINDB__H_
#define __PALMVEINDB__H_

#include "palmVeinDbStruct.h"
#include <stdlib.h>
#include <string.h>
class palmVeinDb : public baseSqlite
{
public:
	~palmVeinDb()
	{
		delete m_dbInstance;
	}

public:
	static bool initDataBase();//完成了创建数据库、创建了表
	static bool insertOneRecord(const std::string &tableName, void *dataStruct);
	static void synTableData(const std::string &pTableName, std::vector<selectResult_t> &tableData);

	static selectResult_t *getQueryData();
	static bool  isExistInPvTb(const queryInfo &queryData);
	static bool clearTableData(const std::string &tableName);
	static bool deleteRecordByValuePvTb(const deleteInfo &queryData);
	
	bool isExisInTable(const std::string &tableName, const memberInfo_t *pMember, const queryInfo &queryData);
    //bool isExistIntInTable(const string &tableName, const memberInfo_t *pMember,const int iValue);


	int getFeatureRecordNum();
	bool PVDeleteRecordByValue(const std::string &tableName, const memberInfo_t *pMember, const deleteInfo &deleteData);
	
	static int getMavIdValue(const std::string &name);
private:
	bool openDb(); //�����ݿ�
	bool  createTb(const palmTableInfo &palmVeinTableInfo);
	const memberInfo_t *getTableInfo(int iTableNum, int iMemberOffsetInStruct);

private:
	static const std::string m_dbName;

	palmVeinDb(){}

public:
	static bool getInstance();
	static palmVeinDb *m_dbInstance;
	static const BYTE TABLE_MEM_NUM_MAX = 50;
};

extern const std::string g_pFeaTable;

#endif
