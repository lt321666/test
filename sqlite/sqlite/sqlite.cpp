// sqlite.cpp: 定义应用程序的入口点。
//

#include "sqlite.h"
#include "../include/sqlite3.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "palmVeinDbApi.h"

using namespace std;
int main(void) {

	if (true != palmVeinDbInit()) {
		std::cout << "建表失败" << std::endl;
	}
}





//int main()
//{
//	sqlite3* pdb = NULL; // 一个打开的数据库实例
//	const char* path = "./test.db";//某个sql文件的路径
//	int r;
//	 // 根据文件路径打开数据库连接。如果数据库不存在，则创建。
//	 // 数据库文件的路径必须以C字符串传入。
//	int result = sqlite3_open_v2(path, &pdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
//
//	if (result == SQLITE_OK) {
//		std::cout << "打开数据库连接成功";
//
//	}
//	else {
//		std::cout << "打开数据库连接失败";
//
//	}
//
//	/*step 2: sql语句对象。*/
//	sqlite3_stmt* pStmt;
//	char* sql = "SELECT id,name,addr FROM test;";
//	r = sqlite3_prepare_v2(
//		pdb, //数据库连接对象
//		sql, //指向原始sql语句字符串
//		strlen(sql), //
//		&pStmt,
//		NULL
//	);
//	if (r != SQLITE_OK)
//	{
//		perror("sqlite3_prepare_v2 error:");
//		return -1;
//	}
//	while (1)
//	{
//		static int first = 1;
//		int i;
//		/*2.2 执行SQL语句*/
//		r = sqlite3_step(pStmt);
//		if (r == SQLITE_DONE)
//		{
//			printf("o la\n");
//			break;//执行完成
//		}
//		else if (r == SQLITE_ROW) //获取到一行的结果
//		{
//			int cNum = 0;//结果集中有多少列
//			cNum = sqlite3_column_count(pStmt);
//			if (first)
//			{
//				for (i = 0; i < cNum; i++)
//				{
//					//返回结果集中第i列的名字
//					const char* p = sqlite3_column_name(pStmt, i);
//					printf("%s\t", p);
//				}
//				printf("\n---------------------------\n");
//				
//				first = 0;
//			}
//			int iType; //第i列的数据类型
//			for (i = 0; i < cNum; i++)
//			{
//				//获取结果集中第i列的数据类型
//				iType = sqlite3_column_type(pStmt, i);
//				if (iType == SQLITE_INTEGER)
//				{
//					int iValue;
//					//获取结果集中第i列的数据值
//					iValue = sqlite3_column_int(pStmt, i);
//					printf("%d\t", iValue);
//				}
//				else if (iType == SQLITE_FLOAT)
//				{
//					double iValue;
//					//获取结果集中第i列的数据值
//					iValue = sqlite3_column_double(pStmt, i);
//					printf("%g\t", iValue);
//
//
//				}
//				else if (iType == SQLITE_TEXT)
//				{
//					const char* iValue;
//					//获取结果集中第i列的数据值
////					iValue = sqlite3_column_text(pStmt, i);
//					printf("%s\t", iValue);
//				}
//			}
//			printf("\n");
//		}
//	}
//	//销毁一个SQL语句对象
//	std::cout << "数据库程序执行到这";
//	sqlite3_finalize(pStmt);
//
//	/*step 3: 关闭数据库连接对象*/
//	sqlite3_close(pdb);
//	return 0;
//}
