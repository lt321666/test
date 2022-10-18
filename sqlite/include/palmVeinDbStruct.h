#ifndef __PALMVEINDBSTRUCT__H_
#define __PALMVEINDBSTRUCT__H_
#define CV_NODISCARD_STD
#include <stddef.h>
#include <vector>
#include "baseSqlite.h"

typedef struct
{
	int camera_id;
	int id;
	char event_url[4096];
	char cavans_json[128];
	char event_report_time[512];

} featureTable;

typedef struct
{
	int iPort;
	char ip[32];
} paraTable;

typedef struct
{
	memberInfo_t *tableInfo;
	std::string tableName;
	bool fTable;
} palmTableInfo;

typedef struct
{
	std::string strValue;
	int iValue;
    int nameType;
    int sqliteType;
} queryInfo;

typedef struct
{
	std::string strValue;
	int iValue;
	int nameType;
	int sqliteType;
} deleteInfo;
#define FEATURE_TABLE 1
#define PARA_TABLE 2


#define NAME 1
#define PATH 2
#define ID 3

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_TEXT     3
#define SQLITE_BLOB     4
#endif