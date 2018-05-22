#include "Back.h"
#include "Bitmap.h"
#include <string>


Back::Back()
{
	m_isFirst = true;
}


Back::~Back()
{
}

void Back::Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, Bitmap *Image4, Bitmap *Image5, int x, int y,int whatAmI,int objX,int objY)
{
	m_iX = x;
	m_iY = y;
	m_Image[0] = Image1;
	m_Image[1] = Image2;
	m_Image[2] = Image3;
	m_Image[3] = Image4;
	m_Image[4] = Image5;
	m_whatAmI = whatAmI;
	m_objX = objX;
	m_objY = objY;
	m_MeterValue = 100;
}
void Back::Draw(HDC hdc)
{
	if (m_whatAmI == 0)
	{
		m_Image[0]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
	
	else if (m_whatAmI == 1)
	{
		m_Image[1]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
	else if (m_whatAmI == 2)
	{
		m_Image[2]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
	else if (m_whatAmI == 3)
	{
		m_Image[3]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
	else if (m_whatAmI == 4)
	{
		m_Image[4]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
}
