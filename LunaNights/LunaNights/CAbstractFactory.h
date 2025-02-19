#pragma once

#include "CObj.h"

template<typename T>
class CAbstractFactory	// CObj ������ �߻� ���丮
{
public:
	
	// ��ü�� �����ϰ�, Initialize ȣ�� �� �ش� ��ü�� �����մϴ�.
	static CObj* Create()
	{
		CObj* pObj = new T;

		pObj->Initialize();

		return pObj;
	}

	// ��ü�� �����ϰ�, Initialize ȣ�� �� ��ġ�� ������ �ݿ��� �� �ش� ��ü�� �����մϴ�.
	//static CObj* Create(float fX, float fY, float fAngle = 0.f)
	//{
	//	CObj* pObj = new T;

	//	pObj->Initialize();
	//	pObj->Set_Pos(fX, fY);
	//	pObj->Set_Angle(fAngle);

	//	return pObj;
	//}

	// ��ü�� �����ϰ�, Initialize ȣ�� �� ��ġ�� ������ �ݿ��� ��,
	// �ش� ��ü�� m_pFrameKey�� �������� �� �ش� ��ü�� �����մϴ�.
	static CObj* Create(float _fX, float _fY, float _fAngle = 0.f, const TCHAR* _pFrameKey = nullptr, bool _isWorldFix = true)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);

		if (_pFrameKey != nullptr)
			pObj->Set_FrameKey(_pFrameKey);

		pObj->Set_WorldFix(_isWorldFix);

		return pObj;
	}
};

