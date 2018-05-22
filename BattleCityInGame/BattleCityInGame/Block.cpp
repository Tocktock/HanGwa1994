#include "Block.h"
#include "Bitmap.h"
#include "GameManager.h"
#include <Windows.h>


Block::Block()
{
	m_bCollisionFlag = true;
	m_CollisionRt.left = 0;
	m_CollisionRt.right = 0;
	m_CollisionRt.top = 0;
	m_CollisionRt.bottom = 0;

	m_iHitDirection_Brick = 5;
}


Block::~Block()
{
}

void Block::Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5,
	Bitmap * Image6, Bitmap * Image7, Bitmap * Image8, Bitmap * Image9, Bitmap * Image10
	, Bitmap * Image11, Bitmap * Image12, Bitmap * Image13, Bitmap * Image14, Bitmap * Image15, int x, int y, int WhatAmI)
{
	m_BlockImage[BLOCK_BRICK_01] = Image1;
	m_BlockImage[BLOCK_BRICK_02] = Image2;
	m_BlockImage[BLOCK_BRICK_03] = Image3;
	m_BlockImage[BLOCK_BRICK_04] = Image4;
	m_BlockImage[BLOCK_BRICK_05] = Image5;
	m_BlockImage[BLOCK_SLIDE] = Image6;
	m_BlockImage[BLOCK_FOREST] = Image7;
	m_BlockImage[BLOCK_SEA] = Image8;
	m_BlockImage[BLOCK_STEEL_ALL] = Image9;
	m_BlockImage[BLOCK_STEEL_UP] = Image10;
	m_BlockImage[BLOCK_STEEL_LEFT] = Image11;
	m_BlockImage[BLOCK_STEEL_BOTTOM] = Image12;
	m_BlockImage[BLOCK_STEEL_RIGHT] = Image13;
	m_BlockImage[BLOCK_EAGLE] = Image14;
	m_BlockImage[BLOCK_SURREND] = Image15;
	m_iX = x;
	m_iY = y;
	m_WhatAmI = WhatAmI;

	if (WhatAmI == BLOCK_FOREST)         //숲 블록은 충돌체크 안하니까 rt설정 불필요
	{
		return;
	}

	m_CollisionRt.left = GameManager::GetInstance()->GetPixelPosX(m_iX);
	m_CollisionRt.top = GameManager::GetInstance()->GetPixelPosY(m_iY);
	m_CollisionRt.right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
	m_CollisionRt.bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;

}

void Block::Draw(HDC hdc)
{

	if (m_WhatAmI == BLOCK_BRICK_01)
	{
		m_BlockImage[BLOCK_BRICK_01]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_BRICK_02)
	{
		m_BlockImage[BLOCK_BRICK_02]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_BRICK_03)
	{
		m_BlockImage[BLOCK_BRICK_03]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_BRICK_04)
	{
		m_BlockImage[BLOCK_BRICK_04]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_BRICK_05)
	{
		m_BlockImage[BLOCK_BRICK_05]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_SLIDE)
	{
		m_BlockImage[BLOCK_SLIDE]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_FOREST)
	{
		m_BlockImage[BLOCK_FOREST]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_SEA)
	{
		m_BlockImage[BLOCK_SEA]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_STEEL_ALL)
	{
		m_BlockImage[BLOCK_STEEL_ALL]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_STEEL_UP)
	{
		m_BlockImage[BLOCK_STEEL_UP]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_STEEL_LEFT)
	{
		m_BlockImage[BLOCK_STEEL_LEFT]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_STEEL_BOTTOM)
	{
		m_BlockImage[BLOCK_STEEL_BOTTOM]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_STEEL_RIGHT)
	{
		m_BlockImage[BLOCK_STEEL_RIGHT]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_EAGLE)
	{
		m_BlockImage[BLOCK_EAGLE]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
	else if (m_WhatAmI == BLOCK_SURREND)
	{
		m_BlockImage[BLOCK_SURREND]->Draw(hdc, GameManager::GetInstance()->GetPixelPosX(m_iX),
			GameManager::GetInstance()->GetPixelPosY(m_iY), BLOCKWIDTH, BLOCKHEIGHT);
	}
}

bool Block::CollisionCheck_First(RECT rt)
{
	if (m_bCollisionFlag == false)
		return false;

	if (m_WhatAmI == BLOCK_BRICK_01)
	{
		return CollisionCheck_Brick(rt);
	}
	RECT temp;

	if (IntersectRect(&temp, &m_CollisionRt, &rt))
	{
		return true;
	}

	else
		return false;
}

void Block::DestroyBlock()
{
	if (m_WhatAmI >= BLOCK_STEEL_ALL && m_WhatAmI <= BLOCK_STEEL_LEFT)          //강철벽은 못뚫음
		return;

	if (m_WhatAmI == BLOCK_SEA || m_WhatAmI == BLOCK_FOREST)
		return;

	if (m_WhatAmI == BLOCK_BRICK_01)
	{
		if (m_iHitDirection_Brick == BRICKCOLLISIONDIRECTION_UP)
		{
			m_WhatAmI = BLOCK_BRICK_04;
		}
		
		else if (m_iHitDirection_Brick == BRICKCOLLISIONDIRECTION_LEFT)
		{
			m_WhatAmI = BLOCK_BRICK_05;
		}

		else if (m_iHitDirection_Brick == BRICKCOLLISIONDIRECTION_RIGHT)
		{
			m_WhatAmI = BLOCK_BRICK_03;
		}

		else if (m_iHitDirection_Brick == BRICKCOLLISIONDIRECTION_DOWN)
		{
			m_WhatAmI = BLOCK_BRICK_02;
		}

		SetRt();
		return;
	}

	m_WhatAmI = EMPTYBLOCK;
	m_bCollisionFlag = false;
	m_CollisionRt.left = 0;
	m_CollisionRt.right = 0;
	m_CollisionRt.top = 0;
	m_CollisionRt.bottom = 0;
}

void Block::SetRt()
{
	if (m_WhatAmI == BLOCK_BRICK_01)
	{
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_LEFT].left = GameManager::GetInstance()->GetPixelPosX(m_iX);				       //왼쪽방향
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_LEFT].top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_LEFT].right = GameManager::GetInstance()->GetPixelPosX(m_iX) + 5;
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_LEFT].bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) +BLOCKHEIGHT;

		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_RIGHT].left = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH -5;		//오른쪽
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_RIGHT].top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_RIGHT].right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_RIGHT].bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;

		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_UP].left = GameManager::GetInstance()->GetPixelPosX(m_iX);			           //위쪽
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_UP].top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_UP].right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_UP].bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + 5;

		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_DOWN].left = GameManager::GetInstance()->GetPixelPosX(m_iX);                      //밑에쪽
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_DOWN].top = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT - 5;
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_DOWN].right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_DOWN].bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;

	}

	else if (m_WhatAmI == BLOCK_BRICK_02 || m_WhatAmI == BLOCK_STEEL_UP)
	{

		m_CollisionRt.left = GameManager::GetInstance()->GetPixelPosX(m_iX);
		m_CollisionRt.right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRt.top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRt.bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT/2;
	}
	else if (m_WhatAmI == BLOCK_BRICK_03|| m_WhatAmI == BLOCK_STEEL_LEFT)
	{
		m_CollisionRt.left = GameManager::GetInstance()->GetPixelPosX(m_iX);
		m_CollisionRt.right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH/2;
		m_CollisionRt.top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRt.bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;
	}

	else if (m_WhatAmI == BLOCK_BRICK_04 || m_WhatAmI == BLOCK_STEEL_BOTTOM)
	{
		m_CollisionRt.left = GameManager::GetInstance()->GetPixelPosX(m_iX);
		m_CollisionRt.right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRt.top = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT/2;
		m_CollisionRt.bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;
	}


	else if (m_WhatAmI == BLOCK_BRICK_05 || m_WhatAmI == BLOCK_STEEL_RIGHT)
	{
		m_CollisionRt.left = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH / 2;
		m_CollisionRt.right = GameManager::GetInstance()->GetPixelPosX(m_iX) + BLOCKWIDTH;
		m_CollisionRt.top = GameManager::GetInstance()->GetPixelPosY(m_iY);
		m_CollisionRt.bottom = GameManager::GetInstance()->GetPixelPosY(m_iY) + BLOCKHEIGHT;
	}
	else
		return;

	
}

bool Block::CollisionCheck_Brick(RECT rt)
{
	RECT temp;

	if (IntersectRect(&temp, &rt, &m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_UP]) == true)
	{
		m_iHitDirection_Brick = BRICKCOLLISIONDIRECTION_UP;
		return true;
	}
	else if (IntersectRect(&temp, &rt, &m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_LEFT]) == true)
	{

		m_iHitDirection_Brick = BRICKCOLLISIONDIRECTION_LEFT;
		return true;
	}
	else if (IntersectRect(&temp, &rt, &m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_RIGHT]) == true)
	{

		m_iHitDirection_Brick = BRICKCOLLISIONDIRECTION_RIGHT;
		return true;
	}
	else if (IntersectRect(&temp, &rt, &m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_DOWN]) == true)
	{
		m_iHitDirection_Brick = BRICKCOLLISIONDIRECTION_DOWN;
		return true;
	}
	else
		return false;
}