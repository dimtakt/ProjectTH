#pragma once

#include "CObj.h"

template<typename T>
class CAbstractFactory	// CObj 생성용 추상 팩토리
{
public:
	
	// 객체를 생성하고, Initialize 호출 후 해당 객체를 리턴합니다.
	static CObj* Create()
	{
		CObj* pObj = new T;

		pObj->Initialize();

		return pObj;
	}

	// 객체를 생성하고, Initialize 호출 및 위치와 각도를 반영한 후 해당 객체를 리턴합니다.
	//static CObj* Create(float fX, float fY, float fAngle = 0.f)
	//{
	//	CObj* pObj = new T;

	//	pObj->Initialize();
	//	pObj->Set_Pos(fX, fY);
	//	pObj->Set_Angle(fAngle);

	//	return pObj;
	//}

	// 객체를 생성하고, Initialize 호출 및 위치와 각도를 반영한 후,
	// 해당 객체에 m_pFrameKey를 지정해준 뒤 해당 객체를 리턴합니다.
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

