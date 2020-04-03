#ifndef _PACKDEF_H
#define _PACKDEF_H


#define SERVER_IP  "127.0.0.1"        //老师ip地址	182.92.120.128
#define _DEF_PORT  1234           //端口号
#define _DEF_LISTENNUM   10      //最多待处理的人数
#define _DEF_SQLLEN      300
#define _DEF_SIZE 45
#define _DEF_FRIENDNUM   50
#define _DEF_CONTENTNUM  1000

#define _DEF_PROTOCOL_BASE  10



//1.注册用户
#define _DEF_PROTOCOL_REGISTER_RQ   _DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_REGISTER_RS   _DEF_PROTOCOL_BASE +2
//2.登录
#define _DEF_PROTOCOL_LOGIN_RQ   _DEF_PROTOCOL_BASE +3
#define _DEF_PROTOCOL_LOGIN_RS   _DEF_PROTOCOL_BASE +4

//3.获取好友列表
#define _DEF_PROTOCOL_GETFRIENDLIST_RQ   _DEF_PROTOCOL_BASE +5
#define _DEF_PROTOCOL_GETFRIENDLIST_RS   _DEF_PROTOCOL_BASE +6
//3.1.获取未读消息（添加好友，删除好友，文件）
#define _DEF_PROTOCOL_GETUNPROCESSMSG_RQ   _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_GETUNPROCESSMSG_RS   _DEF_PROTOCOL_BASE +8
//3.2获取未读的聊天
#define _DEF_PROTOCOL_GETUNPROCESSCHATRECORD_RQ   _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_GETUNPROCESSCHATRECORD_RS   _DEF_PROTOCOL_BASE +10
//4.选择好友聊天
#define _DEF_PROTOCOL_SELECTFRIEND_RQ   _DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_SELECTFRIEND_RS   _DEF_PROTOCOL_BASE +12
//5.添加好友
#define _DEF_PROTOCOL_ADDFRIEND_RQ   _DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_ADDFRIEND_RS   _DEF_PROTOCOL_BASE +14
//6.删除好友
#define _DEF_PROTOCOL_DELETEFRIEND_RQ   _DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_DELETEFRIEND_RS   _DEF_PROTOCOL_BASE +16
//7.随机匹配聊天
#define _DEF_PROTOCOL_RANDFRIEND_RQ   _DEF_PROTOCOL_BASE +17
#define _DEF_PROTOCOL_RANDFRIEND_RS   _DEF_PROTOCOL_BASE +18
//8.退出
#define _DEF_PROTOCOL_QUIT_RQ   _DEF_PROTOCOL_BASE +19
#define _DEF_PROTOCOL_QUIT_RS   _DEF_PROTOCOL_BASE +20






#define UM_REGISTERMSG        WM_USER + 1          //注册回复消息
#define UM_LOGINMSG           WM_USER + 2			//登录回复消息
#define UM_GETFRIENDLISTMSG   WM_USER + 3			//获取好友回复消息
#define UM_SELECTFRIENDMSG    WM_USER + 4			//选择好友聊天

typedef char PackType;

#define _register_fail    0
#define _register_success 1
#define _login_fail    0
#define _login_success 1


struct STRU_REGISTER_RQ
{
	PackType	m_ntype;
	char		m_szName[_DEF_SIZE];
	char     m_szPassword[_DEF_SIZE];
};
struct STRU_REGISTER_RS
{
	PackType	m_ntype;
	long long m_userid;
	char		m_szResult;
};
//登录
struct STRU_LOGIN_RQ
{
	PackType m_ntype;
	long long	m_userid;
	char		m_szPassword[_DEF_SIZE];
};
struct STRU_LOGIN_RS
{
	PackType	m_ntype;
	char		m_szResult;
};
//获取好友列表
struct STRU_GETFRIENDLIST_RQ
{
	PackType	m_ntype;
	long long	m_userid;
};

//好友个人信息
struct FriendInfo
{
	long long	 m_userid;
	char		m_szName[_DEF_SIZE];
	char		m_state; 
};
//获得好友
struct STRU_GETFRIENDLIST_RS
{
	PackType	m_ntype;
	FriendInfo	 m_aryFriend[_DEF_FRIENDNUM];
	int				m_nFriendNum;
};
//选择好友聊天
struct STRU_SELECTFRIEND_RQ
{
	PackType m_ntype;
	long long	 m_userid;
	long long	m_Frienid;
	char      m_szContent[_DEF_CONTENTNUM];
};
struct STRU_SELECTFRIEND_RS
{
	PackType   m_ntype;
	char       m_szResult;
};
//添加或者删除好友
typedef struct STRU_ADDFRIEND_RQ
{
	PackType      m_ntype;
	long long     m_userid;
	long long     m_Frienid;
}STRU_DELETEFRIEND_RQ;

typedef struct STRU_ADDFRIEND_RS
{
	PackType m_ntype;
	char      m_szResult;
}STRU_DELETEFRIEND_RS;

//选择随机聊天
struct STRU_RANDFriend_RQ
{
	PackType m_ntype;
	long long m_userid;
};
struct STRU_RANDFriend_RS
{
	PackType	m_ntype;
	long long	m_Friendid;
};

//退出
struct STRU_QUIT_RQ
{
	PackType	m_ntype;
	long long m_userid;
};
struct STRU_QUIT_RS
{
	PackType m_ntype;
	char      m_szResult;
};















#endif

