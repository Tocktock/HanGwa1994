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


void ResourceManager::DrawBack(HDC hdc)
{
	m_pback->DrawBack(hdc);
}

Bitmap* ResourceManager::GetBitmap(string FileName)
{
	for (auto iter = m_mapBitmap.begin(); iter != m_mapBitmap.end(); iter++)
	{
		if (iter->first == FileName)
			return iter->second;
	}

	return InitBitmap(FileName);


}

void ResourceManager::InitBack(HDC hdc, int x, int y)
{
	m_pback = new Bitmap();
	m_pback->InitBack(hdc, x, y);
}

Bitmap* ResourceManager::InitBitmap(string FileName)
{
	Bitmap* pBitmap = new Bitmap();
	pBitmap->Init(m_pback->GetDC(), FileName);
	m_mapBitmap.insert(make_pair(FileName, pBitmap));
	return pBitmap;

}
void ResourceManager::Release()
{
	SAFE_DELETE(m_pback);

	for (auto iter = m_mapBitmap.begin(); iter != m_mapBitmap.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}

}

