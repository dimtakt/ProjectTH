#include "pch.h"
#include "CCameraMgr.h"
#include "CObj.h"

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

void CCameraMgr::Update_CameraPos()	// Update가 아니라 Update_CameraPos 이런식으로 바꾸고 나머지 지워도 될것같은데
{
	// 타겟이 존재하는 경우, 카메라의 좌표를 타겟으로 변경
	// 타겟이 존재는 하나 사망한 경우, 타겟을 해제
	if (m_pTarget)
	{
		if (m_pTarget->Get_Dead())
			m_pTarget = nullptr;

		m_fCameraX = m_pTarget->Get_Info()->fX;
		m_fCameraY = m_pTarget->Get_Info()->fY;
	}

	CCameraMgr::CalcDiff();
}


void CCameraMgr::CalcDiff()
{
	float fCenterX = WINCX / 2.f;
	float fCenterY = WINCY / 2.f;

	m_fDiffX = m_fCameraX - fCenterX;	// 카메라 좌표가 X만큼 더해지면 오브젝트들은 -X만큼 이동해야 함
	m_fDiffY = m_fCameraY - fCenterY;	// 카메라 좌표가 Y만큼 더해지면 오브젝트들은 -X만큼 이동해야 함
}