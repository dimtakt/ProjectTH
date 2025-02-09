#pragma once

#include "CObj.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	void Initialize()		override;
	int Update()			override;
	void Late_Update()		override;
	void Render(HDC hDC)	override;
	void Release()			override;

private:
	void Key_Input();
	void Jump();
	void Camera_Offset();
	void Motion_Change();

private:
	//POINT m_tPosin;

	float m_fJumpSpeed;
	float m_fTime;
	
	bool m_isJump;
	
	OBJ_STATE m_eCurState;
	OBJ_STATE m_ePreState;

	DWORD m_dwTime;



};