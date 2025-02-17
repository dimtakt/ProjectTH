#pragma once
#include "CScene.h"

class CStage1_02 :
    public CScene
{
public:
    CStage1_02();
    virtual ~CStage1_02();

public:
    void Initialize()       override;
    void Update()           override;
    void Late_Update()      override;
    void Render(HDC _DC)    override;
    void Release()          override;

private:
};

