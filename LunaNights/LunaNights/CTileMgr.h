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
	void		Save_Tile();
	void		Load_Tile();
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
};

