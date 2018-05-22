#pragma once
#include "Scene.h"

#define WM_SOCKET (WM_USER+1)
class Bitmap;
class LobbyScene : public Scene
{

	Bitmap *m_pBitLobbyBack;

	HWND	m_hWnd;
	SOCKET	m_Sock;


	int		m_iRetval;
	int		m_iBuflen;
	char	m_buf[512];
	string	m_ChatSTR[19];
	char	m_ChatBuf[256];
	string	m_ShowMyChat;
	int		m_ChatBufCount;
	bool	m_ChatFlag;
	int		m_iSTRCount;

	bool	m_LbuttonDownFlag;

	RECT	m_rtLoginBox;
	bool	m_LogFlag;
	char	m_myNick[256];
	int		m_iNickCount;
	string	m_ShowMyNick;
public:

	virtual void Init(HWND hWnd, SOCKET sock);
	virtual bool Input(char key);
	virtual bool LbuttonDown(int x, int y);
	virtual bool LbuttonUp(int x, int y);
	virtual void Update(float Elapse);
	virtual void Draw(HDC hdc);
	virtual void Release();
	virtual void ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool RecvPacket(char*szBuf, int retval, int Buflen);
	virtual void SendPacket(char* buf, int packetLen);
	virtual void Chat(char key);

	LobbyScene();
	virtual ~LobbyScene();
};

