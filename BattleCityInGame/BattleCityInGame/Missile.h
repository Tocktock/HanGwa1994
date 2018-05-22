#pragma once
#include <Windows.h>

#define MISSILEWIDTH 8
#define MISSILEHEIGHT 5

#define BLOCKWIDTH 33
#define BLOCKHEIGHT 25
#define MAPWIDTHNUM 14
#define MAPHEIGHTNUM 16
#define EMPTYBLOCK 1000



enum MISSILEDIRECTION
{
	MISSILEDIRECTION_DOWN,
	MISSILEDIRECTION_LEFT,
	MISSILEDIRECTION_RIGHT, 
	MISSILEDIRECTION_UP,
	MISSILEDIRECTION_END
};


class Bitmap;
class Missile
{
private:

	float m_fX;
	float m_fY;
	float m_Team;
	int m_iDirection;
	Bitmap * m_pBitmapMissile;
	RECT m_myRt;
	bool m_bOnflag;					//발사됬는지 안됬는지 설정.

public:

	void Init(Bitmap *Image, float x, float y, int Team);
	void Draw(HDC hdc);
	void FireMove(float ElapseValue);

	void SetXY(float x,float y);
	void SetRt();

	void SetDirection(int Direction);
	bool CheckCollision_First(RECT rt);
	
	inline RECT GetRt()
	{
		return m_myRt;
	}

	inline float GetX()
	{
		return m_fX;
	}

	inline float GetY()
	{
		return m_fY;
	}

	inline void FlagOn()
	{
		m_bOnflag = true;
	}

	inline bool GetFlag()
	{
		return m_bOnflag;
	}

	inline void FlagOff()
	{
		m_bOnflag = false;
		m_myRt.left = 1000;
		m_myRt.right= 1000;
		m_myRt.top = 1000;
		m_myRt.bottom = 1000;
	}

	Missile();
	~Missile();
};