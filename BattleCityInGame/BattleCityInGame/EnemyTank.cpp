#include "EnemyTank.h"
#include "Bitmap.h"


EnemyTank::EnemyTank()
{
	m_iNextMoveDirection = TANKDIRECTION_DOWN;
}


EnemyTank::~EnemyTank()
{
}

void EnemyTank::Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, Bitmap *Image4, Bitmap *Image5,
	Bitmap *Image6, Bitmap *Image7, Bitmap *Image8, Bitmap *Image9,
	Bitmap *Image10, Bitmap *Image11, Bitmap *Image12, float x, float y, bool team)
{
	m_pBitmapEnemyTank[ENEMYTANK_UP00] = Image1;
	m_pBitmapEnemyTank[ENEMYTANK_UP01] = Image2;
	m_pBitmapEnemyTank[ENEMYTANK_UP02] = Image3;
	m_pBitmapEnemyTank[ENEMYTANK_LEFT00] = Image4;
	m_pBitmapEnemyTank[ENEMYTANK_LEFT01] = Image5;
	m_pBitmapEnemyTank[ENEMYTANK_LEFT02] = Image6;
	m_pBitmapEnemyTank[ENEMYTANK_RIGHT00] = Image7;
	m_pBitmapEnemyTank[ENEMYTANK_RIGHT01] = Image8;
	m_pBitmapEnemyTank[ENEMYTANK_RIGHT02] = Image9;
	m_pBitmapEnemyTank[ENEMYTANK_DOWN00] = Image10;
	m_pBitmapEnemyTank[ENEMYTANK_DOWN01] = Image11;
	m_pBitmapEnemyTank[ENEMYTANK_DOWN02] = Image12;
	m_fX = x;
	m_fY = y;
	m_Team = team;
	m_fElapse_AutoFire = 0;
	m_fElapse_AutoMove_EnemyTank = 0;
	m_AliveFlag = false;
	m_bDeadFlag = false;
	m_iDirection = TANKDIRECTION_LEFT;
	m_myRt.left = m_fX +3 ;							//각각의 상수는 적당히 콜리전 크기를 잡기위해서 넣은거임.
	m_myRt.top = m_fY + 2;
	m_myRt.right = m_fX - 3 + TANKWIDTH;
	m_myRt.bottom = m_fY - 2 + TANKHEIGHT;
}

void EnemyTank::Draw(HDC hdc)
{

	if (m_AliveFlag == false || m_bDeadFlag == true)
		return;

	if (m_iDirection == TANKDIRECTION_DOWN)
	{
		if (m_State == STATE00)
		{
			m_pBitmapEnemyTank[ENEMYTANK_DOWN00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pBitmapEnemyTank[ENEMYTANK_DOWN01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_LEFT)
	{
		if (m_State == STATE00)
		{
			m_pBitmapEnemyTank[ENEMYTANK_LEFT00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pBitmapEnemyTank[ENEMYTANK_LEFT01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_RIGHT)
	{
		if (m_State == STATE00)
		{
			m_pBitmapEnemyTank[ENEMYTANK_RIGHT00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pBitmapEnemyTank[ENEMYTANK_RIGHT01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_UP)
	{
		if (m_State == STATE00)
		{
			m_pBitmapEnemyTank[ENEMYTANK_UP00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pBitmapEnemyTank[ENEMYTANK_UP01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

}

void EnemyTank::AutoMoving(float Elapse,int FormerDirection,bool CollisionFlag)
{
	if (m_AliveFlag == false)
		return;


	if (m_bDeadFlag == true)
		return;

	m_fElapse_AutoMove += Elapse;
	AutoMoving_Change_Direction();

	if (CollisionFlag == true)
		return;

	if (m_iDirection == TANKDIRECTION_UP)
		MoveUp(Elapse,FormerDirection);

	else if (m_iDirection == TANKDIRECTION_DOWN)
		MoveDown(Elapse, FormerDirection);

	else if (m_iDirection == TANKDIRECTION_RIGHT)
		MoveRight(Elapse, FormerDirection);

	else if (m_iDirection == TANKDIRECTION_LEFT)
		MoveLeft(Elapse, FormerDirection);

	else return;


}

void EnemyTank::CreateEnemy()
{
	if (m_bDeadFlag == true)
		return;

	else
	{
		m_AliveFlag = true;
		m_fElapse_AutoMove = 0;
	}
}

void EnemyTank::AutoMoving_Change_Direction()
{


	if (m_bDeadFlag == true)
		return;

	if (m_fElapse_AutoMove > 3.0f)
	{
		m_iDirection = rand() % TANKDIRECTION_END;
		m_fElapse_AutoMove = 0;
	}

}

void EnemyTank::Reset_EnemyTank()
{
	m_fX = 1000;
	m_fY = 1000;
	m_fElapse_AutoFire = 0;
	m_fElapse_AutoMove_EnemyTank = 0;
	m_AliveFlag = false;
	m_bDeadFlag = false;
	m_iDirection = TANKDIRECTION_LEFT;
	m_bInitialOnMap = true;
}