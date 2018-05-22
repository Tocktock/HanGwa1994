#include <Windows.h>
#include "GameManager.h"
#include "Scene.h"
#include "Macro.h"
#include "LobbyScene.h"
#include "GameScene.h"

GameManager* GameManager::s_this = NULL;

GameManager::GameManager()
{
	m_SocketInitFlag = false;
}


GameManager::~GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_iBufLen = 0;
	m_fWideElapse = 0;
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	ResourceManager::GetInstance()->InitBack(hdc, 1024, 768);
	ReleaseDC(hWnd, hdc);
	RegistScene(new LobbyScene);
	RegistScene(new GameScene);
	m_iGameState = GAME_STATE_LOBBY;
	m_CurScene = m_vecScene[0];
	m_CurScene->Init(hWnd,m_Sock);
	m_bGameEnd = false;
	m_dwFormerElapse = GetTickCount();

}

void GameManager::SocketInit(SOCKET sock)
{
	m_Sock = sock;
	m_iGameState = GAME_STATE_NORMAL;
	LoadScene(SCENE_GAME);
}

void GameManager::StartSocketInit()
{
	m_iGameState = GAME_STATE_INITIAL_GAME;
}

void GameManager::RegistScene(Scene* SceneValue)
{
	m_vecScene.push_back(SceneValue);
}

bool GameManager::InputManager(WPARAM key)
{
	if (m_CurScene->Input(key))
	{
		return true;
	}
	return false;
}

void GameManager::SetStone(POINTXY pt,bool StoneColor)
{
	m_wMap[pt.x][pt.y] = StoneColor;
}

void GameManager::Release()
{
	SAFE_DELETE(s_this);
	for (auto iter = m_vecScene.begin(); iter != m_vecScene.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecScene.clear();
}

void GameManager::Update()
{
	m_dwLaterElapse = GetTickCount();
	m_fCurElapse = (m_dwLaterElapse - m_dwFormerElapse) / 1000.0f;
	m_dwFormerElapse = m_dwLaterElapse;

	m_CurScene->Update(m_fCurElapse);

	//if (m_iGameState == GAME_STATE_INITIAL_GAME)
	//	m_iGameState = GAME_STATE_INITIAL_GAME;

	HDC hdc = GetDC(m_hWnd);
	ResourceManager::GetInstance()->DrawBack(hdc);
	m_CurScene->Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}


void GameManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int retval = 0;

	if (WSAGETSELECTERROR(lParam))
	{
		int errcode = WSAGETSELECTERROR(lParam);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		char szBuf[1024];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
				Sleep(10);
				SendMessage(hWnd, uMsg, wParam, lParam);
				return;
			}
		}

		//리시브처리
		while (true)
		{
			if (m_CurScene->RecvPacket(szBuf,retval,m_iBufLen) == false)
			{
				SendMessage(hWnd, WM_SOCKET, wParam, lParam);
				break;
			}
			else if (m_iBufLen > sizeof(PACKET_HEADER))
				continue;

			break;
		}
		//m_CurScene->RecvPacket(szBuf);
	}
}

void GameManager::ProcessPacket(SOCKET sock, char* szBuf, int len)
{
	//m_CurScene->RecvPacket(szBuf);

	/*
	PACKET_HEADER Header;
	memcpy(&Header, szBuf, sizeof(Header));

	switch (Header.wPacketIndex)
	{
	case PACKET_INDEX_LOGIN_RESULT:
	{
		//PACKET_LOGIN_RESULT packet;
	//	memcpy(&packet, szBuf, sizeof(packet));
		m_CurScene->RecvPacket(szBuf);

		//GameManager::GetInstance()->SetStoneColor(packet.bStoneColor);
		//GameManager::GetInstance()->SetMap(*packet.Map);
	}
	break;
	case PACKET_INDEX_PLAYER_MAP:
	{
		m_CurScene->RecvPacket(szBuf);
		//GameManager::GetInstance()->SetStone(packet.PlayerXY, packet.bStoneColor);
	}
	break;

	case PACKET_INDEX_GAMERESULT:
	{

	}
	break;
	}
	*/
}

void GameManager::SendPos()
{
	m_CurScene->SendPacket();
}

void GameManager::ChatManager(char key)
{
	m_CurScene->Chat(key);
}

void GameManager::LoadScene(int SceneIndex)
{
	m_LastScene = m_CurScene;
	m_CurScene = m_vecScene[SceneIndex];
	m_LastScene->Release();
	m_CurScene->Init(m_hWnd, m_Sock);
}