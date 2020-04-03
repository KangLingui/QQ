#ifndef _IKERNEL_H
#define _IKERNEL_H
class IKernel
{
public:
	IKernel()
	{}

	virtual ~IKernel()
	{}
public:
	virtual bool Open()  =0;
	virtual void Close() =0;
	virtual void DealData(unsigned int sock,char*szbuf,short nport=0) =0;
	virtual bool SendData(unsigned int sock,char *szbuf,int nLen ,short nPort) = 0;
};

#endif