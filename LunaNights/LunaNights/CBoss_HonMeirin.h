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


public:
    void Set_Pattern (int _pattern)     { m_iPattern = _pattern;}
    int Get_Pattern ()                  { return m_iPattern; }

private:
    void LoadImages();
    void Jump();
    void Motion_Change();



private:
    bool m_isStartStage;
    bool m_isReady;

    bool m_isPatternPropCalced; // 행동할 방향 등이 정해졌는지를 의미. 패턴 도중에 방향 바꾸고 그러는 걸 방지 위함.
    bool m_isMoveDirStretch;    // 실제로 움직이는 방향. m_isStretch 는 바라보는 방향으로 별개임.

    DWORD m_dwPatternStartTime;
    DWORD m_dwPatternElapsedTime;
    //bool m_isPattern
    int m_iPattern;

    OBJ_STATE m_ePreState;


    FPOINT m_tPrePos;


};