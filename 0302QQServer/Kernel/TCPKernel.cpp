#include "TCPKernel.h"


TCPKernel::TCPKernel()
{

	m_pTcpnet =		new TCPNet(this);
}

TCPKernel::~TCPKernel()
{
	delete	m_pTcpnet;
	m_pTcpnet	= NULL;
}



void TCPKernel::registerRq(unsigned int sock,char*szbuf)
{
	STRU_REGISTER_RQ *psrr = (STRU_REGISTER_RQ *)szbuf;  //�յ�ע�������
	//����Ϣ���뵽���ݿ�
	char szsql[_DEF_SQLLEN]= {0};
	STRU_REGISTER_RS srr;
	srr.m_ntype=_DEF_PROTOCOL_REGISTER_RS;

	srr.m_szResult=_register_fail;
	list<string> lststr;
	//�����ݿ��в���ע���û���Ϣ
	    sprintf_s(szsql,"insert into user(u_name,u_password) values('%s','%s');",
											psrr->m_szName,psrr->m_szPassword);


	if(m_sql.UpdateMySql(szsql))
	{
		//����û�����ɹ�  ��Ҫ��ע��ɹ��ظ�
		srr.m_szResult = _register_success;
		sprintf_s(szsql,"select u_id from user where u_name = '%s'and u_password = '%s';",
														psrr->m_szName,psrr->m_szPassword);

		//ͨ���û�����ȡ�û���ID
		                      //1��  �ŵ�lststr������
		m_sql.SelectMySql(szsql,1,lststr);

			if(lststr.size() >0)
			{
				string struserid =lststr.front();  //ȡ����Ӧ��ID
				lststr.pop_front();                //�൱�ڴ����������ID
				srr.m_userid   =   _atoi64(struserid.c_str());

				//�û�ע����ɺ����״̬��Ϣ--------������
				sprintf_s( szsql,"insert into userstate values(%lld,0,now());",srr.m_userid);
				m_sql.UpdateMySql( szsql );

			}

	}

		//���ظ����ͻ�ȥ
		m_pTcpnet->SendData(sock,(char*)&srr,sizeof(srr),0);

}



void TCPKernel::loginRq(unsigned int sock,char*szbuf)
{
	//��¼����-----------------��Ҫ���û��ĵ�¼��Ϣ�����ݿ��е���Ϣ�Ƚ�
	STRU_LOGIN_RQ *  pslr=(STRU_LOGIN_RQ*)szbuf;   //��õ�¼�����
	char szsql[_DEF_SQLLEN]={0};
	list<string> lststr;
	STRU_LOGIN_RS slr;
	slr.m_ntype=_DEF_PROTOCOL_LOGIN_RS;
	slr.m_szResult =_login_fail;
	     //�Ƚ�����                                                     //%lld ����Ϊ��long  long 
	sprintf_s(szsql,"select u_password from user where u_id = %lld",pslr->m_userid);
	m_sql.SelectMySql( szsql,1,lststr );

	if( lststr.size() >0  )
	{

		string strPassword =   lststr.front();   //ȡ������
		lststr.pop_front();  //���

		if(0 ==strcmp(pslr->m_szPassword,strPassword.c_str()))   //�Ƚ�����
		{
			//�����¼�ɹ� --------  ��Ҫ-----------  �ı����״̬ �� �޸ĵ�¼ʱ��  
				slr.m_szResult =_login_success;
			 
				sprintf_s(szsql,"update userstate set u_state = 1 where u_id =%lld",
					pslr->m_userid );
				m_sql.UpdateMySql(szsql);    //�����ݿ��е�״̬����

				sprintf_s(szsql,"update userstate set u_datetime = now() where u_id =%lld",
					pslr->m_userid );
				m_sql.UpdateMySql(szsql);          //��¼�û�����ʱ��
		
				m_mapuserIDtosocket[pslr->m_userid] = sock;  //��sock���û�ID����ӳ���ϵ



		 }
	   
	}


	//�ظ���¼��Ϣ
		m_pTcpnet->SendData(sock,(char*)&slr,sizeof(slr),0);

}


void TCPKernel::getfriendlistRq(unsigned int sock,char*szbuf)//��ȡ�����б�����
{
	STRU_GETFRIENDLIST_RQ *psgr=(STRU_GETFRIENDLIST_RQ*)szbuf;  //��ȡ��������İ�
	char szsql[_DEF_SQLLEN]={0};

	list<string>   lststr;
	STRU_GETFRIENDLIST_RS	sgr;      //�ظ���

		sgr.m_ntype=_DEF_PROTOCOL_GETFRIENDLIST_RS;
		//������������������������������ϰ
	sprintf_s(szsql,"select userfriend.f_id,u_name,u_state from userfriend inner join \
		     userstate on userfriend.f_id = userstate.u_id and userfriend.u_id = %lld \
					inner join user on userfriend.f_id = user.u_id;",psgr->m_userid);

		m_sql.SelectMySql(szsql,3,lststr);  //�������ݿ�
		int i = 0;
		 //ͨ��������Ϣ����
		while(lststr.size() >0)
		{
				string strfriendId=lststr.front();  //ȡ��ID
				lststr.pop_front();//����ID

				string strfriendName=lststr.front();   //
				lststr.pop_front();

				string strfriendState=lststr.front();
				lststr.pop_front();

			sgr.m_aryFriend[i].m_state =   *strfriendState.c_str();    //
				strcpy_s(sgr.m_aryFriend[i].m_szName,_DEF_SIZE,strfriendName.c_str());
				sgr.m_aryFriend[i].m_userid  =   _atoi64(strfriendId.c_str());

				i++;
			
			//�ж������Ƿ�ȡ�� ==0    ����������� i==_DEF_FRIENDNUM  
					if(lststr.size()==0 ||i==_DEF_FRIENDNUM)
					{
						sgr.m_nFriendNum =i;
						m_pTcpnet->SendData(sock,(char*)&sgr,sizeof(sgr),0);   //����
						i=0;
					} 
		}
}


void TCPKernel::selectfriendlistRq(unsigned int sock,char*szbuf)//����������ѡ��������������
{
	STRU_SELECTFRIEND_RQ *pssr = (STRU_SELECTFRIEND_RQ*)szbuf;

	SOCKET sockfriend =   m_mapuserIDtosocket[pssr->m_Frienid];  //���������ѵ�ID

	if(sockfriend)//������ѵ�ǰ���ߣ�����������Ϣֱ�ӷ��ͳ�ȥ
	{
		m_pTcpnet->SendData(sockfriend,szbuf,sizeof(STRU_SELECTFRIEND_RQ),0);

	}
	else    //����������� ����Ϣ��¼�����ݿ�
	{

	}





}



bool TCPKernel::Open()
{
if(!m_pTcpnet->InitNetWork())   //��ʼ������
{
return false;
}
		
	
if(!m_sql.ConnectMySql("localhost","root","237341","0303qq")  )  //��ʼ�����ݿ�
	 
{
return false;
}


	return true;
}

void TCPKernel::Close()
{
		m_sql.DisConnect();
		m_pTcpnet->UnInitNetWork();
	}



void TCPKernel::DealData(unsigned int sock,char*szbuf,short nport)
{
	//��������
	PackType *ptype = (PackType *)szbuf;
	//������  
	//��ν�����  -----���ݰ��ĵ�һ���ֽ�  ����Э�����ݽ��н���
	switch (*ptype)
	{
		
		case _DEF_PROTOCOL_REGISTER_RQ:  
			{
					registerRq(sock,szbuf);//ע������
			}
			break;

		case _DEF_PROTOCOL_LOGIN_RQ:    
			{
					loginRq(sock,szbuf); //��¼����
			}
			break;

			case _DEF_PROTOCOL_GETFRIENDLIST_RQ:  //
				{
						getfriendlistRq(sock,szbuf);//��ȡ�����б�
				}
				break;

			case _DEF_PROTOCOL_SELECTFRIEND_RQ:
				{
				selectfriendlistRq( sock,szbuf);//ѡ�������������

				}
				break;



		default:
			break;
	}

}