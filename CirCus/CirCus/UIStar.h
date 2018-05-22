#pragma once
#include "UI.h"
class UIStar :
	public UI
{
private:
	Bitmap *m_Image[3];
	int m_firstX;
	int m_firstY;
public:


	void Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, int x, int y, int objX, int objY, int whatAmI);
	void Draw(HDC hdc);
	inline int GetState()
	{
		return m_state;
	}

	inline void ResetXY()
	{
		m_iX = m_firstX;
		m_iY = m_firstY;
	}

	inline void StatePlus()
	{
		m_state++;
	}
	inline void StateSet(int Value)
	{
		m_state = Value;
	}
	inline void StarXPlus(int value)
	{
		m_iX += value;
	}
	inline void StarYPlus(int value)
	{
		m_iY += value;
	}
	inline void StarXLineSet(int value)
	{
		m_iX = value;
	}

	inline void StarYLineSet(int value)
	{
		m_iY = value;
	}
	UIStar();
	virtual ~UIStar();
};

