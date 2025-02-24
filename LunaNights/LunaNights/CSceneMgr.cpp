#include "pch.h"
#include "CSceneMgr.h"
#include "SoundMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() : m_pScene(nullptr)
, m_eCurScene(SC_LOGO), m_ePreScene(SC_END), m_isEnterStage(false)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eScene)
{
	// 스테이지 들어갈 때 1회에 한하여 BGM 호출
	if ((eScene != SC_LOGO && eScene != SC_EDIT) && m_isEnterStage == false)
	{
		CSoundMgr::Get_Instance()->PlayBGM(L"bgm00.ogg", 0.05f);
		m_isEnterStage = true;
	}

	m_eCurScene = eScene;


	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_STAGE1_01:
			m_pScene = new CStage1_01;
			break;

		case SC_STAGE1_02:
			m_pScene = new CStage1_02;
			break;

		case SC_STAGE1_03:
			m_pScene = new CStage1_03;
			break;

		case SC_STAGE1_BOSS:
			m_pScene = new CStage1_Boss;
			break;

		case SC_EDIT:
			m_pScene = new CEdit;
			break;

		//case SC_STAGE:
		//	m_pScene = new CStage;
		//	break;
		}


		m_pScene->Initialize();
		m_pScene->Update();


		m_ePreScene = m_eCurScene;
	}

}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
