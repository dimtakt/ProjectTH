#pragma once

#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	void Set_Pos(float _fX, float _fY)		{ m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	void Set_Scale(float _fCX, float _fCY)	{ m_tInfo.fCX = _fCX; m_tInfo.fCY = _fCY; }
	
	void Set_State(OBJ_STATE _eState)		{ m_eState = _eState; }
	void Set_Angle(float _fAngle)			{ m_fAngle = _fAngle; }

	const RECT* Get_Rect() const			{ return &m_tRect; }
	const INFO* Get_Info() const			{ return &m_tInfo; }
	const INFO* Get_CollideInfo() const		{ return &m_tCollideInfo; }

	bool Get_Dead()							{ return m_isDead; }
	void Set_Dead()							{ m_isDead = true; }
	void Set_Target(CObj* pTarget)			{ m_pTarget = pTarget; }	// Auto Aim 구현할 때 필요할 듯

	void Set_PosX(float _fX)					{ m_tInfo.fX += _fX; }
	void Set_PosY(float _fY)					{ m_tInfo.fY += _fY; }
	void Set_FrameKey(const TCHAR* pFrameKey)	{ m_pFrameKey = pFrameKey; }

	RENDERID Get_RenderID()					{ return m_eRender; }

	
	// 상태별 스프라이트의 양식(크기, 가로세로 갯수, 총 갯수)가 동일하지 않음으로, 매 상태가 바뀔 때 마다 사용할 FRAME 수정 함수 제작.
	// 멤버 m_tFrame 구조체에, 스프라이트의 가로세로 갯수만을 전달
	void Set_FrameProperty(FRAME_PROP _property)
	{
		m_tFrame.iCX			= _property.iCX;
		m_tFrame.iCY			= _property.iCY;

		m_tFrame.iFrameMaxX		= _property.iFrameMaxX;
		m_tFrame.iFrameMaxY		= _property.iFrameMaxY;
		m_tFrame.iFrameAmount	= _property.iFrameAmount;
	}
	void Set_FrameProperty(FRAME_PROP _property, FRAME& _frame)
	{
		_frame.iCX = _property.iCX;
		_frame.iCY = _property.iCY;

		_frame.iFrameMaxX = _property.iFrameMaxX;
		_frame.iFrameMaxY = _property.iFrameMaxY;
		_frame.iFrameAmount = _property.iFrameAmount;
	}



public:
	virtual void Initialize()	PURE;
	virtual int	 Update()		PURE;
	virtual void Late_Update()  PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()		PURE;

public:
	// 현재의 Pos를 중앙점 삼아서, INFO 상 좌표를 반영하는 함수
	// (will_o_wisp 같은 체공형 몬스터와 같이, 중점으로부터 범위를 확인해야 하는 개체에 사용
	void		Update_Rect();
	void		Update_Rect2X();			// 2배크기 렌더 기준 좌표보정
	// 현재의 Pos를 최하단 중앙점 삼아서, INFO 상 좌표를 반영하는 함수
	// (플레이어 및 인간형 몬스터와 같이, 중점으로부터 위에 서 있어야 하는 개체에 사용)
	void		Update_Rect_UpStand();
	void		Update_Rect_UpStand2X();	// 2배크기 렌더 기준 좌표보정


	void		Move_Frame();
	void		Move_Frame(FRAME& _tFrame);

	// PlgBlt 를 통해 사진의 돌리기를 위해 만든 함수
	void		RotatePoints(float centerX, float centerY, POINT points[3], float angle = 0.f);

protected:
	INFO		m_tInfo;
	INFO		m_tCollideInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	FRAME_PROP	m_tFramePropCur;
	OBJ_STATE	m_eState;
	RENDERID	m_eRender;
	


	CObj* m_pTarget;
	

	float		m_fSpeed;
	float		m_fVelocityY;
	bool		m_isDead;
	float		m_fAngle;
	//float		m_fDistance;
	bool		m_isStretch;

	bool		m_isFlying;
	bool		m_isJumping;

	const TCHAR* m_pFrameKey;

	std::map<TCHAR, FRAME_PROP> m_mapFrameProp;

};

