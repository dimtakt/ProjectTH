#pragma once

#include "Define.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	Late_Update()	PURE;
	virtual void	Render(HDC _DC) PURE;
	virtual void	Release()		PURE; 

public:
	void Get_TileTimes(int& iX, int& iY)
	{
		iX = m_iTileTimesX;
		iY = m_iTileTimesY;
	}

protected:
	int m_iTileTimesX;	// �ش� ���������� Ÿ�� ���� ���
	int m_iTileTimesY;	// �ش� ���������� Ÿ�� ���� ���
};

