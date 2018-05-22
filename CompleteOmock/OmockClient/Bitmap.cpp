#include "Bitmap.h"
#include "ResourceManager.h"


Bitmap::Bitmap()
{
	m_ptAnchor.x = 0;
	m_ptAnchor.y = 0;
}


Bitmap::~Bitmap()
{
	SelectObject(m_hMemDC, m_hOldBit);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}


void Bitmap::Init(HDC hdc, string Filename)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = (HBITMAP)LoadImage(NULL, Filename.c_str(), IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (m_hBitmap == NULL)
	{
		MessageBox(NULL, Filename.c_str(), "File Not Find", MB_OK);
		return;
	}

	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	BITMAP bmp_info;
	GetObject(m_hBitmap, sizeof(bmp_info), &bmp_info);
	m_size.cx = (int)bmp_info.bmWidth;
	m_size.cy = (int)bmp_info.bmHeight;
}

void Bitmap::InitBack(HDC hdc, int x, int y)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, x, y);
	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	m_size.cx = x;
	m_size.cy = y;

}

void Bitmap::Draw(int x, int y)
{
	AdjustAnchor(x, y);
	TransparentBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}

void Bitmap::TestDraw(HDC hdc,int x, int y)
{
	AdjustAnchor(x, y);
	TransparentBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}
void Bitmap::DrawBack(HDC hdc)
{
	BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
}

void Bitmap::AdjustDrawSize(int x, int y, int AdjustX, int AdjustY)
{
	AdjustAnchor(x, y);
	TransparentBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, AdjustX, AdjustY, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}

void Bitmap::AdjustAnchor(int &x, int &y)
{
	x -= m_size.cx * m_ptAnchor.x;
	y -= m_size.cy * m_ptAnchor.y;
}

void Bitmap::SetAnchor(int Anchor)
{
	switch (Anchor)
	{
	case ANCHOR_LT:
		m_ptAnchor.x = 0.0f;
		m_ptAnchor.y = 0.0f;
		break;
	case ANCHOR_TOP:
		m_ptAnchor.x = 0.5f;
		m_ptAnchor.y = 0.0f;
		break;
	case ANCHOR_RT:
		m_ptAnchor.x = 1.0f;
		m_ptAnchor.y = 0.0f;
		break;
	case ANCHOR_LEFT:
		m_ptAnchor.x = 0.0f;
		m_ptAnchor.y = 0.5f;
		break;
	case ANCHOR_CENTER:
		m_ptAnchor.x = 0.5f;
		m_ptAnchor.y = 0.5f;
		break;
	case ANCHOR_RIGHT:
		m_ptAnchor.x = 1.0f;
		m_ptAnchor.y = 0.5f;
		break;
	case ANCHOR_LB:
		m_ptAnchor.x = 0.0f;
		m_ptAnchor.y = 1.0f;
		break;
	case ANCHOR_BOTTOM:
		m_ptAnchor.x = 0.5f;
		m_ptAnchor.y = 1.0f;
		break;
	case ANCHOR_RB:
		m_ptAnchor.x = 1.0f;
		m_ptAnchor.y = 1.0f;
		break;
	}
}