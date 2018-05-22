#include "UIStar.h"
#include "Bitmap.h"


UIStar::UIStar()
{
	
}


UIStar::~UIStar()
{
}


void UIStar::Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, int x, int y, int objX, int objY, int whatAmI)
{
	m_Image[0] = Image1;
	m_Image[1] = Image2;
	m_Image[2] = Image3;
	m_iX = x;
	m_iY = y;
	m_whatAmI = whatAmI;
	m_objX = objX;
	m_objY = objY;
	m_firstX = x;
	m_firstY = y;
}
void UIStar::Draw(HDC hdc)
{
	if (m_state == 1)
	{
		m_Image[0]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}

	else if (m_state == 2)
	{
		m_Image[1]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}

	else if (m_state == 3)
	{
		m_Image[2]->Draw(hdc, m_iX, m_iY, m_objX, m_objY);
	}
}