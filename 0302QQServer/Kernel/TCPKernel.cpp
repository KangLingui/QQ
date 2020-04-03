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
	STRU_REGISTER_RQ *psrr = (STRU_REGISTER_RQ *)szbuf;  //收到注册请求包
	//将信息加入到数据库
	char szsql[_DEF_SQLLEN]= {0};
	STRU_REGISTER_RS srr;
	srr.m_ntype=_DEF_PROTOCOL_REGISTER_RS;

	srr.m_szResult=_register_fail;
	list<string> lststr;
	//在数据库中插入注册用户信息
	    sprintf_s(szsql,"insert into user(u_name,u_password) values('%s','%s');",
											psrr->m_szName,psrr->m_szPassword);


	if(m_sql.UpdateMySql(szsql))
	{
		//如果用户插入成功  需要发注册成功回复
		srr.m_szResult = _register_success;
		sprintf_s(szsql,"select u_id from user where u_name = '%s'and u_password = '%s';",
														psrr->m_szName,psrr->m_szPassword);

		//通过用户名获取用户的ID
		                      //1列  放到lststr链表中
		m_sql.SelectMySql(szsql,1,lststr);

			if(lststr.size() >0)
			{
				string struserid =lststr.front();  //取出对应的ID
				lststr.pop_front();                //相当于从链表中清除ID
				srr.m_userid   =   _atoi64(struserid.c_str());

				//用户注册完成后添加状态信息--------触发器
				sprintf_s( szsql,"insert into userstate values(%lld,0,now());",srr.m_userid);
				m_sql.UpdateMySql( szsql );

			}

	}

		//将回复发送回去
		m_pTcpnet->SendData(sock,(char*)&srr,sizeof(srr),0);

}



void TCPKernel::loginRq(unsigned int sock,char*szbuf)
{
	//登录请求-----------------需要将用户的登录信息与数据库中的信息比较
	STRU_LOGIN_RQ *  pslr=(STRU_LOGIN_RQ*)szbuf;   //获得登录请求包
	char szsql[_DEF_SQLLEN]={0};
	list<string> lststr;
	STRU_LOGIN_RS slr;
	slr.m_ntype=_DEF_PROTOCOL_LOGIN_RS;
	slr.m_szResult =_login_fail;
	     //比较密码                                                     //%lld 是因为是long  long 
	sprintf_s(szsql,"select u_password from user where u_id = %lld",pslr->m_userid);
	m_sql.SelectMySql( szsql,1,lststr );

	if( lststr.size() >0  )
	{

		string strPassword =   lststr.front();   //取出密码
		lststr.pop_front();  //清除

		if(0 ==strcmp(pslr->m_szPassword,strPassword.c_str()))   //比较密码
		{
			//如果登录成功 --------  需要-----------  改变好友状态 ， 修改登录时间  
				slr.m_szResult =_login_success;
			 
				sprintf_s(szsql,"update userstate set u_state = 1 where u_id =%lld",
					pslr->m_userid );
				m_sql.UpdateMySql(szsql);    //将数据库中的状态更新

				sprintf_s(szsql,"update userstate set u_datetime = now() where u_id =%lld",
					pslr->m_userid );
				m_sql.UpdateMySql(szsql);          //记录用户上线时间
		
				m_mapuserIDtosocket[pslr->m_userid] = sock;  //将sock和用户ID建立映射关系



		 }
	   
	}


	//回复登录消息
		m_pTcpnet->SendData(sock,(char*)&slr,sizeof(slr),0);

}


void TCPKernel::getfriendlistRq(unsigned int sock,char*szbuf)//获取好友列表请求
{
	STRU_GETFRIENDLIST_RQ *psgr=(STRU_GETFRIENDLIST_RQ*)szbuf;  //获取好友请求的包
	char szsql[_DEF_SQLLEN]={0};

	list<string>   lststr;
	STRU_GETFRIENDLIST_RS	sgr;      //回复包

		sgr.m_ntype=_DEF_PROTOCOL_GETFRIENDLIST_RS;
		//？？？？？？？？？？？？？？复习
	sprintf_s(szsql,"select userfriend.f_id,u_name,u_state from userfriend inner join \
		     userstate on userfriend.f_id = userstate.u_id and userfriend.u_id = %lld \
					inner join user on userfriend.f_id = user.u_id;",psgr->m_userid);

		m_sql.SelectMySql(szsql,3,lststr);  //传到数据库
		int i = 0;
		 //通过链表将信息返回
		while(lststr.size() >0)
		{
				string strfriendId=lststr.front();  //取出ID
				lststr.pop_front();//消除ID

				string strfriendName=lststr.front();   //
				lststr.pop_front();

				string strfriendState=lststr.front();
				lststr.pop_front();

			sgr.m_aryFriend[i].m_state =   *strfriendState.c_str();    //
				strcpy_s(sgr.m_aryFriend[i].m_szName,_DEF_SIZE,strfriendName.c_str());
				sgr.m_aryFriend[i].m_userid  =   _atoi64(strfriendId.c_str());

				i++;
			
			//判断链表是否取完 ==0    如果好友满了 i==_DEF_FRIENDNUM  
					if(lststr.size()==0 ||i==_DEF_FRIENDNUM)
					{
						sgr.m_nFriendNum =i;
						m_pTcpnet->SendData(sock,(char*)&sgr,sizeof(sgr),0);   //发送
						i=0;
					} 
		}
}


void TCPKernel::selectfriendlistRq(unsigned int sock,char*szbuf)//服务器处理选择好友聊天的请求
{
	STRU_SELECTFRIEND_RQ *pssr = (STRU_SELECTFRIEND_RQ*)szbuf;

	SOCKET sockfriend =   m_mapuserIDtosocket[pssr->m_Frienid];  //传过来朋友的ID

	if(sockfriend)//如果好友当前在线，服务器将消息直接发送出去
	{
		m_pTcpnet->SendData(sockfriend,szbuf,sizeof(STRU_SELECTFRIEND_RQ),0);

	}
	else    //如果好友离线 ，消息记录在数据库
	{

	}





}



bool TCPKernel::Open()
{
if(!m_pTcpnet->InitNetWork())   //初始化网络
{
return false;
}
		
	
if(!m_sql.ConnectMySql("localhost","root","237341","0303qq")  )  //初始化数据库
	 
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
	//处理数据
	PackType *ptype = (PackType *)szbuf;
	//解析包  
	//如何解析包  -----根据包的第一个字节  按照协议内容进行解析
	switch (*ptype)
	{
		
		case _DEF_PROTOCOL_REGISTER_RQ:  
			{
					registerRq(sock,szbuf);//注册请求
			}
			break;

		case _DEF_PROTOCOL_LOGIN_RQ:    
			{
					loginRq(sock,szbuf); //登录请求
			}
			break;

			case _DEF_PROTOCOL_GETFRIENDLIST_RQ:  //
				{
						getfriendlistRq(sock,szbuf);//获取好友列表
				}
				break;

			case _DEF_PROTOCOL_SELECTFRIEND_RQ:
				{
				selectfriendlistRq( sock,szbuf);//选择好友聊天请求

				}
				break;



		default:
			break;
	}

}