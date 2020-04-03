#include <iostream>
#include "TCPKernel.h"
using namespace std;


int main()
{
	IKernel *pKernel = new TCPKernel;

	if(!pKernel->Open())
	{
		cout<<"服务器连接失败"<<endl;
	}
	else
	{
		cout<<"服务器运行中"<<endl;
	}
		

	system("pause");
	return 0;
}