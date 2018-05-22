#include "Piece.h"
#include "Bitmap.h"
#include "GameManager.h"
#include "Macro.h"


Piece::Piece()
{
	m_iState = CLOSE;

	m_leftFlag = true;
	m_topFlag = true;
	m_rightFlag = true;
	m_bottomFlag = true;
}


Piece::~Piece()
{
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecPiece.clear();
}
void Piece::Init(Bitmap* pBitmap, Bitmap* pBackBitmap,Bitmap* pFlagBitmap, int x, int y, int whatAmI)
{
	m_pFlagBitmap = pFlagBitmap;
	m_pBitmap = pBitmap;
	m_pBackBitmap = pBackBitmap;
	m_iX = x;
	m_iY = y;
	m_whatAmI = whatAmI;
}
void Piece::Draw(HDC hdc)
{

	if (m_iState == OPEN)
	{
		m_pBackBitmap->Draw(hdc, GameManager::GetPixelPosX(m_iX), GameManager::GetPixelPosX(m_iY));
	}

	else if (m_iState == FLAG)
	{
		m_pFlagBitmap->Draw(hdc, GameManager::GetPixelPosX(m_iX), GameManager::GetPixelPosX(m_iY));
	}

	else if (m_iState == CLOSE)
	{
		m_pBitmap->Draw(hdc, GameManager::GetPixelPosX(m_iX), GameManager::GetPixelPosX(m_iY));
	}
}

void Piece::SetOpen(int x, int y, vector<Piece*>m_vecPiece, HWND hWnd, int DirectionFlag)
{
	
	if (x >= GameManager::GetInstance()->GameLevelX()+2 || x< 2||y<2|| y >= GameManager::GetInstance()->GameLevelY() +2)
		return;

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->isOpen() == OPEN || (*iter)->isOpen() == FLAG)
				return;
		}
	}


	SetOpenOperate(x, y, m_vecPiece, hWnd, DirectionFlag);

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->m_whatAmI == MINEBLOCK ||
				(*iter)->m_whatAmI == FLAGBLOCK)
					return;

			if ((*iter)->m_whatAmI > EMPTYBLOCK && (*iter)->m_whatAmI <= EIGHTBLOCK)
			{
				return;
			}

		}
	}



	SetOpen(x - 1, y, m_vecPiece, hWnd, 1);
	SetOpen(x - 1, y - 1, m_vecPiece, hWnd, 1);
	SetOpen(x - 1, y + 1, m_vecPiece, hWnd, 1);
	SetOpen(x + 1, y, m_vecPiece, hWnd, 2);
	SetOpen(x + 1, y - 1, m_vecPiece, hWnd, 2);
	SetOpen(x + 1, y + 1, m_vecPiece, hWnd, 2);
	SetOpen(x, y - 1, m_vecPiece, hWnd, 3);
	SetOpen(x, y + 1, m_vecPiece, hWnd, 4);

}

void Piece::SetOpenOperate(int x, int y, vector<Piece*> m_vecPiece, HWND hWnd,int DirectionFlag)
{
	

	////////////////////////////////////////////////////////////////////////////////////Flag


	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->m_iState == FLAG)
				return;
		}
	}



	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		
		if ((*iter)->GetX() == x && (*iter)->GetY() == y && (*iter)->m_whatAmI == EMPTYBLOCK)
		{
			                  //클릭한 자리 오픈
			(*iter)->m_iState = OPEN;
				return;
			
		}

		else if ((*iter)->GetX() == x && (*iter)->GetY() == y && (*iter)->m_whatAmI == MINEBLOCK)
		{
			(*iter)->m_iState = OPEN;
			MessageBox(hWnd, "어서와", "지뢰는 처음이지?", MB_OK);
			return;
		}

		else if((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			(*iter)->m_iState = OPEN;
			return;
		}
	}

	
}

void Piece::SetLROpen(int x, int y, vector<Piece*> m_vecPiece, HWND hWnd)
{
	int MineCt = 2;
	int whatAmI;
	static int Ct = 0;
	bool WrongCheck = false;

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->isOpen() == CLOSE && m_firstCheckInLR == true)
				return;

			if ((*iter)->isOpen() == OPEN && m_firstCheckInLR != true)
				return;
		}
	}


	if (x >= GameManager::GetInstance()->GameLevelX() + 2 || x < 2 || y < 2 || y >= GameManager::GetInstance()->GameLevelX() + 2)
	{
		return;
	}
		
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x  && (*iter)->GetY() == y )
		{
			whatAmI = (*iter)->m_whatAmI;
		}
	}

	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x-1 && (*iter)->GetY() == y -1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x && (*iter)->GetY() == y - 1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}
		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y - 1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x - 1 && (*iter)->GetY() == y)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x - 1 && (*iter)->GetY() == y + 1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x && (*iter)->GetY() == y + 1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}

		else if ((*iter)->GetX() == x + 1 && (*iter)->GetY() == y+1)
		{
			if ((*iter)->isMine() == false && (*iter)->m_iState == FLAG)
			{
				WrongCheck = true;
			}
			if ((*iter)->m_iState == FLAG)
			{
				MineCt++;
			}
		}
	}

	if (whatAmI != MineCt && m_firstCheckInLR == true)
		return;

	if (whatAmI == 11 || whatAmI == 12)
	{
		return;
	}
	if (WrongCheck == true)
	{
		SetOpenOperate(x-1, y-1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x-1, y+1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x-1, y, m_vecPiece, hWnd, 0);
		SetOpenOperate(x+1, y-1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x+1, y+1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x+1, y, m_vecPiece, hWnd, 0);
		SetOpenOperate(x, y-1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x, y+1, m_vecPiece, hWnd, 0);
		SetOpenOperate(x, y, m_vecPiece, hWnd, 0);
		GameManager::GetInstance()->MakeEndTrue();
		return;

	}

	

	SetOpenOperate(x, y, m_vecPiece, hWnd, 0);

	
	Ct++;
	 
	for (auto iter = m_vecPiece.begin(); iter != m_vecPiece.end(); iter++)
	{
		if ((*iter)->GetX() == x && (*iter)->GetY() == y)
		{
			if ((*iter)->m_whatAmI > EMPTYBLOCK && (*iter)->m_whatAmI <= EIGHTBLOCK && m_firstCheckInLR !=true)
			{
				return;
			}

		}
	}


	m_firstCheckInLR = false;


	SetLROpen(x - 1, y, m_vecPiece, hWnd);
	SetLROpen(x - 1, y - 1, m_vecPiece, hWnd);
	SetLROpen(x - 1, y + 1, m_vecPiece, hWnd);
	SetLROpen(x + 1, y, m_vecPiece, hWnd);
	SetLROpen(x + 1, y - 1, m_vecPiece, hWnd);
	SetLROpen(x + 1, y + 1, m_vecPiece, hWnd);
	SetLROpen(x, y - 1, m_vecPiece, hWnd);
	SetLROpen(x, y + 1, m_vecPiece, hWnd);

}