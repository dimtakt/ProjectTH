#pragma once

#include "Define.h"
#include "CObj.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;
	CObjMgr& operator=(CObjMgr& rObj) = delete;
	virtual ~CObjMgr();

public:
	CObj* Get_Target(OBJ_ID eID, CObj* pObj);

public:
	void	Add_Object(OBJ_ID eID, CObj* pObj);
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	void	Delete_ID(OBJ_ID eID);
	void	Delete_SpecificObj(OBJ_ID _eID, CObj* _obj);

	void Optimize_DeleteOutsideScreen();

private:
	std::list<CObj*>	m_ObjList[OBJ_END];	// update ¿ë
	std::list<CObj*>	m_RenderList[RENDER_END];

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
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

	CObj* Get_Player()
	{
		return *m_ObjList[OBJ_PLAYER].begin();
	}

private:
	static CObjMgr* m_pInstance;
	

};

