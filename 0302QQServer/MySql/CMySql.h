#pragma once
#include <mysql.h>
#pragma comment(lib,"libmysql.lib")
//
#include <list>
using namespace std;
//
class CMySql
{
public:
    CMySql(void);
    ~CMySql(void);
public:
    bool  ConnectMySql(char *host,char *user,char *pass,char *db,short nport = 3306);
    void  DisConnect();
    bool  SelectMySql(char* szSql,int nColumn,list<string>& lstStr);
	//从数据库中获得表
    bool GetTables(char* szSql,list<string>& lstStr);
    bool  UpdateMySql(char* szSql);
 
private:
    MYSQL *sock;   
	MYSQL_RES *results;   
	MYSQL_ROW record; 
   
};

