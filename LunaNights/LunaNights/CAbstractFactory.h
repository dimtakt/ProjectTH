#pragma once

#include "CObj.h"
#include "CEffect.h"

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

		pObj->Update();

		return pObj;
	}

	// �ð��� ��ȿ�� �Ϲ� ����Ʈ ���� �ÿ� ���
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

	// �ݶ��̴� ��ü ���� ����. �ٸ� ��ü ������ �� ��� X
	// �»�, ���� ��ǥ ������ �޾ƿ� �ش� ���� ������� Info ������ ���� ��ü ����
	static CObj* CreateRectCollider(float _fX, float _fY, float _fX2, float _fY2)
	{
		CObj* pObj = new T;

		pObj->Initialize();

		// fX fY�� ������, fX2 fY2 �� ���� ����
		// �ش� ���� �������� �ϴ� Info ��ġ�� ����Ͽ� �ݿ��ؾ� ��
		pObj->Set_Pos((_fX + _fX2) / 2, (_fY + _fY2) / 2);
		pObj->Set_Scale(abs(_fX - _fX2), abs(_fY - _fY2));

		
		return pObj;
	}

	// ����(������) ����Ʈ ��ü ���� ����. �ٸ� ��ü ������ �� ��� X
	static CObj* CreateNumEffect(float _fX, float _fY, float _fAngle = 0.f, bool _isWorldFix = true, int _damageType = 0, int _damageAmount = 0, float _loopTimes = 1.5f)
	{
		CObj* pObj = new T;

		// �� ������ ���� ������ �����ϴ� ���� �߰�
		// �������� ��ġ�� �޴� ���� �߰�

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

	// �������ͽ� ����Ʈ ��ü ���� ����. �ٸ� ��ü ������ �� ��� X
	static CObj* CreateStatusEffect(float _fX, float _fY, float _fAngle = 0.f, bool _isWorldFix = true, int _statusType = 0, float _loopTimes = 1.5f)
	{
		CObj* pObj = new T;

		// �� ������ ���� ������ �����ϴ� ���� �߰�
		// �������� ��ġ�� �޴� ���� �߰�

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

