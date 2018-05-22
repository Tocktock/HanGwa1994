#include <WinSock2.h>
#include <time.h>
#include "Bitmap.h"
#include "LobbyScene.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "PACKET_HEADER.h"

LobbyScene::LobbyScene()
{
}


LobbyScene::~LobbyScene()
{
}
bool LobbyScene::LbuttonUp(int x, int y)
{
	m_LbuttonDownFlag = false;
	return false;
}
bool LobbyScene::LbuttonDown(int x, int y)
{
	m_LbuttonDownFlag = true;
	POINT pt;
	pt.x = x;
	pt.y = y;

	if (PtInRect(&m_rtLoginBox, pt))
	{
		m_LogFlag = true;
	}

	else
	{
		m_LogFlag = false;
	}

	return false;
}

void LobbyScene::Init(HWND hWnd, SOCKET sock)
{
	m_LbuttonDownFlag = false;
	m_LogFlag = false;
	m_rtLoginBox.left = 500; //500, 130, 700, 150
	m_rtLoginBox.top = 130;
	m_rtLoginBox.right = 700;
	m_rtLoginBox.bottom = 150;
	m_iNickCount = 0;

	m_Sock = sock;
	m_hWnd = hWnd;

	m_pBitLobbyBack = ResourceManager::GetInstance()->GetBitmap("RES//LobbyBack.bmp");

	m_ChatBufCount = 0;
	m_iBuflen = 0;
	m_ChatFlag = false;
	
}
bool LobbyScene::Input(char key)
{
	if (key == '\b' && m_iNickCount == 0)
		return false;

	if (key == '\t' || key == '\r')
	{
		if (m_iNickCount == 0)
			return false;

		if (m_LogFlag == true)
		{
			PACKET_NICKNAME packetNick;
			packetNick.header.HeaderIndex = PACKET_INDEX_NICKNAME;
			packetNick.header.PacketLen = sizeof(packetNick.header)+
				sizeof(packetNick.PlayerNumber) + m_iNickCount;
			strcpy(packetNick.str, m_myNick);

			send(m_Sock,(char*)&packetNick, packetNick.header.PacketLen,0);
			GameManager::GetInstance()->RegistMyNick(m_myNick);
			GameManager::GetInstance()->LoadScene(SCENE_INDEX_GAME);
		}
		return false;
	}

	if (key == '\b' && m_iNickCount > 0)
	{
		m_myNick[--m_iNickCount] = '\0'; 
		m_ShowMyNick = m_myNick;
		return false;
	}

	
	if (m_iNickCount < 255 && m_LogFlag == true)
	{
		m_myNick[m_iNickCount] = key;
		m_myNick[m_iNickCount + 1] = '\0';
		m_iNickCount++;
		m_ShowMyNick = m_myNick;
	}

	return false;

}
void LobbyScene::Update(float Elapse)
{

}
void LobbyScene::Draw(HDC hdc)
{

	m_pBitLobbyBack->Draw(0, 0);

	HBRUSH Brush, OldBrush;

	Brush = CreateSolidBrush(RGB(136, 136, 136));
	OldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->GetBackDC(), Brush);

	SetBkColor(ResourceManager::GetInstance()->GetBackDC(), RGB(136, 136, 136));
	Rectangle(ResourceManager::GetInstance()->GetBackDC(), 500, 130, 700, 150);

	SelectObject(ResourceManager::GetInstance()->GetBackDC(), OldBrush);
	DeleteObject(Brush);

	TextOut(ResourceManager::GetInstance()->GetBackDC(),
		500, 130, m_ShowMyNick.c_str(), m_ShowMyNick.length());

}
void LobbyScene::Release()
{

}

void LobbyScene::ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		int errcode = WSAGETSELECTERROR(lParam);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		char szBuf[1024];

		m_iRetval = recv(wParam, szBuf, BUFSIZE, 0);

		if (m_iRetval == SOCKET_ERROR)
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

			if (RecvPacket(szBuf, m_iRetval, m_iBuflen) == true)
			{
				SendMessage(hWnd, WM_SOCKET, wParam, lParam);
				break;
			}

			else if (m_iBuflen > sizeof(PACKET_HEADER))
				continue;

			break;
		}
	}

}

bool LobbyScene::RecvPacket(char*szBuf, int retval, int Buflen)
{

	memcpy(m_buf, szBuf, retval);
	Buflen += retval;
	retval = 0;

	PACKET_HEADER header;
	memcpy(&header, m_buf, sizeof(PACKET_HEADER));

	switch (header.HeaderIndex)
	{
	case PACKET_INDEX_NICKNAME:
		{

		}
	break;

	case PACKET_INDEX_COMMAND:
		{

		}
	break;

	case PACKET_INDEX_STRING:
		{

		}
	break;

	case PACKET_INDEX_XY:
		{

		}
	break;
	}

	return false;



	return false;
}
void LobbyScene::SendPacket(char* buf, int packetLen)
{
	
}
void LobbyScene::Chat(char key)
{

}
