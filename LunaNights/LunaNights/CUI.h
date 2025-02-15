#pragma once
#include "CObj.h"
class CUI :
    public CObj
{
public:
    CUI();
    virtual ~CUI();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    FRAME tFrameHPMP;
    FRAME tFrameGauge_Stop;
    FRAME tFrameTime_Number;
};

