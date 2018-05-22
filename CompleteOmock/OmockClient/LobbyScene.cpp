#include <cstdio>
#include "LobbyScene.h"
#include "ResourceManager.h"
#include "GameManager.h"


LobbyScene::LobbyScene()
{
	m_InitFlag = false;
}


LobbyScene::~LobbyScene()
{
}

void LobbyScene::Init(HWND hWnd, SOCKET sock)
{
	m_hWnd = hWnd;
	m_bEnterOn = ENTER;
	m_OutputNum = 0;
	m_bStartedAnim = false;
	for (int i = 0; i < LOBBY_RES_END; i++)
	{
		char szTemp[256];
		sprintf(szTemp, "RES//Lobby0%d.bmp",i);
		m_pBitmapLobby[i] = ResourceManager::GetInstance()->GetBitmap(szTemp);
		m_pBitmapLobby[i]->SetAnchor(ANCHOR_RT);
	}
	m_pBitmapText = ResourceManager::GetInstance()->GetBitmap("RES//LobbyText.bmp");
	m_pBitmapWhiteBack = ResourceManager::GetInstance()->GetBitmap("RES//WhiteBack.bmp");
	m_fAnimationElapse = 0;
	m_iAnimationCount = 0;
}
bool LobbyScene::Input(WPARAM key)
{
	switch (key)
	{
	case VK_UP:
		m_bEnterOn = ENTER;
		break;
	case VK_DOWN:
		m_bEnterOn = EXIT;
		break;
	case VK_RETURN:
		if (m_bStartedAnim == true)
			break;

		if (m_bEnterOn == ENTER)
		{
			m_bStartedAnim = false;
			GameManager::GetInstance()->StartSocketInit();
		}
		
		else if (m_bEnterOn == EXIT)
			m_bStartedAnim = true;
		
		break;
	case VK_ESCAPE:
		return true;
	}

	return false;
}
void LobbyScene::Update(float Elapse)
{
	m_fElase = Elapse;

	if(m_bStartedAnim == true)
		EnterGame();

	if (m_OutputNum == LOBBY_RES06)
	{
		m_OutputNum = LOBBY_RES00;
		m_iAnimationCount++;
	}
	
}
void LobbyScene::Draw(HDC hdc)
{
	m_pBitmapWhiteBack->Draw(0, 0);
	m_pBitmapLobby[m_OutputNum]->Draw(600, 300);
	m_pBitmapText->Draw(100, 100);

	if (m_bEnterOn == ENTER)
	{
		m_pBitmapLobby[LOBBY_RES06]->Draw(110, 90);
	}

	else if (m_bEnterOn == EXIT)
	{
		m_pBitmapLobby[LOBBY_RES07]->Draw(110, 170);
	}

}
void LobbyScene::Release()
{

}

bool LobbyScene::RecvPacket(char*szBuf, int &retval, int &Buflen)
{
	return false;
}

void LobbyScene::SendPacket()
{

}

void LobbyScene::EnterGame()
{

	if (m_bStartedAnim == false)
		return;

	m_fAnimationElapse += m_fElase;
	
	if (m_fAnimationElapse > 0.2f && m_bEnterOn == false)
	{
		m_OutputNum++;
		m_fAnimationElapse = 0;
	}
		
	else if (m_iAnimationCount == 4 && m_InitFlag == true)
	{
		SendMessage(m_hWnd, WM_DESTROY, 0, 0);
	}
	
}
void LobbyScene::Chat(char key)
{

}