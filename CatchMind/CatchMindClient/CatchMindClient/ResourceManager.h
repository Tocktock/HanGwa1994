#pragma once
#include <Windows.h>
#include <map>
#include "Bitmap.h"
#include "ResourceManager.h"
using namespace std;

class Bitmap;
class ResourceManager
{
	Bitmap * m_pback;
	map<string, Bitmap*> m_mapBitmap;

	static ResourceManager* s_this;

	Bitmap* InitBitmap(string Filename);

	ResourceManager();

public:
	void DrawBack(HDC hdc);
	static ResourceManager* GetInstance()
	{
		if (s_this == NULL)
			s_this = new ResourceManager();

		return s_this;
	}

	
	Bitmap* GetBitmap(string FileName);
	void InitBack(HDC hdc, int x, int y);
	void Release();

	inline HDC GetBackDC()
	{
		return m_pback->GetDC();
	}
	~ResourceManager();
};

