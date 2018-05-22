#include <ctime>
#include <Windows.h>
#include <string>
#include "GameManager.h"
#include "Bitmap.h"
#include "ResourceManager.h"
#include "Character.h"
#include "Enemy.h"
#include "PotFlame.h"
#include "CirCusCircle.h"
#include "Back.h"
#include "End.h"
#include "UI.h"
#include "UIIcon.h"
#include "UIStar.h"
#include "LuckyCircle.h"

GameManager* GameManager::s_this = NULL;

GameManager::GameManager()
{
	srand(time(NULL));
	isGameEnd = GAMING;
	m_WindowHegiht = 0;
	m_WindowWidth = 0;
	m_MeterCt = 0;
	m_BackX = 0;
	m_MeterX = 0;
	m_MeterValue = 100;
	m_Life = 3;
	movePoint = 0;
	m_Bonus = 5000;
	m_Stage = 1;
	m_Score = 0;
	m_HighScore = 20000;
	m_isStart = false;
	m_MenuSelect = 360;
	m_CharacterResetX = 0;
	m_TermDraw = false;
	m_arrBackBitmap[0]=ResourceManager::GetInstance()->GetImage(RES_TYPE_BACK);
	m_arrBackBitmap[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_BACKDECO);
	m_arrBackBitmap[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_BACKNORMAL1);
	m_arrBackBitmap[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_BACKNORMAL2);

	m_arrCharacterBitmap[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYER0);
	m_arrCharacterBitmap[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYER1);
	m_arrCharacterBitmap[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYER2);
	m_arrCharacterBitmap[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYERWIN1);
	m_arrCharacterBitmap[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYERWIN2);
	m_arrCharacterBitmap[5] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYERDIE);

	m_Meter = ResourceManager::GetInstance()->GetImage(RES_TYPE_METER);

	m_arrEnemy[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_FRONT1);
	m_arrEnemy[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_FRONT2);

	m_arrEnemy[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_CIRCLE1_B);
	m_arrEnemy[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_CIRCLE1_F);

	m_arrEnemy[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_CIRCLE2_B);
	m_arrEnemy[5] = ResourceManager::GetInstance()->GetImage(RES_TYPE_CIRCLE2_F);

	m_EndCircleBitmap = ResourceManager::GetInstance()->GetImage(RES_TYPE_ENDCIRCLE);

	m_UIBitmap[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_UIBACK);
	m_UIBitmap[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_UISTAR1);
	m_UIBitmap[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_UISTAR2);
	m_UIBitmap[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_UISTAR3);
	m_UIBitmap[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ICON);

	m_Money[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_LUCKYCIRCLE1_F);
	m_Money[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_LUCKYCIRCLE1_B);
	m_Money[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_LUCKYCIRCLE2_F);
	m_Money[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_LUCKYCIRCLE2_B);
	m_Money[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_MONEY);


}

GameManager::~GameManager()
{
	SelectObject(m_hMemDC, m_hOldBit);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
	SAFE_DELETE(m_Character);
	SAFE_DELETE(m_EndCircle);
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecCircle.clear();
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecEnemy.clear();
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}

	m_vecBack.clear();
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecLuckyCircle.clear();
}

void GameManager::DubbleBufferInit(HDC hdc)
{

	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitMap = CreateCompatibleBitmap(hdc, 1024, 768);
	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

}

void GameManager::Init(HWND hWNd)
{
	
	m_Character = new Character();
	m_Character->CharacterInit(m_arrCharacterBitmap[0], m_arrCharacterBitmap[1], m_arrCharacterBitmap[2],
		m_arrCharacterBitmap[3], m_arrCharacterBitmap[4], m_arrCharacterBitmap[5], 30, 300);

	UIStar *pStar1 = new UIStar();
	pStar1->Init(m_UIBitmap[1], m_UIBitmap[2], m_UIBitmap[3], 110, 100, 14, 12,WhatAMI_STAR);
	pStar1->StateSet(1);
	m_vecUIStar.push_back(pStar1);

	UIStar *pStar2 = new UIStar();
	pStar2->Init(m_UIBitmap[1], m_UIBitmap[2], m_UIBitmap[3], 125, 100, 14, 12, WhatAMI_STAR);
	pStar2->StateSet(2);
	m_vecUIStar.push_back(pStar2);

	UIStar *pStar3 = new UIStar();
	pStar3->Init(m_UIBitmap[1], m_UIBitmap[2], m_UIBitmap[3], 140, 100, 14, 12, WhatAMI_STAR);
	pStar3->StateSet(3);
	m_vecUIStar.push_back(pStar3);

	for (int i = 0; i < 100;i++)
	{
		if (rand() % 10 <6)
		{
			if (300 + i * 600 < 7300)
			{
				PotFlame *pPotFlame = new PotFlame();
				pPotFlame->Init(m_arrEnemy[0], m_arrEnemy[1], movePoint + 300 + i * 400, 313);
				m_vecEnemy.push_back(pPotFlame);

			}
		}
	}

	static int BackWidthValue = 0;
	static int BackDecoWidthValue = 0;
	m_MeterCt = 0;

	for (int i = 0; i < 110; i++)
	{
		m_MeterCt++;
		Back *pBack = new Back();
		pBack->Init(m_arrBackBitmap[0], m_arrBackBitmap[1], m_arrBackBitmap[2], m_arrBackBitmap[3],
		m_Meter,BackWidthValue, BACK_HEIGHT, 0, BACK_WIDTH, BACK_HEIGHT);
		m_vecBack.push_back(pBack);
		BackWidthValue += BACK_WIDTH;

		if (m_MeterCt == 10)
		{
			Back *pBack = new Back();
			pBack->Init(m_arrBackBitmap[0], m_arrBackBitmap[1], m_arrBackBitmap[2], m_arrBackBitmap[3],
				m_Meter,BackWidthValue, 367, 4,86,30);
			m_vecBack.push_back(pBack);
			m_MeterCt = 0;
		}
	}

	m_EndCircle = new End();
	m_EndCircle->Init(m_EndCircleBitmap, BackWidthValue, 315);

	for (int i = 0; i < 10; i++)
	{
		Back *pBack = new Back();
		pBack->Init(m_arrBackBitmap[0], m_arrBackBitmap[1], m_arrBackBitmap[2], m_arrBackBitmap[3],
		m_Meter, BackWidthValue, BACK_HEIGHT, 0, BACK_WIDTH, BACK_HEIGHT);
		m_vecBack.push_back(pBack);
		BackWidthValue += BACK_WIDTH;
	}


	for (int i = 0; i < 120;i+=10)
	{
		Back *pBack = new Back(); pBack->Init(m_arrBackBitmap[0], m_arrBackBitmap[1], m_arrBackBitmap[2], m_arrBackBitmap[3],
			m_Meter,BackDecoWidthValue, 150 - BACKDECO_HEIGHT, 1, BACKDECO_WIDTH, BACKDECO_HEIGHT);
		m_vecBack.push_back(pBack);
		BackDecoWidthValue += BACKDECO_WIDTH;
		
		for (int j = 1; j < 10; j++)
		{
			Back *pBack = new Back();
			pBack->Init(m_arrBackBitmap[0], m_arrBackBitmap[1], m_arrBackBitmap[2], m_arrBackBitmap[3],
				m_Meter,BackDecoWidthValue, 150 - BACKDECO_HEIGHT, 2, BACKNORMAL1_WIDTH, BACKNORMAL1_HEIGHT);
			m_vecBack.push_back(pBack);
			BackDecoWidthValue += BACKNORMAL1_WIDTH;
		}
	}


	UI * pUIBack = new UI();
	pUIBack->Init(m_UIBitmap[0], 10, 10, 490,60,WhatAmI_BACK);
	m_vecUI.push_back(pUIBack);

	UI *pUIIcon = new UI();
	pUIIcon->Init(m_UIBitmap[4], 470, 47, 15, 13,WhatAmI_ICON);
	m_vecUI.push_back(pUIIcon);

	BackWidthValue = 0;
	BackDecoWidthValue = 0;
	m_MeterCt = 0;

	WhenDeadXSet();
}



void GameManager::EnemyCircle()
{
	if (rand()%6 == 0)
	{
		LuckyCircle *pLuckyCircle = new LuckyCircle();
		pLuckyCircle->Init(m_Money[0], m_Money[1], m_Money[2], m_Money[3], m_Money[4], m_Character->GetNowX() + 500, 180, 25, 104);
		m_vecLuckyCircle.push_back(pLuckyCircle);
	}
	else
	{
		CirCusCircle *pCirCusCircle = new CirCusCircle();
		pCirCusCircle->Init(m_arrEnemy[2], m_arrEnemy[3], m_arrEnemy[4], m_arrEnemy[5], m_Character->GetNowX() + 500, 180);
		m_vecCircle.push_back(pCirCusCircle);
	}
}
void GameManager::CircleMove()
{
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->CircleMoving();
	}
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->CircleMoving();
	}
}

void GameManager::FinalDraw(HDC hdc)
{

	HBRUSH Brush, oldBrush;
	int backwidth = m_BackX;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(m_hMemDC, Brush);
	Rectangle(m_hMemDC, 0, 0, 512, 512);
	SelectObject(m_hMemDC, oldBrush);
	DeleteObject(Brush);

	if (m_TermDraw == true)
	{
		TermDraw(m_hMemDC);
		
	}

	else if (m_isStart == false)
		MenuDraw(m_hMemDC);

	else if(m_isStart == true)
		GameDraw(m_hMemDC);

	

	BitBlt(hdc, 0, 0, 512, 512, m_hMemDC, 0, 0, SRCCOPY);
	
	
}

void GameManager::EnemyStateCtPlus()
{

	if (m_vecEnemy.size() == NULL && m_vecCircle.size() == NULL)
		return;

	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		if ((*iter)->GetX() > 512)
			break;

		(*iter)->stateCt();
	}

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->stateCt();
	}
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->stateCt();
	}
}

void GameManager:: PotFlameSetRt()
{
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		(*iter)->SetmyRt();
	}
}

void GameManager::CircusCircleSetRt()
{
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->SetmyRt();
	}
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->SetmyRt();
	}
}
void GameManager::CollisionCheck()
{
	if (isGameEnd == WIN || isGameEnd == DEAD)
	{
		return;
	}

	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}

		else if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetSecondCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetSecondCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetSecondCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetSecondCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}
	
		else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}

		else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetSecondCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetSecondCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetSecondCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetSecondCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}
	}

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}

	else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		{
			m_Character->GameEnd();
			isGameEnd = DEAD;
		}
			
	}

	 if (m_Character->GetFirstCollisionRtLeft() < m_EndCircle->GetFirstCollisionRtRight()
		&& m_Character->GetFirstCollisionRtLeft() > m_EndCircle->GetFirstCollisionRtLeft()
		&& m_Character->GetFirstCollisionRtLeft() < m_EndCircle->GetFirstCollisionRtBottom()
		&& m_Character->GetFirstCollisionRtLeft() > m_EndCircle->GetFirstCollisionRtTop())
	{
		m_Character->GameWinFirst();
		isGameEnd = WIN;
		m_Character->GameWin(m_EndCircle->GetX(), m_EndCircle->GetY()-60);
	}

	 else if (m_Character->GetSecondCollisionRtLeft() < m_EndCircle->GetFirstCollisionRtRight()
		 && m_Character->GetSecondCollisionRtRight() > m_EndCircle->GetFirstCollisionRtLeft()
		 && m_Character->GetSecondCollisionRtTop() <  m_EndCircle->GetFirstCollisionRtBottom()
		 && m_Character->GetSecondCollisionRtBottom() > m_EndCircle->GetFirstCollisionRtTop())
	 {
		 m_Character->GameWinFirst();
		 isGameEnd = WIN;
		 m_Character->GameWin(m_EndCircle->GetX(), m_EndCircle->GetY()-60);
	 }

	 for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	 {
		 if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			 && m_Character->GetFirstCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			 && m_Character->GetFirstCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			 && m_Character->GetFirstCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		 {
			 m_Character->GameEnd();
			 isGameEnd = DEAD;
		 }

		 else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetFirstCollisionRight()
			 && m_Character->GetSecondCollisionRtRight() > (*iter)->GetFirstCollisionLeft()
			 && m_Character->GetSecondCollisionRtTop() < (*iter)->GetFirstCollisionBottom()
			 && m_Character->GetSecondCollisionRtBottom() > (*iter)->GetFirstCollisionTop())
		 {
			 m_Character->GameEnd();
			 isGameEnd = DEAD;
		 }
	 }
}

void GameManager::CharacterLeft()
{
	if (movePoint < 0)
		return;

	if (movePoint > 7000)
	{
		m_Character->MoveLeft();
		return;
	}
	
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{

		(*iter)->MoveLeft();
	}

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->MoveLeft();
	}
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		(*iter)->MoveLeft();

	}
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->MoveLeft();
	}

	m_EndCircle->MovingLeft();

	movePoint -= 2;
}
void GameManager::CharacterRight()
{


	if (movePoint > 7000)
	{
		m_Character->MoveRight();
		return;
	}
			
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		(*iter)->MoveRight();
	}

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->MoveRight();
	}
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		(*iter)->MoveRight();
	}

	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->MoveRight();
	}
	m_EndCircle->MoveRight();

	movePoint += 2;
}

void GameManager::EndSetRt()
{
	m_EndCircle->SetmyRect();
}
void GameManager::WinFirstState()
{
	m_Character->GameWinFirst();
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == 2)
		{
			(*iter)->WinFirst();
		}
	}
}
void GameManager::WinSecondState()
{
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == 3)
		{
			(*iter)->WinSecond();
		}
	}
	m_Character->GameWinSecond();
}

void GameManager::WriteTextNum(HDC hdc,int value,int x,int y)
{
	string WriteText;
	WriteText = to_string(value);
	HFONT myFont = CreateFont(20, 10, 0, 0, 2000, false, true, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Yj BUTGOT");
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(255, 255, 255));											//텍스트배경 색깔
	SetBkMode(hdc, TRANSPARENT);
	HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
	TextOut(hdc, x, y, WriteText.c_str(), WriteText.length());
	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}

void GameManager::WriteTextChar(HDC hdc, string temp, int x, int y, int r, int g, int b)
{
	HFONT myFont = CreateFont(23, 10, 0, 0, 200, false, false, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "고딕체");
	SetTextColor(hdc, RGB(r, g, b));
	SetBkColor(hdc, RGB(255, 255, 255));											//텍스트배경 색깔
	SetBkMode(hdc, TRANSPARENT);
	HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
	TextOut(hdc, x, y,temp.c_str(), temp.length());
	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
}

void  GameManager::Reset()
{
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecEnemy.clear();
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBack.clear();
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecCircle.clear();
	for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecUI.clear();

	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecLuckyCircle.clear();

	SAFE_DELETE(m_EndCircle);
	SAFE_DELETE(m_Character);

	isGameEnd = GAMING;
	m_WindowHegiht = 0;
	m_WindowWidth = 0;
	m_MeterCt = 0;
	m_BackX = 0;
	m_MeterX = 0;
	m_MeterValue = 100;
	m_Life  -=1;

	m_CharacterResetX = movePoint ;
	
	m_Stage = 1;
	m_HighScore = 20000;
}

void GameManager::MenuDraw(HDC hdc)
{

	HBRUSH Brush, oldBrush;
	int backwidth = m_BackX;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, 0, 0, 512, 512);
	SelectObject(hdc, oldBrush);
	DeleteObject(Brush);

	int temp = 0;
	for (auto iter = m_vecUIStar.begin(); iter != m_vecUIStar.end(); iter++)
	{
		
		for (int i = 0; i < 6; i++)
		{
			(*iter)->Draw(hdc);
			(*iter)->StarYPlus(195);
			(*iter)->Draw(hdc);
			(*iter)->StarYPlus(-195);
			(*iter)->StarXPlus(45);
		}

		if (temp == 0)
			(*iter)->StarYPlus(15);

		else if (temp == 1)
			(*iter)->StarYPlus(30);

		else if (temp == 2)
			(*iter)->StarYPlus(45);

		temp++;
		(*iter)->StarXLineSet(380);
	}

	for (auto iter = m_vecUIStar.begin(); iter != m_vecUIStar.end(); iter++)
	{

		for (int i = 0; i < 4; i++)
		{
			(*iter)->Draw(hdc);
			(*iter)->StarXPlus(-285);
			(*iter)->Draw(hdc);
			(*iter)->StarXPlus(285);
			(*iter)->StarYPlus(45);

		}

		(*iter)->ResetXY();
			
	}

	WriteTextChar(hdc, "START", 230, 360, 255, 255, 255);
	WriteTextChar(hdc, "EXIT", 230, 390, 255, 255, 255);
	WriteTextChar(hdc, "->", 190, m_MenuSelect, 255, 255, 255);
}

void GameManager::GameDraw(HDC hdc)
{
	HBRUSH Brush, oldBrush;
	int backwidth = m_BackX;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, 0, 0, 512, 512);
	SelectObject(hdc, oldBrush);
	DeleteObject(Brush);





	//RECT tempRt = m_Character->GetFirstCollisionRect();
	//RECT tempRt2 = m_Character->GetSecondCollisionRect();
	//Rectangle(hdc, tempRt.left, tempRt.top, tempRt.right, tempRt.bottom);
	//Rectangle(hdc, tempRt2.left, tempRt2.top, tempRt2.right, tempRt2.bottom);

	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			continue;
		}

		(*iter)->Draw(hdc);

		if ((*iter)->GetWhatAmI() == 4)
		{
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(255, 255, 255));											//텍스트배경 색깔
			SetBkMode(hdc, TRANSPARENT);
			string szMeter_str = to_string(m_MeterValue);
			TextOut(hdc, (*iter)->GetX() + 10, (*iter)->GetY() + 5, szMeter_str.c_str(), szMeter_str.length());
			m_MeterValue -= 10;
		}
	}

	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			break;
		}

		(*iter)->Draw(hdc);
		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//		RECT szTEmpRt2 = (*iter)->GetSecondCollisionRect();
		//		Rectangle(hdc, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);
		//		Rectangle(hdc, szTEmpRt2.left, szTEmpRt2.top, szTEmpRt2.right, szTEmpRt2.bottom);
	}


	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			continue;
		}
		(*iter)->FrontDraw(hdc);
		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

	}


	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			continue;
		}
		(*iter)->FrontDraw(hdc);
		(*iter)->MoneyDraw(hdc);
		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

	}

	//RECT szTempRt = m_EndCircle->GetFirstCollisionRect();
	//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

	m_EndCircle->Draw(hdc);
	m_Character->Draw(hdc);

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			continue;
		}
		(*iter)->BackDraw(hdc);
		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

	}


	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{

		if ((*iter)->GetX() > 512)
		{
			continue;
		}
		(*iter)->BackDraw(hdc);
		(*iter)->MoneyDraw(hdc);
		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

		//	RECT szTempRt = (*iter)->GetFirstCollisionRect();
		//	Rectangle(m_hMemDC, szTempRt.left, szTempRt.top, szTempRt.right, szTempRt.bottom);

	}

	for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == WhatAmI_BACK)
		{
			(*iter)->Draw(hdc);
		}

		else if ((*iter)->GetWhatAmI() == WhatAmI_ICON)
		{
			for (int i = 0; i < m_Life; i++)
			{
				(*iter)->Draw(hdc);
				(*iter)->Xminus();
			}

			(*iter)->XSet(470);
		}

	}


	WriteTextChar(hdc, "점수", 55, 20, 241, 95, 95);
	WriteTextNum(hdc, m_Score, 70, 40);

	WriteTextChar(hdc, "보너스", 185, 20, 255, 0, 0);
	WriteTextNum(hdc, m_Bonus, 195, 40);

	WriteTextChar(hdc, "최고점수", 295, 20, 255, 200, 0);
	WriteTextNum(hdc, m_HighScore, 310, 40);

	WriteTextChar(hdc, "목숨", 442, 20, 107, 102, 255);
	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		if ((*iter)->isEaten() == true)
		{
			WriteTextNum(hdc, 500, (*iter)->GetX() + 30, 200);
		}
	}
	m_MeterValue = 100;

}

void GameManager::StarStatePlus()
{
	for (auto iter = m_vecUIStar.begin(); iter != m_vecUIStar.end(); iter++)
	{
		(*iter)->StatePlus();
	}
}

void GameManager::StatePlus()
{
	for (auto iter = m_vecUIStar.begin(); iter != m_vecUIStar.end(); iter++)
	{
		(*iter)->StatePlus();

		if ((*iter)->GetState() > 3)
		{
			(*iter)->StateSet(1);
		}
	}
}

int GameManager::EnemyScoreCheck()
{

	int temp = 0;

	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{
			
			temp += 100;
		}



		else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{
		
			temp += 100;
		}
	}

	
	return temp;
}

int GameManager::CircleScoreCheck()
{
	int temp = 0;
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{

			temp += 100;
		}


		else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{

			temp += 100;
		}

	}

	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		if (m_Character->GetFirstCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetFirstCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetFirstCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetFirstCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{
			(*iter)->Eaten();
			temp += 500;;
		}


		else if (m_Character->GetSecondCollisionRtLeft() < (*iter)->GetScoreCollisionRight()
			&& m_Character->GetSecondCollisionRtRight() > (*iter)->GetScoreCollisionLeft()
			&& m_Character->GetSecondCollisionRtTop() < (*iter)->GetScoreCollisionBottom()
			&& m_Character->GetSecondCollisionRtBottom() > (*iter)->GetScoreCollisionTop())
		{
			(*iter)->Eaten();
			temp += 500;
		}

	}
	return temp;
}

void GameManager::ToMenuReset()
{
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecEnemy.clear();
	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBack.clear();
	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecCircle.clear();
	for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecUI.clear();

	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecLuckyCircle.clear();

	SAFE_DELETE(m_EndCircle);
	SAFE_DELETE(m_Character);

	isGameEnd = GAMING;
	m_WindowHegiht = 0;
	m_WindowWidth = 0;
	m_MeterCt = 0;
	m_BackX = 0;
	m_MeterX = 0;
	m_MeterValue = 100;
	m_Life = 3;
	movePoint = 0;
	m_CharacterResetX = 0;
	m_isStart = false;
	m_Bonus = 5000;
	m_Stage = 1;
	m_HighScore = 20000;
	m_Score = 0;

}

void GameManager::TermDraw(HDC hdc)
{

	HBRUSH Brush, oldBrush;
	int backwidth = m_BackX;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, Brush);
	Rectangle(hdc, 0, 0, 512, 512);
	SelectObject(hdc, oldBrush);
	DeleteObject(Brush);

	WriteTextChar(hdc, "Stage 01", 230, 230, 255, 255, 255);
	
}

void GameManager::WhenDeadXSet()
{

	for (auto iter = m_vecBack.begin(); iter != m_vecBack.end(); iter++)
	{
		(*iter)->SetX(m_CharacterResetX);
	}

	for (auto iter = m_vecCircle.begin(); iter != m_vecCircle.end(); iter++)
	{
		(*iter)->SetX(m_CharacterResetX);
	}
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		(*iter)->SetX(m_CharacterResetX);
	}

	for (auto iter = m_vecLuckyCircle.begin(); iter != m_vecLuckyCircle.end(); iter++)
	{
		(*iter)->SetX(m_CharacterResetX);
	}
	m_EndCircle->SetX(m_CharacterResetX);

}