#include "End.h"
#include"Bitmap.h"


End::End()
{
}


End::~End()
{
}

void End::Init(Bitmap * Image,int x,int y)
{
	m_iX = x;
	m_iY = y;
	m_Image = Image;

}

void End::Draw(HDC hdc)
{
	m_Image->Draw(hdc, m_iX, m_iY, ENDWIDTH, ENDHEIGHT);
}

void End::SetmyRect()
{
	m_myRt.left = m_iX;
	m_myRt.right = m_iX + 76;
	m_myRt.top = m_iY + 0;
	m_myRt.bottom = m_iY + 49;
}