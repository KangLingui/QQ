#ifndef _INET_H			//��ֹͷ�ļ��ظ�����  
#define _INET_H        //
#include <winsock2.h>
#include "Packef.h"
#pragma comment(lib, "ws2_32.lib")
class INet         //�ӿ���
{
public:
	INet(){}
	virtual ~INet(){}
public:
		virtual bool InitNetWork()   =	0;   //��������
		virtual void UnInitNetWork()  =	0;  //ж������
		virtual bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort)  =	0;//�������� 
								   //ͨ��sock˵      ��С                  �˿ں�
};






#endif