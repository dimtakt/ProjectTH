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
	static CObj* Create(float fX, float fY, float fAngle = 0.f)
	{
		CObj* pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Angle(fAngle);

		return pObj;
	}
};

