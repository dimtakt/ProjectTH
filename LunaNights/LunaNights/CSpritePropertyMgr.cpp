#include "pch.h"
#include "CSpritePropertyMgr.h"

CSpritePropertyMgr* CSpritePropertyMgr::m_pInstance = nullptr;

CSpritePropertyMgr::CSpritePropertyMgr()
{
}

CSpritePropertyMgr::~CSpritePropertyMgr()
{
	Release();
}

void CSpritePropertyMgr::Insert_Property(FRAME_PROP pProperty, const TCHAR* pKey)
{
	auto iter = find_if(m_mapProperty.begin(), m_mapProperty.end(), tagStringCmp(pKey));

	if (iter == m_mapProperty.end())
	{
		FRAME_PROP* pProp = new FRAME_PROP;
		*pProp = pProperty;

		m_mapProperty.insert({ pKey, pProp });
	}
}

FRAME_PROP CSpritePropertyMgr::Find_Property(const TCHAR* pKey)
{
	auto iter = find_if(m_mapProperty.begin(), m_mapProperty.end(), tagStringCmp(pKey));

	if (iter == m_mapProperty.end())
		return FRAME_PROP();

	return *iter->second;
}

void CSpritePropertyMgr::Release()
{
	for_each(m_mapProperty.begin(), m_mapProperty.end(), [](auto& MyPair)
		{
			if (MyPair.second)
			{
				delete MyPair.second;
				MyPair.second = nullptr;
			}
		});

	m_mapProperty.clear();
}
