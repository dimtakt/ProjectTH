#pragma once

#include "Define.h"

// FRAME_PROP �� TCHAR �� Key ���� ���� �����ϰ� ������ �� �ְ� ������ �̱���.

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

