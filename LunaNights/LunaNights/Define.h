#pragma once


#pragma region Defines..


#define			WINCX			1280
#define			WINCY			720

// #define			TILEX			30
// #define			TILEY			20

// #define			TILECX			64
// #define			TILECY			64

#define			PURE			= 0 

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1

#define			PI				3.141592f
#define			GRAVITY			9.80665f

#define			VK_MAX			0xff


#pragma endregion




#pragma region Extern Variables..


extern HWND g_hWnd;


#pragma endregion




#pragma region Enums..


// 오브젝트의 종류를 나타내는 열거형
enum OBJ_ID		{	OBJ_PLAYER,
					OBJ_BULLET,
					OBJ_MONSTER,
					OBJ_ITEM,
					OBJ_TILE,
					OBJ_UI,
					OBJ_END
				};



// 오브젝트의 상태를 나타내는 열거형
enum OBJ_STATE	{	// IDLE, 이동, 이동시작, 이동중정지, 이동중방향전환, 앉기
					OBJST_IDLE, OBJST_RUN, OBJ_RUNSTART, OBJ_RUNEND, OBJ_RUNCHANGE, OBJST_DOWN,
					// 점프, 착지중, 활공
					OBJST_JUMP, OBJST_FALL, OBJST_GLIDE,
					
					// 공격 단계 별 상태
					OBJST_ACTION1, OBJST_ACTION2, OBJST_ACTION3, OBJST_ACTION4,
					// 윗공격, 점프 중 공격, 점프 중 대각공격
					OBJST_UPACTION, OBJST_JUMPACTION,	OBJST_UNDERACTION,

					// 공격받음
					OBJST_DAMAGED,

					OBJST_END
				};


// 오브젝트의 렌더 순서를 나타내는 열거형
enum RENDERID	{	RENDER_PRIORITY,				// 가장 우선순위 높게 렌더링
					RENDER_UI,						// UI
					RENDER_GAMEOBJECT,				// GAMEOBJECT
					RENDER_TILE_BACKGROUND,			// 배경 (타일)
					REDNER_FRONT_BACKGROUND,		// 배경 (타일 뒤의 나무와 같은, 배경보단 위의 이미지)
					RENDER_BACKGROUND,				// 배경	(하늘과 같은, 제일 뒤쪽의 이미지)
					RENDER_END
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



#pragma endregion


