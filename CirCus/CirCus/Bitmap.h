#pragma once
#include<Windows.h>
#include<string>

#define CHARACTER_WIDTH 66
#define CHARACTER_HEIGHT 63

#define BACK_WIDTH	67
#define BACK_HEIGHT	183

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
	void Draw(HDC hdc, int x, int y,int objWidth,int objHeight);
	void BackDraw(HDC hdc, int x, int y);

	Bitmap();
	~Bitmap();
};

