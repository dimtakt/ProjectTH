#pragma once


#pragma region Defines..


#define			WINCX			1280
#define			WINCY			720


// #define			TILEX			30
// #define			TILEY			20

// #define			TILECX			64
// #define			TILECY			64


#define			TILECX			68			// Ÿ�� �ϳ��� ũ�⸦ �ǹ�
#define			TILECY			68			// Ÿ�� �ϳ��� ũ�⸦ �ǹ�

//#define			TILESIZERATIO	1			// ��������Ʈ�� ��½�ų ������ �ǹ�

#define			TILEX			20	// ���� Ÿ���� ������ �ǹ� ksta
#define			TILEY			12 // ceil(WINCY / (TILECX / TILESIZERATIO))	// ���� Ÿ���� ������ �ǹ� ksta


#define			PURE			= 0 

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1

#define			PI				3.141592f
#define			GRAVITY			9.80665f

#define			VK_MAX			0xff

#define			DEG2RAD			(PI / 180.0f)
#define			RAD2DEG			(180.0f / PI)


#pragma endregion




#pragma region Extern Variables..


extern HWND g_hWnd;
extern bool g_isStop;	// UI �� ����Ʈ�� �����ϰ� �Ͻ������ϴ� ����. �޼����� Ȱ���� ��


#pragma endregion




#pragma region Enums..


// ������Ʈ�� ������ ��Ÿ���� ������
enum OBJ_ID		{	OBJ_PLAYER = 0,
					OBJ_NPC = 1,
					OBJ_PLAYERBULLET = 2,
					OBJ_MONSTERBULLET,
					OBJ_EFFECT = 5,
					OBJ_MONSTER_WOLF = 20,
					OBJ_MONSTER_WISP,
					OBJ_ITEM = 30,
					OBJ_TILE = 40,
					OBJ_COLLIDERECT = 41,
					OBJ_UI = 50,

					OBJ_END
				};



// ������Ʈ�� ���¸� ��Ÿ���� ������
enum OBJ_STATE	{	// IDLE, �̵�, �̵�����, �̵�������, �̵��߹�����ȯ, �ɱ�
					OBJST_IDLE, OBJST_RUN, OBJST_RUNSTART, OBJST_RUNEND, OBJST_RUNCHANGE, OBJST_DOWN,
					// ����, ������, Ȱ��
					OBJST_JUMP, OBJST_FALL, OBJST_GLIDE,
					
					// ���� �ܰ� �� ����
					OBJST_ACTION1, OBJST_ACTION2, OBJST_ACTION3, OBJST_ACTION4,

					// ���� �ܰ� �� ����
					OBJST_JUMP_ATTACK1, OBJST_JUMP_ATTACK2, OBJST_JUMP_ATTACK3, OBJST_JUMP_ATTACK4,
					
					// ���� �ܰ� �� ����
					OBJST_RUN_ATTACK1, OBJST_RUN_ATTACK2, OBJST_RUN_ATTACK3, OBJST_RUN_ATTACK4,

					// ������, ���� �� ����, ���� �� �밢����
					OBJST_UPACTION, OBJST_JUMPACTION,	OBJST_UNDERACTION,

					// ���ݹ���
					OBJST_DAMAGED,

					OBJST_END
				};


// ������Ʈ�� ���� ������ ��Ÿ���� ������
enum RENDERID	{	
					RENDER_BACKGROUND,				// ���	(�ϴð� ����, ���� ������ �̹���)
					REDDER_FRONT_BACKGROUND,		// ��� (Ÿ�� ���� ������ ����, ��溸�� ���� �̹���)
					RENDER_TILE_BACKGROUND,			// ��� (Ÿ��)
					RENDER_NPC,						// NPC
					RENDER_GAMEOBJECT,				// GAMEOBJECT
					RENDER_EFFECT,					// ����Ʈ
					RENDER_UI,						// UI
					RENDER_UI_EFFECT,				// UI���� ���� ��ġ�Ͽ��� �ϴ� ����Ʈ
					RENDER_PRIORITY,				// ���� �켱���� ���� ������
					RENDER_END
				};


// �Ҹ��� ������ ��Ÿ���� ������
enum CHANNELID
{
	SOUND_ATTACK,
	SOUND_DESTROY,
	SOUND_WOLF,
	SOUND_TIME_SLOW,
	SOUND_TIME_SLOWREADY,
	SOUND_TIME_SKILL,
	SOUND_TIME_END,
	SOUND_TIME_END2,

	SOUND_SKILLGET,

	SOUND_VO_TPMAX,
	SOUND_VO_MPMAX,

	SOUND_BGM,
	MAXCHANNEL
};

#pragma endregion





#pragma region Structs..


typedef struct tagInfo
{
	float			fX;
	float			fY;
	float			fCX = 0;	// ���� ����
	float			fCY = 0;	// ���� ����

}INFO;

typedef struct tagFrame
{
	int	iFrameCur;	// ���� �������� �ǹ�

	int iCX;			// ��������Ʈ �� ���� X ����
	int iCY;			// ��������Ʈ �� ���� Y ����

	int iFrameMaxX;		// ��������Ʈ�� X �� ����
	int iFrameMaxY;		// ��������Ʈ�� Y �� ����
	int iFrameAmount;	// ��������Ʈ�� �� ����

	//FRAME_PROP m_tProp;

	DWORD dwSpeed;
	DWORD dwTime;

}FRAME;

// ����� ��������Ʈ�� FRAME �ʼ� ������ ���� ��ü
typedef struct tagFrameProperty
{
	int iCX;			// ��������Ʈ �� ���� X ����
	int iCY;			// ��������Ʈ �� ���� Y ����

	int iFrameMaxX = 1;		// ��������Ʈ�� X �� ����
	int iFrameMaxY = 1;		// ��������Ʈ�� Y �� ����
	int iFrameAmount = 1;	// ��������Ʈ�� �� ����
	
	int iFrameStart = 0;	// ������ ��������Ʈ�� �ε���. ��������Ʈ�� 0���� �ݺ���� �� ����� ��쿡 ���.
}FRAME_PROP;


// �浹�� ��ġ ������ �Ǵ� ������,
// �»� ��� ���� ���� ��ǥ�� ��� ����ü
typedef struct tagCollisionPoint
{
	POINT ptLUp;
	POINT ptRUp;
	POINT ptLDown;
	POINT ptRDown;
	bool isHasCollide = true;	// Ÿ�� �ݶ��̴��� �����ϴ� ���� 5���� �ʿ��� ���� ���

}COL_POINT;

typedef struct tagLinePoint
{
	POINT ptStart;
	POINT ptEnd;
}LINE_POINT;

typedef struct tagFloatPoint
{
	float x;
	float y;
}FPOINT;

#pragma endregion




#pragma region Global Functions..


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

struct tagStringCmp
{
public:
	tagStringCmp(const TCHAR* pString)
		: m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

public:
	const TCHAR* m_pString;
};

struct DeleteObj
{
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

struct DeleteMap
{
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};

#pragma endregion


