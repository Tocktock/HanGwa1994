#pragma once
#include "ExplosionEffect.h"

#define ELSEEFFECT_WIDTH 33
#define ELSEEFFECT_HEGIHT 25

enum ELSEEFFECT_TYPE
{
	ELSEEFFECT_NORMAL,
	ELSEEFFECT_CROSS,
	ELSEEFFECT_END
};
class Tank;
class ElseEffect :
	public ExplosionEffect
{
protected:

	Bitmap *m_pBitmapImage[ELSEEFFECT_END];
	Tank *m_pTank_ElseEffect;
	int m_iEffectCt;
public:

	void Init(Bitmap *Image1, Bitmap *Image2);
	void Draw(HDC hdc, float Elapse);

	inline void SetXY_ElseEffect(Tank * temp)
	{
		m_pTank_ElseEffect = temp;
	}
	inline void Reset_ElseEffect()
	{
		m_bSwitchFlag = false;
		m_iEffectCt = 0;
	}
	ElseEffect();
	virtual ~ElseEffect();
};

