#include <conio.h>
#include "GlobalDefine.h"
#include "GameScene.h"
#include "Bitmap.h"



GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

void GameScene::SetStone(POINTXY pt, bool StoneColor)
{
	m_wMap[pt.x][pt.y] = StoneColor;
}


void GameScene::Init(HWND hWnd, SOCKET sock)
{
	m_bResetFlag = false;
	m_ChatBufCount = 0;
	m_iSTRCount = 0;
	m_ChatFlag = false;
	m_wGameState = GAME_STATE_OMOCK_BEFORE_GAME;
	m_MyPt.x = 10;
	m_MyPt.y = 10;
	m_bStoneColor = STONE_COLOR_BLACK;
	m_Sock = sock;
	m_pBitmapBoard = ResourceManager::GetInstance()->GetBitmap("RES//Board.bmp");
	m_pBitmapSelect = ResourceManager::GetInstance()->GetBitmap("RES//Selected.bmp");
	m_pBitmapStone[STONE_COLOR_BLACK] = ResourceManager::GetInstance()->GetBitmap("RES//BlackStone.bmp");
	m_pBitmapStone[STONE_COLOR_WHITE] = ResourceManager::GetInstance()->GetBitmap("RES//WhiteStone.bmp");
	
}

bool GameScene::Input(WPARAM key)
{
	switch (key)
	{
		case VK_LEFT:
		{
			if(m_MyPt.x>0)
				m_MyPt.x--;
			break;
		}
		case VK_RIGHT:
		{
			if(m_MyPt.x <18)
				m_MyPt.x++;
			break;
		}
		case VK_UP:
		{
			if (m_MyPt.y >0)
				m_MyPt.y--;
			break;
		}
		case VK_DOWN:
		{
			if (m_MyPt.y <18)
			m_MyPt.y++;
			break;
		}
		case VK_TAB:
			if (m_ChatFlag == true)
				m_ChatFlag = false;
			else
				m_ChatFlag = true;

			break;

		case VK_RETURN:
		{
			if (m_ChatFlag == false)
			{
				if (m_bMyTurn == false || m_wGameState != GAME_STATE_OMOCK_INGAME)
					break;
				if (m_wMap[m_MyPt.y][m_MyPt.x] != EMPTYBLOCK)
				{
					if (m_iSTRCount == 19)
					{
						ArrangeChatSTR();
					}

					m_ChatSTR[m_iSTRCount] = "이미 돌이 있습니다.";
					m_iSTRCount++;

					break;

				}

				m_wMap[m_MyPt.y][m_MyPt.x] = m_bStoneColor;
				if (m_bMyTurn == true)
					m_bMyTurn = false;

			}

			else
			{
				if (m_ChatBufCount == 0)
					return false;

				if (m_iSTRCount == 19)
				{
					ArrangeChatSTR();
				}

				m_ChatSTR[m_iSTRCount] = m_ShowMyChat;
			
				if (m_iSTRCount != 19)
					m_iSTRCount++;
			}
			
			return true;
		}
		break;
	}
	return false;
}

void GameScene::ArrangeChatSTR()
{
		string StrTemp[19];
		m_iSTRCount--;
		for (int i = 18; i >= 1; i--)
		{
			StrTemp[i] = m_ChatSTR[i];
		}

		for (int i = 18; i >= 1; i--)
		{
			m_ChatSTR[i - 1] = StrTemp[i];
		}
}


void GameScene::Chat(char key)
{
	if (key == 'r' && m_bResetFlag == true && m_wGameState != GAME_STATE_OMOCK_INGAME 
		&& m_ChatFlag == false) //리셋검사
	{
		SendPacket();
		return;
	}
	
	if (key == '\t' || key == '\r')
		return;

	if (key == '\b' && m_ChatBuf !=0)
	{
		m_ChatBuf[--m_ChatBufCount] = '\0';
		m_ShowMyChat = m_ChatBuf;
		return;
	}

	if (m_ChatBufCount < 255 && m_ChatFlag == true)
	{
		m_ChatBuf[m_ChatBufCount] = key;
		m_ChatBuf[m_ChatBufCount + 1] = '\0';
		m_ChatBufCount++;
		m_ShowMyChat = m_ChatBuf;
	}

}
void GameScene::Update(float Elapse)
{

}

void GameScene::Draw(HDC hdc)
{
	m_pBitmapBoard->Draw(0, 0);

	for (int y = 0; y < 19; y++)
	{
		for (int x = 0; x < 19; x++)
		{
			if (m_wMap[y][x] == STONE_COLOR_BLACK)
				m_pBitmapStone[STONE_COLOR_BLACK]->Draw(ADJUSTX + x* STONEWIDTH, ADJUSTY + y*STONEHEIGHT);

			else if (m_wMap[y][x] == STONE_COLOR_WHITE)
				m_pBitmapStone[STONE_COLOR_WHITE]->Draw(ADJUSTX + x* STONEWIDTH, ADJUSTY + y*STONEHEIGHT);
		}
	}
	Rectangle(ResourceManager::GetInstance()->GetBackDC(), 640, 0, 1024, 400);
	for (int i = 0; i < 19; i++)
	{
		TextOut(ResourceManager::GetInstance()->GetBackDC(), 650, i * 20 + 10,m_ChatSTR[i].c_str(),m_ChatSTR[i].length());
	} //채팅출력

	if (m_ChatFlag == true)
	{
		HBRUSH Brush, OldBrush;

		Brush = CreateSolidBrush(RGB(136, 136, 136));
		OldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->GetBackDC(), Brush);

		SetBkColor(ResourceManager::GetInstance()->GetBackDC(), RGB(136, 136, 136));
		Rectangle(ResourceManager::GetInstance()->GetBackDC(), 645, 415, 860, 440);

		SelectObject(ResourceManager::GetInstance()->GetBackDC(), OldBrush);
		DeleteObject(Brush);

		TextOut(ResourceManager::GetInstance()->GetBackDC(), 650, 420, m_ShowMyChat.c_str(), m_ShowMyChat.length());
		SetBkColor(ResourceManager::GetInstance()->GetBackDC(), RGB(255, 255, 255));

	} //채팅 입력칸 출력

	else
	{
		HBRUSH Brush, OldBrush;

		Brush = CreateSolidBrush(RGB(213, 213, 213));
		OldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->GetBackDC(), Brush);
		Rectangle(ResourceManager::GetInstance()->GetBackDC(), 645, 415, 860, 440);

		SelectObject(ResourceManager::GetInstance()->GetBackDC(), OldBrush);
		DeleteObject(Brush);
	}

	
	
	m_pBitmapSelect->Draw(m_MyPt.x * STONEWIDTH + ADJUSTX, m_MyPt.y*STONEHEIGHT + ADJUSTY);

}

void GameScene::Release()	
{

}

bool GameScene::RecvPacket(char*szBuf, int &retval, int &Buflen)
{
	memcpy(m_Buf, szBuf, retval);
	Buflen += retval;
	retval = 0;

	if (Buflen < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, m_Buf, sizeof(header));

	if (header.bTrashPacket == true)
		return true;

	if (Buflen < header.wLen)
		return false;


	switch(header.wPacketIndex)
	{
		case PACKET_INDEX_LOGIN_RESULT:
		{
		PACKET_LOGIN_RESULT packet;
		memcpy(&packet, m_Buf, sizeof(packet));
		m_bStoneColor = packet.bStoneColor;

		if (m_bStoneColor == BLACKSTONE)
			m_bMyTurn = true;
		else
			m_bMyTurn = false;
		
		memcpy(m_wMap, packet.Map, sizeof(m_wMap));
		m_wGameState = GAME_STATE_OMOCK_INGAME;
		}

		break;
	
		case PACKET_INDEX_PLAYER_MAP:
		{
			PACKET_PUTXY_INFO packet;
			memcpy(&packet, m_Buf, sizeof(packet));
			m_wMap[packet.PlayerXY.y][packet.PlayerXY.x] = packet.bStoneColor;
			if(m_bMyTurn == false)
				m_bMyTurn = true;
		}
		break;

		case PACKET_INDEX_STRING:
		{
			if (m_iSTRCount == 19)
			{
				ArrangeChatSTR();
			}

			PACKET_STRING packetSTR;
			memcpy(&packetSTR, m_Buf, header.wLen);
			m_ChatSTR[m_iSTRCount] = packetSTR.str;

			if(m_iSTRCount != 19)
				m_iSTRCount++;
		}
		break;
	
		case PACKET_INDEX_COMMAND:
		{
			PACKET_COMMAND packetCmd;
			memcpy(&packetCmd, m_Buf, sizeof(packetCmd));

			CommandProcess(packetCmd.ucCommand);
		}
		break;
		case PACKET_INDEX_GAMERESULT:
		{
			PACKET_RESULT packetRST;
			memcpy(&packetRST, m_Buf, sizeof(packetRST));
			if (packetRST.bGameWin == BLACKSTONE && m_bStoneColor == BLACKSTONE)
			{
				m_wGameState = GAME_STATE_OMOCK_WIN;
			}	
			else if (packetRST.bGameWin == WHITESTONE && m_bStoneColor == WHITESTONE)
			{
				m_wGameState = GAME_STATE_OMOCK_WIN;
			}
			else
			{
				m_wGameState = GAME_STATE_OMOCK_DEFEAT;
			}
		}	
		break;
	}

	memcpy(m_Buf, &m_Buf[header.wLen], Buflen - header.wLen);
	Buflen -= header.wLen;
/*
	if (Buflen < sizeof(PACKET_HEADER)&& Buflen != 0)
		return false;*/

	return true;
}

void GameScene::SendPacket()
{
	
	if (m_bResetFlag == true)
	{
		m_bResetFlag = false;

		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		if (m_bStoneColor == WHITESTONE)
			m_bStoneColor = BLACKSTONE;

		else
			m_bStoneColor = WHITESTONE;

		m_ChatSTR[m_iSTRCount] = "대국이 시작되었습니다.";
		m_iSTRCount++;

		m_wGameState = GAME_STATE_OMOCK_INGAME;
		
		for (int y = 0; y < OMOCK_WHNUM; y++)
		{
			for (int x = 0; x < OMOCK_WHNUM; x++)
			{
				m_wMap[y][x] = EMPTYBLOCK;
			}
		}

		PACKET_COMMAND packetCmd;
		packetCmd.header.bTrashPacket = false;
		packetCmd.header.wLen = sizeof(packetCmd);
		packetCmd.header.wPacketIndex = PACKET_INDEX_COMMAND;
		packetCmd.ucCommand = COMMAND_RESET_GAME;

		send(m_Sock, (char*)&packetCmd, sizeof(packetCmd), 0);

		
		return;

	}

	if (m_ChatFlag == false)
	{

		if (m_wGameState != GAME_STATE_OMOCK_INGAME)
		{

		}
		else
		{
			PACKET_PUTXY_INFO packet2;
			packet2.header.wPacketIndex = PACKET_INDEX_PLAYER_MAP;
			packet2.bStoneColor = m_bStoneColor;
			packet2.header.bTrashPacket = false;
			packet2.PlayerXY = m_MyPt;
			packet2.header.wLen = sizeof(packet2);

			send(m_Sock, (char*)&packet2, sizeof(packet2),0);

		}
	}

	else
	{
		PACKET_STRING packetStr;
		packetStr.header.bTrashPacket = false;
		packetStr.header.wPacketIndex = PACKET_INDEX_STRING;
		strcpy(packetStr.str, m_ChatBuf);

		packetStr.header.wLen = m_ChatBufCount + sizeof(PACKET_HEADER) + 1;

		m_ChatBufCount = 0;
		memset(&m_ChatBuf, 0, sizeof(m_ChatBuf));
		m_ShowMyChat = m_ChatBuf;



		send(m_Sock, (char*)&packetStr, packetStr.header.wLen, 0);
	
	}
	
}

void GameScene::CommandProcess(unsigned char Command)
{
	switch (Command)
	{
	case COMMAND_BLACKTURN:
		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}

		m_ChatSTR[m_iSTRCount] = "검은 돌 차례입니다";
		m_iSTRCount++;

		break;

	case COMMAND_WHITETURN:
		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}

		m_ChatSTR[m_iSTRCount] = "흰 돌 차례입니다";
		m_iSTRCount++;
		break;

	case COMMAND_BLACKWIN:

		m_bResetFlag = true;
		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}

		if (m_bStoneColor == BLACKSTONE)
		{
			m_wGameState = GAME_STATE_OMOCK_WIN;
		
		}
		else
		{
			m_wGameState = GAME_STATE_OMOCK_DEFEAT;
		}

		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		m_ChatSTR[m_iSTRCount] = "검은 돌이 승리했습니다.";

		m_iSTRCount++;

		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		m_ChatSTR[m_iSTRCount] = "다시 시작 할려면 r버튼을 누르세요.";

		m_iSTRCount++;

	
	
		break;
	case COMMAND_WHITEWIN:
		m_bResetFlag = false;
		if (m_bStoneColor == WHITESTONE)
			m_wGameState = GAME_STATE_OMOCK_WIN;
		else
			m_wGameState = GAME_STATE_OMOCK_DEFEAT;

		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		m_ChatSTR[m_iSTRCount] = "흰 돌이 승리했습니다.\n 다시 시작할려면 R버튼을 누르세요";
		m_iSTRCount++;

	
		break;
	case COMMAND_RESET_GAME:
		

		m_bResetFlag = false;

		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}

		m_ChatSTR[m_iSTRCount] = "대국이 시작되었습니다.";
		m_iSTRCount++;

		if (m_bStoneColor == WHITESTONE)
			m_bStoneColor = BLACKSTONE;

		else
			m_bStoneColor = WHITESTONE;


		m_wGameState = GAME_STATE_OMOCK_INGAME;

		for (int y = 0; y < OMOCK_WHNUM; y++)
		{
			for (int x = 0; x < OMOCK_WHNUM; x++)
			{
				m_wMap[y][x] = EMPTYBLOCK;
			}
		}
		break;
	case COMMAND_END_OF_GAME:
		break;
	case COMMAND_RETRY_PUT_33_BLACK:
		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		m_ChatSTR[m_iSTRCount] = "검은돌 33입니다 다시 두세요";

		if (m_bStoneColor == BLACKSTONE)
		{
			m_wMap[m_MyPt.y][m_MyPt.x] = EMPTYBLOCK;
			m_bMyTurn = true;
		}
		
		m_iSTRCount++;
		break;
	case COMMAND_RETRY_PUT_6_BLACK:
		if (m_iSTRCount == 19)
		{
			ArrangeChatSTR();
		}
		m_ChatSTR[m_iSTRCount] = "검은돌 육목입니다 다시 두세요"; 
		if (m_bStoneColor == BLACKSTONE)
		{
			m_wMap[m_MyPt.y][m_MyPt.x] = EMPTYBLOCK;

			m_bMyTurn = true;
		}
		
		m_iSTRCount++;
		break;	
	case COMMAND_RETRY_PUT_33_WHITE:
			if (m_iSTRCount == 19)
			{
				ArrangeChatSTR();
			}
			m_ChatSTR[m_iSTRCount] = "흰돌 33입니다 다시 두세요";
			if (m_bStoneColor == WHITESTONE)
			{
				m_wMap[m_MyPt.y][m_MyPt.x] = EMPTYBLOCK;

				m_bMyTurn = true;
			}
				
			m_iSTRCount++;
			break;
		case COMMAND_RETRY_PUT_6_WHITE:
			if (m_iSTRCount == 19)
			{
				ArrangeChatSTR();
			}
			m_ChatSTR[m_iSTRCount] = "흰돌 육목입니다 다시 두세요"; 
			if (m_bStoneColor == WHITESTONE)
			{
				m_bMyTurn = true;
				m_wMap[m_MyPt.y][m_MyPt.x] = EMPTYBLOCK;


			}
			m_iSTRCount++;

			break;

		case COMMAND_START_GAME:
			if (m_iSTRCount == 19)
			{
				ArrangeChatSTR();
			}
			m_ChatSTR[m_iSTRCount] = "대국이 시작되었습니다.";
			m_iSTRCount++;

			if (m_iSTRCount == 19)
			{
				ArrangeChatSTR();
			}
			if (m_bStoneColor == BLACKSTONE)
			{
			m_ChatSTR[m_iSTRCount] = "검은 돌입니다.";
			m_iSTRCount++;
			}

			else if (m_bStoneColor == WHITESTONE)
			{
				m_ChatSTR[m_iSTRCount] = "흰색 돌입니다.";
				m_iSTRCount++;
			}
			

	default:
		break;
	}


}