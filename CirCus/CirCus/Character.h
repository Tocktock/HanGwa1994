#pragma once
#include<Windows.h>

#define CHARACTER_WIDTH 66
#define CHARACTER_HEIGHT 63
#define pi 3.141592

class Bitmap;
class Character
{
private:

	Bitmap *m_player[6];
	int m_Characterstate;
	int m_iX;
	int m_iY;
	int m_CenterX;
	int m_CenterY;
	RECT m_myRt[2];

public:
	void CharacterInit(Bitmap *m_player1, Bitmap *m_player2, Bitmap *m_player3,
		Bitmap *m_player4, Bitmap *m_player5, Bitmap *m_player6,int x,int y);

	void Draw(HDC hdc);
	void Moving();
	void LeftMoving();
	void Jumping(float Seta);
	void SetmyRect();

	inline void ReSetCharacterState()
	{
		m_Characterstate = 0;
	}

	inline void GameWin(int x,int y)
	{
		m_iX = x;
		m_iY = y;
	}
	inline RECT GetFirstCollisionRect()
	{
		return m_myRt[0];
	}

	inline int GetNowX()
	{
		return m_iX;
	}

	inline RECT GetSecondCollisionRect()
	{
		return m_myRt[1];
	}

	inline void GameEnd()
	{
		m_Characterstate = 5;
	}

	inline void GameWinFirst()
	{
		m_Characterstate = 3;
	}
	inline void GameWinSecond()
	{
		m_Characterstate = 4;
	}


	inline int GetFirstCollisionRtLeft()
	{
		return m_myRt[0].left;
	}
	inline int GetFirstCollisionRtRight()
	{
		return m_myRt[0].right;
	}

	inline int GetFirstCollisionRtTop()
	{
		return m_myRt[0].top;
	}

	inline int GetFirstCollisionRtBottom()
	{
		return m_myRt[0].bottom;
	}

	inline int GetSecondCollisionRtLeft()
	{
		return m_myRt[1].left;
	}
	inline int GetSecondCollisionRtRight()
	{
		return m_myRt[1].right;
	}

	inline int GetSecondCollisionRtTop()
	{
		return m_myRt[1].top;
	}

	inline int GetSecondCollisionRtBottom()
	{
		return m_myRt[1].bottom;
	}

	inline void SetX(int x)
	{
		m_iX -= x;
	}

	inline void SetY(int x)
	{
		m_iY = x;
	}
	inline void JumpMotion()
	{

		if (m_Characterstate == 5)
			return;
		m_Characterstate = 2;
	}


	inline void SetCenterX()
	{
		m_CenterX = m_iX + 30;
	}

	inline void SetCenterY()
	{
		m_CenterY = m_iY;
	}

	inline void MoveLeft()
	{
		m_iX -= 2;
	}
	inline void MoveRight()
	{
		m_iX += 2;
	}
	inline void MoveSet()
	{
		m_Characterstate = 0;
	}
	inline int GetCharactertate()
	{
		return m_Characterstate;
	}
	
	Character();
	~Character();
};

