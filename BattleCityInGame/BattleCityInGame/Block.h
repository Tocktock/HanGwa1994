#pragma once
#include<Windows.h>


#define BLOCKWIDTH 33
#define BLOCKHEIGHT 25

enum BLOCK_TYPE
{
	BLOCK_BRICK_01,
	BLOCK_BRICK_02,
	BLOCK_BRICK_03,
	BLOCK_BRICK_04,
	BLOCK_BRICK_05,
	BLOCK_SLIDE,
	BLOCK_FOREST,
	BLOCK_SEA,
	BLOCK_STEEL_ALL,
	BLOCK_STEEL_UP,
	BLOCK_STEEL_LEFT,
	BLOCK_STEEL_BOTTOM,
	BLOCK_STEEL_RIGHT,
	BLOCK_EAGLE,
	BLOCK_SURREND,

	BLOCK_TYPE_END

};

enum BRICKCOLLISIONDIRECTION
{
	BRICKCOLLISIONDIRECTION_UP,
	BRICKCOLLISIONDIRECTION_LEFT,
	BRICKCOLLISIONDIRECTION_RIGHT,
	BRICKCOLLISIONDIRECTION_DOWN,
	BRICKCOLLISIONDIRECTION_END
};
class Bitmap;
class Block
{
protected:
	int m_iX;
	int m_iY;
	int m_WhatAmI;
	Bitmap * m_BlockImage[BLOCK_TYPE_END];
	RECT m_CollisionRt;
	RECT m_CollisionRT_Brick[BRICKCOLLISIONDIRECTION_END];
	bool m_bCollisionFlag;
	int m_iHitDirection_Brick;
public:

	void Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5,
		Bitmap * Image6, Bitmap * Image7, Bitmap * Image8, Bitmap * Image9, Bitmap * Image10
		, Bitmap * Image11, Bitmap * Image12, Bitmap * Image13, Bitmap * Image14, Bitmap * Image15,int x, int y, int WhatAmI);
	void Draw(HDC hdc);
	void DestroyBlock();
	void SetRt();								///////////////블록 종류별로 Rt를 다르게 짜야함.
	bool CollisionCheck_First(RECT rt);
	bool CollisionCheck_Brick(RECT rt);
	void DestroyBrick();

	inline void SetWhatAmI(int value)
	{
		m_WhatAmI = value;
	}
	inline RECT GetRt()
	{ 
		return m_CollisionRt;
	}

	inline int GetWhatAmI()
	{
		return m_WhatAmI;
	}

	inline int GetX()
	{
		return m_iX;
	}

	inline int GetY()
	{
		return m_iY;
	}

	Block();
	virtual ~Block();
};

