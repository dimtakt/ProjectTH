#pragma once
#pragma once

#include "Define.h"

// Scene 만들 때 마다 여기에 #include 로 해당 헤더를 추가해주어야 함
#include "CLogo.h"
#include "CStage1_01.h"

#include "CEdit.h"

class CSceneMgr
{
public:
	enum SCENEID { SC_LOGO, SC_STAGE1_01, SC_EDIT, SC_END };

private:
	CSceneMgr();
	virtual ~CSceneMgr();

public:
	void		Scene_Change(SCENEID eScene);
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	SCENEID		Get_CurScene() { return m_eCurScene; }

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneMgr;
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
	static CSceneMgr*		m_pInstance;
	CScene*					m_pScene;

	SCENEID					m_eCurScene;
	SCENEID					m_ePreScene;

};


