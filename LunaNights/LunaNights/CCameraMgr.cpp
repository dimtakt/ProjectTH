#include "pch.h"
#include "CCameraMgr.h"
#include "CObj.h"

#include "CKeyMgr.h"
#include "CSceneMgr.h"

CCameraMgr* CCameraMgr::m_pInstance = nullptr;


CCameraMgr::CCameraMgr()
	: m_pTarget(nullptr)
{
	m_fCameraX = WINCX / 2.f;
	m_fCameraY = WINCY / 2.f;

	m_fDiffX = 0;
	m_fDiffY = 0;
}

CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::Update_CameraPos(int _iMaxX, int _iMaxY, int _iMinX, int _iMinY)
{
	// 타겟이 존재하는 경우, 카메라의 좌표를 타겟으로 변경
	// 타겟이 존재는 하나 사망한 경우, 타겟을 해제
	if (m_pTarget && !m_pTarget->Get_Dead())
	{
		m_fCameraX = m_pTarget->Get_Info()->fX;
		m_fCameraY = m_pTarget->Get_Info()->fY - m_pTarget->Get_Info()->fCY / 2;
	}

#pragma region Camera Test Code with WASD

	// Edit Mode 에서만 카메라 조작 가능
	if (CSceneMgr::Get_Instance()->Get_CurScene() == CSceneMgr::SC_EDIT)
	{

		int iCameraSpeed = 5.f;
		if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
			m_fCameraY -= iCameraSpeed;
		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			m_fCameraY += iCameraSpeed;
		if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
			m_fCameraX -= iCameraSpeed;
		if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			m_fCameraX += iCameraSpeed;
	}

#pragma endregion

	Lock_Camera(_iMaxX, _iMaxY, _iMinX, _iMinY);
	CCameraMgr::CalcDiff();
}


void CCameraMgr::CalcDiff()
{
	float fCenterX = WINCX / 2.f;
	float fCenterY = WINCY / 2.f;

	m_fDiffX = m_fCameraX - fCenterX;	// 카메라 좌표가 X만큼 더해지면 오브젝트들은 -X만큼 이동해야 함
	m_fDiffY = m_fCameraY - fCenterY;	// 카메라 좌표가 Y만큼 더해지면 오브젝트들은 -Y만큼 이동해야 함
}

void CCameraMgr::Lock_Camera(int _iMaxX, int _iMaxY, int _iMinX, int _iMinY)
{
	//int iMargin = 30;

	if		(_iMinX > m_fCameraX - 640)
		m_fCameraX = _iMinX + 640;
	else if (_iMaxX < m_fCameraX + 640)
		m_fCameraX = _iMaxX - 640;

	if		(_iMinY > m_fCameraY - 360)
		m_fCameraY = _iMinY + 360;
	else if	(_iMaxY < m_fCameraY + 360)
		m_fCameraY = _iMaxY - 360;

	//if (_iMinX > m_fCameraX - WINCX / 2)
	//	m_fCameraX = _iMinX + WINCX / 2;
	//else if (_iMaxY < m_fCameraX + WINCX / 2)
	//	m_fCameraX = _iMaxX - WINCX / 2;

	//if (_iMinX > m_fCameraY - WINCY / 2)
	//	m_fCameraY = _iMinY + WINCY / 2;
	//else if (_iMaxY < m_fCameraY + WINCY / 2)
	//	m_fCameraY = _iMaxY - WINCY / 2;



	//std::cout << "[INFO][CCameraMgr::Lock_Camera] CameraPos : " << m_fCameraX << ", " << m_fCameraY << std::endl;
}