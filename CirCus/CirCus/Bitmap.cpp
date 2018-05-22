#include "Bitmap.h"



Bitmap::Bitmap()
{
}


Bitmap::~Bitmap()
{
	SelectObject(m_hMemDC, m_hOldBit);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}


void Bitmap:: init(HDC hdc, string file_name)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitMap = (HBITMAP)LoadImage(NULL, file_name.c_str(), IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);


	BITMAP bmp_info;
	GetObject(m_hBitMap, sizeof(bmp_info), &bmp_info);
	m_size.cx = (int)bmp_info.bmWidth;
	m_size.cy = (int)bmp_info.bmHeight;
}
void Bitmap::Draw(HDC hdc, int x, int y,int objWidth,int objHeight)
{
	//TransparentBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, objWidth, objHeight, RGB(255, 0, 255));
	TransparentBlt(hdc, x, y, objWidth, objHeight, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));

}

void Bitmap::BackDraw(HDC hdc, int x, int y)
{
	TransparentBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, BACK_WIDTH, BACK_HEIGHT, RGB(255, 0, 255));

}