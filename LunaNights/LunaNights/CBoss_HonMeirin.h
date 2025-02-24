#pragma once
#include "CObj.h"
class CBoss_HonMeirin :
    public CObj
{
public:
    CBoss_HonMeirin();
    virtual ~CBoss_HonMeirin();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    void LoadImages();
    void Jump();

    void Motion_Change();


private:
    bool m_isStartStage;

    bool m_isReady;
    int m_iPattern;

    OBJ_STATE m_ePreState;


    FPOINT m_tPrePos;

};