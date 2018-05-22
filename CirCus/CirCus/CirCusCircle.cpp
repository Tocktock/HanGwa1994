#include<Windows.h>
#include "CirCusCircle.h"
#include "Bitmap.h"


CirCusCircle::CirCusCircle()
{
	m_stateCt = 0;
}


CirCusCircle::~CirCusCircle()
{
}

void CirCusCircle::Init(Bitmap* Image1, Bitmap* Image2, Bitmap * Image3, Bitmap * Image4, int x, int y)
{
	m_Image[0] = Image1;
	m_Image[1] = Image2;
	m_Image[2] = Image3;
	m_Image[3] = Image4;
	m_iX = x;
	m_iY = y;
	
}


void CirCusCircle::FrontDraw(HDC hdc)
{
	if (m_state == true)
	{
		m_Image[0]->Draw(hdc, m_iX, m_iY, CIRCLEWIDTH, CIRCLEHEIGHT);
	

		if (m_stateCt > 10)
		{
			m_stateCt = 0;
			m_state = false;
		}
	}

	else if (m_state == false)
	{
		m_Image[2]->Draw(hdc, m_iX, m_iY, 26, 132);
	

		if (m_stateCt > 10)
		{
			m_stateCt = 0;
			m_state = true;
		}
	}
}


void CirCusCircle::BackDraw(HDC hdc)
{
	if (m_state == true)
	{
	
		m_Image[1]->Draw(hdc, m_iX + 25, m_iY, CIRCLEWIDTH, CIRCLEHEIGHT);

	
	}

	else if (m_state == false)
	{
		
		m_Image[3]->Draw(hdc, m_iX + 25, m_iY, 23, 132);

	
	}
}
void CirCusCircle::SetmyRt()
{
	m_myRt[0].left = m_iX +23;
	m_myRt[0].right = m_iX + 26;
	m_myRt[0].top = m_iY + 120;
	m_myRt[0].bottom = m_iY +130;
	
	m_myRt[2].left = m_iX + 13;
	m_myRt[2].right = m_iX + 17;
	m_myRt[2].top = m_iY + 10;
	m_myRt[2].bottom = m_iY + 120;

}