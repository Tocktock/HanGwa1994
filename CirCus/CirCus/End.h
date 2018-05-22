#pragma once
#include<Windows.h>

#define ENDWIDTH 76
#define	ENDHEIGHT 49
class Bitmap;
class End
{

private:
	int m_iX;
	int m_iY;
	Bitmap * m_Image;
	RECT m_myRt;
public:
	void Init(Bitmap *Image, int x, int y);
	void Draw(HDC hdc);
	void SetmyRect();
	inline void SetX(int x)
	{
		m_iX -= x;
	}

	inline RECT GetFirstCollisionRect()
	{
		return m_myRt;
	}
	inline int GetX()
	{
		return m_iX;
	}
	inline int GetY()
	{
		return m_iY;
	}
	inline void MovingLeft()
	{
		m_iX += 2;
	}

	inline void MoveRight()
	{
		m_iX -= 2;
	}

	inline int GetFirstCollisionRtLeft()
	{
		return m_myRt.left;
	}
	inline int GetFirstCollisionRtRight()
	{
		return m_myRt.right;
	}

	inline int GetFirstCollisionRtTop()
	{
		return m_myRt.top;
	}

	inline int GetFirstCollisionRtBottom()
	{
		return m_myRt.bottom;
	}

	End();
	~End();
};

