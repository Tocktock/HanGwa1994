#pragma once
#include <Windows.h>
#include <string>

using namespace std;

#define IMAGE_WIDTH 26
#define IMAGE_HEIGHT 26
#define MAPIMAGE_WIDTH 870
#define MAPIMAGE_HEIGHT 512

class Bitmap
{
private:

	HDC		m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldBit;

	SIZE	m_size;
public:

	void init(HDC hdc, string file_name);
	void Draw(HDC hdc, int x, int y);
	void BackDraw(HDC hdc, int x, int y);

	Bitmap();
	~Bitmap();
};

