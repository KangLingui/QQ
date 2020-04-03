#ifndef _PACKDEF_H
#define _PACKDEF_H


#define SERVER_IP  "127.0.0.1"        //��ʦip��ַ	182.92.120.128
#define _DEF_PORT  1234           //�˿ں�
#define _DEF_LISTENNUM   10      //�������������
#define _DEF_SQLLEN      300
#define _DEF_SIZE 45
#define _DEF_FRIENDNUM   50
#define _DEF_CONTENTNUM  1000

#define _DEF_PROTOCOL_BASE  10



//1.ע���û�
#define _DEF_PROTOCOL_REGISTER_RQ   _DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_REGISTER_RS   _DEF_PROTOCOL_BASE +2
//2.��¼
#define _DEF_PROTOCOL_LOGIN_RQ   _DEF_PROTOCOL_BASE +3
#define _DEF_PROTOCOL_LOGIN_RS   _DEF_PROTOCOL_BASE +4

//3.��ȡ�����б�
#define _DEF_PROTOCOL_GETFRIENDLIST_RQ   _DEF_PROTOCOL_BASE +5
#define _DEF_PROTOCOL_GETFRIENDLIST_RS   _DEF_PROTOCOL_BASE +6
//3.1.��ȡδ����Ϣ����Ӻ��ѣ�ɾ�����ѣ��ļ���
#define _DEF_PROTOCOL_GETUNPROCESSMSG_RQ   _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_GETUNPROCESSMSG_RS   _DEF_PROTOCOL_BASE +8
//3.2��ȡδ��������
#define _DEF_PROTOCOL_GETUNPROCESSCHATRECORD_RQ   _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_GETUNPROCESSCHATRECORD_RS   _DEF_PROTOCOL_BASE +10
//4.ѡ���������
#define _DEF_PROTOCOL_SELECTFRIEND_RQ   _DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_SELECTFRIEND_RS   _DEF_PROTOCOL_BASE +12
//5.��Ӻ���
#define _DEF_PROTOCOL_ADDFRIEND_RQ   _DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_ADDFRIEND_RS   _DEF_PROTOCOL_BASE +14
//6.ɾ������
#define _DEF_PROTOCOL_DELETEFRIEND_RQ   _DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_DELETEFRIEND_RS   _DEF_PROTOCOL_BASE +16
//7.���ƥ������
#define _DEF_PROTOCOL_RANDFRIEND_RQ   _DEF_PROTOCOL_BASE +17
#define _DEF_PROTOCOL_RANDFRIEND_RS   _DEF_PROTOCOL_BASE +18
//8.�˳�
#define _DEF_PROTOCOL_QUIT_RQ   _DEF_PROTOCOL_BASE +19
#define _DEF_PROTOCOL_QUIT_RS   _DEF_PROTOCOL_BASE +20






#define UM_REGISTERMSG        WM_USER + 1          //ע��ظ���Ϣ
#define UM_LOGINMSG           WM_USER + 2			//��¼�ظ���Ϣ
#define UM_GETFRIENDLISTMSG   WM_USER + 3			//��ȡ���ѻظ���Ϣ
#define UM_SELECTFRIENDMSG    WM_USER + 4			//ѡ���������

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
//��¼
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
//��ȡ�����б�
struct STRU_GETFRIENDLIST_RQ
{
	PackType	m_ntype;
	long long	m_userid;
};

//���Ѹ�����Ϣ
struct FriendInfo
{
	long long	 m_userid;
	char		m_szName[_DEF_SIZE];
	char		m_state; 
};
//��ú���
struct STRU_GETFRIENDLIST_RS
{
	PackType	m_ntype;
	FriendInfo	 m_aryFriend[_DEF_FRIENDNUM];
	int				m_nFriendNum;
};
//ѡ���������
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
//��ӻ���ɾ������
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

//ѡ���������
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

//�˳�
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

