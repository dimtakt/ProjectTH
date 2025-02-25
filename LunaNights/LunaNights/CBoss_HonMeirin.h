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

    bool m_isPatternPropCalced; // �ൿ�� ���� ���� ������������ �ǹ�. ���� ���߿� ���� �ٲٰ� �׷��� �� ���� ����.
    bool m_isMoveDirStretch;    // ������ �����̴� ����. m_isStretch �� �ٶ󺸴� �������� ������.

    DWORD m_dwPatternStartTime;
    DWORD m_dwPatternElapsedTime;
    //bool m_isPattern
    int m_iPattern;

    OBJ_STATE m_ePreState;


    FPOINT m_tPrePos;


};