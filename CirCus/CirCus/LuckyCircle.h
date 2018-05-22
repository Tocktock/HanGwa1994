#pragma once
#include "CirCusCircle.h"
class LuckyCircle :
	public CirCusCircle
{
private:
	bool m_isEat;
public:

	void Init(Bitmap * Image1, Bitmap * Image2, Bitmap * Image3, Bitmap * Image4, Bitmap * Image5, int x, int y, int objX, int objY);
	void FrontDraw(HDC hdc);
	void BackDraw(HDC hdc);
	void MoneyDraw(HDC hdc);

	inline void Eaten()
	{
		m_isEat = true;
	}
	inline bool isEaten()
	{
		return m_isEat;
	}
	void SetmyRt();
	LuckyCircle();
	virtual~LuckyCircle();
};

