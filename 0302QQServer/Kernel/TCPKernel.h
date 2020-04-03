#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H
#include "IKernel.h"
#include <map>
#include "TCPNet.h"  
#include "CMySql.h"  

using namespace std;
class  TCPKernel :public  IKernel   
{
public:
	TCPKernel();
	virtual ~TCPKernel();
	
public:
	virtual bool Open();
	virtual void Close();
	virtual void DealData(unsigned int sock,char*szbuf,short nport=0);


public:
	void registerRq(unsigned int sock,char*szbuf); //注册请求
	void loginRq(unsigned int sock,char*szbuf);//登录请求
	void getfriendlistRq(unsigned int sock,char*szbuf);  //获取好友列表
	void selectfriendlistRq(unsigned int sock,char*szbuf);//服务器处理选择好友聊天的请求

private:
	INet  *m_pTcpnet;
	CMySql m_sql;
	map<long long ,unsigned int > m_mapuserIDtosocket;  //通过用户ID与sock建立映射关系,从而查找
};

#endif