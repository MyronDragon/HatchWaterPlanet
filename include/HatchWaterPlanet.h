#pragma once
#include "syati.h"
#include "Game/LiveActor/LiveActor.h"

class HatchWaterPlanet : public LiveActor {
public:
    HatchWaterPlanet(const char*);

    virtual ~HatchWaterPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeWait();
    void exeOpen();
    void exeWaitAfterOpen();

    LodCtrl* mPlanetLODCtrl;          // 0x8C
    CollisionParts* mCollisionParts;  // 0x90
    TVec3f* mTranslation;             // _14
    bool mPlaySound; // NEW (arg_0)
};

namespace NrvHatchWaterPlanet {
    NERVE(NrvWait);
    NERVE(NrvOpen);
    NERVE(NrvWaitAfterOpen);
};