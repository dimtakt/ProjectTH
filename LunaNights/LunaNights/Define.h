#pragma once


#pragma region Defines..


#define			WINCX			1280
#define			WINCY			720


// #define			TILEX			30
// #define			TILEY			20

// #define			TILECX			64
// #define			TILECY			64


#define			TILECX			68			// 타일 하나의 크기를 의미
#define			TILECY			68			// 타일 하나의 크기를 의미

//#define			TILESIZERATIO	1			// 스프라이트를 출력시킬 배율을 의미

#define			TILEX			20	// 만들 타일의 갯수를 의미 ksta
#define			TILEY			12 // ceil(WINCY / (TILECX / TILESIZERATIO))	// 만들 타일의 갯수를 의미 ksta


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
extern bool g_isStop;	// UI 및 이펙트를 제외하고 일시정지하는 변수. 메세지에 활용할 것


#pragma endregion




#pragma region Enums..


// 오브젝트의 종류를 나타내는 열거형
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



// 오브젝트의 상태를 나타내는 열거형
enum OBJ_STATE	{	// IDLE, 이동, 이동시작, 이동중정지, 이동중방향전환, 앉기
					OBJST_IDLE, OBJST_RUN, OBJST_RUNSTART, OBJST_RUNEND, OBJST_RUNCHANGE, OBJST_DOWN,
					// 점프, 착지중, 활공
					OBJST_JUMP, OBJST_FALL, OBJST_GLIDE,
					
					// 공격 단계 별 상태
					OBJST_ACTION1, OBJST_ACTION2, OBJST_ACTION3, OBJST_ACTION4,

					// 공격 단계 별 상태
					OBJST_JUMP_ATTACK1, OBJST_JUMP_ATTACK2, OBJST_JUMP_ATTACK3, OBJST_JUMP_ATTACK4,
					
					// 공격 단계 별 상태
					OBJST_RUN_ATTACK1, OBJST_RUN_ATTACK2, OBJST_RUN_ATTACK3, OBJST_RUN_ATTACK4,

					// 윗공격, 점프 중 공격, 점프 중 대각공격
					OBJST_UPACTION, OBJST_JUMPACTION,	OBJST_UNDERACTION,

					// 공격받음
					OBJST_DAMAGED,

					OBJST_END
				};


// 오브젝트의 렌더 순서를 나타내는 열거형
enum RENDERID	{	
					RENDER_BACKGROUND,				// 배경	(하늘과 같은, 제일 뒤쪽의 이미지)
					REDDER_FRONT_BACKGROUND,		// 배경 (타일 뒤의 나무와 같은, 배경보단 위의 이미지)
					RENDER_TILE_BACKGROUND,			// 배경 (타일)
					RENDER_NPC,						// NPC
					RENDER_GAMEOBJECT,				// GAMEOBJECT
					RENDER_EFFECT,					// 이펙트
					RENDER_UI,						// UI
					RENDER_UI_EFFECT,				// UI보다 위에 위치하여야 하는 이펙트
					RENDER_PRIORITY,				// 가장 우선순위 높게 렌더링
					RENDER_END
				};


// 소리의 유형을 나타내는 열거형
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
	float			fCX = 0;	// 가로 길이
	float			fCY = 0;	// 세로 길이

}INFO;

typedef struct tagFrame
{
	int	iFrameCur;	// 현재 프레임을 의미

	int iCX;			// 스프라이트 한 장의 X 넓이
	int iCY;			// 스프라이트 한 장의 Y 넓이

	int iFrameMaxX;		// 스프라이트의 X 축 갯수
	int iFrameMaxY;		// 스프라이트의 Y 축 갯수
	int iFrameAmount;	// 스프라이트의 총 갯수

	//FRAME_PROP m_tProp;

	DWORD dwSpeed;
	DWORD dwTime;

}FRAME;

// 사용할 스프라이트의 FRAME 필수 정보만 담은 객체
typedef struct tagFrameProperty
{
	int iCX;			// 스프라이트 한 장의 X 넓이
	int iCY;			// 스프라이트 한 장의 Y 넓이

	int iFrameMaxX = 1;		// 스프라이트의 X 축 갯수
	int iFrameMaxY = 1;		// 스프라이트의 Y 축 갯수
	int iFrameAmount = 1;	// 스프라이트의 총 갯수
	
	int iFrameStart = 0;	// 시작할 스프라이트의 인덱스. 스프라이트가 0부터 반복출력 시 어색한 경우에 사용.
}FRAME_PROP;


// 충돌의 위치 기준이 되는 각각의,
// 좌상 우상 좌하 우하 좌표를 담는 구조체
typedef struct tagCollisionPoint
{
	POINT ptLUp;
	POINT ptRUp;
	POINT ptLDown;
	POINT ptRDown;
	bool isHasCollide = true;	// 타일 콜라이더를 구성하는 점이 5개가 필요할 때만 사용

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


