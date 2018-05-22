#pragma once
#include "Tank.h"



enum ENEMYTANKSTATE_TYPE
{
	ENEMYTANK_UP00,
	ENEMYTANK_UP01,
	ENEMYTANK_UP02,
	ENEMYTANK_LEFT00,
	ENEMYTANK_LEFT01,
	ENEMYTANK_LEFT02,
	ENEMYTANK_RIGHT00,
	ENEMYTANK_RIGHT01,
	ENEMYTANK_RIGHT02,
	ENEMYTANK_DOWN00,
	ENEMYTANK_DOWN01,
	ENEMYTANK_DOWN02,
	ENEMYTANK_END
};

class Bitmap;
class EnemyTank :
	public Tank
{
	float m_fElapse_AutoMove_EnemyTank;
	float m_fElapse_AutoFire;
	float m_fElapse_AutoMove;
	Bitmap *m_pBitmapEnemyTank[ENEMYTANK_END];
	int m_iNextMoveDirection;

public:
	void Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, Bitmap *Image4, Bitmap *Image5, 
		Bitmap *Image6, Bitmap *Image7, Bitmap *Image8, Bitmap *Image9,
		Bitmap *Image10, Bitmap *Image11, Bitmap *Image12,float x,float y,bool team );
	void Draw(HDC hdc);
	void AutoMoving(float Elpase,int FormorDirection, bool CollisionFlag);
	void AutoMoving_Change_Direction();
	void CreateEnemy();
	void Reset_EnemyTank();
	inline void DestroyTank()
	{
 		m_fX = 1200;
		m_fY = 1200;
		SetRt();
		m_bDeadFlag = true;
	}
	inline void fElapsePlus_AutoMove(float Elapse)
	{
		m_fElapse_AutoMove_EnemyTank += Elapse;
	}
	inline void Set_fElapse_AutoMove(float Value)
	{
		m_fElapse_AutoMove_EnemyTank = Value;
	}
	inline float GetfElapse_AutoMove()
	{
		return m_fElapse_AutoMove_EnemyTank;
	}

	inline void fElapsePlus_AutoFire(float Elapse)
	{
		m_fElapse_AutoFire += Elapse;
	}
	inline void Set_fElapse_AutoFire(float Value)
	{
		m_fElapse_AutoFire = Value;
	}
	inline float GetfElapse_AutoFire()
	{
		return m_fElapse_AutoFire;
	}
	inline void SetNextDirection(int Value)
	{
		m_iNextMoveDirection = Value;
	}
	inline int GetNextMoveDirection()
	{
		return m_iNextMoveDirection;
	}

	EnemyTank();
	virtual ~EnemyTank();
};

