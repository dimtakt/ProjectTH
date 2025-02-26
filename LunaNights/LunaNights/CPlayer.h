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
	// 0 Idle, 1 �ý���, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	void Set_MessageWith(int _with)		{ m_iMessageWith = _with; }
	// 0 Idle, 1 �ý���, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	int Get_MessageWith()				{ return m_iMessageWith; }
	// 0 Idle, 1 �ý���, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	void Set_MessagePic(int _pic) { m_iMessagePic = _pic; }
	// 0 Idle, 1 �ý���, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	int Get_MessagePic() { return m_iMessagePic; }

	void Set_MessageOrder(int _order)	{ m_iMessageOrder = _order; }
	int Get_MessageOrder()				{ return m_iMessageOrder; }
	

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

		ISTALKED_AKYUU,
		ISTALKED_NITORI,
		ISTALKED_MEIRIN,

		STAT_END
	};

	int Get_Stat(PLAYERSTAT _statType);
	bool Get_Stretch() { return m_isStretch; }

	void Set_Stat_isGetWatch(bool _watch) { m_isGetWatch = _watch; }
	bool Get_isBossStart() { return m_isBossStart; }
	void Set_isBossStart(bool _start) { m_isBossStart = _start; }
	
	// 1. ���̸�
	bool Get_isClearedBoss(int _index) {
		switch (_index)
		{
		case 1:		return m_isClearedMeirin;

		default:
			break;
		}
	}
	// 1. ���̸�
	void Set_isClearedBoss(int _index) {
		switch (_index)
		{
		case 1:		m_isClearedMeirin = true;		break;

		default:
			break;
		}
	}


	// 1 ��ť, 2 ���丮, 3 ���̸�
	void Set_isTalkedNpc(int _index) {
		switch (_index)
		{
		case 1:		m_isTalkedAkyuu = true;		break;
		case 2:		m_isTalkedNitori = true;	break;
		case 3:		m_isTalkedMeirin = true;	break;

		default:
			break;
		}
	}


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

	DWORD m_dwFallTime;


	FPOINT m_tPrePos;

	bool m_isStartStage;
	bool m_isMaxTP;
	bool m_isMaxMP;

	int m_iMessageWith;	// 0 Idle, 1 System, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	int m_iMessagePic;	// 0 Idle, 1 System, 2 ����, 3 ��ť, 4 ���丮, 5 ���̸�
	int m_iMessageOrder; // �޼����� ���� ���� ���, ���� Ȯ������ �޼����� ��°�� ��Ÿ��

	// Player Stats.

	float m_fTp;

	int m_iKnife;
	bool m_isGetWatch;

	int m_iTimeMode;	// 0 Idle, 1 Snail, 2 Stop

	bool m_isBossStart;




	bool m_isTalkedAkyuu;
	bool m_isTalkedNitori;
	bool m_isTalkedMeirin;

	bool m_isClearedMeirin;

};