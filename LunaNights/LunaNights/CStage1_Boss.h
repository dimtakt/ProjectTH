#pragma once
#include "CScene.h"

class CStage1_Boss :
    public CScene
{
public:
    CStage1_Boss();
    virtual ~CStage1_Boss();

public:
    void Initialize()       override;
    void Update()           override;
    void Late_Update()      override;
    void Render(HDC _DC)    override;
    void Release()          override;

private:
};

