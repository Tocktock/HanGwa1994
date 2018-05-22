#include <WinSock2.h>
#include "Bitmap.h"
#include "GameManager.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "PACKET_HEADER.h"

GameScene::GameScene()
{
	m_iBuflen = 0;
	m_iRetval = 0;
}


GameScene::~GameScene()
{
}

void GameScene::Init(HWND hWnd, SOCKET sock)
{
	InitializeCriticalSection(&m_crit);
	m_fNextTurnElapse = 0.0f;
	m_NextTurnCommandFlag = false;
	
	PACKET_COMMAND packetCmd;
	packetCmd.CommandIndex = COMMAND_INDEX_ASK_PLAYERINFO;
	packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
	packetCmd.Header.PacketLen = sizeof(packetCmd);
	send(sock, (char *)&packetCmd, sizeof(packetCmd), 0);
	m_iGameTurn = 0;
	m_iGameState = GAMESTATE_SCENE_ENTER;
	m_drawCount = 0;
	m_pBitmapBack = ResourceManager::GetInstance()->GetBitmap("RES//CatchMindBack.bmp");
	m_pBitmapReadyBox = ResourceManager::GetInstance()->GetBitmap("RES//ReadyBox.bmp");
	m_pBitmapPlayBox = ResourceManager::GetInstance()->GetBitmap("RES//PlayBox.bmp");
	m_pBitmapIngameBox = ResourceManager::GetInstance()->GetBitmap("RES//InGameBox.bmp");
	m_pBitmapChatBox[0] = ResourceManager::GetInstance()->GetBitmap("RES//ChatBoxLeft.bmp");
	m_pBitmapChatBox[1] = ResourceManager::GetInstance()->GetBitmap("RES//ChatBoxRight.bmp");
	m_pBitmapCorrectAnswer = ResourceManager::GetInstance()->GetBitmap("RES//CorrectAnswer.bmp");
	m_pBitmapWordBack = ResourceManager::GetInstance()->GetBitmap("RES//WordBack.bmp");
	
	for (int i = 0; i < 10; i++)
	{
		char sztemp[256];
		sprintf(sztemp, "RES//Num0%d.bmp", i);
		m_pBitmapNum[i] = ResourceManager::GetInstance()->GetBitmap(sztemp);
	}

	
	m_iPenColor = PEN_INFO_BLACK;
	
	m_iPenThickness = PEN_WIDTH_MEDIUM;

	m_sock = sock;
	m_hWnd = hWnd;

	m_iPlayerNum = 0;
	m_rgb.red = 0;
	m_rgb.blue = 0;
	m_rgb.green = 0;
	
	m_backDC = ResourceManager::GetInstance()->GetBackDC();

	SetTextColor(m_backDC, RGB(0, 0, 0));
	SetBkMode(m_backDC, TRANSPARENT);

	m_ChatBufCount = 0;
	m_ChatFlag = false;

	m_DrawFlag = false;
	m_LbuttonDownFlag = false;

	m_rtReadyBox.left = 680;
	m_rtReadyBox.top = 530;
	m_rtReadyBox.right = 780;
	m_rtReadyBox.bottom = 560;


	m_DrawingRect.left = 243;
	m_DrawingRect.top = 33;
	m_DrawingRect.right = 780;
	m_DrawingRect.bottom = 505;

	for (int i = 0; i < PEN_INFO_WHITE + 1; i++)
	{
		m_rtColor[i].left = 260 + i * 34;
		m_rtColor[i].top = 525;
		m_rtColor[i].right = m_rtColor[i].left + 34;
		m_rtColor[i].bottom = m_rtColor[i].top + 40;
	}

	m_rtEraser.left = m_rtColor[PEN_INFO_WHITE].left + 40;
	m_rtEraser.top = 525;
	m_rtEraser.right = m_rtEraser.left + 50;
	m_rtEraser.bottom = 565;

	m_rtEraseAll.left = 570;
	m_rtEraseAll.top = 525;
	m_rtEraseAll.right = m_rtEraseAll.left + 90;
	m_rtEraseAll.bottom = 575;

	m_GameTimeFlag = false;

	m_endFlag = false;

	m_fElapseEnd = 10.0f;
}

void GameScene::Reset()
{
	
	m_fNextTurnElapse = 0.0f;
	m_NextTurnCommandFlag = false;
		
	m_iGameTurn = 0;
	m_iGameState = GAMESTATE_SCENE_ENTER;
	m_drawCount = 0;

	m_iPenColor = PEN_INFO_BLACK;

	m_iPenThickness = PEN_WIDTH_MEDIUM;

	m_rgb.red = 0;
	m_rgb.blue = 0;
	m_rgb.green = 0;

	m_DrawFlag = false;
	m_LbuttonDownFlag = false;

	m_GameTimeFlag = false;

	m_endFlag = false;
	m_fElapseEnd = 10.0f;

	for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
	{
		iter->second->PlayerScore = 0;
	}
}

void GameScene::ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

			if (RecvPacket(szBuf, m_iRetval, m_iBuflen ) == true)
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

bool GameScene::Input(char key)
{
	if (key == '\r')
	{
		if (m_ChatFlag == false)
			m_ChatFlag = true;

		else if (m_ChatBufCount > 0 && m_ChatFlag == true)
		{
			PACKET_STRING packetStr;
			packetStr.header.HeaderIndex = PACKET_INDEX_STRING;
			packetStr.header.PacketLen = sizeof(packetStr.header) + 
				sizeof(packetStr.PlayerNumber) + m_ChatBufCount+1;
			packetStr.PlayerNumber = 255;
			memcpy(packetStr.str, m_ChatBuf, m_ChatBufCount+1);

			send(m_sock, (char*)&packetStr, packetStr.header.PacketLen, 0);
			
			m_mapPlayerInfo[m_iPlayerMyNum]->PlayerChat = m_ChatBuf;
			m_mapPlayerInfo[m_iPlayerMyNum]->ChatFlag = true;
			m_mapPlayerInfo[m_iPlayerMyNum]->ChatElapse = 3.0f;


			m_ChatFlag = false;
			memset(m_ChatBuf, 0, m_ChatBufCount);
			m_ShowMyChat = m_ChatBuf;
			m_ChatBufCount = 0;
			//센드 보내야함
		}
		else if (m_ChatBufCount == 0 && m_ChatFlag == true)
			m_ChatFlag = false;
		return false;
	}

	if (key == '\t')
		return false;

	if (key == '\b' && m_ChatBufCount != 0)
	{
		m_ChatBuf[--m_ChatBufCount] = '\0';
		m_ShowMyChat = m_ChatBuf;
		return false;
	}
	else if (key == '\b')
		return false;

	if (m_ChatBufCount < 255 && m_ChatFlag == true)
	{
		m_ChatBuf[m_ChatBufCount] = key;
		m_ChatBuf[m_ChatBufCount + 1] = '\0';
		m_ChatBufCount++;
		m_ShowMyChat = m_ChatBuf;
	}
	
	return false;

}
bool GameScene::LbuttonUp(int x, int y)
{
	if (m_iGameState == GAMESTATE_SCENE_MY_TURN && m_DrawFlag == true) //클릭 업일 때는 0을 보냄
	{
		POINT temp;
		temp.x = 0;
		temp.y = 0;

		m_vecPoint.push_back(temp);
		m_vecRGB.push_back(m_rgb);
		m_drawCount++;

		PACKET_XY packetXY;
		packetXY.Header.HeaderIndex = PACKET_INDEX_XY;
		packetXY.Header.PacketLen = sizeof(packetXY);
		packetXY.x = 0;
		packetXY.y = 0;

		send(m_sock, (char*)&packetXY, sizeof(packetXY), 0);
	}
	
	m_LbuttonDownFlag = false;
	return false;
}
bool GameScene::LbuttonDown(int x, int y)
{

	m_LbuttonDownFlag = true;

	POINT pt;
	pt.x = x;
	pt.y = y;
	
	for (int i = 0; i < PEN_INFO_WHITE + 1; i++)
	{

		if (PtInRect(&m_rtColor[i], pt))
		{
			m_iPenColor = i;

			
			PACKET_PENINFO packetPen;
			packetPen.Header.HeaderIndex = PACKET_INDEX_PENINFO;
			packetPen.Header.PacketLen = sizeof(packetPen);
			packetPen.penIndex = i;

			ProcessPenInfo(i);

			send(m_sock, (char*)&packetPen, sizeof(packetPen), 0);
			break;
		}

	}

	if (PtInRect(&m_rtEraser, pt))
	{
		m_iPenColor = PEN_INFO_WHITE;

		PACKET_PENINFO packetPen;
		packetPen.Header.HeaderIndex = PACKET_INDEX_PENINFO;
		packetPen.Header.PacketLen = sizeof(packetPen);
		packetPen.penIndex = PEN_INFO_ERASE;
		
		ProcessPenInfo(PEN_INFO_WHITE);

		send(m_sock, (char*)&packetPen, sizeof(packetPen), 0);

	}
	
	else if (PtInRect(&m_rtEraseAll, pt))
	{
	//	m_vecX.clear();
	//	m_vecY.clear();
		m_vecPoint.clear();
		m_vecRGB.clear();
		m_drawCount = 0;

		PACKET_PENINFO packetPen;
		packetPen.Header.HeaderIndex = PACKET_INDEX_PENINFO;
		packetPen.Header.PacketLen = sizeof(packetPen);
		packetPen.penIndex = PEN_INFO_ERASEALL;

		send(m_sock, (char*)&packetPen, sizeof(packetPen),0);

	}

	if (m_iGameState == GAMESTATE_SCENE_ENTER
		&& PtInRect(&m_rtReadyBox,pt) == true)
	{
		m_iGameState = GAMESTATE_SCENE_READY;

		PACKET_COMMAND packetCmd;
		packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
		packetCmd.Header.PacketLen = sizeof(packetCmd);

		packetCmd.CommandIndex = COMMAND_INDEX_STATE_READY;


		send(m_sock,(char*)&packetCmd ,sizeof(packetCmd),0 );
	}

	else if (m_iGameState == GAMESTATE_SCENE_READY
		&& PtInRect(&m_rtReadyBox, pt) == true)
	{
		m_iGameState = GAMESTATE_SCENE_ENTER;
		PACKET_COMMAND packetCmd;
		packetCmd.CommandIndex = COMMAND_INDEX_STATE_NOT_READY;
		packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
		packetCmd.Header.PacketLen = sizeof(packetCmd);

		send(m_sock, (char*)& packetCmd, sizeof(packetCmd), 0);
	}


	//그외에 색깔 선택영역, 지우개영역 채팅영역 설정해야함.
	return false;
}

void GameScene::PaintPicture()
{

	HPEN hPen, oldPen;
	int formerX, formerY;

	for (int i = 1; i < m_drawCount; i++)
	{
		formerX = m_vecPoint[i-1].x;
		formerY = m_vecPoint[i-1].y;

		if (m_vecPoint[i].x == 0)
			continue;

		hPen = CreatePen(PS_SOLID, m_iPenThickness,
			RGB(m_vecRGB[i].red, m_vecRGB[i].green, m_vecRGB[i].blue));

		oldPen = (HPEN)SelectObject(m_backDC, hPen);

		if (m_vecPoint[i - 1].x == 0 && m_vecPoint[i - 1].y == 0)
		{
			SetPixel(m_backDC, m_vecPoint[i].x, m_vecPoint[i].y, 
				RGB(m_vecRGB[i].red, m_vecRGB[i].green, m_vecRGB[i].blue));
			SelectObject(m_backDC, oldPen);
			DeleteObject(hPen);
			continue;

		}


		MoveToEx(m_backDC, formerX, formerY, NULL);
		LineTo(m_backDC, m_vecPoint[i].x, m_vecPoint[i].y);

		SelectObject(m_backDC, oldPen);
		DeleteObject(hPen);
	}

}

void GameScene::Update(float Elapse)
{

	GetCursorPos(&m_ptMouse);
	ScreenToClient(m_hWnd, &m_ptMouse);

	if (m_ptMouse.x > 243 && m_ptMouse.x < 780
		&& m_ptMouse.y>33 && m_ptMouse.y < 505) //그리는 영역
	{
		m_DrawFlag = true;
	}

	else
		m_DrawFlag = false;

	if (m_LbuttonDownFlag == true && m_DrawFlag == true
		&& m_iGameState == GAMESTATE_SCENE_MY_TURN )
	{
		if (m_drawCount > 0)
		{
			if (m_vecPoint[m_drawCount-1].x == m_ptMouse.x
				&&m_vecPoint[m_drawCount - 1].y == m_ptMouse.y)
			{
				return;
			}
		}

		m_vecPoint.push_back(m_ptMouse);
		m_vecRGB.push_back(m_rgb);

//		m_vecX.push_back(m_ptMouse.x);
//		m_vecY.push_back(m_ptMouse.y);
		
		PACKET_XY packetXY;
		packetXY.Header.HeaderIndex = PACKET_INDEX_XY;
		packetXY.Header.PacketLen = sizeof(packetXY);
		packetXY.x = m_vecPoint[m_drawCount].x;
		packetXY.y = m_vecPoint[m_drawCount].y;


		send(m_sock,(char*)&packetXY,sizeof(packetXY),0);
		m_drawCount++;
	}

	for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
	{
		if (iter->second->ChatFlag == true)
		{
			iter->second->ChatElapse -= Elapse;

			if (iter->second->ChatElapse <= 0)
				iter->second->ChatFlag = false;
		}

		if (iter->second->AnswerBoxFlag == true)
		{
			iter->second->fElapaseAnswerBox -= Elapse;

			if (iter->second->fElapaseAnswerBox <= 0)
				iter->second->AnswerBoxFlag = false;
		}
	}

	if (m_GameTimeFlag == true)
	{

		if (m_fGameTime > 0)
		{
			m_fGameTime -= Elapse;
		}
		
		else if (m_fGameTime < 0 && m_iGameState == GAMESTATE_SCENE_MY_TURN)
		{
			m_fGameTime = 0.0f;
			m_NextTurnCommandFlag = true;
		}

	}

	if (m_endFlag == true)
	{
		m_fElapseEnd -= Elapse;
		if (m_fElapseEnd < 0)
			Reset();
	}

	NextTurnCommand(Elapse);
	
}

void GameScene::Draw(HDC hdc)
{
	m_pBitmapBack->Draw(0, 0);
	/*HBRUSH Brush, OldBrush;

	Brush = CreateSolidBrush(RGB(136, 136, 136));
	OldBrush = (HBRUSH)SelectObject(ResourceManager::GetInstance()->GetBackDC(), Brush);

	Rectangle(ResourceManager::GetInstance()->GetBackDC(), 243, 33, 780, 505);

	SelectObject(ResourceManager::GetInstance()->GetBackDC(), OldBrush);
	DeleteObject(Brush);*/

	//for (int y = m_DrawingRect.top; y < m_DrawingRect.bottom; y++)
	//{
	//	for (int x = m_DrawingRect.left; x < m_DrawingRect.right; x++)
	//	{
	//		if (m_PictureBoard[y][x] == (int)COLOR_EMPTY)
	//		{
	//			continue;
	//		}

	//		else if (m_PictureBoard[y][x] == (int)COLOR_BLACK)
	//		{

	//			GetCursorPos(&m_ptMouse);
	//			ScreenToClient(m_hWnd, &m_ptMouse);
	//			MoveToEx(ResourceManager::GetInstance()->GetBackDC(),
	//				m_ptMouse.x, m_ptMouse.y, NULL);
	//			LineTo(ResourceManager::GetInstance()->GetBackDC(),
	//				x, y);
	//			return;
	//		}
	//	}
	//}



	//GetCursorPos(&m_ptMouse);
	//ScreenToClient(m_hWnd, &m_ptMouse);
	//LineTo(ResourceManager::GetInstance()->GetBackDC(),
	//	m_ptMouse.x, m_ptMouse.y);

	//for (int y = m_DrawingRect.top; y < m_DrawingRect.bottom; y++)
	//{
	//	for (int x = m_DrawingRect.left; x < m_DrawingRect.right; x++)
	//	{
	//		m_PictureBoard[m_ptMouse.y][m_ptMouse.x] = m_drawColor;
	//	}
	//}

	/*if (m_iGameState == GAMESTATE_SCENE_MY_TURN)
	{
		string str = "그릴 차례입니다";
		TextOut(m_backDC, 10, 10, str.c_str(), str.length());
	}
	*/
	PaintPicture();

	//Rectangle(m_backDC, m_rtReadyBox.left, m_rtReadyBox.top,
	//	m_rtReadyBox.right, m_rtReadyBox.bottom);
/*
	for (int i = 0; i < PEN_INFO_WHITE + 1; i++) //색깔
	{
		Rectangle(m_backDC, m_rtColor[i].left, m_rtColor[i].top,
			m_rtColor[i].right, m_rtColor[i].bottom);
	}*/
	//Rectangle(m_backDC, m_rtEraser.left, m_rtEraser.top, // 지우개
	//	m_rtEraser.right, m_rtEraser.bottom);

	//Rectangle(m_backDC, m_rtEraseAll.left, m_rtEraseAll.top,
	//	m_rtEraseAll.right, m_rtEraseAll.bottom);


	if (m_iGameState == GAMESTATE_SCENE_ENTER)
		m_pBitmapPlayBox->Draw(m_rtReadyBox.left, m_rtReadyBox.top);

	else if (m_iGameState == GAMESTATE_SCENE_READY)
		m_pBitmapReadyBox->Draw(m_rtReadyBox.left, m_rtReadyBox.top);

	else
		m_pBitmapIngameBox->Draw(m_rtReadyBox.left, m_rtReadyBox.top);


	int temp = 0;
	HFONT myFont = CreateFont(17, 0, 0, 0, 30, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "굴림체");
	HFONT oldFont = (HFONT)SelectObject(m_backDC, myFont);
	for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
	{
		if (temp < 4)
		{
			TextOut(m_backDC, 35, 58 + temp * 157,
				m_mapPlayerInfo[iter->first]->PlayerNickName.c_str(),
				m_mapPlayerInfo[iter->first]->PlayerNickName.length());
			//	TextOut(m_backDC, 20, 100 + i*200 , m_mapPlayerInfo[i]->PlayerNickName.c_str()
			//		, m_mapPlayerInfo[i]->PlayerNickName.length());
			string szTemp;
			szTemp = to_string(m_mapPlayerInfo[iter->first]->PlayerScore);
			TextOut(m_backDC, 35, 90 + temp * 157, szTemp.c_str(), szTemp.length());

			if (iter->second->ChatFlag == true)
			{
				m_pBitmapChatBox[0]->Draw(150, 5 + temp * 157);
				TextOut(m_backDC, 170, 30 + temp * 157,
					iter->second->PlayerChat.c_str(), iter->second->PlayerChat.length());
			}

			if (iter->second->AnswerBoxFlag == true)
			{
				m_pBitmapCorrectAnswer->Draw(10, 20 + temp * 157);
			}
		}

		else if (temp >= 4 && temp < 8)
		{
			TextOut(m_backDC, 920, 58 + temp * 157
				, m_mapPlayerInfo[iter->first]->PlayerNickName.c_str(),
				m_mapPlayerInfo[iter->first]->PlayerNickName.length());

			string szTemp;
			szTemp = to_string(m_mapPlayerInfo[iter->first]->PlayerScore);
			TextOut(m_backDC, 920, 90 + temp * 157, szTemp.c_str(), szTemp.length());

			if (iter->second->ChatFlag == true)
			{
				m_pBitmapChatBox[1]->Draw(750, 5 + temp * 157);
				TextOut(m_backDC, 765, 30 + temp * 157,
					iter->second->PlayerChat.c_str(), iter->second->PlayerChat.length());
			}

			if (iter->second->AnswerBoxFlag == true)
			{
				m_pBitmapCorrectAnswer->Draw(800, 20 + temp * 157);
			}
		}
		temp++;
	}
	SelectObject(m_backDC, myFont);
	DeleteObject(myFont);


	/*for (int i = 0; i < 4; i++)
	{
		TextOut(m_backDC, 35, 90 + i * 157, "string", 6);
		m_pBitmapChatBox[0]->Draw(150, 5 + i * 157);
		m_pBitmapChatBox[1]->Draw(750, 5 + i * 157);

		SetTextColor(m_backDC,RGB(0,0,0));
		SetBkMode(m_backDC, TRANSPARENT);
		TextOut(m_backDC, 170, 30 + i * 157, "Chatstring", 10);
		TextOut(m_backDC, 765, 30 + i * 157, "ChatString",10);
	}*/
	if (m_ChatFlag == true)
	{
		HBRUSH hBrush, oldBrush;
		hBrush = CreateSolidBrush(RGB(136, 136, 136)); //회색
		oldBrush = (HBRUSH)SelectObject(m_backDC, hBrush);
		Rectangle(m_backDC, 500, 600, 700, 630); //채팅창 보여주는거
		SelectObject(m_backDC, oldBrush);
		DeleteObject(hBrush);

		TextOut(m_backDC, 510, 605, m_ShowMyChat.c_str(), m_ShowMyChat.length());
	}
	else
	{
		Rectangle(m_backDC, 500, 600, 700, 630);
	}

	if (m_iGameState == GAMESTATE_SCENE_MY_TURN)
		AnswerDraw();

//	Rectangle(m_backDC, 700, 40, 735, 75);
//	Rectangle(m_backDC, 730, 470, 773, 500);
	string sztemp;

	myFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	oldFont = (HFONT)SelectObject(m_backDC, myFont);
	if (m_fElapseEnd < 10 && m_endFlag == true)
	{
		sztemp = "나의 점수 : ";
		TextOut(m_backDC, 400, 100, sztemp.c_str(), sztemp.length());
		sztemp = to_string(m_mapPlayerInfo[m_iPlayerMyNum]->PlayerScore);
		TextOut(m_backDC, 600, 100, sztemp.c_str(), sztemp.length());
	}

	if (m_fElapseEnd < 9.2f && m_endFlag == true)
	{
		sztemp = "나의 등수 : ";
		TextOut(m_backDC, 400, 150, sztemp.c_str(), sztemp.length());
		RankCalculation();
		sztemp = to_string(m_mapPlayerInfo[m_iPlayerMyNum]->PlayerRank);
		TextOut(m_backDC, 600, 150, sztemp.c_str(), sztemp.length());

	}

	if (m_fElapseEnd < 8.6f && m_endFlag == true)
	{
		sztemp = "승자 : ";
		TextOut(m_backDC, 400, 200, sztemp.c_str(), sztemp.length());
		
		sztemp = m_mapPlayerInfo[m_winnerPlayerNum]->PlayerNickName + " 님!!";
		TextOut(m_backDC, 500, 200, sztemp.c_str(), sztemp.length());
	}

	SelectObject(m_backDC, myFont);
	DeleteObject(myFont);

	if (m_iGameState == GAMESTATE_SCENE_INGAME ||
		m_iGameState == GAMESTATE_SCENE_MY_TURN ||
		m_iGameState == GAMESTATE_SCENE_NOT_MY_TURN
		&&m_fGameTime > 0)
	{
			int temptemp = m_fGameTime / 10;
			m_pBitmapNum[temptemp]->Draw(700, 40);
			temptemp = (int)m_fGameTime % 10;
			m_pBitmapNum[temptemp]->Draw(738, 40);

			myFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
			oldFont = (HFONT)SelectObject(m_backDC, myFont);

			string strtemp = to_string(m_iGameTurn) + "/ 10";
			TextOut(m_backDC, 730, 470, strtemp.c_str(), strtemp.length());
			

			SelectObject(m_backDC, myFont);
			DeleteObject(myFont);
	}


}
	
void GameScene::Release()
{

}
bool GameScene::RecvPacket(char*szBuf, int retval, int Buflen)
{
	memcpy(m_buf, szBuf, retval);
	Buflen += retval;
	retval = 0;

	while(true)
	{
		PACKET_HEADER header;
		memcpy(&header, m_buf, sizeof(PACKET_HEADER));

		switch (header.HeaderIndex)
		{
		case PACKET_INDEX_CORRECT_ANSWER:
		{
			PACKET_CORRECT_ANSWER packetCorAns;
			memcpy(&packetCorAns, m_buf, sizeof(packetCorAns));
			m_mapPlayerInfo[packetCorAns.PlayerNumber]->PlayerScore += packetCorAns.Score;
			m_mapPlayerInfo[packetCorAns.PlayerNumber]->AnswerBoxFlag = true;
			m_mapPlayerInfo[packetCorAns.PlayerNumber]->fElapaseAnswerBox = 3.0f;
			if(m_iGameState == GAMESTATE_SCENE_MY_TURN)
				m_NextTurnCommandFlag = true;
		}
		break;
			case PACKET_INDEX_NICKNAME:
			{
			PACKET_NICKNAME packetNick;
			memcpy(&packetNick, m_buf, sizeof(packetNick));
			PLAYERINFO_SCENE *pInfo = new PLAYERINFO_SCENE;
			pInfo->PlayerRank = 0;
			pInfo->PlayerNickName = packetNick.str;
			pInfo->PlayerScore = 0;
			pInfo->ChatElapse = 0;
			pInfo->ChatFlag = false;
			pInfo->fElapaseAnswerBox = 0;
			pInfo->AnswerBoxFlag = false;

			if (m_iPlayerMyNum == packetNick.PlayerNumber)
				break;

			if (GameManager::GetInstance()->GetMyNick() == pInfo->PlayerNickName)
				m_iPlayerMyNum = packetNick.PlayerNumber;


			m_mapPlayerInfo.insert(make_pair(packetNick.PlayerNumber, pInfo));
			m_iPlayerNum++;
			}
			break;
			case PACKET_INDEX_COMMAND:
			{
			PACKET_COMMAND packetCmd;
			memcpy(&packetCmd, m_buf, sizeof(packetCmd));
			ProcessCommand(packetCmd.CommandIndex);
			}
			break;
			case PACKET_INDEX_STRING:
			{
			PACKET_STRING packetStr;
			memcpy(&packetStr, m_buf, sizeof(packetStr));
			m_mapPlayerInfo[packetStr.PlayerNumber]->PlayerChat = packetStr.str;
			m_mapPlayerInfo[packetStr.PlayerNumber]->ChatFlag = true;
			m_mapPlayerInfo[packetStr.PlayerNumber]->ChatElapse = 3.0f;

			}
			break;
			case PACKET_INDEX_XY:
			{
			PACKET_XY packetXY;
			memcpy(&packetXY, m_buf, sizeof(packetXY));
			POINT temp;
			temp.x = packetXY.x;
			temp.y = packetXY.y;
			m_vecPoint.push_back(temp);
			m_vecRGB.push_back(m_rgb);
			m_drawCount++;
			}
			break;
			case PACKET_INDEX_PENINFO:
			{
			PACKET_PENINFO packetPen;
			memcpy(&packetPen, m_buf, sizeof(packetPen));
			ProcessPenInfo(packetPen.penIndex);

			}
			break;

			case PACKET_INDEX_ANSWER:
			{
				PACKET_ANSWER packetAns;
				memcpy(&packetAns, m_buf, sizeof(packetAns));
				m_iAnswer = packetAns.answerIndex;
			}
			break;

			case PACKET_INDEX_QUIT_PLAYER:
			{
				PACKET_QUIT_PLAYER packetQuitPlayer;
				memcpy(&packetQuitPlayer, m_buf, sizeof(packetQuitPlayer));

				m_iPlayerMyNum = 10000; //임의의값

				
				for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
				{
					delete(iter->second);
					
				}
				m_mapPlayerInfo.clear();
				
				if (m_mapPlayerInfo.size() <= 1)
				{
					m_iGameState = GAMESTATE_SCENE_ENTER;
				}

				PACKET_COMMAND packetCmd;
				packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
				packetCmd.Header.PacketLen = sizeof(packetCmd);
				packetCmd.CommandIndex = COMMAND_INDEX_ASK_PLAYERINFO;

				send(m_sock, (char*)&packetCmd, sizeof(packetCmd), 0);

			}
			break;

		}

		memcpy(m_buf, &m_buf[header.PacketLen], Buflen - header.PacketLen);
		Buflen -= header.PacketLen;

		if (Buflen == 0)
			break;

	}

	
	return false;

}
void GameScene::SendPacket(char* buf, int packetLen)
{

}
void GameScene::Chat(char key)
{

}

void GameScene::ProcessPenInfo(int Color)
{
	switch (Color)
	{
	case PEN_INFO_BLACK:
		{
			m_rgb.red = 0;
			m_rgb.blue = 0;
			m_rgb.green = 0;
		}
		break;

	case PEN_INFO_RED:
		{
			m_rgb.red = 255;
			m_rgb.green = 0;
			m_rgb.blue = 0;
		}
		break;
	case PEN_INFO_BLUE:
		{

			m_rgb.red = 0;
			m_rgb.green = 0;
			m_rgb.blue = 255;
		}
		break;
	case PEN_INFO_GREEN:
		{
			m_rgb.red = 0;
			m_rgb.green = 255;
			m_rgb.blue = 0;
		}
		break;
	case PEN_INFO_YELLOW:
		{
			m_rgb.red = 255;
			m_rgb.green = 228;
			m_rgb.blue = 0;
		}
		break;
	case PEN_INFO_WHITE:
		{
			m_rgb.red = 255;
			m_rgb.green = 255;
			m_rgb.blue = 255;
		}
		
		break;

	case PEN_INFO_ERASE:
	{
		m_rgb.red = 255;
		m_rgb.green = 255;
		m_rgb.blue = 255;
	}
	break;

	case PEN_INFO_ERASEALL:
	{
		m_vecPoint.clear();
		m_vecRGB.clear();
		m_drawCount = 0;
	}
		break;
	case PEN_INFO_THIN:
		m_iPenThickness = PEN_WIDTH_THIN;
		break;
	case PEN_INFO_MEDIUM:
		m_iPenThickness = PEN_WIDTH_MEDIUM;
		break;
	case PEN_INFO_THICK:
		m_iPenThickness = PEN_WIDTH_THICK; 
		break;
				
	}
}

void GameScene::ProcessCommand(int Command)
{
	switch (Command)
	{

	case COMMAND_INDEX_END_OF_GAME:
		{
			m_endFlag = true;
		}
		break;

		case COMMAND_INDEX_STARTGAME:
		{
			m_iGameState = GAMESTATE_SCENE_INGAME;
			m_iGameTurn++;
			m_fGameTime = 60.0f;
			m_GameTimeFlag = true;
			m_vecPoint.clear();
			m_vecRGB.clear();
			m_drawCount = 0;
		}
		break;
		
		case COMMAND_INDEX_STATE_MY_TURN:
		{
			m_iGameState = GAMESTATE_SCENE_MY_TURN;
			m_fGameTime = 60.0f;
			m_vecPoint.clear();
			m_vecRGB.clear();
			m_drawCount = 0;
		}
		break;

		case COMMAND_INDEX_STATE_NOT_MY_TURN:
		{

			m_iGameState = GAMESTATE_SCENE_INGAME;
			m_iGameTurn++;
			m_fGameTime = 60.0f;
			m_GameTimeFlag = true;
			m_vecPoint.clear();
			m_vecRGB.clear();
			m_drawCount = 0;
			m_AnswerStr = "";
		
		}
		break;
	}
	
}

void GameScene::AnswerDraw()
{
	switch (m_iAnswer)
	{
	case QUESTION_INDEX_COMPUTER:
	{
		m_AnswerStr = "컴퓨터";
	}
	break;
	case QUESTION_INDEX_TELEPHONE:
	{

		m_AnswerStr = "휴대폰";
	}
	break;
	case QUESTION_INDEX_DRUG:
	{

		m_AnswerStr = "알약";
	}
	break;
	case QUESTION_INDEX_TOOTH:
	{

		m_AnswerStr = "이빨";
	}
	break;
	
	case QUESTION_INDEX_SINGER:
	{

		m_AnswerStr = "가수";
	}
	break;

	case QUESTION_INDEX_SKY:
	{

		m_AnswerStr = "하늘";
	}
	break;
	case QUESTION_INDEX_BACKPACK:
	{

		m_AnswerStr = "가방";
	}
	break;

	case QUESTION_INDEX_CAT:
	{

		m_AnswerStr = "고양이";
	}
	break;
	}

	m_pBitmapWordBack->Draw(240, 35);
	
	TextOut(m_backDC, 255, 55, m_AnswerStr.c_str(), m_AnswerStr.length());


}


void GameScene::NextTurnCommand(float elapse)
{
	if (m_iGameState != GAMESTATE_SCENE_MY_TURN)
		return;

	if (m_NextTurnCommandFlag == true)
	{
		m_fNextTurnElapse += elapse;

		if (m_fNextTurnElapse >= 3.0f)
		{
			PACKET_COMMAND packetCmd;
			packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
			packetCmd.Header.PacketLen = sizeof(packetCmd);
			packetCmd.CommandIndex = COMMAND_INDEX_NEXT_TURN;

			send(m_sock, (char*)&packetCmd, sizeof(packetCmd), 0);

			m_NextTurnCommandFlag = false;
			m_fNextTurnElapse = 0.0f;

		}

	}
}

void GameScene::RankCalculation()
{
	for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter ++ )
	{
		iter->second->PlayerRank = m_mapPlayerInfo.size();
		for (auto iter2 = m_mapPlayerInfo.begin(); iter2 != m_mapPlayerInfo.end(); iter2 ++ )
		{
			if (iter->second->PlayerScore > iter2->second->PlayerScore)
				iter->second->PlayerRank--;
		}

		if (iter->second->PlayerRank == 1)
			m_winnerPlayerNum = iter->first;
	}

	
}