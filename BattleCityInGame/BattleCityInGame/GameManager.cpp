#include <iostream>
#include <ctime>
#include <string>
#include <Windows.h>
#include "GameManager.h"
#include "ResourceManager.h"
#include "Block.h"
#include "Tank.h"
#include "Missile.h"
#include "ExplosionEffect.h"
#include "EnemyTank.h"
#include "ElseEffect.h"
#include "Bitmap.h"
GameManager* GameManager::s_this = NULL;

GameManager::GameManager()
{
	m_iBlockType = 0;

	for (int y = 0; y < MAPHEIGHTNUM; y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{
			m_iMap[y][x] = EMPTYBLOCK;
		}
	}
	m_bFirstTank = false;
	m_iTemp_Gameover = 10;
	m_iNumEnemy = 10;
	m_bLeftKeyDown = false;
	m_bRightKeyDown = false;
	m_bUpKeyDown = false;
	m_bDownKeyDown = false;
	m_bSpaceKeyDown = false;
	m_bDownKeyDown_Menu = false;
	m_bUpKeyDown_Menu = false;
	m_bEnterKeyDown_Menu = false;
	m_dwElapseLater = GetTickCount();
	m_fElapseSapceKey = 0;
	m_bMenuSelect = GAMESTART;
	m_eGameState = GAME_STATE_MENU;
	m_iLife = 3;
	m_bFirstBulletFired = false;
	m_bSecondBulletFired = false;
	m_iScore = 0;
	m_fElpase_ResetTimer = 0;

	m_WallRt[0].left = 0;
	m_WallRt[0].top = 0;
	m_WallRt[0].right = 1;
	m_WallRt[0].bottom = MAPHEIGHT;

	m_WallRt[1].left = 0;
	m_WallRt[1].top = 0;
	m_WallRt[1].right = MAPWIDTH;
	m_WallRt[1].bottom = 1;

	m_WallRt[2].left = MAPWIDTH-1;
	m_WallRt[2].top = 0;
	m_WallRt[2].right = MAPWIDTH;
	m_WallRt[2].bottom = MAPHEIGHT;

	m_WallRt[3].left = 0;
	m_WallRt[3].top = MAPHEIGHT-1;
	m_WallRt[3].right = MAPWIDTH;
	m_WallRt[3].bottom = MAPHEIGHT;

	m_iSpeed_Tank = 1;
	m_ActionFlag = true;
	m_iTemp_menu = 2.0f;

	m_arrfTextGradient[0] = 0;
	m_arrfTextGradient[1] = 0;
	m_arrfTextGradient[2] = 0;
}


GameManager::~GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	
	srand(time(NULL));
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitMap = CreateCompatibleBitmap(hdc, MAPWIDTHNUM * BLOCKWIDTH +100, MAPHEIGHTNUM * BLOCKHEIGHT);
	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
	ReleaseDC(hWnd, hdc);

	for (int i = 0; i <= RES_TYPE_BLOCK14; i++)
	{
		m_pBitmapBlock[i] = ResourceManager::GetInstance()->GetImage((RES_TYPE)i);
	}

	m_pBitmapTank[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_DOWN00);
	m_pBitmapTank[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_DOWN01);
	m_pBitmapTank[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_LEFT00);
	m_pBitmapTank[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_LEFT01);
	m_pBitmapTank[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_RIGHT00);
	m_pBitmapTank[5] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_RIGHT01);
	m_pBitmapTank[6] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_UP00);
	m_pBitmapTank[7] = ResourceManager::GetInstance()->GetImage(RES_TYPE_TANK_UP01);

	m_pBitmapMissile = ResourceManager::GetInstance()->GetImage(RES_TYPE_MISSILE);
	m_pBitmapExplosion[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_EXPLOSION0);
	m_pBitmapExplosion[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_EXPLOSION1);
	m_pBitmapExplosion[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_EXPLOSION2);
	m_pBitmapExplosion[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_EXPLOSION3);
	m_pBitmapExplosion[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_EXPLOSION4);
	

	m_pBitmapEnemyTank[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_UP00);
	m_pBitmapEnemyTank[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_UP01);
	m_pBitmapEnemyTank[2] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_UP02);
	m_pBitmapEnemyTank[3] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_LEFT00);
	m_pBitmapEnemyTank[4] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_LEFT01);
	m_pBitmapEnemyTank[5] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_LEFT02);
	m_pBitmapEnemyTank[6] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_RIGHT00);
	m_pBitmapEnemyTank[7] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_RIGHT01);
	m_pBitmapEnemyTank[8] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_RIGHT02);
	m_pBitmapEnemyTank[9] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_DOWN00);
	m_pBitmapEnemyTank[10] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_DOWN01);
	m_pBitmapEnemyTank[11] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ETANK_DOWN02);
	
	m_pBitmapShield[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_SHIELD00);
	m_pBitmapShield[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_SHIELD01);
	m_pBitmapIcon[0] = ResourceManager::GetInstance()->GetImage(RES_TYPE_PLAYERICON);
	m_pBitmapIcon[1] = ResourceManager::GetInstance()->GetImage(RES_TYPE_ENEMYICON);

	m_pEnemyTank = new EnemyTank();
	m_pEnemyTank->Init(m_pBitmapEnemyTank[0], m_pBitmapEnemyTank[1], m_pBitmapEnemyTank[2], m_pBitmapEnemyTank[3],
		m_pBitmapEnemyTank[4], m_pBitmapEnemyTank[5], m_pBitmapEnemyTank[6], m_pBitmapEnemyTank[7]
		, m_pBitmapEnemyTank[8], m_pBitmapEnemyTank[9], m_pBitmapEnemyTank[10], m_pBitmapEnemyTank[11], 1000, 1000, REDTEAM);


	LoadData(hWnd);
	ConstructBlock();
	for (int i = 0; i < 10; i++)                       //폭발 max 개수는 10개.
	{
		ExplosionEffect *pExplosion_Bullet = new ExplosionEffect();
		pExplosion_Bullet->Init(m_pBitmapExplosion[0], m_pBitmapExplosion[1], m_pBitmapExplosion[2],
			m_pBitmapExplosion[3], m_pBitmapExplosion[4]);
		m_vecExplosionEffect.push_back(pExplosion_Bullet);
	}


	bool btemp = false;
	for (int y = 0; y < MAPHEIGHTNUM;y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{
			if (m_iMap[y][x] == 1000 && rand() % 100 == 0)
			{
				m_pMyTank = new Tank();
				m_pMyTank->Init(m_pBitmapTank[0], m_pBitmapTank[1], m_pBitmapTank[2], m_pBitmapTank[3],
					m_pBitmapTank[4], m_pBitmapTank[5], m_pBitmapTank[6], m_pBitmapTank[7],
					GetPixelPosX(x),GetPixelPosY(y), BLUETEAM);
				btemp = true;
				break;
			}
		
		}
		if (btemp == true)
			break;
		
		if (y == MAPHEIGHTNUM - 1)
			y = 0;
	}
	
	m_pMyTankMissileFirst = new Missile();
	m_pMyTankMissileFirst->Init(m_pBitmapMissile, 0, 0, BLUETEAM);

	m_pMyTankMissileSecond = new Missile();
	m_pMyTankMissileSecond->Init(m_pBitmapMissile, 0, 0, BLUETEAM);


	for (int i = 0; i < 15; i++)
	{
		Missile *pEnemyMissile = new Missile();
		pEnemyMissile->Init(m_pBitmapMissile, 0, 0, REDTEAM);
		m_vecEnemyMissile.push_back(pEnemyMissile);
	}
	
	for (int i = 0; i < 10; i++) //10은 적탱크의 총 갯수
	{
		EnemyTank *pEnemyTank = new EnemyTank();
		pEnemyTank->Init(m_pBitmapEnemyTank[0], m_pBitmapEnemyTank[1], m_pBitmapEnemyTank[2], m_pBitmapEnemyTank[3],
			m_pBitmapEnemyTank[4], m_pBitmapEnemyTank[5], m_pBitmapEnemyTank[6], m_pBitmapEnemyTank[7]
			, m_pBitmapEnemyTank[8], m_pBitmapEnemyTank[9], m_pBitmapEnemyTank[10], m_pBitmapEnemyTank[11], 1000, 1000, REDTEAM);
		m_vecEnemyTank.push_back(pEnemyTank);
		
	}
	for (int i = 0; i < 5; i++)
	{
		ElseEffect *pElseEffect = new ElseEffect();
		pElseEffect->Init(m_pBitmapShield[0], m_pBitmapShield[1]);
		m_vecShield.push_back(pElseEffect);
	}
}

void GameManager::SaveData(HWND hWnd)
{
	FILE *fp = fopen("MapData.txt", "w");

	if (fp == NULL)
	{
		MessageBox(hWnd, "파일이 존재하지 않습니다!!!", "ERROR", MB_OK);
	}

	for (int y = 0; y < MAPHEIGHTNUM; y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{
			fprintf(fp, "%d ", m_iMap[y][x]);
		}
	}
	fclose(fp);
}

void GameManager::LoadData(HWND hWnd)
{
	FILE *fp = fopen("MapData.txt", "r");

	if (fp == NULL)
	{
		MessageBox(hWnd, "파일이 존재하지 않습니다!!!", "ERROR", MB_OK);
	}

	for (int y = 0; y < MAPHEIGHTNUM; y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{
			fscanf(fp, "%d", &m_iMap[y][x]);
		}
	}
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////블럭관련
void GameManager::ConstructBlock()
{

	for (int y = 0; y < MAPHEIGHTNUM; y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{

			if (m_iMap[y][x] != EMPTYBLOCK)
			{
				Block *pBlock = new Block();
				pBlock->Init(m_pBitmapBlock[BLOCK_BRICK_01], m_pBitmapBlock[BLOCK_BRICK_02], m_pBitmapBlock[BLOCK_BRICK_03],
					m_pBitmapBlock[BLOCK_BRICK_04], m_pBitmapBlock[BLOCK_BRICK_05], m_pBitmapBlock[BLOCK_SLIDE],
					m_pBitmapBlock[BLOCK_FOREST], m_pBitmapBlock[BLOCK_SEA], m_pBitmapBlock[BLOCK_STEEL_ALL],
					m_pBitmapBlock[BLOCK_STEEL_UP], m_pBitmapBlock[BLOCK_STEEL_LEFT], m_pBitmapBlock[BLOCK_STEEL_BOTTOM],
					m_pBitmapBlock[BLOCK_STEEL_RIGHT], m_pBitmapBlock[BLOCK_EAGLE], m_pBitmapBlock[BLOCK_SURREND], x, y, m_iMap[y][x]);
				m_vecBlock.push_back(pBlock);
			}

		}
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		(*iter)->SetRt();
	}

}


void GameManager::OperateLbutton(int x, int y)
{
	x = GetTilePosX(x);
	y = GetTilePosY(y);

}

void GameManager::OperateRbutton(int x, int y)
{
	x = GetTilePosX(x);
	y = GetTilePosY(y);


}

void GameManager::Draw()
{
	HDC hdc = GetDC(m_hWnd);

	HBRUSH Brush, oldBrush;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(m_hMemDC, Brush);
	Rectangle(m_hMemDC, 0, 0, MAPWIDTHNUM * BLOCKWIDTH, MAPHEIGHTNUM * BLOCKHEIGHT);
	SelectObject(m_hMemDC, oldBrush);
	DeleteObject(Brush);




	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == BLOCK_SEA && (*iter)->GetWhatAmI() != EMPTYBLOCK
			||(*iter)->GetWhatAmI() == BLOCK_SLIDE)
			(*iter)->Draw(m_hMemDC);
	}

	m_pMyTank->Draw(m_hMemDC);
	m_pMyTankMissileFirst->Draw(m_hMemDC);
	m_pMyTankMissileSecond->Draw(m_hMemDC);

	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
		(*iter)->Draw(m_hMemDC);
	}

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		(*iter)->Draw(m_hMemDC);
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() != BLOCK_SEA && (*iter)->GetWhatAmI() != EMPTYBLOCK
			&& (*iter)->GetWhatAmI() != BLOCK_SLIDE)
			(*iter)->Draw(m_hMemDC);
		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
		{
		//	RECT temp = (*iter)->GetRt();
		//	Rectangle(m_hMemDC, temp.left, temp.top, temp.right, temp.bottom);

		}
	}


	string text;
	/*text = to_string(m_pMyTank->GetX());
	TextOut(hdc, 700, 100, text.c_str(), text.length());

	text = to_string(m_pMyTank->GetY());
	TextOut(hdc, 700, 130, text.c_str(), text.length());

	RECT enemytankrt;
	enemytankrt = m_pMyTank->GetRt();

	text = to_string(enemytankrt.left);
	TextOut(hdc, 700, 160, text.c_str(), text.length());
	text = to_string(enemytankrt.top);
	TextOut(hdc, 700, 190, text.c_str(), text.length());
	text = to_string(enemytankrt.right);
	TextOut(hdc, 700, 220, text.c_str(), text.length());
	text = to_string(enemytankrt.bottom);
	TextOut(hdc, 700, 250, text.c_str(), text.length());
	*/
	//RECT temp2 = m_pEnemyMissile->GetRt();
	//Rectangle(m_hMemDC, temp2.left, temp2.top, temp2.right, temp2.bottom);
//	Rectangle(m_hMemDC, enemytankrt.left, enemytankrt.top, enemytankrt.right, enemytankrt.bottom);

	//RECT temp = m_pMyTank->GetRt();
	//Rectangle(m_hMemDC, temp.left, temp.top, temp.right, temp.bottom);
	
	for (auto iter = m_vecShield.begin(); iter != m_vecShield.end(); iter++)
	{
		(*iter)->Draw(m_hMemDC, m_fElapseValue);
	}
	for (auto iter = m_vecExplosionEffect.begin(); iter != m_vecExplosionEffect.end(); iter++)
	{
		(*iter)->Draw(m_hMemDC, m_fElapseValue);
	}

	Brush = CreateSolidBrush(RGB(213, 213, 213));
	oldBrush = (HBRUSH)SelectObject(m_hMemDC, Brush);
	Rectangle(m_hMemDC, MAPWIDTHNUM * BLOCKWIDTH, 0, MAPWIDTHNUM * BLOCKWIDTH + 100, MAPHEIGHTNUM * BLOCKHEIGHT);
	SelectObject(m_hMemDC, oldBrush);
	DeleteObject(Brush);

	HFONT myFont = CreateFont(15, 0, 0, 80, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "고딕");
	HFONT oldFont = (HFONT)SelectObject(m_hMemDC, myFont);
	SetTextColor(m_hMemDC, RGB(0, 0, 0));
	SetBkColor(m_hMemDC, RGB(255, 255, 255));
	SetBkMode(m_hMemDC, TRANSPARENT);

	TextOut(m_hMemDC, MAPWIDTH + 10,130, "ENEMYLIFE", 9);

	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);


	myFont = CreateFont(15, 0, 0, 80, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "고딕");
	oldFont = (HFONT)SelectObject(m_hMemDC, myFont);
	SetTextColor(m_hMemDC, RGB(0, 0, 0));
	SetBkColor(m_hMemDC, RGB(255, 255, 255));
	SetBkMode(m_hMemDC, TRANSPARENT);
	text = to_string(m_iScore);
	TextOut(m_hMemDC, MAPWIDTH + 10, 40, "SCORE", 5);
	TextOut(m_hMemDC, MAPWIDTH + 10, 55, text.c_str(),text.length());

	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);

	for (int i = 0; i < m_iNumEnemy; i++)
	{
			if (i < 5)
			
				m_pBitmapIcon[1]->Draw(m_hMemDC, MAPWIDTH + 10, 150 + i * 20, 14, 12);
			
			
			if (i >= 5)
				m_pBitmapIcon[1]->Draw(m_hMemDC, MAPWIDTH + 30, 150 + (i-5) * 20, 14, 12);
		
	}

	myFont = CreateFont(15, 0, 0, 80, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "고딕");
	oldFont = (HFONT)SelectObject(m_hMemDC, myFont);
	SetTextColor(m_hMemDC, RGB(0, 0, 0));
	SetBkColor(m_hMemDC, RGB(255, 255, 255));
	SetBkMode(m_hMemDC, TRANSPARENT);

	TextOut(m_hMemDC, MAPWIDTH + 10, 280, "LIFE", 4);

	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);
	for (int i = 0; i < m_iLife; i++)
	{
		m_pBitmapIcon[0]->Draw(m_hMemDC, MAPWIDTH + 10 + i*20, 300, 14, 12);
	}

	BitBlt(hdc, 0, 0, MAPWIDTHNUM * BLOCKWIDTH + 100 , MAPHEIGHTNUM * BLOCKHEIGHT, m_hMemDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd,hdc);
}

void GameManager::Release()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_vecBlock.clear();

	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_vecEnemyMissile.clear();


	for (auto iter = m_vecExplosionEffect.begin(); iter != m_vecExplosionEffect.begin(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_vecExplosionEffect.clear();

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_vecEnemyTank.clear();

	for (auto iter = m_vecShield.begin(); iter != m_vecShield.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	m_vecShield.clear();
	SAFE_DELETE(m_pEnemyTank);
	SAFE_DELETE(m_pMyTank);
	SAFE_DELETE(m_pMyTankMissileFirst);
	SAFE_DELETE(m_pMyTankMissileSecond);
	SAFE_DELETE(s_this);
}
///////////////////////////////////////////////////////////////////////////////미사일 관련
void GameManager::MissileFire()
{
	////////////////////////////발사 지점을 지정.  각 상수는 탱크 발사 부분 조정을 위한 상수임.
	///////////////////////////맵에서는 내 탱크의 미사일이 오직 한발이여야 하며 표적에 적중될 때 까지 다시 발사할 수 없음
	//////////////////////////한발만 발사 하는건 게임매니저에서 만들어야할까 미사일에서 만들어야 할까...

	

	if (m_bFirstBulletFired == false && m_bSecondBulletFired == false)
	{
		m_fElapseSapceKey = 0;
		if (m_pMyTank->GetDirection() == TANKDIRECTION_DOWN)
		{
			m_pMyTankMissileFirst->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileFirst->SetXY(m_pMyTank->GetX() + 11, m_pMyTank->GetY() + 23);
		}

		else if (m_pMyTank->GetDirection() == TANKDIRECTION_LEFT)
		{
			m_pMyTankMissileFirst->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileFirst->SetXY(m_pMyTank->GetX()+2, m_pMyTank->GetY() + 12);
		}

		else if (m_pMyTank->GetDirection() == TANKDIRECTION_RIGHT)
		{
			m_pMyTankMissileFirst->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileFirst->SetXY(m_pMyTank->GetX() + 31, m_pMyTank->GetY() + 12);

		}


		else if (m_pMyTank->GetDirection() == TANKDIRECTION_UP)
		{
			m_pMyTankMissileFirst->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileFirst->SetXY(m_pMyTank->GetX() + 11, m_pMyTank->GetY()+2);
		}

		m_pMyTankMissileFirst->SetRt();
		m_pMyTankMissileFirst->FlagOn();
 		m_bFirstBulletFired = true;
	} 

	else if (m_pMyTankMissileFirst->GetFlag() == true && m_fElapseSapceKey > 1.5f &&
		m_bSecondBulletFired == false)
	{
 		if (m_pMyTank->GetDirection() == TANKDIRECTION_DOWN)
		{
			m_pMyTankMissileSecond->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileSecond->SetXY(m_pMyTank->GetX() + 11, m_pMyTank->GetY() + 25);
		}

		else if (m_pMyTank->GetDirection() == TANKDIRECTION_LEFT)
		{
			m_pMyTankMissileSecond->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileSecond->SetXY(m_pMyTank->GetX(), m_pMyTank->GetY() + 12);
		}

		else if (m_pMyTank->GetDirection() == TANKDIRECTION_RIGHT)
		{
			m_pMyTankMissileSecond->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileSecond->SetXY(m_pMyTank->GetX() + 33, m_pMyTank->GetY() + 12);

		}


		else if (m_pMyTank->GetDirection() == TANKDIRECTION_UP)
		{
			m_pMyTankMissileSecond->SetDirection(m_pMyTank->GetDirection());
			m_pMyTankMissileSecond->SetXY(m_pMyTank->GetX() + 11, m_pMyTank->GetY());
		}

		m_pMyTankMissileSecond->SetRt();
		m_pMyTankMissileSecond->FlagOn();
		m_bSecondBulletFired = true;
	}
}

void GameManager::RemoveMissile()
{
}

void GameManager::FireMove()
{
	m_pMyTankMissileFirst->FireMove(m_fElapseValue);
	m_pMyTankMissileSecond->FireMove(m_fElapseValue);

	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
		(*iter)->FireMove(m_fElapseValue);
	}

}

void GameManager::MissileCollision()
{
	if (IntersectRect_Wall(m_pMyTankMissileFirst->GetRt()))
	{
		Operate_Explosion_Bullet(m_pMyTankMissileFirst->GetX(), m_pMyTankMissileFirst->GetY());
		m_pMyTankMissileFirst->FlagOff();
		m_bFirstBulletFired = false;
		return;
	}

	if (IntersectRect_Wall(m_pMyTankMissileSecond->GetRt()))
	{
		Operate_Explosion_Bullet(m_pMyTankMissileSecond->GetX(), m_pMyTankMissileSecond->GetY());
		m_pMyTankMissileSecond->FlagOff();
		m_bSecondBulletFired = false;
		return;
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->CollisionCheck_First(m_pMyTankMissileFirst->GetRt()))
		{
		
			if ((*iter)->GetWhatAmI() == BLOCK_SEA || (*iter)->GetWhatAmI()==BLOCK_SLIDE)
				return;

			
			Operate_Explosion_Bullet(m_pMyTankMissileFirst->GetX(), m_pMyTankMissileFirst->GetY());
			m_pMyTankMissileFirst->FlagOff();
 			m_bFirstBulletFired = false;

			if ((*iter)->GetWhatAmI() == BLOCK_EAGLE)
			{
				Operate_Explosion_Tank(GetPixelPosX((*iter)->GetX())+15, GetPixelPosY((*iter)->GetY())+7);
				m_eGameState = GAME_STATE_DEAD;
				m_pMyTank->DestroyTank();
				(*iter)->SetWhatAmI(BLOCK_SURREND);
				return;
			}
				
			else if ((*iter)->GetWhatAmI() != BLOCK_EAGLE && (*iter)->GetWhatAmI() != BLOCK_SURREND)
			{
   				(*iter)->DestroyBlock();
				return;
			}
			
		}

		if ((*iter)->CollisionCheck_First(m_pMyTankMissileSecond->GetRt()))
		{

			if ((*iter)->GetWhatAmI() == BLOCK_SEA)
				return;

			

			Operate_Explosion_Bullet(m_pMyTankMissileSecond->GetX(), m_pMyTankMissileSecond->GetY());
			m_pMyTankMissileSecond->FlagOff();
			m_bSecondBulletFired = false;
			
			if ((*iter)->GetWhatAmI() == BLOCK_EAGLE)
			{
				Operate_Explosion_Tank((*iter)->GetX(), (*iter)->GetY());
				m_eGameState = GAME_STATE_DEAD;
				m_pMyTank->DestroyTank();
				(*iter)->SetWhatAmI(BLOCK_SURREND);
				return;
			}

			else if ((*iter)->GetWhatAmI() != BLOCK_EAGLE &&
				(*iter)->GetWhatAmI() != BLOCK_SURREND)
			{
				(*iter)->DestroyBlock();
				return;
			}
		}
	}

	
	RECT temp;
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if (IntersectRect(&temp, &(*iter)->GetRt(), &m_pMyTankMissileFirst->GetRt()))
		{
			Operate_Explosion_Tank((*iter)->GetX() + 17, (*iter)->GetY() + 12);
			m_pMyTankMissileFirst->FlagOff();
			m_bFirstBulletFired = false;
			m_iScore += 100;
			(*iter)->DestroyTank();
		}


		if (IntersectRect(&temp, &(*iter)->GetRt(), &m_pMyTankMissileSecond->GetRt()))
		{
			(*iter)->DestroyTank();
			Operate_Explosion_Tank((*iter)->GetX() + 17, (*iter)->GetY() + 12);
			m_pMyTankMissileSecond->FlagOff();
			m_bSecondBulletFired = false;
			m_iScore += 100;
			(*iter)->DestroyTank();
		}
	}
	

}

////////////////////////////////////////////////////////////////////탱크움직임. 충돌 관련

void GameManager::MoveRight()
{
	RECT temprt;

	m_iFormer_Direction = m_pMyTank->GetDirection();

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if (m_pMyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_RIGHT) == true)
		{
			m_pMyTank->SetDirection(TANKDIRECTION_RIGHT);
			return;
		}

	}
	

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
			continue;

		if(m_pMyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_RIGHT))
		{

			m_pMyTank->SetDirection(TANKDIRECTION_RIGHT);
			return;
		}
	}
	m_pMyTank->MoveRight(m_fElapseValue*m_iSpeed_Tank, m_iFormer_Direction);

}

void GameManager::MoveLeft()
{
	RECT temprt;
	m_iFormer_Direction = m_pMyTank->GetDirection();


	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{

		if (m_pMyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_LEFT) == true)
		{
			m_pMyTank->SetDirection(TANKDIRECTION_LEFT);
			return;
		}

	}


	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{

		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
			continue;

		if (m_pMyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_LEFT))
		{
			m_pMyTank->SetDirection(TANKDIRECTION_LEFT);
			m_pMyTank->Interpolate_Tank(m_iFormer_Direction);
			return;
		}
	}
	m_pMyTank->MoveLeft(m_fElapseValue*m_iSpeed_Tank, m_iFormer_Direction);
}

void GameManager::MoveUp()
{
	RECT temprt;

	m_iFormer_Direction = m_pMyTank->GetDirection();

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{

		if (m_pMyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_UP) == true)
		{
			m_pMyTank->SetDirection(TANKDIRECTION_UP);
			return;
		}

	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{

		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
			continue;

		if (m_pMyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_UP))
		{
			m_pMyTank->SetDirection(TANKDIRECTION_UP);
			m_pMyTank->Interpolate_Tank(m_iFormer_Direction);
			return;
		}
	}
	m_pMyTank->MoveUp(m_fElapseValue*m_iSpeed_Tank, m_iFormer_Direction);
}

void GameManager::MoveDown()
{
	RECT temprt;

	m_iFormer_Direction = m_pMyTank->GetDirection();

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{


		if (m_pMyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_DOWN) == true)
		{
			m_pMyTank->SetDirection(TANKDIRECTION_DOWN);
			return;
		}
	}

	
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{

		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
			continue;

		if (m_pMyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_DOWN))
		{
			m_pMyTank->SetDirection(TANKDIRECTION_DOWN);
			m_pMyTank->Interpolate_Tank(m_iFormer_Direction);
			return;
		}
	}
	m_pMyTank->MoveDown(m_fElapseValue*m_iSpeed_Tank, m_iFormer_Direction);
}

void GameManager::SetRt()
{
	m_pMyTank->SetRt();
	m_pMyTankMissileFirst->SetRt();
	m_pMyTankMissileSecond->SetRt();
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		(*iter)->SetRt();
	}
	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
		(*iter)->SetRt();
	}
	//for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	//{
	//	(*iter)->SetRt();
	//}
}


int GameManager::GetPixelPosX(int TileX)
{
	return TileX * BLOCKWIDTH;
}

int GameManager::GetPixelPosY(int TileY)
{
	return TileY * BLOCKHEIGHT;
}

int GameManager::GetTilePosX(int PixelX)
{
	return PixelX / BLOCKWIDTH;
}

int GameManager::GetTilePosY(int PixelY)
{
	return PixelY / BLOCKHEIGHT;
}

void GameManager::KeyManager()
{
	if (GetKeyState(VK_LEFT) & 0x8000
		&& m_bRightKeyDown == false && m_bUpKeyDown ==false && m_bDownKeyDown ==false)
	{
		m_bLeftKeyDown = true;
		MoveLeft();
	}

	else
	{
		if (m_bLeftKeyDown == true)
		{
			ResetMove();
			m_bLeftKeyDown = false;

		}
	}

	if (GetKeyState(VK_RIGHT) & 0x8000
		&& m_bLeftKeyDown == false && m_bUpKeyDown == false && m_bDownKeyDown == false)
	{
 		m_bRightKeyDown = true;
		MoveRight();

	}

	else
	{
		if (m_bRightKeyDown == true)
		{
			ResetMove();
			m_bRightKeyDown = false;

		}
	}

	if (GetKeyState(VK_UP) & 0x8000
		&& m_bLeftKeyDown == false && m_bRightKeyDown == false && m_bDownKeyDown == false)
	{
		m_bUpKeyDown = true;
		MoveUp();
	}

	else
	{
		if (m_bUpKeyDown == true)
		{
			ResetMove();
			m_bUpKeyDown = false;
		}
	}

	if (GetKeyState(VK_DOWN) & 0x8000
		&& m_bLeftKeyDown == false && m_bRightKeyDown == false && m_bUpKeyDown == false)
	{
		m_bDownKeyDown = true;
		MoveDown();
	}

	else
	{
		if (m_bDownKeyDown == true)
		{
			ResetMove();
			m_bDownKeyDown = false;
		}
	}

	if (GetKeyState(VK_SPACE) & 0x8000 && m_eGameState !=GAME_STATE_DEAD)
	{
		if (m_bSpaceKeyDown == false)
		{
			//if(m_bSecondBulletFired == true)

			MissileFire();
			m_bSpaceKeyDown = true;
		}

	}
	else
	{

		if (m_bSpaceKeyDown == true)
		{
			m_bSpaceKeyDown = false;
		}
	}
}
void GameManager::KeyManager_Menu()
{

	if (GetKeyState(VK_UP) & 0x8000
		&& m_bDownKeyDown_Menu == false)
	{
		m_bUpKeyDown_Menu = true;
	}

	else
	{
		if (m_bUpKeyDown_Menu == true)
		{

			SelectChange_Menu();
			m_bUpKeyDown_Menu = false;
		}
	}

	if (GetKeyState(VK_DOWN) & 0x8000
		 && m_bUpKeyDown_Menu == false)
	{
		m_bDownKeyDown_Menu = true;
	}

	else
	{
		if (m_bDownKeyDown_Menu == true)
		{
			SelectChange_Menu();
			m_bDownKeyDown_Menu = false;
		}
	}

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (m_bEnterKeyDown_Menu == false)
		{
			//if(m_bSecondBulletFired == true)

			m_bEnterKeyDown_Menu = true;
		}

	}
	else
	{

		if (m_bEnterKeyDown_Menu == true && m_bMenuSelect ==GAMESTART)
		{
			GameStart();
			m_bEnterKeyDown_Menu = false;
		}
		
		else if (m_bEnterKeyDown_Menu == true && m_bMenuSelect == EXIT)
		{
			m_eGameState = GAME_STATE_EXIT;
			m_bEnterKeyDown_Menu = false;
		}
	}
}

void GameManager::Operate_Explosion_Bullet(float x,float y)
{
	for (auto iter = m_vecExplosionEffect.begin(); iter != m_vecExplosionEffect.end(); iter++)
	{
		if ((*iter)->GetSwitchFlag() == false)
		{
			(*iter)->SwitchFlagOn();
			(*iter)->SetXY(x, y);
			(*iter)->SetExplosionType(EXPLOSION_BULLET);
			break;
		}
	
	}

}
void GameManager::Operate_Explosion_Tank(float x, float y)
{
	for (auto iter = m_vecExplosionEffect.begin(); iter != m_vecExplosionEffect.end(); iter++)
	{
		if ((*iter)->GetSwitchFlag() == false)
		{
			(*iter)->SwitchFlagOn();
			(*iter)->SetXY(x, y);
			(*iter)->SetExplosionType(EXPLOSION_TANK);
			break;
		}

	}
}


void GameManager::Interpolate_Tank()
{
	int iDirection;
	int Tile_first = 0;		//탱크위치
	int Tile_Second = 0;	//보간된 탱크위치

	
	iDirection = m_pMyTank->GetDirection();

	if (iDirection == m_iFormer_Direction)
		return;

	if (iDirection == TANKDIRECTION_LEFT || iDirection == TANKDIRECTION_RIGHT 
		&& m_iFormer_Direction == TANKDIRECTION_UP)
	{
		
				//보간된 탱크위치

		Tile_first = GetTilePosY(m_pMyTank->GetY()+TANKHEIGHT);
		Tile_Second = GetTilePosY(m_pMyTank->GetY() + 18); //보간위치
		 
		if (Tile_first != Tile_Second)
		{
			m_pMyTank->SetY(GetPixelPosY(Tile_Second));
		}
	}

	else if (iDirection == TANKDIRECTION_LEFT || iDirection == TANKDIRECTION_RIGHT
		&& m_iFormer_Direction == TANKDIRECTION_DOWN)
	{

		//보간된 탱크위치

		Tile_first = GetTilePosY(m_pMyTank->GetY());
		Tile_Second = GetTilePosY(m_pMyTank->GetY() + 6);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_pMyTank->SetY(GetPixelPosY(Tile_Second));
		}
	}

	else if (iDirection == TANKDIRECTION_UP || iDirection == TANKDIRECTION_DOWN
		&&m_iFormer_Direction == TANKDIRECTION_LEFT)
	{
		
		Tile_first = GetTilePosX(m_pMyTank->GetX() +TANKWIDTH);
		Tile_Second = GetTilePosX(m_pMyTank->GetX()+25);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_pMyTank->SetX(GetPixelPosX(Tile_Second));
		}
	}

	else if (iDirection == TANKDIRECTION_UP || iDirection == TANKDIRECTION_DOWN
		&&m_iFormer_Direction == TANKDIRECTION_RIGHT)
	{

		Tile_first = GetTilePosX(m_pMyTank->GetX());
		Tile_Second = GetTilePosX(m_pMyTank->GetX() + 8);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_pMyTank->SetX(GetPixelPosX(Tile_Second));
		}
	}

}

bool GameManager::IntersectRect_Wall(RECT rt)
{
	RECT temp;
	for (int i = 0; i < 4; i++)
	{
		if (IntersectRect(&temp, &m_WallRt[i], &rt))
			return true;
	}
	return false;
}

void GameManager::NewTank()
{
	
	/*if (true)
	{
		for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
		{
			if ((*iter)->GetAliveFlag() == false)
			{
				(*iter)->CreateEnemy();
				m_iNumEnemy--;
				
				while (true)
				{
					for (int y = 0; y < MAPHEIGHTNUM; y++)
					{
						for (int x = 0; x < MAPWIDTHNUM; x++)
						{
							if (m_iMap[y][x] == EMPTYBLOCK &&rand()%100 ==0)
							{
								(*iter)->SetX(GetPixelPosX(x));
								(*iter)->SetY(GetPixelPosY(y));

								return;
							}

						}
					}
				}
			}
		}
	}
	*/
	static float fElapse_EnemyTank = 0.0f;
	fElapse_EnemyTank += m_fElapseValue;

	if (m_bFirstTank == true)
	{


		if (m_iNumEnemy == 0)
			return;


		if (fElapse_EnemyTank < 4.0f)
		{
			return;

		}
		fElapse_EnemyTank = 0;
		if (rand() % 5 != 0)
		{
			return;
		}

	}
	m_bFirstTank = true;
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter)->GetAliveFlag() == true)
			continue;

		while (true)
		{
			int x = rand() % MAPWIDTHNUM;
			int y = rand() % MAPHEIGHTNUM;

			if (m_iMap[y][x] == EMPTYBLOCK)
			{

				(*iter)->CreateEnemy();
				(*iter)->SetX(GetPixelPosX(x));
				(*iter)->SetY(GetPixelPosY(y));
				(*iter)->SetInitialFlag(true);
				RECT temp;
				if (IntersectRect(&temp, &(*iter)->GetRt(), &m_pMyTank->GetRt()) == false)
				{
					m_iNumEnemy--;
					return;
				}
			}
		}
	}
}

void GameManager::AutoMove_EnemyTank()
{

	
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter)->GetAliveFlag() != true)
			continue;

		(*iter)->fElapsePlus_AutoFire(m_fElapseValue);
		(*iter)->fElapsePlus_AutoMove(m_fElapseValue);

		if ((*iter)->GetfElapse_AutoFire() > 1.5f)
		{
			if (rand() % 5 == 0)
			{
				MissileFire_EnemyTank(*iter);
			}

			(*iter)->Set_fElapse_AutoFire(0);
		}


		if ((*iter)->GetfElapse_AutoMove() > 3.0f)
		{
			(*iter)->SetNextDirection(rand() % 4);
			(*iter)->Set_fElapse_AutoMove(0);
		}

		switch ((*iter)->GetNextMoveDirection())
		{

		case TANKDIRECTION_DOWN:
			MoveDown_EnemyTank((*iter));
			break;
		case TANKDIRECTION_LEFT:
			MoveLeft_EnemyTank((*iter));
			break;
		case TANKDIRECTION_RIGHT:
			MoveRight_EnemyTank((*iter));
			break;
		case TANKDIRECTION_UP:
			MoveUp_EnemyTank((*iter));
			break;
		default:
			break;

		}

		if ((*iter)->GetAliveFlag() == true)
			(*iter)->ResetMove();
	}


	

/*	int Direction;
	bool Collide2 = false;
	Direction = m_pEnemyTank->GetDirection();
	for (auto iter2 = m_vecBlock.begin(); iter2 != m_vecBlock.end(); iter2++)
	{
		if (m_pEnemyTank->CheckCollision_for_Block((*iter2)->GetRt(), Direction) == true) ///////////////벽돌에 닿으면 오토무빙이 실행이 안되버림
		{
			Collide2 = true;
		}
	}
		m_pEnemyTank->AutoMoving(m_fElapseValue, Direction,Collide2);


	m_pEnemyTank->ResetMove();*/

	/*
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		bool Collide = false;
		Direction = (*iter)->GetDirection();
		for (auto iter2 = m_vecBlock.begin(); iter2 != m_vecBlock.end(); iter2++)
		{
			if ((*iter)->CheckCollision_for_Block((*iter2)->GetRt(), Direction) ==true)
			{
				Collide = true;
			}
		}
		if(Collide == false)
			(*iter)->AutoMoving(m_fElapseValue, Direction);

		(*iter)->ResetMove();
	}
	*/
}


void GameManager::MoveRight_EnemyTank(EnemyTank *pEnemyTank)
{
	int iFormerDirection_EnemyTank;
	iFormerDirection_EnemyTank = pEnemyTank->GetDirection();

	if (pEnemyTank->CheckCollision_for_Tank(m_pMyTank->GetRt(), TANKDIRECTION_RIGHT) == true)
	{
		pEnemyTank->SetDirection(TANKDIRECTION_RIGHT);
		return;
	}
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter) == pEnemyTank)
			continue;

		if (pEnemyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_RIGHT) == true)
		{
			pEnemyTank->SetDirection(TANKDIRECTION_RIGHT);
			return;
		}
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (pEnemyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_RIGHT))
		{

			pEnemyTank->SetDirection(TANKDIRECTION_RIGHT);
			pEnemyTank->Interpolate_Tank(iFormerDirection_EnemyTank);
			return;
		}
	}
	pEnemyTank->MoveRight(m_fElapseValue, iFormerDirection_EnemyTank);
}
void GameManager::MoveLeft_EnemyTank(EnemyTank *pEnemyTank)
{

	int iFormerDirection_EnemyTank;
	iFormerDirection_EnemyTank = pEnemyTank->GetDirection();

	if (pEnemyTank->CheckCollision_for_Tank(m_pMyTank->GetRt(), TANKDIRECTION_LEFT) == true)
	{
		pEnemyTank->SetDirection(TANKDIRECTION_LEFT);
		return;
	}
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter) == pEnemyTank)
			continue;

		if (pEnemyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_LEFT) == true)
		{
			pEnemyTank->SetDirection(TANKDIRECTION_LEFT);
			return;
		}
	}
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (pEnemyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_LEFT))
		{

			pEnemyTank->SetDirection(TANKDIRECTION_LEFT);
			pEnemyTank->Interpolate_Tank(iFormerDirection_EnemyTank);
			return;
		}
	}
	pEnemyTank->MoveLeft(m_fElapseValue, iFormerDirection_EnemyTank);
}
void GameManager::MoveUp_EnemyTank(EnemyTank *pEnemyTank)
{
	int iFormerDirection_EnemyTank;
	iFormerDirection_EnemyTank = pEnemyTank->GetDirection();

	if (pEnemyTank->CheckCollision_for_Tank(m_pMyTank->GetRt(), TANKDIRECTION_UP) == true)
	{
		pEnemyTank->SetDirection(TANKDIRECTION_UP);
		return;
	}

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter) == pEnemyTank)
			continue;

		if (pEnemyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_UP) == true)
		{
			pEnemyTank->SetDirection(TANKDIRECTION_UP);
			return;
		}
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (pEnemyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_UP))
		{

			pEnemyTank->SetDirection(TANKDIRECTION_UP);
			pEnemyTank->Interpolate_Tank(iFormerDirection_EnemyTank);
			return;
		}
	}
	pEnemyTank->MoveUp(m_fElapseValue, iFormerDirection_EnemyTank);
}
void GameManager::MoveDown_EnemyTank(EnemyTank *pEnemyTank)
{

	int iFormerDirection_EnemyTank;
	iFormerDirection_EnemyTank = pEnemyTank->GetDirection();

	if (pEnemyTank->CheckCollision_for_Tank(m_pMyTank->GetRt(), TANKDIRECTION_DOWN) == true)
	{
		pEnemyTank->SetDirection(TANKDIRECTION_DOWN);
		return;
	}
	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter) == pEnemyTank)
			continue;

		if (pEnemyTank->CheckCollision_for_Tank((*iter)->GetRt(), TANKDIRECTION_DOWN) == true)
		{
			pEnemyTank->SetDirection(TANKDIRECTION_DOWN);
			return;
		}
	}

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
			continue;

		if (pEnemyTank->CheckCollision_for_Block((*iter)->GetRt(), TANKDIRECTION_DOWN))
		{

			pEnemyTank->SetDirection(TANKDIRECTION_DOWN);
			pEnemyTank->Interpolate_Tank(iFormerDirection_EnemyTank);
			return;
		}
	}
	pEnemyTank->MoveDown(m_fElapseValue, iFormerDirection_EnemyTank);
}

void GameManager::MissileCollision_EnemyTank()
{
	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
	//	if ((*iter)->GetFlag() == false)
	//		continue;

		if (IntersectRect_Wall((*iter)->GetRt()))
		{
			Operate_Explosion_Bullet((*iter)->GetX(), (*iter)->GetY());
			(*iter)->FlagOff();
			return;
		}

		for (auto iter2 = m_vecBlock.begin(); iter2 != m_vecBlock.end(); iter2++)
		{
			if ((*iter2)->CollisionCheck_First((*iter)->GetRt()))
			{
				if ((*iter2)->GetWhatAmI() == BLOCK_SEA|| (*iter2)->GetWhatAmI() == BLOCK_SLIDE)
					return;

				Operate_Explosion_Bullet((*iter)->GetX(), (*iter)->GetY());
				(*iter)->FlagOff();

				if ((*iter2)->GetWhatAmI() == BLOCK_EAGLE)
				{
					Operate_Explosion_Tank((*iter2)->GetX(), (*iter2)->GetY());
					m_eGameState = GAME_STATE_DEAD;
					m_pMyTank->DestroyTank();
					(*iter2)->SetWhatAmI(BLOCK_SURREND);
					return;
				}

				else if ((*iter2)->GetWhatAmI() != BLOCK_EAGLE &&
					(*iter2)->GetWhatAmI() != BLOCK_SURREND)
				{
					(*iter2)->DestroyBlock();
					return;
				}
				return;
			}

		}


		RECT temp;
		if (IntersectRect(&temp, &m_pMyTank->GetRt(), &(*iter)->GetRt()) == true)
		{
			Operate_Explosion_Tank(m_pMyTank->GetX() + 17, m_pMyTank->GetY() + 12);
			(*iter)->FlagOff();
			m_iLife--;
			
			if (m_iLife == 0)
			{
				m_pMyTank->DestroyTank();
				m_eGameState = GAME_STATE_DEAD;
			}
		}
	}
	

}

void GameManager::MissileFire_EnemyTank(EnemyTank *pEnemyTank)
{
	for (auto iter = m_vecEnemyMissile.begin(); iter != m_vecEnemyMissile.end(); iter++)
	{
		if ((*iter)->GetFlag() == true)
			break;

		if (pEnemyTank->GetDirection() == TANKDIRECTION_DOWN)
		{
			(*iter)->SetDirection(pEnemyTank->GetDirection());
			(*iter)->SetXY(pEnemyTank->GetX() + 11, pEnemyTank->GetY() + 23);
		}

		else if (pEnemyTank->GetDirection() == TANKDIRECTION_LEFT)
		{
			(*iter)->SetDirection(pEnemyTank->GetDirection());
			(*iter)->SetXY(pEnemyTank->GetX() + 2, pEnemyTank->GetY() + 12);
		}

		else if (pEnemyTank->GetDirection() == TANKDIRECTION_RIGHT)
		{
			(*iter)->SetDirection(pEnemyTank->GetDirection());
			(*iter)->SetXY(pEnemyTank->GetX() + 31, pEnemyTank->GetY() + 12);

		}

		else if (pEnemyTank->GetDirection() == TANKDIRECTION_UP)
		{
			(*iter)->SetDirection(pEnemyTank->GetDirection());
			(*iter)->SetXY(pEnemyTank->GetX() + 11, pEnemyTank->GetY() + 2);
		}

		(*iter)->FlagOn();
		break;
	}
		
}

void GameManager::Draw_Menu()
{
	HDC hdc = GetDC(m_hWnd);
	
	
	m_fElapse_Menu += m_fElapseValue;
	if (m_fElapse_Menu > 1.0f && m_ActionFlag == true)
	{
			if (m_fElapse_Menu > m_iTemp_menu)
			{
				Operate_Explosion_Tank(rand() % MAPWIDTH, rand() % MAPHEIGHT);
				m_iTemp_menu += 0.2f;
			}
			if (m_iTemp_menu > 5.0f)
			{
				m_ActionFlag = false;
				m_iTemp_menu = 2.0f;
			}
			
		
	}
	if (m_fElapse_Menu > 6.0f &&m_arrfTextGradient[0] > -90)
		m_arrfTextGradient[0]--;

	if (m_fElapse_Menu > 6.5f &&m_arrfTextGradient[1] > -90)
		m_arrfTextGradient[1]--;

	if (m_fElapse_Menu > 6.7f &&m_arrfTextGradient[2] > -90)
		m_arrfTextGradient[2]--;


	HBRUSH Brush, oldBrush;

	Brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(m_hMemDC, Brush);
	Rectangle(m_hMemDC, 0, 0, MAPWIDTHNUM * BLOCKWIDTH, MAPHEIGHTNUM * BLOCKHEIGHT);
	SelectObject(m_hMemDC, oldBrush);
	DeleteObject(Brush);

	HFONT myFont = CreateFont(40, 0, m_arrfTextGradient[0], 80, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Stencil");
	HFONT oldFont = (HFONT)SelectObject(m_hMemDC, myFont);
	SetTextColor(m_hMemDC, RGB(255, 255, 255));
	SetBkColor(m_hMemDC, RGB(255, 255, 255));											//텍스트배경 색깔
	SetBkMode(m_hMemDC, TRANSPARENT);

	string temp;
	temp = "BattleCity";
	TextOut(m_hMemDC, 100, 100, temp.c_str(), temp.length());

	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);

	myFont = CreateFont(20, 0, m_arrfTextGradient[1], 80, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Stencil");
	oldFont = (HFONT)SelectObject(m_hMemDC, myFont);

	temp = "GameStart";
	TextOut(m_hMemDC, 170, 200, temp.c_str(), temp.length());
	if (m_bMenuSelect == GAMESTART)
	{
		temp = "->";
		TextOut(m_hMemDC, 140, 200, temp.c_str(), temp.length());
	}
	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);

	myFont = CreateFont(20, 0, m_arrfTextGradient[2], 80, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Stencil");
	oldFont = (HFONT)SelectObject(m_hMemDC, myFont);

	temp = "Exit";
	TextOut(m_hMemDC, 170, 240, temp.c_str(), temp.length());
	if (m_bMenuSelect == EXIT)
	{
		temp = "->";
		TextOut(m_hMemDC, 140, 240, temp.c_str(), temp.length());
	}
	SelectObject(m_hMemDC, oldFont);
	DeleteObject(myFont);




	for (auto iter = m_vecExplosionEffect.begin(); iter != m_vecExplosionEffect.end(); iter++)
	{
		(*iter)->Draw(m_hMemDC, m_fElapseValue);
	}

	BitBlt(hdc, 0, 0, MAPWIDTHNUM * BLOCKWIDTH, MAPHEIGHTNUM * BLOCKHEIGHT, m_hMemDC, 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}

void GameManager::Update()
{

	if (m_eGameState == GAME_STATE_EXIT)
		return;

	m_dwElapseFormer = GetTickCount();

	//if (m_dwElapseLater - m_dwElapseFormer < 1000 / 60)		프레임제한
	//	return;
	m_fElapseValue = (m_dwElapseFormer - m_dwElapseLater) / 1000.0f;
	m_dwElapseLater = m_dwElapseFormer;
	m_fElapseSapceKey += m_fElapseValue;               //발사 간격 시간 조정


	if (m_eGameState == GAME_STATE_MENU)
	{

		KeyManager_Menu();

		if (m_eGameState == GAME_STATE_EXIT)
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
			return;

		}
			
		Draw_Menu();
		return;
	}

	
	NewTank();
	AutoMove_EnemyTank();
	KeyManager();
	MissileCollision();
	MissileCollision_EnemyTank();
	SetRt();
	FireMove();
	Check_Initailized_Tank();
	Operate_Slide_Tank();
	Draw();

	if(m_eGameState==GAME_STATE_DEAD)
		Gameover();

	InvalidateRect(m_hWnd, NULL, false);
}
void GameManager::Operate_Initial_Tank(Tank *pTank)
{

	for (auto iter = m_vecShield.begin(); iter != m_vecShield.end(); iter++)
	{
		if ((*iter)->GetSwitchFlag() == false)
		{
			(*iter)->SwitchFlagOn();
			(*iter)->SetXY_ElseEffect(pTank);
			break;
		}
	}

}

void GameManager::Check_Initailized_Tank()
{
	if (m_pMyTank->GetInitialOnMap() == true)
	{
		Operate_Initial_Tank(m_pMyTank);
		m_pMyTank->SetInitialFlag(false);
	}

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		if ((*iter)->GetInitialOnMap() == true)
		{
			Operate_Initial_Tank(*iter);
			(*iter)->SetInitialFlag(false);
		}
	}
}

void GameManager::Gameover()
{
	HDC hdc = GetDC(m_hWnd);
	
	if (m_iTemp_Gameover  < 300)
		m_iTemp_Gameover += m_fElapseValue * 150.0f;
	
	
	HFONT myFont = CreateFont(60, 0, 0, 80, 800, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Stencil");
	HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(255, 255, 255));
										//텍스트배경 색깔
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 100, MAPHEIGHT - m_iTemp_Gameover ,"GAME OVER", 9);

	SelectObject(hdc, oldFont);
	DeleteObject(myFont);

	m_fElpase_ResetTimer += m_fElapseValue;
	if (m_fElpase_ResetTimer > 5)
	{
		Reset_Game();
	}
}

void GameManager::Reset_Game()
{
	m_iNumEnemy = 10;
	
	m_eGameState = GAME_STATE_MENU;
	m_iLife = 3;
	m_bFirstBulletFired = false;
	m_bSecondBulletFired = false;
	m_iScore = 0;

	DeleteBlock();
	LoadData(m_hWnd);
	ConstructBlock();

	bool btemp = false;
	for (int y = 0; y < MAPHEIGHTNUM; y++)
	{
		for (int x = 0; x < MAPWIDTHNUM; x++)
		{
			if (m_iMap[y][x] == 1000 && rand() % 100 == 0)
			{
				m_pMyTank->Reset_Tank();
				m_pMyTank->SetX(GetPixelPosX(x));
				m_pMyTank->SetY(GetPixelPosY(y));
				btemp = true;
				break;
			}

		}
		if (btemp == true)
			break;

		if (y == MAPHEIGHTNUM - 1)
			y = 0;
	}

	for (auto iter = m_vecEnemyTank.begin(); iter != m_vecEnemyTank.end(); iter++)
	{
		(*iter)->Reset_EnemyTank();
	}

	for (auto iter = m_vecShield.begin(); iter != m_vecShield.end(); iter++)
	{
		(*iter)->Reset_ElseEffect();
	}

	m_ActionFlag = true;
	m_iTemp_menu = 2.0f;
	m_bFirstTank = false;
	m_arrfTextGradient[0] = 0;
	m_arrfTextGradient[1] = 0;
	m_arrfTextGradient[2] = 0;
	m_iTemp_Gameover = 10;
	m_fElpase_ResetTimer = 0;
}

void GameManager::DeleteBlock()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();
}

void GameManager::Operate_Slide_Tank()
{
	RECT temp;
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if ((*iter)->GetWhatAmI() == BLOCK_SLIDE)
		{
			if (IntersectRect(&temp, &m_pMyTank->GetRt(), &(*iter)->GetRt()) == true)
			{
				m_iSpeed_Tank = 2;
				break;
			}
			else
			{
				m_iSpeed_Tank = 1;

			}
		}
			
	}
}