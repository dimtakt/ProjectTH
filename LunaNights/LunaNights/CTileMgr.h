#pragma once

#include "CTile.h"

class CTileMgr
{
private:
	CTileMgr();
	virtual ~CTileMgr();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	void		Picking_Tile(POINT ptMouse, int _iDrawID, int _iOption);
	void		Save_Tile(const TCHAR* _dataFileName);
	void		Load_Tile(const TCHAR* _dataFileName, const TCHAR* _propertyName);

	void		Set_PropName(const TCHAR* _propertyName) { m_tPropertyName = _propertyName; } // 렌더 내에서 반영하는 코드 짜야함

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
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
	static CTileMgr*		m_pInstance;
	std::vector<CObj*>		m_vecTile;
	const TCHAR* m_tPropertyName;
};

