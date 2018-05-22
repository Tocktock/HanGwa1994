#include "UI.h"
#include "Bitmap.h"



UI::UI()
{
}


UI::~UI()
{
}

void UI::Init(Bitmap * Image, int x, int y,int objX,int objY,int WhatAmI)
{
	m_iX = x;
	m_iY = y;
	m_objX = objX;
	m_objY = objY;
	m_Image = Image;
	m_whatAmI = WhatAmI;
}

void UI::Draw(HDC hdc)
{
	m_Image->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
}

