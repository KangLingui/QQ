#ifndef _IKERNEL_H
#define _IKERNEL_H
class IKernel  //接口类
{
public:
	IKernel(){}
	virtual ~IKernel(){}
public:
	virtual bool Open()  =  0;   //打开  调用的是初始化网络
	virtual void Close() =  0;   //停止服务器
	virtual void DealData(unsigned int sock,char*szbuf,short nport=0) =0;    //处理数据
	                        //   数据谁发来的  数据是什么    UDP需要端口号  TCP 不需要 
};

#endif