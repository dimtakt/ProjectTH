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
	int m_iTileTimesX;	// 해당 스테이지의 타일 갯수 배수
	int m_iTileTimesY;	// 해당 스테이지의 타일 갯수 배수
};

