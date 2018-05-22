#pragma once
#include<Windows.h>
class Bitmap;
class UI
{
protected:

	int m_iX;
	int m_iY;
	int m_objX;
	int m_objY;
	int m_state;
	int m_stateCt;
	int m_whatAmI;
	Bitmap *m_Image;

public:

	void Init(Bitmap *Image,int x,int y,int objX,int objY,int whatAmI);
	
	void Draw(HDC hdc);

	inline void Xminus()
	{
		m_iX -= 20;
	}


	inline void SetX(int x)
	{
		m_iX -= x;
	}

	inline void XSet(int x)
	{
		m_iX = x;
	}

	inline int GetWhatAmI()
	{
		return m_whatAmI;
	}
	inline void StatePlus()
	{
		m_state++;
	}

	UI();
	~UI();
};

