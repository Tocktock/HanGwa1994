#include <iostream>
#include<string>
#include "GameManager.h"
#include "ResouceManager.h"
#include "Bitmap.h"
#include "Macro.h"
#include "Piece.h"
#include "Mine.h"
#include "Block.h"

using namespace std;


GameManager* GameManager::s_this = NULL;

GameManager::GameManager()
{
	m_pBitmapBlock = ResouceManager::GetInstance()->GetImage(RES_TYPE_BACK);
	m_eGameState = GAME_STATE_WAIT;
	m_gameLevelX = 8;
	m_gameLevelY = 8;
}


GameManager::~GameManager()
{
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecPiece.clear();

	SAFE_DELETE(s_this);
}

void GameManager::EraseMap()
{
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}

	m_vecPiece.clear();

}

void GameManager::Init(HWND hWnd)
{

	m_END = false;
	m_MineNumber = 0;


	for (int i = 0; i < m_gameLevelX; i++)
	{
		for (int j = 0; j < m_gameLevelY; j++)
		{
			if (rand() % 6 == 0)
			{
				Mine *pMine = new Mine();
				pMine->Init(ResouceManager::GetInstance()->GetImage(RES_TYPE_BLOCK), 
				ResouceManager::GetInstance()->GetImage(RES_TYPE_MINE),ResouceManager::GetInstance()->GetImage(RES_TYPE_FLAG),i+2,j+2,RES_TYPE_MINE);
				m_vecPiece.push_back(pMine);

				m_MineNumber++;
			}

		}
	}

	int vecPieceMine = 0;
	int vecPieceMineEnd = m_vecPiece.size();

	for (int i = 0; i < m_gameLevelX; i++)
	{
		for (int j = 0; j < m_gameLevelY; j++)
		{
			if (vecPieceMine<  vecPieceMineEnd)
			{
				if (m_vecPiece.at(vecPieceMine)->GetX() == i+2 &&
					m_vecPiece.at(vecPieceMine)->GetY() == j+2)
				{
					vecPieceMine++;
					continue;
				}
			}
			
				Block *pBlock = new Block();
				pBlock->Init(ResouceManager::GetInstance()->GetImage(RES_TYPE_BLOCK),
					ResouceManager::GetInstance()->GetImage((RES_TYPE)pBlock->HowMuchMineIs(i+2,j+2, m_vecPiece))
					,ResouceManager::GetInstance()->GetImage(RES_TYPE_FLAG), i+2, j+2, pBlock->HowMuchMineIs(i+2,j+2 , m_vecPiece));
				m_vecPiece.push_back(pBlock);
		}
	}
}

void GameManager::Draw(HDC hdc)
{
	int MineNumber = m_MineNumber;
	m_pBitmapBlock->BackDraw(hdc, 0, 0);
	
	
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		(*iter) -> Draw(hdc);
		if ((*iter)->isOpen() == FLAG)
			MineNumber--;
	}
	SetBkColor(hdc, RGB(255, 255, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	string szstr = to_string(MineNumber);
		TextOut(hdc, 675, 480,szstr.c_str(),szstr.length());
}



void GameManager::OperateLbutton(HWND hWnd, int x, int y)
{
	if (m_END == true)
		return;

	x = GetTilePosX(x);
	y = GetTilePosY(y);

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->isMine() == true)
				m_END = true;
			
				(*iter)->SetOpen(x, y, m_vecPiece, hWnd, 0);
			
		}
	}

}


void GameManager::OperateRbutton(HWND hWnd, int x, int y)
{

	if (m_END == true)
		return;

	x = GetTilePosX(x);
	y = GetTilePosY(y);

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->isOpen() == OPEN)
				return;

			if ((*iter)->isOpen() == FLAG)
			{
				(*iter)->ResetFlag();
				return;
			}
			
			(*iter)->SetFlag();
		}
	}
}

void GameManager::OperateLRbutton(HWND hWnd, int x, int y)
{

	if (m_END == true)
		return;
	x = GetTilePosX(x);
	y = GetTilePosY(y);

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			(*iter)->FirstSet();
			(*iter)->SetLROpen(x,y, m_vecPiece,hWnd);
		}
	}
}


//Tile ÁÂÇ¥°è ¿¡¼­ Pixcel ÁÂÇ¥°è·Î
int GameManager::GetPixelPosX(int TileX)
{
	return TileX * IMAGE_WIDTH;
}

int GameManager::GetPixelPosY(int TileY)
{
	return TileY * IMAGE_HEIGHT;
}

int GameManager::GetTilePosX(int PixelX)
{
	return PixelX / IMAGE_WIDTH;
}

int GameManager::GetTilePosY(int PixelY)
{
	return PixelY / IMAGE_HEIGHT;
}
void GameManager::ReturnState()
{
	m_eGameState = GAME_STATE_WAIT;
}

void GameManager::SetGameMode(GAME_MODE type)
{
	if (type == GAME_MODE_EASY)
	{
		m_gameLevelX = 8;
		m_gameLevelY = 8;
	}
	else if (type == GAME_MODE_NORMAL)
	{
		m_gameLevelX = 10;
		m_gameLevelY = 10;
	}
	else if (type == GAME_MODE_HARD)
	{
		m_gameLevelX = 16;
		m_gameLevelY = 16;
	}
}

void GameManager::VictoryCheck(HWND hWnd)
{
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->isMine() == false)
		{

			if ((*iter)->isOpen() != OPEN)
			{
				return;
			}

		}
	}

				MessageBox(hWnd, "ÀÌ°å¿À¿ä¿ä¿ä¿À¿À¿À¿À", "¹¹ ÀÌ°å¾î??", MB_OK);
				
}
