#include <iostream>
#include "TCPKernel.h"
using namespace std;


int main()
{
	IKernel *pKernel = new TCPKernel;

	if(!pKernel->Open())
	{
		cout<<"����������ʧ��"<<endl;
	}
	else
	{
		cout<<"������������"<<endl;
	}
		

	system("pause");
	return 0;
}