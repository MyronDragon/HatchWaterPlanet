#include "HatchWaterPlanet.h"
#include "Game/LiveActor/LodCtrl.h"
#include "Game/LiveActor/ModelObj.h"
#include "syati.h"
/*
namespace NrvHatchWaterPlanet {
    FULL_NERVE(HatchWaterPlanetNrvWait, HatchWaterPlanet, Wait);
    FULL_NERVE(HatchWaterPlanetNrvOpen, HatchWaterPlanet, Open);                //OLD NERVES DEFINE
    FULL_NERVE(HatchWaterPlanetNrvWaitAfterOpen, HatchWaterPlanet, WaitAfterOpen);
};  // namespace NrvHatchWaterPlanet
*/

	namespace NrvHatchWaterPlanet {
		void NrvWait::execute(Spine *pSpine) const {
			HatchWaterPlanet *pActor = (HatchWaterPlanet*)pSpine->mExecutor;
			pActor->exeWait();
		}

		void NrvOpen::execute(Spine *pSpine) const {
			HatchWaterPlanet *pActor = (HatchWaterPlanet*)pSpine->mExecutor;
			pActor->exeOpen();
		}

		void NrvWaitAfterOpen::execute(Spine *pSpine) const {
			HatchWaterPlanet *pActor = (HatchWaterPlanet*)pSpine->mExecutor;
			pActor->exeWaitAfterOpen();
		}

		NrvWait(NrvWait::sInstance);
		NrvOpen(NrvOpen::sInstance);
		NrvWaitAfterOpen(NrvWaitAfterOpen::sInstance);
	}

HatchWaterPlanet::HatchWaterPlanet(const char* pName) : LiveActor(pName) {
    mPlanetLODCtrl = nullptr;
    mCollisionParts = nullptr;
    mPlaySound = false;
}

void HatchWaterPlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HatchWaterPlanet", nullptr, nullptr, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "HatchWaterPlanetBefore", getSensor(nullptr), nullptr);
    mCollisionParts = MR::createCollisionPartsFromLiveActor(this, "HatchWaterPlanetAfter", getSensor(nullptr), (MR::CollisionScaleType)2);
    MR::invalidateCollisionParts(mCollisionParts);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false, false, false);      //    initSound(4, false);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvHatchWaterPlanet::NrvOpen::sInstance, nullptr);   //MR::registerDemoActionNerve(this, &NrvHatchWaterPlanet::HatchWaterPlanetNrvOpen::sInstance, nullptr);
    }

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::setClippingFarMax(this);
    mPlanetLODCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    mPlanetLODCtrl->validate();
    initNerve(&NrvHatchWaterPlanet::NrvWait::sInstance, 0);        //initNerve(&NrvHatchWaterPlanet::HatchWaterPlanetNrvWait::sInstance);
    makeActorAppeared();
    MR::getJMapInfoArg0NoInit(rIter, &mPlaySound); //NEW
}

void HatchWaterPlanet::control() {
    mPlanetLODCtrl->update();
}

void HatchWaterPlanet::exeWait() {}

void HatchWaterPlanet::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HatchWaterPlanet", nullptr);
        MR::startBtk(this, "HatchWaterPlanet");
        MR::startBck(mPlanetLODCtrl->mModelObjLow, "HatchWaterPlanetLow", nullptr);      //MR::startBck(mPlanetLODCtrl->_14, "HatchWaterPlanetLow", nullptr);
        MR::setBckFrameAndStop(mPlanetLODCtrl->mModelObjLow, 1.0f);                      //        MR::setBckFrameAndStop(mPlanetLODCtrl->_14, 1.0f);
        MR::startSound(this, "SE_OJ_HATCH_WATER_PNT_ST", -1, -1);
    }

    MR::startLevelSound(this, "SE_OJ_LV_HATCH_WATER_PNT_OP", -1, -1, -1);

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_OJ_HATCH_WATER_PNT_ED", -1, -1);
        //MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        setNerve(&NrvHatchWaterPlanet::NrvWaitAfterOpen::sInstance);        //setNerve(&NrvHatchWaterPlanet::HatchWaterPlanetNrvWaitAfterOpen::sInstance);
        if (!mPlaySound)
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);       //new obj arg_0 part
    }
}

void HatchWaterPlanet::exeWaitAfterOpen() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(this);
        MR::validateCollisionParts(mCollisionParts);
    }
}

HatchWaterPlanet::~HatchWaterPlanet() {}
