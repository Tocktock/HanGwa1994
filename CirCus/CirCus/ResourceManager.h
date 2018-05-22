#pragma once
#include <Windows.h>
#include "Macro.h"

enum RES_TYPE
{
	RES_TYPE_BACK,
	RES_TYPE_BACKDECO,
	RES_TYPE_BACKNORMAL1,
	RES_TYPE_BACKNORMAL2,
	RES_TYPE_PLAYER0,
	RES_TYPE_PLAYER1,
	RES_TYPE_PLAYER2,
	RES_TYPE_PLAYERWIN1,
	RES_TYPE_PLAYERWIN2,
	RES_TYPE_PLAYERDIE,
	RES_TYPE_METER,
	RES_TYPE_FRONT1,
	RES_TYPE_FRONT2,
	RES_TYPE_CIRCLE1_B,
	RES_TYPE_CIRCLE1_F,
	RES_TYPE_CIRCLE2_B,
	RES_TYPE_CIRCLE2_F,
	RES_TYPE_LUCKYCIRCLE1_B,
	RES_TYPE_LUCKYCIRCLE1_F,
	RES_TYPE_LUCKYCIRCLE2_B,
	RES_TYPE_LUCKYCIRCLE2_F,
	RES_TYPE_MONEY,
	RES_TYPE_ENDCIRCLE,
	RES_TYPE_UIBACK,
	RES_TYPE_UISTAR1,
	RES_TYPE_UISTAR2,
	RES_TYPE_UISTAR3,
	RES_TYPE_ICON,
	RES_TYPE_END
};

class Bitmap;
class ResourceManager
{
private:



	Bitmap* m_arrBitmap[RES_TYPE_END];

	static ResourceManager* m_this;
	ResourceManager();

public:

	static ResourceManager* GetInstance()
	{
		if (m_this == NULL)
		{
			m_this = new ResourceManager();
		}

		return m_this;
	}

	void Init(HWND hWnd);
	Bitmap* GetImage(RES_TYPE type);

	void Release()
	{
		SAFE_DELETE(m_this);
	}
	~ResourceManager();
};

