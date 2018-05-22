#pragma once
#include <Windows.h>
#include "Macro.h"

enum RES_TYPE
{
	RES_TYPE_BACK,
	RES_TYPE_BLOCK,
	RES_TYPE_BLOCK_0,
	RES_TYPE_BLOCK_1,
	RES_TYPE_BLOCK_2,
	RES_TYPE_BLOCK_3,

	RES_TYPE_BLOCK_4,
	RES_TYPE_BLOCK_5,
	RES_TYPE_BLOCK_6,
	RES_TYPE_BLOCK_7,
	RES_TYPE_BLOCK_8, 
	RES_TYPE_FLAG,
	RES_TYPE_MINE,
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

	inline void ResetMemResource()
	{
		for (int i = 0; i < RES_TYPE_END; i++)
			SAFE_DELETE(m_arrBitmap[i]);
	}

	void Init(HWND hWnd);
	Bitmap* GetImage(RES_TYPE type);

	~ResouceManager();
};

