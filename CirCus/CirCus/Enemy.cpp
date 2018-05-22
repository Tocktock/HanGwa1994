#include "Enemy.h"
#include "Bitmap.h"


Enemy::Enemy()
{
	m_state = true;
	m_stateCt = 0;
}


Enemy::~Enemy()
{
}

void Enemy::Init(Bitmap *Image1,Bitmap*Image2,int x,int y)
{
	m_Image[0] = Image1;
	m_Image[1] = Image2;
	m_iX = x;
	m_iY = y;
}

void Enemy::Draw(HDC hdc)
{
	
	if (m_state == true)
	{
		m_Image[0]->Draw(hdc, m_iX, m_iY, POTFLAMEX, POTFLAMEY);
		if (m_stateCt == 10)
		{
			m_stateCt = 0;
			m_state = false;
		}

	}

	else if (m_state == false)
	{
		m_Image[1]->Draw(hdc, m_iX, m_iY, POTFLAMEX, POTFLAMEY);

		if (m_stateCt == 10)
		{
			m_stateCt = 0;
			m_state = true;
		}
	}
}

void Enemy::SetmyRt()
{
	m_myRt[0].left = m_iX+5;
	m_myRt[0].right = m_iX + 45;
	m_myRt[0].top = m_iY + 30;
	m_myRt[0].bottom = m_iY + 50;

	m_myRt[1].left = m_iX + 16;
	m_myRt[1].right = m_iX + 32;
	m_myRt[1].top = m_iY +5;
	m_myRt[1].bottom = m_iY + 50;

	m_myRt[2].left = m_iX + 23;
	m_myRt[2].right = m_iX + 27;
	m_myRt[2].top = m_iY -80;
	m_myRt[2].bottom = m_iY -10;

}
void Enemy::CollisionCheck()
{

}
