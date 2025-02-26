#pragma once

#include "CObj.h"
#include "CEffect.h"

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

		pObj->Update();

		return pObj;
	}

	// 시간이 유효한 일반 이펙트 생성 시에 사용
	static CObj* CreateEffect(float _fX, float _fY, float _fAngle = 0.f, const TCHAR* _pFrameKey = nullptr, bool _isWorldFix = true, bool _isLoop = false, float _loopTimes = 0 )
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);

		dynamic_cast<CEffect*>(pObj)->Set_isLoop(_isLoop);
		dynamic_cast<CEffect*>(pObj)->Set_LoopTimes(_loopTimes);

		if (_pFrameKey != nullptr)
			pObj->Set_FrameKey(_pFrameKey);

		pObj->Set_WorldFix(_isWorldFix);

		pObj->Update();

		return pObj;
	}

	// 콜라이더 객체 생성 전용. 다른 객체 생성할 땐 사용 X
	// 좌상, 우하 좌표 정보를 받아와 해당 정보 기반으로 Info 정보를 갖는 객체 생성
	static CObj* CreateRectCollider(float _fX, float _fY, float _fX2, float _fY2)
	{
		CObj* pObj = new T;

		pObj->Initialize();

		// fX fY는 시작점, fX2 fY2 는 끝점 기준
		// 해당 값을 기준으로 하는 Info 위치를 계산하여 반영해야 함
		pObj->Set_Pos((_fX + _fX2) / 2, (_fY + _fY2) / 2);
		pObj->Set_Scale(abs(_fX - _fX2), abs(_fY - _fY2));

		
		return pObj;
	}

	// 숫자(데미지) 이펙트 객체 생성 전용. 다른 객체 생성할 땐 사용 X
	static CObj* CreateNumEffect(float _fX, float _fY, float _fAngle = 0.f, bool _isWorldFix = true, int _damageType = 0, int _damageAmount = 0, float _loopTimes = 1.5f)
	{
		CObj* pObj = new T;

		// 준 뎀인지 받은 뎀인지 구분하는 인자 추가
		// 데미지의 수치를 받는 인자 추가

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);

		dynamic_cast<CEffect*>(pObj)->Set_DamageType(_damageType);
		dynamic_cast<CEffect*>(pObj)->Set_DamageAmount(_damageAmount);
		dynamic_cast<CEffect*>(pObj)->Set_LoopTimes(_loopTimes);

		pObj->Set_FrameKey(L"Effect_Number");

		pObj->Set_WorldFix(_isWorldFix);

		return pObj;
	}

	// 스테이터스 이펙트 객체 생성 전용. 다른 객체 생성할 땐 사용 X
	static CObj* CreateStatusEffect(float _fX, float _fY, float _fAngle = 0.f, bool _isWorldFix = true, int _statusType = 0, float _loopTimes = 1.5f)
	{
		CObj* pObj = new T;

		// 준 뎀인지 받은 뎀인지 구분하는 인자 추가
		// 데미지의 수치를 받는 인자 추가

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);

		dynamic_cast<CEffect*>(pObj)->Set_StatusType(_statusType);
		dynamic_cast<CEffect*>(pObj)->Set_LoopTimes(_loopTimes);

		pObj->Set_FrameKey(L"Effect_Status");

		pObj->Set_WorldFix(_isWorldFix);

		return pObj;
	}

};

