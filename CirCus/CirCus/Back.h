#pragma once
#include <Windows.h>
#include <string>
class Bitmap;
class Back
{
private:
	int m_iX;
	int m_iY;
	int m_whatAmI;
	int m_objX;
	int m_objY;
	int m_MeterValue;
	bool m_isFirst;
	Bitmap * m_Image[5];
public:
	void Init(Bitmap *Image1, Bitmap *Image2, Bitmap *Image3, Bitmap *Image4, Bitmap *Image5, int x,int y,int whatAmI,int objX,int objY);
	void Draw(HDC hdc);

	inline void WinFirst()
	{
		m_whatAmI = 3;
	}
	inline void WinSecond()
	{
		m_whatAmI = 2;
	}

	inline void MeterValueReset()
	{
		m_MeterValue = 100;
	}

	inline int MeterValue()
	{
		return m_MeterValue;
	}

	inline void MeterValuePlus()
	{
		m_MeterValue -= 10;
	}
	inline int GetY()
	{
		return m_iY;
	}
	inline int GetWhatAmI()
	{
		return m_whatAmI;
	}

	inline int GetX()
	{
		return m_iX;
	}
	inline void MoveRight()
	{
		m_iX -= 2;
	}
	inline void MoveLeft()
	{
		m_iX += 2;
	}

	inline void SetX(int x)
	{
		m_iX -= x;
	}
	Back();
	~Back();
};

