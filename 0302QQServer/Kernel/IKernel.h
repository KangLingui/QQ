#ifndef _IKERNEL_H
#define _IKERNEL_H
class IKernel  //�ӿ���
{
public:
	IKernel(){}
	virtual ~IKernel(){}
public:
	virtual bool Open()  =  0;   //��  ���õ��ǳ�ʼ������
	virtual void Close() =  0;   //ֹͣ������
	virtual void DealData(unsigned int sock,char*szbuf,short nport=0) =0;    //��������
	                        //   ����˭������  ������ʲô    UDP��Ҫ�˿ں�  TCP ����Ҫ 
};

#endif