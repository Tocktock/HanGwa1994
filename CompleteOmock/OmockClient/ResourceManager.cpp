#include "ResourceManager.h"
#include "Bitmap.h"
#include "Macro.h"

ResourceManager* ResourceManager::s_this = NULL;
ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::InitBack(HDC hdc, int cx, int cy)
{
	m_pback = new Bitmap();
	m_pback->InitBack(hdc, cx, cy);
	TestHDC = hdc;
}
void ResourceManager::DrawBack(HDC hdc)
{
	m_pback->DrawBack(hdc);
}


Bitmap* ResourceManager::InitBitmap(string Filename)
{
	Bitmap * pBitmap = new Bitmap();
	pBitmap->Init(m_pback->GetDC(), Filename);
	m_mapBitmap.insert(make_pair(Filename, pBitmap));

	auto iter = m_mapBitmap.find(Filename);
	return iter->second;
}
Bitmap * ResourceManager::GetBitmap(string Filename)
{
	auto iter = m_mapBitmap.find(Filename);

	if (iter != m_mapBitmap.end())
		return iter->second;

	else
		return InitBitmap(Filename);

}

void ResourceManager::Release()
{
	SAFE_DELETE(m_pback);

	for (auto iter = m_mapBitmap.begin(); iter != m_mapBitmap.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}

}