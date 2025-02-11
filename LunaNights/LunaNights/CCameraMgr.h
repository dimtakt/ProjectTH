#pragma once

#include "Define.h"

class CObj;

// ī�޶� �������� ������ : https://www.youtube.com/watch?v=uAaS-8sBvds


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

	// ī�޶� ��ü�� ��ǥ�� �޾ƿ��� �Լ�
	void Get_CameraPos(float& _rX, float& _rY)
	{
		_rX = m_fCameraX;
		_rY = m_fCameraY;
	}

	// ���������� ������ų ��ǥ�� �޾ƿ��� �Լ�.
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
	static CCameraMgr*	m_pInstance;	// ī�޶� �ν��Ͻ�. �̱����� ���� ��ü �ϳ��θ� ���� �� ���
	
	float				m_fCameraX;		// ī�޶� ��ü�� ���� X��ǥ
	float				m_fCameraY;		// ī�޶� ��ü�� ���� Y��ǥ

	CObj*				m_pTarget;		// ī�޶� Ÿ������ ���� ������Ʈ

	float				m_fDiffX;		// ī�޶� ������ǥ�� Ÿ�� ������Ʈ ���� X��ǥ ����
	float				m_fDiffY;		// ī�޶� ������ǥ�� Ÿ�� ������Ʈ ���� Y��ǥ ����
};