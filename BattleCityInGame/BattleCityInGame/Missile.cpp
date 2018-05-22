#include "Missile.h"
#include"Bitmap.h"


Missile::Missile()
{
	m_bOnflag = false;
}

Missile::~Missile()
{
}

void Missile::Init(Bitmap *Image, float x, float y, int Team)
{
	m_pBitmapMissile = Image;
	m_fX = x;
	m_fY = y;
	m_Team = Team;

	m_myRt.left = 1000;
	m_myRt.right = 1000;
	m_myRt.top = 1000;
	m_myRt.bottom = 1000;
}

void Missile::Draw(HDC hdc)
{

	if (m_bOnflag == false)
		return;

	else
	{
		m_pBitmapMissile->Draw(hdc, (int)m_fX, (int)m_fY, MISSILEWIDTH, MISSILEHEIGHT);
	}

}

void Missile::FireMove(float ElapseValue)
{
	if (m_fY < 0 || m_fX >= MAPWIDTHNUM * BLOCKWIDTH || m_fX < 0 || m_fY >= MAPHEIGHTNUM * BLOCKHEIGHT)
	{
		m_bOnflag = false;
		return;
	}

	if (m_bOnflag == false)
		return;

	if (m_iDirection == MISSILEDIRECTION_DOWN)
	{
		m_fY += 200.0f * ElapseValue;
	}

	else if (m_iDirection == MISSILEDIRECTION_LEFT)
	{
		m_fX -= 200 * ElapseValue;
	}
	
	else if (m_iDirection == MISSILEDIRECTION_RIGHT)
	{
		m_fX += 200 * ElapseValue;
	}

	else if (m_iDirection == MISSILEDIRECTION_UP)
	{
		m_fY -= 200 * ElapseValue;
	}
}

void Missile::SetXY(float x, float y)
{
	if (m_bOnflag == true)
		return;
	
	m_fX = x;
	m_fY = y;
}

void Missile::SetRt()
{
	if (m_bOnflag == false)
		return;

	m_myRt.left = m_fX;
	m_myRt.top = m_fY;
	m_myRt.right = m_fX + MISSILEWIDTH;
	m_myRt.bottom = m_fY + MISSILEHEIGHT;
}

void Missile::SetDirection(int Direction)
{
	if (m_bOnflag == true)
		return;


	m_iDirection = Direction;
}

bool Missile::CheckCollision_First(RECT rt)
{
	RECT temp; ///////°Á ÀÓÀÇÀÇ°ª
	if (IntersectRect(&temp, &m_myRt, &rt))
	{
		return false;
	}
	else
		return true;
}