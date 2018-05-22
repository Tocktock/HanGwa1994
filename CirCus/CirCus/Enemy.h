#pragma once
#include<Windows.h>
#define POTFLAMEX 50
#define POTFLAMEY 50
class Bitmap;
class Enemy
{
protected:

	Bitmap * m_Image[6];
	int m_iX;
	int m_iY;
	int m_stateCt;
	int m_objX;
	int m_objY;
	bool m_state;
	RECT m_myRt[3];

public:

	void CollisionCheck();
	void Init(Bitmap* Image1, Bitmap* Image2,int x,int y);
	void Draw(HDC hdc);
	void SetmyRt();
	
	inline int GetX()
	{
		return m_iX;
	}


	inline void SetX(int x)
	{
		m_iX -= x;
	}
	

	inline RECT GetFirstCollisionRect()
	{
		return m_myRt[0];
	}

	inline RECT GetSecondCollisionRect()
	{
		return m_myRt[1];
	}

	inline void MoveRight()
	{
		m_iX -= 2;
	}
	inline void MoveLeft()
	{
		m_iX += 2;
	}

	inline int GetFirstCollisionLeft()
	{
		return m_myRt[0].left;
	}
	inline int GetFirstCollisionRight()
	{
		return m_myRt[0].right;
	}

	inline int GetFirstCollisionTop()
	{
		return m_myRt[0].top;
	}

	inline int GetFirstCollisionBottom()
	{
		return m_myRt[0].bottom;
	}

	inline int GetSecondCollisionLeft()
	{
		return m_myRt[1].left;
	}
	inline int GetSecondCollisionRight()
	{
		return m_myRt[1].right;
	}

	inline int GetSecondCollisionTop()
	{
		return m_myRt[1].top;
	}

	inline int GetSecondCollisionBottom()
	{
		return m_myRt[1].bottom;
	}

	inline int GetScoreCollisionLeft()
	{
		return m_myRt[2].left;
	}
	inline int GetScoreCollisionRight()
	{
		return m_myRt[2].right;
	}

	inline int GetScoreCollisionTop()
	{
		return m_myRt[2].top;
	}

	inline int GetScoreCollisionBottom()
	{
		return m_myRt[2].bottom;
	}

	inline void stateCt()
	{
		m_stateCt++;
	}

	Enemy();
	virtual ~Enemy();

};

