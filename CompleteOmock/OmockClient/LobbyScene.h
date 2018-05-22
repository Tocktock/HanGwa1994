#pragma once
#include <Windows.h>
#include "Scene.h"
#include "PACKET_HEADER.h"
#define ENTER	true
#define	EXIT	false

enum LOBBY_RES
{
	LOBBY_RES00,
	LOBBY_RES01,
	LOBBY_RES02,
	LOBBY_RES03,
	LOBBY_RES04,
	LOBBY_RES05,
	LOBBY_RES06,
	LOBBY_RES07,
	LOBBY_RES_END
};
class Bitmap;
class LobbyScene : public Scene
{
private:
	Bitmap *m_pBitmapLobby[LOBBY_RES_END];
	Bitmap *m_pBitmapText;
	Bitmap *m_pBitmapWhiteBack;
	bool	m_bEnterOn;
	bool	m_bStartedAnim;
	bool	m_bEnterGame;
	int		m_OutputNum;
	HWND	m_hWnd;
	bool	m_InitFlag;

	float	m_fElase;
	float	m_fAnimationElapse;
	int		m_iAnimationCount;

public:
	virtual void Init(HWND hWnd, SOCKET sock);
	virtual bool Input(WPARAM key);
	virtual void Update(float Elapse);
	virtual void Draw(HDC hdc);
	virtual void Release();
	virtual bool RecvPacket(char*szBuf, int &retval, int &Buflen);
	virtual void SendPacket();

	virtual void Chat(char key);


	void	EnterGame();
	LobbyScene();
	~LobbyScene();
};

