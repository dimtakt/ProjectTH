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

	void LoadImages();


public:
	enum PLAYERSTAT
	{
		HP, MAXHP,
		MP, MAXMP,
		TP,
		GOLD,
		ATK,
		KNIFE,
		ISGETWATCH,

		TIMEMODE,
		STAT_END
	};

	int Get_Stat(PLAYERSTAT _statType);
	bool Get_Stretch() { return m_isStretch; }

	

private:
	float m_fPosinLength;

	float m_fJumpSpeed;
	float m_fTime;


	
	//OBJ_STATE m_eCurState;
	OBJ_STATE m_ePreState;

	DWORD m_dwTime;
	DWORD m_dwStateChangeTime;

	DWORD m_dwMpRegenTime;
	DWORD m_dwTpRegenTime;
	DWORD m_dwGodTime;

	DWORD m_dwSnailReadyTime;
	DWORD m_dwSnailTime;

	FPOINT m_tPrePos;

	bool m_isStartStage;

	

	// Player Stats.

	float m_fTp;

	int m_iKnife;
	bool m_isGetWatch;

	int m_iTimeMode;	// 0 Idle, 1 Snail, 2 Stop



};