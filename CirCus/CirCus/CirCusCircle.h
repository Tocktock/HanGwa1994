#pragma once
#include "Enemy.h"

#define CIRCLEWIDTH 25
#define CIRCLEHEIGHT 130
class CirCusCircle :
	public Enemy
{
public:

	void Init(Bitmap* Image1, Bitmap* Image2,Bitmap * Image3, Bitmap * Image4, int x, int y);
	void SetmyRt();
	void FrontDraw(HDC hdc);
	void BackDraw(HDC hdc);
	inline RECT GetFirstCollisionRect()
	{
		return m_myRt[0];
	}

	inline void CircleMoving()
	{
		m_iX -= 3;
	}


	inline void SetX(int x)
	{
		m_iX -= x;
	}


	CirCusCircle();
	virtual ~CirCusCircle();
};

