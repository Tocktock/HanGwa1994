#include "ResourceManager.h"
#include "Bitmap.h"
#include "Macro.h"


ResourceManager* ResourceManager::m_this = NULL;


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{

}


void ResourceManager::Init(HWND hWnd)
{

	HDC hdc = GetDC(hWnd);

	for (int i = 0; i < RES_TYPE_END; i++)
		m_arrBitmap[i] = new Bitmap();

	for (int i = RES_TYPE_BLOCK00; i <= RES_TYPE_BLOCK14; i++)
	{
		char szFileName[256];
		sprintf(szFileName, "RES\\block%.2d.bmp", i);
		m_arrBitmap[i]->init(hdc, szFileName);
	}

	m_arrBitmap[RES_TYPE_TANK_DOWN00]->init(hdc, "RES\\tank_down_00.bmp");
	m_arrBitmap[RES_TYPE_TANK_DOWN01]->init(hdc, "RES\\tank_down_01.bmp");
	m_arrBitmap[RES_TYPE_TANK_UP00]->init(hdc, "RES\\tank_up_00.bmp");
	m_arrBitmap[RES_TYPE_TANK_UP01]->init(hdc, "RES\\tank_up_01.bmp");
	m_arrBitmap[RES_TYPE_TANK_RIGHT00]->init(hdc, "RES\\tank_right_00.bmp");
	m_arrBitmap[RES_TYPE_TANK_RIGHT01]->init(hdc, "RES\\tank_right_01.bmp");
	m_arrBitmap[RES_TYPE_TANK_LEFT00]->init(hdc, "RES\\tank_left_00.bmp");
	m_arrBitmap[RES_TYPE_TANK_LEFT01]->init(hdc, "RES\\tank_left_01.bmp");
	
	m_arrBitmap[RES_TYPE_ETANK_DOWN00]->init(hdc, "RES\\e_down_00.bmp");
	m_arrBitmap[RES_TYPE_ETANK_DOWN01]->init(hdc, "RES\\e_down_01.bmp");
	m_arrBitmap[RES_TYPE_ETANK_DOWN02]->init(hdc, "RES\\e_down_02.bmp");
	m_arrBitmap[RES_TYPE_ETANK_LEFT00]->init(hdc, "RES\\e_left_00.bmp");
	m_arrBitmap[RES_TYPE_ETANK_LEFT01]->init(hdc, "RES\\e_left_01.bmp");
	m_arrBitmap[RES_TYPE_ETANK_LEFT02]->init(hdc, "RES\\e_left_02.bmp");
	m_arrBitmap[RES_TYPE_ETANK_RIGHT00]->init(hdc, "RES\\e_right_00.bmp");
	m_arrBitmap[RES_TYPE_ETANK_RIGHT01]->init(hdc, "RES\\e_right_01.bmp");
	m_arrBitmap[RES_TYPE_ETANK_RIGHT02]->init(hdc, "RES\\e_right_02.bmp");
	m_arrBitmap[RES_TYPE_ETANK_UP00]->init(hdc, "RES\\e_up_00.bmp");
	m_arrBitmap[RES_TYPE_ETANK_UP01]->init(hdc, "RES\\e_up_01.bmp");
	m_arrBitmap[RES_TYPE_ETANK_UP02]->init(hdc, "RES\\e_up_02.bmp");

	m_arrBitmap[RES_TYPE_ENEMYICON]->init(hdc, "RES\\enemy_icon.bmp");


	m_arrBitmap[RES_TYPE_EXPLOSION0]->init(hdc, "RES\\explosion_00.bmp");
	m_arrBitmap[RES_TYPE_EXPLOSION1]->init(hdc, "RES\\explosion_01.bmp");
	m_arrBitmap[RES_TYPE_EXPLOSION2]->init(hdc, "RES\\explosion_02.bmp");
	m_arrBitmap[RES_TYPE_EXPLOSION3]->init(hdc, "RES\\explosion_03.bmp");
	m_arrBitmap[RES_TYPE_EXPLOSION4]->init(hdc, "RES\\explosion_04.bmp");

	m_arrBitmap[RES_TYPE_MISSILE]->init(hdc, "RES\\missile_00.bmp");

	m_arrBitmap[RES_TYPE_PLAYERICON]->init(hdc, "RES\\player_icon.bmp");

	m_arrBitmap[RES_TYPE_SHIELD00]->init(hdc, "RES\\shield_00.bmp");
	m_arrBitmap[RES_TYPE_SHIELD01]->init(hdc, "RES\\shield_01.bmp");
	m_arrBitmap[RES_TYPE_STAGEICON]->init(hdc, "RES\\stage_icon.bmp");

	
	ReleaseDC(hWnd, hdc);
}

Bitmap* ResourceManager::GetImage(RES_TYPE type)
{
	if (type > RES_TYPE_END || type < 0)
	{
		MessageBox(NULL, "이미지가 없습니다!!", "ERROR MESSAGE", MB_ICONERROR);
		return NULL;
	}

	return m_arrBitmap[type];
}
