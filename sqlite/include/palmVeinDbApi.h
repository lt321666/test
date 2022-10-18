#ifndef __PALMVEINDBAPI_H_
#define __PALMVEINDBAPI_H_

#include "palmVeinDbStruct.h"
bool palmVeinDbInit();
bool insertOneRecord(int tableCode, void *dataStruct);
std::string getTableNameByCode(int tableCode);
bool synData(int tableCode, std::vector<selectResult_t> &tableData);
bool getFeatureTableData(std::vector<featureTable> &featureData);
//bool isExistInPalmVeinTable(const queryInfo &queryData);
bool isExistInPalmVeinTable(const int iIntValue,const int iType);
bool isExistInPalmVeinTable(const char *pStr,const int iType);
bool isExistInPalmVeinTable(const std::string sStr,const int iType);
bool deleteTbAllData(int tableCode);
int	getMaxIdValue(int tableCode);
bool deleteRecordInPalmVeinTable(const deleteInfo &deleteData);
// bool insertOneRecord(int tableCode, void* dataStruct);
// bool isExistNameInFeatureTable(const string& name);
// bool isExistIdInFeatureTable(const int id);
// void synFeatureData();
// int  getFeatureRecordNum();
// bool clearFeatureTableData();
#endif
