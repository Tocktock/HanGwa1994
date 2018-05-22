#pragma once
#include <Windows.h>
#include <string>

enum ANCHOR_DIRECTION
{
	ANCHOR_LT,
	ANCHOR_TOP,
	ANCHOR_RT,
	ANCHOR_LEFT,
	ANCHOR_CENTER,
	ANCHOR_RIGHT,
	ANCHOR_LB,
	ANCHOR_BOTTOM,
	ANCHOR_RB,
	ANCHOR_END
};

struct POINTF
{
	float x;
	float y;
};

using namespace std;
class Bitmap
{

	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBit;
	SIZE m_size;
	POINTF m_ptAnchor;
public:

	void Init(HDC hdc, string Filename);
	void InitBack(HDC hdc, int x, int y);
	void TestDraw(HDC hdc, int x, int y);
	void Draw(int x, int y);
	void DrawBack(HDC hdc);
	void AdjustDrawSize(int x, int y, int AdjustX, int AdjustY);
	void AdjustAnchor(int &x, int &y);
	void SetAnchor(int Anchor);

	inline HDC GetDC()
	{
		return m_hMemDC;
	}

	Bitmap();
	~Bitmap();
};

