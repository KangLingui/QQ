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
	void registerRq(unsigned int sock,char*szbuf); //ע������
	void loginRq(unsigned int sock,char*szbuf);//��¼����
	void getfriendlistRq(unsigned int sock,char*szbuf);  //��ȡ�����б�
	void selectfriendlistRq(unsigned int sock,char*szbuf);//����������ѡ��������������

private:
	INet  *m_pTcpnet;
	CMySql m_sql;
	map<long long ,unsigned int > m_mapuserIDtosocket;  //ͨ���û�ID��sock����ӳ���ϵ,�Ӷ�����
};

#endif