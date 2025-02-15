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
	void Set_Target(CObj* pTarget)			{ m_pTarget = pTarget; }	// Auto Aim ������ �� �ʿ��� ��

	void Set_PosX(float _fX)					{ m_tInfo.fX += _fX; }
	void Set_PosY(float _fY)					{ m_tInfo.fY += _fY; }
	void Set_FrameKey(const TCHAR* pFrameKey)	{ m_pFrameKey = pFrameKey; }

	RENDERID Get_RenderID()					{ return m_eRender; }

	
	// ���º� ��������Ʈ�� ���(ũ��, ���μ��� ����, �� ����)�� �������� ��������, �� ���°� �ٲ� �� ���� ����� FRAME ���� �Լ� ����.
	// ��� m_tFrame ����ü��, ��������Ʈ�� ���μ��� �������� ����
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
	// ������ Pos�� �߾��� ��Ƽ�, INFO �� ��ǥ�� �ݿ��ϴ� �Լ�
	// (will_o_wisp ���� ü���� ���Ϳ� ����, �������κ��� ������ Ȯ���ؾ� �ϴ� ��ü�� ���
	void		Update_Rect();
	void		Update_Rect2X();			// 2��ũ�� ���� ���� ��ǥ����
	// ������ Pos�� ���ϴ� �߾��� ��Ƽ�, INFO �� ��ǥ�� �ݿ��ϴ� �Լ�
	// (�÷��̾� �� �ΰ��� ���Ϳ� ����, �������κ��� ���� �� �־�� �ϴ� ��ü�� ���)
	void		Update_Rect_UpStand();
	void		Update_Rect_UpStand2X();	// 2��ũ�� ���� ���� ��ǥ����


	void		Move_Frame();
	void		Move_Frame(FRAME& _tFrame);

	// PlgBlt �� ���� ������ �����⸦ ���� ���� �Լ�
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

