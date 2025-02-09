#pragma once

#include "Define.h"

// FRAME_PROP 을 TCHAR 의 Key 값을 통해 저장하고 꺼내쓸 수 있게 구현한 싱글톤.

class CSpritePropertyMgr
{
private:
	CSpritePropertyMgr();
	virtual ~CSpritePropertyMgr();

public:
	void		Insert_Property(FRAME_PROP pProperty, const TCHAR* pKey);
	FRAME_PROP	Find_Property(const TCHAR* pKey);
	void		Release();

public:
	static CSpritePropertyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSpritePropertyMgr;
		}

		return m_pInstance;
	}
	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSpritePropertyMgr* m_pInstance;
	std::map<const TCHAR*, FRAME_PROP*>		m_mapProperty;
};

