#include "LuckyCircle.h"
#include "Bitmap.h"


LuckyCircle::LuckyCircle()
{
	m_isEat = false;
}


LuckyCircle::~LuckyCircle()
{
}

void LuckyCircle::Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5,
	int x, int y, int objX, int objY)
{
	m_Image[0] = Image1;
	m_Image[1] = Image2;
	m_Image[2] = Image3;
	m_Image[3] = Image4;
	m_Image[4] = Image5;

	m_iX = x;
	m_iY = y;
	m_objX = objX;
	m_objY = objY;
}


void LuckyCircle::FrontDraw(HDC hdc)
{
	if (m_state == true)
	{
		m_Image[0]->Draw(hdc, m_iX, m_iY, 25, 104);


		if (m_stateCt > 10)
		{
			m_stateCt = 0;
			m_state = false;
		}
	}

	else if (m_state == false)
	{
		m_Image[2]->Draw(hdc, m_iX, m_iY, 25, 104);


		if (m_stateCt > 10)
		{
			m_stateCt = 0;
			m_state = true;
		}
	}
}


void LuckyCircle::BackDraw(HDC hdc)
{
	if (m_state == true)
	{

		m_Image[1]->Draw(hdc, m_iX + 25, m_iY, 25, 104);


	}

	else if (m_state == false)
	{

		m_Image[3]->Draw(hdc, m_iX + 25, m_iY, 25, 104);


	}
}
void LuckyCircle::SetmyRt()
{
	m_myRt[0].left = m_iX + 23;
	m_myRt[0].right = m_iX + 26;
	m_myRt[0].top = m_iY + 94;
	m_myRt[0].bottom = m_iY + 104;

	m_myRt[2].left = m_iX + 12;                                      //복주머니 컬리전
	m_myRt[2].right = m_iX + 37;
	m_myRt[2].top = m_iY + 10;
	m_myRt[2].bottom = m_iY + 36;
}

void LuckyCircle::MoneyDraw(HDC hdc)
{
	if (m_isEat == false)
		m_Image[4]->Draw(hdc, m_iX + 12, m_iY + 20, 25, 26);

	else
		return;
}