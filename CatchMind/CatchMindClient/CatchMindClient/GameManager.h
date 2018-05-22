#pragma once
#include "GlobalDefines.h"
#define BUFSIZE 512

using namespace std;


class PLAYERINFO_GAMEMANAGER
{
public:
	string	PlayerChat;
	int		PlayerScore;
	char	buf[BUFSIZE];
	int		buflen;

};

enum GAMESTATE
{
	GAMESTATE_MANAGER_LOBBY,
	GAMESTATE_MANAGER_INGAME,
	GAMESTATE_MANAGER_END_OF_GAME,
	GAMESTATE_MANAGER_END
};
class Scene;
class GameManager
{
private:

	int m_GameState;
	SOCKET m_Sock;
	HWND m_hWnd;
	HDC m_HDC;
	float m_fElapse;
	DWORD m_dwNewElapse;
	DWORD m_dwOldElapse;

	string		m_MyNickName;
	
	PLAYERINFO_GAMEMANAGER *m_pMyInfo;
	vector<Scene*> m_vecScene;

	Scene* m_curScene;

	static GameManager * s_this;
	
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

	void QuitGame();
	void Update();
	void RegistScene(Scene* SceneValue);
	void InputManager(char key);
	void ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Init(HWND hWnd , SOCKET Sock);

	void RegistPlayerInfo(string PlayerName);
	
	inline void LbuttonDown(int x, int y)
	{
		m_curScene->LbuttonDown(x, y);
	}

	inline void LbuttonUp(int x,int y)
	{
		m_curScene->LbuttonUp(x, y);
	}

	inline void LoadScene(int SceneIndex)
	{
		m_curScene = m_vecScene[SceneIndex];
		m_curScene->Init(m_hWnd, m_Sock);
		
	}

	inline void RegistMyNick(string MyNick)
	{
		m_MyNickName = MyNick;
	}


	inline string GetMyNick()
	{
		return m_MyNickName;
	}


	void Release();
	~GameManager();
};

