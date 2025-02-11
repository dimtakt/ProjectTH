#pragma once

#include "Define.h"

class CObj;

// 카메라 구현관련 참고영상 : https://www.youtube.com/watch?v=uAaS-8sBvds


class CCameraMgr
{

private:
	CCameraMgr();
	virtual ~CCameraMgr();

public:
	void	Update_CameraPos();


	void Set_Pos(float _fX, float _fY)
	{
		m_fCameraX = _fX;
		m_fCameraY = _fY;
	}
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

	// 카메라 자체의 좌표를 받아오는 함수
	void Get_CameraPos(float& _rX, float& _rY)
	{
		_rX = m_fCameraX;
		_rY = m_fCameraY;
	}

	// 최종적으로 렌더시킬 좌표를 받아오는 함수.
	void Get_RenderPos(int& _rX, int& _rY) 
	{
		_rX = _rX - static_cast<int>(m_fDiffX);
		_rY = _rY - static_cast<int>(m_fDiffY);
	}
	void Get_RenderPos(LONG& _rX, LONG& _rY)
	{
		_rX = _rX - static_cast<int>(m_fDiffX);
		_rY = _rY - static_cast<int>(m_fDiffY);
	}

public:
	static CCameraMgr*	Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCameraMgr;
		}

		return m_pInstance;
	}

	static void			Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void CalcDiff();


private:
	static CCameraMgr*	m_pInstance;	// 카메라 인스턴스. 싱글톤을 통해 객체 하나로만 관리 및 사용
	
	float				m_fCameraX;		// 카메라 자체가 보는 X좌표
	float				m_fCameraY;		// 카메라 자체가 보는 Y좌표

	CObj*				m_pTarget;		// 카메라가 타겟으로 삼을 오브젝트

	float				m_fDiffX;		// 카메라 기준좌표와 타겟 오브젝트 간의 X좌표 차이
	float				m_fDiffY;		// 카메라 기준좌표와 타겟 오브젝트 간의 Y좌표 차이
};