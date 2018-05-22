#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "Bitmap.h"
using namespace std;


class ResourceManager
{
private:

	Bitmap * m_pback;
	map<string, Bitmap*> m_mapBitmap;
	HDC TestHDC;
	Bitmap* InitBitmap(string Filename);
	static ResourceManager *s_this;
	ResourceManager();
public:
	static ResourceManager* GetInstance()
	{
		if (s_this == NULL)
		{
			s_this = new ResourceManager();
		}
		return s_this;
	}
	void InitBack(HDC hdc, int cx, int cy);
	void DrawBack(HDC hdc);
	Bitmap* GetBitmap(string Filename);

	void Release();

	inline HDC GetBackDC()
	{
		return m_pback->GetDC();
	}

	inline Bitmap* GetTest()
	{
		return m_pback;
	}
	~ResourceManager();
};

