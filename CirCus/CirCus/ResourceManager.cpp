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


void ResourceManager:: Init(HWND hWnd)
{

	HDC hdc = GetDC(hWnd);

	for (int i = 0; i < RES_TYPE_END; i++)
		m_arrBitmap[i] = new Bitmap();

	m_arrBitmap[RES_TYPE_BACK]->init(hdc, "CIRCUS_RES\\back.bmp");
	m_arrBitmap[RES_TYPE_BACKDECO]->init(hdc, "CIRCUS_RES\\back_deco.bmp");
	m_arrBitmap[RES_TYPE_BACKNORMAL1]->init(hdc, "CIRCUS_RES\\back_normal.bmp");
	m_arrBitmap[RES_TYPE_BACKNORMAL2]->init(hdc, "CIRCUS_RES\\back_normal2.bmp");
	m_arrBitmap[RES_TYPE_PLAYER0]->init(hdc, "CIRCUS_RES\\player0.bmp");
	m_arrBitmap[RES_TYPE_PLAYER1]->init(hdc, "CIRCUS_RES\\player1.bmp");
	m_arrBitmap[RES_TYPE_PLAYER2]->init(hdc, "CIRCUS_RES\\player2.bmp");
	m_arrBitmap[RES_TYPE_PLAYERWIN1]->init(hdc, "CIRCUS_RES\\win.bmp");
	m_arrBitmap[RES_TYPE_PLAYERWIN2]->init(hdc, "CIRCUS_RES\\win2.bmp");
	m_arrBitmap[RES_TYPE_PLAYERDIE]->init(hdc, "CIRCUS_RES\\die.bmp");
	m_arrBitmap[RES_TYPE_METER]->init(hdc, "CIRCUS_RES\\miter.bmp");
	m_arrBitmap[RES_TYPE_FRONT1]->init(hdc, "CIRCUS_RES\\front.bmp");
	m_arrBitmap[RES_TYPE_FRONT2]->init(hdc, "CIRCUS_RES\\front2.bmp");
	m_arrBitmap[RES_TYPE_CIRCLE1_B]->init(hdc, "CIRCUS_RES\\enemy_1b.bmp");
	m_arrBitmap[RES_TYPE_CIRCLE1_F]->init(hdc, "CIRCUS_RES\\enemy_1f.bmp");
	m_arrBitmap[RES_TYPE_CIRCLE2_B]->init(hdc, "CIRCUS_RES\\enemy_b.bmp");
	m_arrBitmap[RES_TYPE_CIRCLE2_F]->init(hdc, "CIRCUS_RES\\enemy_f.bmp");
	m_arrBitmap[RES_TYPE_UIBACK]->init(hdc, "CIRCUS_RES\\UI.bmp");
	m_arrBitmap[RES_TYPE_UISTAR1]->init(hdc, "CIRCUS_RES\\star.bmp");
	m_arrBitmap[RES_TYPE_UISTAR2]->init(hdc, "CIRCUS_RES\\star1.bmp");
	m_arrBitmap[RES_TYPE_UISTAR3]->init(hdc, "CIRCUS_RES\\star2.bmp");
	m_arrBitmap[RES_TYPE_ICON]->init(hdc, "CIRCUS_RES\\icon.bmp");
	m_arrBitmap[RES_TYPE_ENDCIRCLE]->init(hdc, "CIRCUS_RES\\end.bmp");
	m_arrBitmap[RES_TYPE_LUCKYCIRCLE1_B]->init(hdc,"CIRCUS_RES\\LuckyCircle1b.bmp");
	m_arrBitmap[RES_TYPE_LUCKYCIRCLE1_F]->init(hdc, "CIRCUS_RES\\LuckyCircle1f.bmp");
	m_arrBitmap[RES_TYPE_LUCKYCIRCLE2_B]->init(hdc, "CIRCUS_RES\\LuckyCircle2b.bmp");
	m_arrBitmap[RES_TYPE_LUCKYCIRCLE2_F]->init(hdc, "CIRCUS_RES\\LuckyCircle2f.bmp");
	m_arrBitmap[RES_TYPE_MONEY]->init(hdc, "CIRCUS_RES\\cash.bmp");




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
