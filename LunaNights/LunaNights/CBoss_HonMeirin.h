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
    bool Get_Stretch()                  { return m_isStretch; }
    void Set_Pattern (int _pattern)     { m_iPattern = _pattern;}
    int Get_Pattern ()                  { return m_iPattern; }

    bool Get_isShown()                  { return m_isShown; }
    void Set_isShown(bool _shown)       { m_isShown = _shown; }

private:
    void LoadImages();
    void Jump();
    void Motion_Change();



private:
    bool m_isStartStage;
    bool m_isReady;

    bool m_isPatternPropCalced; // �ൿ�� ���� ���� ������������ �ǹ�. ���� ���߿� ���� �ٲٰ� �׷��� �� ���� ����.
    bool m_isMoveDirStretch;    // ������ �����̴� ����. m_isStretch �� �ٶ󺸴� �������� ������.
    bool m_isGuard; // Į���� �����̿� �ִ���, GetSet ���� �ݶ��̴��Ŵ������� �Ҵ��ؾ��ҵ�

    bool m_isShown;
    bool m_isShownTrigger;

    bool m_isChangeFrame;

    DWORD m_dwPatternElapsedFrame;
    DWORD m_dwPatternNeedFrame;
    //bool m_isPattern
    int m_iPattern;
    int m_iEndPattern;

    OBJ_STATE m_ePreState;


    FPOINT m_tPrePos;


};