#include "ElseEffect.h"
#include "Bitmap.h"
#include "Tank.h"

ElseEffect::ElseEffect()
{
}


ElseEffect::~ElseEffect()
{
}
void ElseEffect::Init(Bitmap *Image1, Bitmap *Image2)
{
	m_pBitmapImage[ELSEEFFECT_NORMAL] = Image1;
	m_pBitmapImage[ELSEEFFECT_CROSS] = Image2;
	m_iEffectCt = 0;
	m_bSwitchFlag = false;
	m_iWhatExlosion = ELSEEFFECT_NORMAL;
	m_Count_Elapse = 0; //√ ±‚»≠
}

void ElseEffect::Draw(HDC hdc, float Elapse)
{
	if (m_iEffectCt<300 && m_bSwitchFlag == true)
	{
		m_Count_Elapse += Elapse;
		if (m_iWhatExlosion == ELSEEFFECT_NORMAL)
		{
			m_iEffectCt++;
			m_pBitmapImage[ELSEEFFECT_NORMAL]->Draw(hdc, m_pTank_ElseEffect->GetX(), m_pTank_ElseEffect->GetY(), ELSEEFFECT_WIDTH, ELSEEFFECT_HEGIHT);

			if (m_Count_Elapse > 0.004f)
			{
				m_iWhatExlosion = ELSEEFFECT_NORMAL;
				m_iWhatExlosion = ELSEEFFECT_CROSS;
				m_Count_Elapse = 0;
			}
		}

		else if (m_iWhatExlosion == ELSEEFFECT_CROSS)
		{

			m_pBitmapImage[ELSEEFFECT_CROSS]->Draw(hdc, m_pTank_ElseEffect->GetX(), m_pTank_ElseEffect->GetY(), ELSEEFFECT_WIDTH, ELSEEFFECT_HEGIHT);

			if (m_Count_Elapse > 0.004f)
			{
				m_iWhatExlosion = ELSEEFFECT_CROSS;
				m_iWhatExlosion = ELSEEFFECT_NORMAL;
				m_Count_Elapse = 0;
			}
		}

	}
	else if (m_iEffectCt >= 300)
	{
		m_iEffectCt = 0; 
		m_bSwitchFlag = false;
	}

}