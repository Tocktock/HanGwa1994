#include "Tank.h"
#include "Bitmap.h"
#include <Windows.h>
#include "GameManager.h"
Tank::Tank()
{
	m_MoveSwitchFlag_Up = false;
	m_MoveSwitchFlag_Down = false;
	m_MoveSwitchFlag_Right = false;
	m_MoveSwitchFlag_Left = false;
	m_State = STATE00;
	m_StateCt = 0;
	m_iDirection = TANKDIRECTION_UP;

	m_fElapse_State = 0;
}


Tank::~Tank()
{
}

void Tank::Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5, Bitmap * Image6,
	Bitmap * Image7, Bitmap * Image8, float x, float y, bool Team)
{

	m_pTankImage[TANKSTATE_DOWN00] = Image1;
	m_pTankImage[TANKSTATE_DOWN01] = Image2;
	m_pTankImage[TANKSTATE_LEFT00] = Image3;
	m_pTankImage[TANKSTATE_LEFT01] = Image4;
	m_pTankImage[TANKSTATE_RIGHT00] = Image5;
	m_pTankImage[TANKSTATE_RIGHT01] = Image6;
	m_pTankImage[TANKSTATE_UP00] = Image7;
	m_pTankImage[TANKSTATE_UP01] = Image8;
	m_bInitialOnMap = true;
	m_fX = x;
	m_fY = y;
	m_Team = Team;
	m_AliveFlag = true;
	m_bDeadFlag = false;
	m_myRt.left = m_fX + 3;
	m_myRt.top = m_fY + 2;
	m_myRt.right = m_myRt.left - 3 + TANKWIDTH;
	m_myRt.bottom = m_myRt.top - 2 + TANKHEIGHT;

}

void Tank::Draw(HDC hdc)
{/**/

	
	if (m_iDirection == TANKDIRECTION_DOWN)
	{
		if (m_State == STATE00)
		{
			m_pTankImage[TANKSTATE_DOWN00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pTankImage[TANKSTATE_DOWN01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_LEFT)
	{
		if (m_State == STATE00)
		{
			m_pTankImage[TANKSTATE_LEFT00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pTankImage[TANKSTATE_LEFT01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_RIGHT)
	{
		if (m_State == STATE00)
		{
			m_pTankImage[TANKSTATE_RIGHT00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pTankImage[TANKSTATE_RIGHT01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

	else if (m_iDirection == TANKDIRECTION_UP)
	{
		if (m_State == STATE00)
		{
			m_pTankImage[TANKSTATE_UP00]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
		else
		{
			m_pTankImage[TANKSTATE_UP01]->Draw(hdc, (int)m_fX, (int)m_fY, TANKWIDTH, TANKHEIGHT);
		}
	}

}

void Tank::MoveRight(float ElapseValue, int FormerDirection)
{

	if (m_bDeadFlag == true)
		return;

 	m_fElapse_State += ElapseValue;
	if (m_fElapse_State > 0.5f)
	{
		TankStatePlus();
		m_fElapse_State = 0;
	}


	if (m_MovingLeft == true || m_MovingUp == true || m_MovingDown == true)
		return;

	if (m_fX > BLOCKWIDTH*(MAPWIDTHNUM -1)) //////1은 블럭수를 맞추기 위한 수.
		return;

	m_iDirection = TANKDIRECTION_RIGHT;
	Interpolate_Tank(FormerDirection);
	m_MovingRight = true;
	
	m_fX += 90 * ElapseValue;
}

void Tank::MoveLeft(float ElapseValue, int FormerDirection)
{

	if (m_bDeadFlag == true)
		return;

	m_fElapse_State += ElapseValue;
	if (m_fElapse_State > 0.5f)
	{
		TankStatePlus();
		m_fElapse_State = 0;
	}


	if (m_fX < 0)
		return;

	if (m_MovingRight == true || m_MovingUp == true || m_MovingDown == true)
		return;

	m_iDirection = TANKDIRECTION_LEFT;
	Interpolate_Tank(FormerDirection);
	m_MovingLeft = true;
	
	m_fX -= 90 * ElapseValue;
}

void Tank::MoveUp(float ElapseValue, int FormerDirection)
{

	if (m_bDeadFlag == true)
		return;

	m_fElapse_State += ElapseValue;
	if (m_fElapse_State > 0.5f)
	{
		TankStatePlus();
		m_fElapse_State = 0;
	}

	if (m_fY < 0)
		return;
	if (m_MovingLeft == true || m_MovingRight == true || m_MovingDown == true)
		return;

	m_iDirection = TANKDIRECTION_UP;
	Interpolate_Tank(FormerDirection);
	m_MovingUp = true;
	
	m_fY -= 90 * ElapseValue;
}
void Tank::MoveDown(float ElapseValue, int FormerDirection)
{

	if (m_bDeadFlag == true)
		return;

	m_fElapse_State += ElapseValue;

	if (m_fElapse_State > 0.5f)
	{
		TankStatePlus();
		m_fElapse_State = 0;
	}



	if (m_fY > BLOCKHEIGHT*(MAPHEIGHTNUM-1)) ////////1은 블럭수를 맞추기 위한 수
		return;

	if (m_MovingLeft == true || m_MovingUp == true || m_MovingRight == true)
		return;

	m_iDirection = TANKDIRECTION_DOWN;
	Interpolate_Tank(FormerDirection);
	m_MovingDown = true;
	
	m_fY += 90 * ElapseValue;
}

void Tank::ResetMove()
{
	m_MovingRight = false;
	m_MovingLeft = false;
	m_MovingUp = false;
	m_MovingDown = false;

}


void Tank::MissileFire()
{

}


void Tank::SetRt()
{

	if (m_bDeadFlag == true)
		return;

	m_myRt.left = m_fX + 3;
	m_myRt.top = m_fY + 2;
	m_myRt.right = m_fX + TANKWIDTH -3;
	m_myRt.bottom = m_fY + TANKHEIGHT -2;
}

bool Tank::CheckCollision_for_Block(RECT rt,int Direction)
{

	if (m_bDeadFlag == true)
		return false;

	RECT temp;		//////////////필요없는데 형식상 필요한 변수
	RECT tempRt = m_myRt;	/////////////이거 잠시 저장할 값
	
	if (Direction == TANKDIRECTION_DOWN)
		tempRt.bottom += 2;	

	else if (Direction == TANKDIRECTION_RIGHT)
		tempRt.right += 2;

	else if (Direction == TANKDIRECTION_UP)
		tempRt.top -= 2;

	else if (Direction == TANKDIRECTION_LEFT)
		tempRt.left -= 2;

	if (IntersectRect(&temp, &tempRt, &rt))
	{
		return true;
	}

	else
		return false;
}

void Tank::Interpolate_Tank(int Former_Direction)
{

	if (m_bDeadFlag == true)
		return;

	int iDirection;
	int Tile_first = 0;		//탱크위치
	int Tile_Second = 0;	//보간된 탱크위치

	
	
	iDirection = m_iDirection;

	if (iDirection == Former_Direction)
		return;

	if (iDirection == TANKDIRECTION_LEFT || iDirection == TANKDIRECTION_RIGHT
		&& Former_Direction == TANKDIRECTION_UP)
	{

		//보간된 탱크위치

		Tile_first = GameManager::GetInstance()->GetTilePosY(m_fY + TANKHEIGHT);
		Tile_Second = GameManager::GetInstance()->GetTilePosY(m_fY + 16); //보간위치

		if (Tile_first != Tile_Second)
		{
			m_fY = GameManager::GetInstance()->GetPixelPosY(Tile_Second);
		}
	}

	else if (iDirection == TANKDIRECTION_LEFT || iDirection == TANKDIRECTION_RIGHT
		&& Former_Direction == TANKDIRECTION_DOWN)
	{

		//보간된 탱크위치

		Tile_first = GameManager::GetInstance()->GetTilePosY(m_fY);
		Tile_Second = GameManager::GetInstance()->GetTilePosY(m_fY + 8);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_fY = GameManager::GetInstance()->GetPixelPosY(Tile_Second);
			
		}
	}

	else if (iDirection == TANKDIRECTION_UP || iDirection == TANKDIRECTION_DOWN
		&&Former_Direction == TANKDIRECTION_LEFT)
	{

		Tile_first = GameManager::GetInstance()->GetTilePosX(m_fX + TANKWIDTH);
		Tile_Second = GameManager::GetInstance()->GetTilePosX(m_fX + 22);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_fX = GameManager::GetInstance()->GetPixelPosX(Tile_Second);
		}
	}

	else if (iDirection == TANKDIRECTION_UP || iDirection == TANKDIRECTION_DOWN
		&&Former_Direction == TANKDIRECTION_RIGHT)
	{

		Tile_first = GameManager::GetInstance()->GetTilePosX(m_fX);
		Tile_Second = GameManager::GetInstance()->GetTilePosX(m_fX+ 11);	//보간위치

		if (Tile_first != Tile_Second)
		{
			m_fX = GameManager::GetInstance()->GetPixelPosX(Tile_Second);
		}
	}

}

bool Tank::CheckCollision_for_Tank(RECT rt, int Direction)
{
	if (m_bDeadFlag == true)
		return false;

	RECT temp;		//////////////필요없는데 형식상 필요한 변수
	RECT tempRt = m_myRt;	/////////////이거 잠시 저장할 값

	if (Direction == TANKDIRECTION_DOWN)
		tempRt.bottom += 3;

	else if (Direction == TANKDIRECTION_RIGHT)
		tempRt.right += 3;

	else if (Direction == TANKDIRECTION_UP)
		tempRt.top -= 3;

	else if (Direction == TANKDIRECTION_LEFT)
		tempRt.left -= 3;

	if (IntersectRect(&temp, &tempRt, &rt))
	{
		return true;
	}

	else
		return false;
}