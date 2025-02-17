#pragma once
#include "CScene.h"

class CObj;

class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC _DC) override;
    void Release() override;

private:
    void Key_Input();
    void LoadImages();

private:
    unsigned int iSelectedTileIndex;
    unsigned int iSelectedMonsterIndex; // 0 은 Wolf, 1 은 Wisp
    TCHAR tTileName;

    int iEditMode;  // 0 : 타일 확인 모드, 1 : 콜라이더 편집 모드
    int iEditStage; // 0 : 1-1, 1 : 1-2, 2 : 1-3, 3 : 1-Boss

    int iTileX; // 스테이지마다 스테이지의 가로/세로 길이가 다름. 그것을 정의.
    int iTileY; // 스테이지마다 스테이지의 가로/세로 길이가 다름. 그것을 정의.

    std::list<CObj*> m_ObjList;
};

