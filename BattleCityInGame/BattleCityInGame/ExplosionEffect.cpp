#include "ExplosionEffect.h"
#include "Bitmap.h"


ExplosionEffect::ExplosionEffect()
{
	m_iExplosiontype = EXPLOSION_BULLET;
}


ExplosionEffect::~ExplosionEffect()
{
}


void ExplosionEffect::Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3,Bitmap * Image4, Bitmap * Image5)
{
	m_iWhatExlosion = EXPLOSION_BULLET00;
	m_pBitmapImage[EXPLOSION_BULLET00] = Image1;
	m_pBitmapImage[EXPLOSION_BULLET01] = Image2;
	m_pBitmapImage[EXPLOSION_BULLET02] = Image3;
	m_pBitmapImage[EXPLOSION_TANK00] = Image4;
	m_pBitmapImage[EXPLOSION_TANK01] = Image5;

	m_bSwitchFlag = false;
	m_Count_Elapse = 0; //ÃÊ±âÈ­
}

void ExplosionEffect::Draw(HDC hdc,float Elapse)
{
	if (m_bSwitchFlag == false)
		return;

	else
	{
		m_Count_Elapse += Elapse;
		if (m_iWhatExlosion == EXPLOSION_BULLET00)
		{
			
			m_pBitmapImage[EXPLOSION_BULLET00]->Draw(hdc,m_fX,m_fY, EXPLOSION_BULLET_WIDTH, EXPLOSION_BULLET_HEIGHT);
		
			if (m_Count_Elapse > 0.04f)
			{
				m_iWhatExlosion = EXPLOSION_BULLET01;
				
				m_Count_Elapse = 0;
			}
		}

		else if (m_iWhatExlosion == EXPLOSION_BULLET01)
		{
			
			m_pBitmapImage[EXPLOSION_BULLET01]->Draw(hdc, m_fX, m_fY, EXPLOSION_BULLET_WIDTH, EXPLOSION_BULLET_HEIGHT);
			
			if (m_Count_Elapse > 0.04f)
			{
				m_iWhatExlosion = EXPLOSION_BULLET02;
				
				m_Count_Elapse = 0;
			}
		}

		else if (m_iWhatExlosion == EXPLOSION_BULLET02)
		{
			m_pBitmapImage[EXPLOSION_BULLET02]->Draw(hdc, m_fX, m_fY, EXPLOSION_BULLET_WIDTH, EXPLOSION_BULLET_HEIGHT);

			if (m_Count_Elapse > 0.04f && m_iExplosiontype == EXPLOSION_BULLET)
			{
				m_iWhatExlosion = EXPLOSION_BULLET00;
				m_Count_Elapse = 0;
				m_bSwitchFlag = false;

			}

			else if(m_Count_Elapse > 0.04f && m_iExplosiontype == EXPLOSION_TANK)
			{
				m_iWhatExlosion = EXPLOSION_TANK00;
				m_Count_Elapse = 0;
			}

		}
		else if (m_iWhatExlosion == EXPLOSION_TANK00)
		{
			m_pBitmapImage[EXPLOSION_TANK00]->Draw(hdc, m_fX-17, m_fY-12, EXPLOSION_TANK00_WIDTH, EXPLOSION_TANK00_HEIGHT);

			if (m_Count_Elapse > 0.08f)
			{
				m_iWhatExlosion = EXPLOSION_TANK01;
				m_Count_Elapse = 0;
			}
		}

		else if (m_iWhatExlosion == EXPLOSION_TANK01)
		{
			m_pBitmapImage[EXPLOSION_TANK00]->Draw(hdc, m_fX -19, m_fY-15, EXPLOSION_TANK01_WIDTH, EXPLOSION_TANK01_HEIGHT);

			if (m_Count_Elapse > 0.12f)
			{
				m_iWhatExlosion = EXPLOSION_BULLET00;
				m_Count_Elapse = 0;
				m_bSwitchFlag = false;
			}
		}
	}


}
