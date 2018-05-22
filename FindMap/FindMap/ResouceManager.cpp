#include "ResouceManager.h"
#include "Macro.h"
#include "Bitmap.h"

ResouceManager* ResouceManager::m_this = NULL;

ResouceManager::ResouceManager()
{
}


ResouceManager::~ResouceManager()
{
	for (int i = 0; i < RES_TYPE_END; i++)
		SAFE_DELETE(m_arrBitmap[i]);

	SAFE_DELETE(m_this);
}

void ResouceManager::Init(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	for (int i = 0; i < RES_TYPE_END; i++)
		m_arrBitmap[i] = new Bitmap();

	m_arrBitmap[RES_TYPE_BACK]->init(hdc, "RES\\back.bmp");
	m_arrBitmap[RES_TYPE_BLOCK]->init(hdc, "RES\\block.bmp");

	m_arrBitmap[RES_TYPE_FLAG]->init(hdc, "RES\\flag.bmp");
	m_arrBitmap[RES_TYPE_MINE]->init(hdc, "RES\\mine.bmp");

	for (int i = RES_TYPE_BLOCK_0; i <= RES_TYPE_BLOCK_8; i++)
	{
		char szFileName[256];
		sprintf(szFileName, "RES\\block_%d.bmp", i - 2);
		
		m_arrBitmap[i]->init(hdc, szFileName);
	}
	

	

	ReleaseDC(hWnd, hdc);
}

Bitmap* ResouceManager::GetImage(RES_TYPE type)
{
	if (type < 0 || type > RES_TYPE_END)
	{
		MessageBox(NULL, "이미지가 없습니다.", "에러", MB_ICONERROR);
		return NULL;
	}

	return m_arrBitmap[type];
}
