#pragma once
#include <Windows.h>
#include <vector>
#include "ResourceManager.h"
#include "Bitmap.h"
#include "PACKET_HEADER.h"


#define WINCX 1024
#define WINCY 768

#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 1024

enum GAME_STATE
{
	GAME_STATE_NORMAL,
	GAME_STATE_LOBBY,
	GAME_STATE_INITIAL_GAME,
	GAME_STATE_END_OF_GAME,
	GAME_STATE_EXIT,
	GAME_STATE_END
};

class Scene;
class GameManager
{
private:

	Bitmap *m_pBitmapBoard;
	Bitmap *m_pBitmapStone[STONE_COLOR_END];
	Bitmap *m_pBitmapSelect;

	vector<Scene*> m_vecScene;
	Scene *m_CurScene;
	Scene *m_LastScene;

	WORD m_wMap[OMOCK_WHNUM][OMOCK_WHNUM];

	DWORD m_dwFormerElapse;
	DWORD m_dwLaterElapse;
	float m_fCurElapse;
	float m_fWideElapse;

	HWND	m_hWnd;
	SOCKET	m_Sock;
	
	char	m_Buf[BUFSIZE];
	int		m_iBufLen;

	bool	m_SocketInitFlag;

	int m_iGameState;

	POINTXY m_MyPt;
	bool m_bStoneColor;
	bool m_bGameEnd;
	static GameManager* s_this;
	GameManager();
public:
	static GameManager* GetInstance()
	{
		if (s_this == NULL)
		{
			s_this = new GameManager();
		}
		return s_this;
	}
	void Init(HWND hWnd);
	void SocketInit(SOCKET sock);
	void StartSocketInit();
	void ChatManager(char key);
	bool InputManager(WPARAM key);
	void Update();
	void RegistScene(Scene* SceneValue);
	void SetStone(POINTXY pt,bool StoneColor);

	void LoadScene(int SceneIndex);

	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ProcessPacket(SOCKET sock, char* szBuf, int len);
	void SendPos();

	inline void SetMap(WORD* Map)
	{
		memcpy(m_wMap, Map, sizeof(m_wMap));
	}

	inline POINTXY GetPointXY()
	{
		return m_MyPt;
	}


	inline int GetGameState()
	{
		return m_iGameState;
	}

	inline void SetGameState(int iGameState)
	{
		m_iGameState = iGameState;
	}

	inline void SetStoneColor(bool StoneColor)
	{
		m_bStoneColor = StoneColor;
	}

	inline bool GetStoneColor()
	{
		return m_bStoneColor;
	}


	void Release();
	~GameManager();
};

