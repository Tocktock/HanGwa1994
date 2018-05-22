#pragma once
#include <Windows.h>

enum RES_TYPE
{
	RES_TYPE_BLOCK_0,
	RES_TYPE_BLOCK_1,
	RES_TYPE_PAWN_B,
	RES_TYPE_KNIGHT_B,
	RES_TYPE_BISHOP_B,
	RES_TYPE_ROOK_B,
	RES_TYPE_QUEEN_B,
	RES_TYPE_KING_B,
	RES_TYPE_PAWN_W,
	RES_TYPE_KNIGHT_W,
	RES_TYPE_BISHOP_W,
	RES_TYPE_ROOK_W,
	RES_TYPE_QUEEN_W,
	RES_TYPE_KING_W,
	RES_TYPE_END
};

class Bitmap;
class ResouceManager
{
private:

	Bitmap* m_arrBitmap[RES_TYPE_END];

	static ResouceManager* m_this;
	ResouceManager();

public:

	static ResouceManager* GetInstance()
	{
		if (m_this == NULL)
		{
			m_this = new ResouceManager();
		}

		return m_this;
	}

	void Init(HWND hWnd);
	Bitmap* GetImage(RES_TYPE type);

	~ResouceManager();
};

