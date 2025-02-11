#pragma once
#include "CScene.h"
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
    TCHAR tTileName;
    int iEditMode;
};

