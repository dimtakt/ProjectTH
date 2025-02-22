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
	
	void Set_State(OBJ_STATE _eState)		{ m_eCurState = _eState; }
	OBJ_STATE Get_State()					{ return m_eCurState; }
	void Set_Angle(float _fAngle)			{ m_fAngle = _fAngle; }

	const RECT* Get_Rect() const			{ return &m_tRect; }
	const INFO* Get_Info() const			{ return &m_tInfo; }
	const INFO* Get_CollideInfo() const		{ return &m_tCollideInfo; }

	bool Get_Dead()							{ return m_isDead; }
	void Set_Dead()							{ m_isDead = true; }

	
	int Get_HP()			{ return m_iHp; }
	void Set_HP(int _iHp)	{ m_iHp = _iHp; }
	int Get_MP()			{ return m_iMp;	}
	void Set_MP(int _iMp)	{ m_iMp = _iMp; }
	float Get_Atk()			{ return m_fAtk;}

	void Set_Gold(int _iGold)	{ m_iGold = _iGold; }
	int Get_Gold()			{ return m_iGold; }

	void Set_God()			{ m_isGod = true; }
	bool Get_God()			{ return m_isGod; }



	void Set_Target(CObj* pTarget)			{ m_pTarget = pTarget; }	// Auto Aim 구현할 때 필요할 듯

	void Set_PosX(float _fX)					{ m_tInfo.fX += _fX; }
	void Set_PosY(float _fY)					{ m_tInfo.fY += _fY; }
	void Set_FrameKey(const TCHAR* pFrameKey)	{ m_pFrameKey = pFrameKey; }
	void Set_WorldFix(bool _isWorldFix)			{ isWorldFix = _isWorldFix; }

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

	// m_tFrame 이 먼저 적용되어야 사용 가능함. 아니면 타 FRAME 타입이 인자로 필요.
	// 사용 전에 Set_FrameProperty 으로 적용하거나 직접 지정 후 불러올 것.
	void		Move_Frame();				
	void		Move_Frame(FRAME& _tFrame);

	// PlgBlt 를 통해 사진의 돌리기를 위해 만든 함수
	void		RotatePoints(float centerX, float centerY, POINT points[3], float angle = 0.f);


	bool		Get_isFlying() { return m_isFlying; }




	


protected:
	INFO		m_tInfo;
	INFO		m_tCollideInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	FRAME_PROP	m_tFramePropCur;
	OBJ_STATE	m_eCurState;
	RENDERID	m_eRender;
	


	CObj* m_pTarget;
	

	float		m_fSpeed;
	float		m_fVelocityX;
	float		m_fVelocityY;
	bool		m_isDead;
	float		m_fAngle;
	//float		m_fDistance;
	bool		m_isStretch;

	bool		m_isFlying;
	bool		m_isJumping;
	bool		m_isGod;
	bool		m_isPreGod;


	
	
	const TCHAR* m_pFrameKey;

	bool isWorldFix;    // 만약 이펙트라면, 월드 좌표에 고정시킬 것인가?

	// Stats.
	int m_iHp;
	int m_iMp;
	float m_fAtk;


	int m_iMaxHp;
	int m_iMaxMp;
	int m_iGold;


	std::map<TCHAR, FRAME_PROP> m_mapFrameProp;

};

