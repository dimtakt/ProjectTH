#pragma once
#include "CObj.h"
class CWolf :
    public CObj
{
public:
    CWolf();
    virtual ~CWolf();

public:

    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Jump();
    void Motion_Change();


private:
    DWORD dwAttackReady;
    bool m_isAttackReady;
    bool m_isPreAttackReady;

    bool m_isStartStage;

    bool m_isJumpDirStretch;

    OBJ_STATE m_ePreState;

    FPOINT m_tPrePos;

};



