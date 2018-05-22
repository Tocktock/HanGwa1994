#include "GameManager.h"
#include "GlobalDefines.h"

GameManager* GameManager::s_this = NULL;



GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::RegistScene(Scene* SceneValue)
{
	m_vecScene.push_back(SceneValue);
}
/*
void GameManager::RegistPlayerInfo(string PlayerName)
{
	PLAYERINFO_GAMEMANAGER *pInfo = new PLAYERINFO_GAMEMANAGER();
	pInfo->PlayerScore = 0;
	pInfo->buflen = 0;
	m_mapPlayerInfo.insert(make_pair(PlayerName, pInfo));
	
}*/
void GameManager::Update()
{
	m_dwNewElapse = GetTickCount();
	m_fElapse = (m_dwNewElapse - m_dwOldElapse) / 1000.0f;
	m_dwOldElapse = m_dwNewElapse;

	m_curScene->Update(m_fElapse);

	m_HDC = GetDC(m_hWnd); //그리는 파트

	m_curScene->Draw(m_HDC);
	ResourceManager::GetInstance()->DrawBack(m_HDC);

	ReleaseDC(m_hWnd, m_HDC);

}

void GameManager::ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_curScene->ProcessRecvPacket(hWnd, uMsg, wParam, lParam);
	
}

void GameManager::Init(HWND hWnd, SOCKET Sock)
{
	m_hWnd = hWnd;
	m_Sock = Sock;

	m_HDC = GetDC(hWnd);
	m_pMyInfo = new PLAYERINFO_GAMEMANAGER();
	ResourceManager::GetInstance()->InitBack(m_HDC, SCREEN_WIDTH, SCREEN_HEIGHT);

	RegistScene(new LobbyScene);
	RegistScene(new GameScene);
	m_curScene = m_vecScene[SCENE_INDEX_LOBBY];
	m_curScene->Init(hWnd, Sock);

	m_dwOldElapse = GetTickCount();
	m_dwNewElapse = GetTickCount();

	m_GameState = GAMESTATE_MANAGER_LOBBY;

	ReleaseDC(m_hWnd, m_HDC);
}

void GameManager::InputManager(char key)
{
	m_curScene->Input(key);
}

void GameManager::Release()
{

	for (auto iter = m_vecScene.begin(); iter != m_vecScene.end(); iter++)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}

	m_vecScene.clear();
	SAFE_DELETE(m_pMyInfo);
}

void GameManager::QuitGame()
{
	PACKET_COMMAND packetCmd;
	packetCmd.CommandIndex = COMMAND_INDEX_GAME_QUIT;
	packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
	packetCmd.Header.PacketLen = sizeof(packetCmd);

	send(m_Sock, (char*)&packetCmd, sizeof(packetCmd), 0);
}