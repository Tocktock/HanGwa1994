#pragma once
#include <Windows.h>

#define BLUETEAM false
#define REDTEAM	true

#define STATE00 false
#define STATE01 true

#define TANKWIDTH 33
#define TANKHEIGHT 25

#define BLOCKWIDTH 33
#define BLOCKHEIGHT 25
#define MAPWIDTHNUM 14
#define MAPHEIGHTNUM 16


enum TANKSTATE_TYPE
{
	TANKSTATE_DOWN00,
	TANKSTATE_DOWN01,
	TANKSTATE_LEFT00,
	TANKSTATE_LEFT01,
	TANKSTATE_RIGHT00,
	TANKSTATE_RIGHT01,
	TANKSTATE_UP00,
	TANKSTATE_UP01,
	TANKSTATE_END
};

enum TANKDIRECTION
{
	TANKDIRECTION_DOWN,
	TANKDIRECTION_LEFT,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_UP,
	TANKDIRECTION_END
};

class Bitmap;
class Tank
{

protected:

	Bitmap *m_pTankImage[TANKSTATE_END];
	float m_fX;
	float m_fY;
	bool m_Team;
	bool m_State;
	int m_StateCt;

	bool m_MovingRight;
	bool m_MovingLeft;
	bool m_MovingUp;
	bool m_MovingDown;

	bool m_AliveFlag;
	bool m_bDeadFlag;

	bool m_MoveSwitchFlag_Left;
	bool m_MoveSwitchFlag_Right;
	bool m_MoveSwitchFlag_Up;
	bool m_MoveSwitchFlag_Down;

	bool m_bInitialOnMap;

	float m_fElapse_State;

	int m_iDirection;
	RECT m_myRt;

public:


	void Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5, Bitmap * Image6,
		Bitmap * Image7, Bitmap * Image8, float x,float y, bool Team);
	
	void Draw(HDC hdc);
	void MissileFire();
	bool CheckCollision_for_Block(RECT rt,int Direction);
	bool CheckCollision_for_Tank(RECT rt, int Direction);
	void ResetMove();
	void MoveRight(float ElapseValue,int FormerDirection);
	void MoveLeft(float ElapseValue, int FormerDirection);
	void MoveUp(float ElapseValue, int FormerDirection);
	void MoveDown(float ElapseValue, int FormerDirection);
	void Interpolate_Tank(int Former_Direction);
	void SetRt();

	inline void DestroyTank()
	{
		m_bDeadFlag = true;
	}
	inline void Reset_Tank()
	{
		m_bDeadFlag = false;

		m_bInitialOnMap = true;
	}
	inline bool GetInitialOnMap()
	{
		return m_bInitialOnMap;
	}
	inline void SetInitialFlag(bool Value)
	{
		m_bInitialOnMap = Value;
	}

	inline bool GetAliveFlag()
	{
		return m_AliveFlag;
	}

	inline void SetAliveFlag(int flag)
	{
		m_AliveFlag = flag;
	}

	inline RECT GetRt()
	{
		return m_myRt;
	}

	inline int GetDirection()
	{
		return m_iDirection;
	}

	inline int GetX()
	{
		return (int)m_fX;
	}

	inline int GetY()
	{
		return (int)m_fY;
	}

	inline void SetX(float x)
	{
		m_fX = x;
	}

	inline void SetY(float y)
	{
		m_fY = y;
	}

	inline void SetDirection(int Direction)
	{
		m_iDirection = Direction;
	}
	inline void TankStatePlus()
	{
		if (m_State == STATE00)
			m_State = STATE01;

		else if (m_State == STATE01)
			m_State = STATE00;

	}
	
	
	
	Tank();
	~Tank();
};

