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


// ������Ʈ�� ������ ��Ÿ���� ������
enum OBJ_ID		{	OBJ_PLAYER,
					OBJ_BULLET,
					OBJ_MONSTER,
					OBJ_ITEM,
					OBJ_TILE,
					OBJ_UI,
					OBJ_END
				};



// ������Ʈ�� ���¸� ��Ÿ���� ������
enum OBJ_STATE	{	// IDLE, �̵�, �̵�����, �̵�������, �̵��߹�����ȯ, �ɱ�
					OBJST_IDLE, OBJST_RUN, OBJ_RUNSTART, OBJ_RUNEND, OBJ_RUNCHANGE, OBJST_DOWN,
					// ����, ������, Ȱ��
					OBJST_JUMP, OBJST_FALL, OBJST_GLIDE,
					
					// ���� �ܰ� �� ����
					OBJST_ACTION1, OBJST_ACTION2, OBJST_ACTION3, OBJST_ACTION4,
					// ������, ���� �� ����, ���� �� �밢����
					OBJST_UPACTION, OBJST_JUMPACTION,	OBJST_UNDERACTION,

					// ���ݹ���
					OBJST_DAMAGED,

					OBJST_END
				};


// ������Ʈ�� ���� ������ ��Ÿ���� ������
enum RENDERID	{	RENDER_PRIORITY,				// ���� �켱���� ���� ������
					RENDER_UI,						// UI
					RENDER_GAMEOBJECT,				// GAMEOBJECT
					RENDER_TILE_BACKGROUND,			// ��� (Ÿ��)
					REDNER_FRONT_BACKGROUND,		// ��� (Ÿ�� ���� ������ ����, ��溸�� ���� �̹���)
					RENDER_BACKGROUND,				// ���	(�ϴð� ����, ���� ������ �̹���)
					RENDER_END
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


