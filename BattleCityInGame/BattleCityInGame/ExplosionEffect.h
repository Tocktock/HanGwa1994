#pragma once
#include<Windows.h>

#define EXPLOSION_BULLET_WIDTH 33
#define EXPLOSION_BULLET_HEIGHT 25

#define EXPLOSION_TANK00_WIDTH 68
#define EXPLOSION_TANK00_HEIGHT 49

#define EXPLOSION_TANK01_WIDTH 70
#define EXPLOSION_TANK01_HEIGHT 55
enum EXPLOSION_STATE
{
	EXPLOSION_BULLET00,
	EXPLOSION_BULLET01,
	EXPLOSION_BULLET02,
	EXPLOSION_TANK00,
	EXPLOSION_TANK01,
	EXPLOSION_STATE_END
};

enum EXPLOSION_TYPE
{
	EXPLOSION_BULLET,
	EXPLOSION_TANK,
	EXPLOSION_TYPE_END
};

class Bitmap;
class ExplosionEffect
{

protected:
	float m_fX;
	float m_fY;
	int m_iWhatExlosion;
	int m_iState;
	int m_iExplosiontype;
	bool m_bSwitchFlag;
	float m_Count_Elapse;
	Bitmap *m_pBitmapImage[EXPLOSION_STATE_END];

public:

	void Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3,Bitmap * Image4, Bitmap * Image5);
	void Draw(HDC hdc,float Elapse);

	inline void SetXY(int x,int y)
	{
		m_fX = x - 12;
		m_fY = y -12;
	}

	inline void SetExplosionType(int x)
	{
		m_iExplosiontype = x;
	}

	inline bool GetSwitchFlag()
	{
		return m_bSwitchFlag;
	}
	inline void SwitchFlagOn()
	{
		m_bSwitchFlag = true;

	}
	inline void SwitchFlagOff()
	{
		m_bSwitchFlag = false;
	}

	ExplosionEffect();
	~ExplosionEffect();
};

