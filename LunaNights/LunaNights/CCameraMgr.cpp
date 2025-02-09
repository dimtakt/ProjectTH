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

void CCameraMgr::Update_CameraPos()	// Update�� �ƴ϶� Update_CameraPos �̷������� �ٲٰ� ������ ������ �ɰͰ�����
{
	// Ÿ���� �����ϴ� ���, ī�޶��� ��ǥ�� Ÿ������ ����
	// Ÿ���� ����� �ϳ� ����� ���, Ÿ���� ����
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

	m_fDiffX = m_fCameraX - fCenterX;	// ī�޶� ��ǥ�� X��ŭ �������� ������Ʈ���� -X��ŭ �̵��ؾ� ��
	m_fDiffY = m_fCameraY - fCenterY;	// ī�޶� ��ǥ�� Y��ŭ �������� ������Ʈ���� -X��ŭ �̵��ؾ� ��
}