#include "Character.h"
#include "Bitmap.h"
#include "GameManager.h"
#include<cmath>
Character::Character()
{
}


Character::~Character()
{
}

void Character::CharacterInit(Bitmap *player1, Bitmap *player2, Bitmap *player3,
	Bitmap *player4, Bitmap *player5, Bitmap *player6, int x, int y)
{
	m_player[0] = player1;
	m_player[1] = player2;
	m_player[2] = player3;
	m_player[3] = player4;
	m_player[4] = player5;
	m_player[5] = player6;
	m_Characterstate = 0;
	m_iX = x;
	m_iY = y;
}

void Character::Draw(HDC hdc)
{

	if (m_Characterstate == 5)
	{
		m_player[5]->Draw(hdc, m_iX, m_iY, 65, 72);                                   //win 3
	}
	else if (m_Characterstate == 0)
	{
		m_player[0]->Draw(hdc, m_iX, m_iY, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	}
		
	else if (m_Characterstate == 1)
	{
		m_player[1]->Draw(hdc, m_iX, m_iY, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	}
	else if (m_Characterstate == 2)
	{
		m_player[2]->Draw(hdc, m_iX, m_iY, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	}
	else if (m_Characterstate == 3)
	{
		m_player[3]->Draw(hdc, m_iX, m_iY, 66 , 65);                         //win 1
	}
	else if (m_Characterstate == 4)
	{
		m_player[4]->Draw(hdc, m_iX, m_iY, 66, 65);                               //win 2
	}
	
}

void Character::Jumping(float Seta)
{
	if (m_Characterstate == 5)
		return;

	//m_iX =  m_CenterX + cosf(GameManager::GetInstance()->ToRadian(Seta))*30;
	m_iY =  m_CenterY + sinf(GameManager::GetInstance()->ToRadian(Seta))*95;

}

void Character::Moving()
{
	if (m_Characterstate == 5)
		return;
	
	if (m_Characterstate < 3)
	{
		m_Characterstate++;
	}
	if (m_Characterstate == 3)
	{
		m_Characterstate = 0;
	}
}

void Character::LeftMoving()
{
	if (m_Characterstate < 2)
	{
		m_Characterstate++;
	}
	if (m_Characterstate == 2)
	{
		m_Characterstate = 0;
	}
}

void Character::SetmyRect()
{
	if (m_Characterstate == 5)
		return;

	m_myRt[0].left = m_iX + 5;
	m_myRt[0].right = m_iX + 61;
	m_myRt[0].top = m_iY + 40;
	m_myRt[0].bottom = m_iY + 50;

	m_myRt[1].left = m_iX +20;
	m_myRt[1].right = m_iX + 45;
	m_myRt[1].top = m_iY;
	m_myRt[1].bottom = m_iY + 50;
}