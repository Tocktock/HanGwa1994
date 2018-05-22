#pragma once
#include<Windows.h>
#include<string>

using namespace std;

class Bitmap
{
private:

	HDC		m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldBit;
	SIZE	m_size;

public:

	void init(HDC hdc, string file_name);
	void Draw(HDC hdc, int x, int y, int objWidth, int objHeight);
	void BackDraw(HDC hdc, int x, int y, int objWidth, int objHeight);

	Bitmap();
	~Bitmap();
};

