#pragma once
#include "CObj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Set_DamageAmount(int _amount)      { iDamageAmount = _amount; }
    void Set_DamageType(int _dmgType)       { iDamageType = _dmgType; }
    void Set_StatusType(int _statusType)    { iStatusType = _statusType; }
    void Set_LoopTimes(float _loopTime)     { fLoopTimes = _loopTime; }

    enum DMGTYPE {
        DMG_ATK_PLAYER,
        DMG_ATK_MONSTER,
        DMG_GRAZE_STOP,
        DMG_GRAZE_NORMAL,
        DMG_HP,
        DMG_MP,
        DMG_ALP,
        
        DMG_END
    };

    enum STATUSTYPE {
        STT_DESTROY,
        STT_NOMP,
        STT_MPRECOVERY,
        STT_MPHPRECOVERY,
        STT_GUARD,
        STT_TIMEMAX,
        STT_NOTIME,

        STT_END
    };


private:
    bool isLoop;
    int fLoopTimes;

    int iDamageType;        // 자세한 건 DMGTYPE enum    참고
    int iStatusType;        // 자세한 건 STATUSTYPE enum 참고
    int iDamageAmount;

    DWORD dwStartTime;
};

