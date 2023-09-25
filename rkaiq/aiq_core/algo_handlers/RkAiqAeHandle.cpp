/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "RkAiqAeHandle.h"
#include "RkAiqAfHandle.h"

#include "RkAiqCore.h"

namespace RkCam {

DEFINE_HANDLE_REGISTER_TYPE(RkAiqAeHandleInt);

void RkAiqAeHandleInt::init() {
    ENTER_ANALYZER_FUNCTION();

    RkAiqHandle::deInit();
    mConfig       = (RkAiqAlgoCom*)(new RkAiqAlgoConfigAe());
    mPreInParam   = (RkAiqAlgoCom*)(new RkAiqAlgoPreAe());
    mPreOutParam  = NULL;
    mProcInParam  = (RkAiqAlgoCom*)(new RkAiqAlgoProcAe());
    mProcOutParam = NULL;
    mPostInParam  = (RkAiqAlgoCom*)(new RkAiqAlgoPostAe());
    mPostOutParam = (RkAiqAlgoResCom*)(new RkAiqAlgoPostResAe());

    EXIT_ANALYZER_FUNCTION();
}

XCamReturn RkAiqAeHandleInt::updateConfig(bool needSync) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifndef DISABLE_HANDLE_ATTRIB
    if (needSync) mCfgMutex.lock();
    // if something changed, api will modify aecCfg in mAlgoCtx
    if (updateExpSwAttr) {
        mCurExpSwAttr   = mNewExpSwAttr;
        rk_aiq_uapi_ae_convExpSwAttr_v1Tov2(&mCurExpSwAttr, &mCurExpSwAttrV2);
        rk_aiq_uapi_ae_setExpSwAttr(mAlgoCtx, &mCurExpSwAttrV2, false, false);
        updateExpSwAttr = false;
        updateAttr |= UPDATE_EXPSWATTR;
        sendSignal();
    }

    if (updateLinExpAttr) {
        mCurLinExpAttr   = mNewLinExpAttr;
        rk_aiq_uapi_ae_convLinExpAttr_v1Tov2(&mCurLinExpAttr, &mCurLinExpAttrV2);
        rk_aiq_uapi_ae_setLinExpAttr(mAlgoCtx, &mCurLinExpAttrV2, false, false);
        updateLinExpAttr = false;
        updateAttr |= UPDATE_LINEXPATTR;
        sendSignal();
    }

    if (updateHdrExpAttr) {
        mCurHdrExpAttr   = mNewHdrExpAttr;
        rk_aiq_uapi_ae_convHdrExpAttr_v1Tov2(&mCurHdrExpAttr, &mCurHdrExpAttrV2);
        rk_aiq_uapi_ae_setHdrExpAttr(mAlgoCtx, &mCurHdrExpAttrV2, false, false);
        updateHdrExpAttr = false;
        updateAttr |= UPDATE_HDREXPATTR;
        sendSignal();
    }

    // TODO: update v2

    if (updateExpSwAttrV2) {
        mCurExpSwAttrV2   = mNewExpSwAttrV2;
        rk_aiq_uapi_ae_setExpSwAttr(mAlgoCtx, &mCurExpSwAttrV2, false, false);
        updateExpSwAttrV2 = false;
        updateAttr |= UPDATE_EXPSWATTR;
        sendSignal(mCurExpSwAttrV2.sync.sync_mode);
    }

    if (updateLinExpAttrV2) {
        mCurLinExpAttrV2   = mNewLinExpAttrV2;
        rk_aiq_uapi_ae_setLinExpAttr(mAlgoCtx, &mCurLinExpAttrV2, false, false);
        updateLinExpAttrV2 = false;
        updateAttr |= UPDATE_LINEXPATTR;
        sendSignal(mCurLinExpAttrV2.sync.sync_mode);
    }

    if (updateHdrExpAttrV2) {
        mCurHdrExpAttrV2   = mNewHdrExpAttrV2;
        rk_aiq_uapi_ae_setHdrExpAttr(mAlgoCtx, &mCurHdrExpAttrV2, false, false);
        updateHdrExpAttrV2 = false;
        updateAttr |= UPDATE_HDREXPATTR;
        sendSignal(mCurHdrExpAttrV2.sync.sync_mode);
    }

    if (updateLinAeRouteAttr) {
        mCurLinAeRouteAttr   = mNewLinAeRouteAttr;
        rk_aiq_uapi_ae_setLinAeRouteAttr(mAlgoCtx, &mCurLinAeRouteAttr, false, false);
        updateLinAeRouteAttr = false;
        updateAttr |= UPDATE_LINAEROUTEATTR;
        sendSignal(mCurLinAeRouteAttr.sync.sync_mode);
    }
    if (updateHdrAeRouteAttr) {
        mCurHdrAeRouteAttr   = mNewHdrAeRouteAttr;
        rk_aiq_uapi_ae_setHdrAeRouteAttr(mAlgoCtx, &mCurHdrAeRouteAttr, false, false);
        updateHdrAeRouteAttr = false;
        updateAttr |= UPDATE_HDRAEROUTEATTR;
        sendSignal(mCurHdrAeRouteAttr.sync.sync_mode);
    }
    if (updateIrisAttr) {
        mCurIrisAttr   = mNewIrisAttr;
        rk_aiq_uapi_ae_setIrisAttr(mAlgoCtx, &mCurIrisAttr, false);
        updateIrisAttr = false;
        updateAttr |= UPDATE_IRISATTR;
        sendSignal(mCurIrisAttr.sync.sync_mode);
    }
    if (updateSyncTestAttr) {
        mCurAecSyncTestAttr = mNewAecSyncTestAttr;
        rk_aiq_uapi_ae_setSyncTest(mAlgoCtx, &mCurAecSyncTestAttr, false, false);
        updateSyncTestAttr  = false;
        updateAttr |= UPDATE_SYNCTESTATTR;
        sendSignal(mCurAecSyncTestAttr.sync.sync_mode);
    }
    if (updateExpWinAttr) {
        mCurExpWinAttr   = mNewExpWinAttr;
        rk_aiq_uapi_ae_setExpWinAttr(mAlgoCtx, &mCurExpWinAttr, false);
        updateExpWinAttr = false;
        updateAttr |= UPDATE_EXPWINATTR;
        sendSignal(mCurExpWinAttr.sync.sync_mode);
    }

    // once any params are changed, run reconfig to convert aecCfg to paectx
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= updateAttr;
    updateAttr = 0;
    if (needSync) mCfgMutex.unlock();
#endif
    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setExpSwAttr(Uapi_ExpSwAttr_t ExpSwAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();
#ifdef DISABLE_HANDLE_ATTRIB
    Uapi_ExpSwAttrV2_t att2;
    rk_aiq_uapi_ae_convExpSwAttr_v1Tov2(&ExpSwAttr, &att2);
    rk_aiq_uapi_ae_setExpSwAttr(mAlgoCtx, &att2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_EXPSWATTR;
#else
    // TODO
    // check if there is different between att & mCurAtt
    // if something changed, set att to mNewAtt, and
    // the new params will be effective later when updateConfig
    // called by RkAiqCore

    // if something changed
    if (0 != memcmp(&mCurExpSwAttr, &ExpSwAttr, sizeof(Uapi_ExpSwAttr_t))) {
        mNewExpSwAttr   = ExpSwAttr;
        updateExpSwAttr = true;
        waitSignal();
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getExpSwAttr(Uapi_ExpSwAttr_t* pExpSwAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttrV2_t ExpSwAttrV2;

    mCfgMutex.lock();

    rk_aiq_uapi_ae_getExpSwAttr(mAlgoCtx, &ExpSwAttrV2, false);
    rk_aiq_uapi_ae_convExpSwAttr_v2Tov1(&ExpSwAttrV2, pExpSwAttr);

    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setExpSwAttr(Uapi_ExpSwAttrV2_t ExpSwAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setExpSwAttr(mAlgoCtx, &ExpSwAttrV2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_EXPSWATTR;
#else
    // check if there is different between att & mCurAtt(sync)/mNewAtt(async)
    // if something changed, set att to mNewAtt, and
    // the new params will be effective later when updateConfig
    // called by RkAiqCore
    bool isChanged = false;
    if (ExpSwAttrV2.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewExpSwAttrV2, &ExpSwAttrV2, sizeof(ExpSwAttrV2)))
        isChanged = true;
    else if (ExpSwAttrV2.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurExpSwAttrV2, &ExpSwAttrV2, sizeof(ExpSwAttrV2)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewExpSwAttrV2   = ExpSwAttrV2;
        updateExpSwAttrV2 = true;
        waitSignal(ExpSwAttrV2.sync.sync_mode);
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getExpSwAttr(Uapi_ExpSwAttrV2_t* pExpSwAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getExpSwAttr(mAlgoCtx, pExpSwAttrV2, false);
    pExpSwAttrV2->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pExpSwAttrV2->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getExpSwAttr(mAlgoCtx, pExpSwAttrV2, false);
        pExpSwAttrV2->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateExpSwAttrV2) {
            memcpy(pExpSwAttrV2, &mNewExpSwAttrV2, sizeof(mNewExpSwAttrV2));
            pExpSwAttrV2->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getExpSwAttr(mAlgoCtx, pExpSwAttrV2, false);
            pExpSwAttrV2->sync.sync_mode = mNewExpSwAttrV2.sync.sync_mode;
            pExpSwAttrV2->sync.done = true;
        }
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setLinExpAttr(Uapi_LinExpAttr_t LinExpAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();
#ifdef DISABLE_HANDLE_ATTRIB
    Uapi_LinExpAttrV2_t att2;
    rk_aiq_uapi_ae_convLinExpAttr_v1Tov2(&LinExpAttr, &att2);
    rk_aiq_uapi_ae_setLinExpAttr(mAlgoCtx, &att2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_LINEXPATTR;
#else
    // TODO
    // check if there is different between att & mCurAtt
    // if something changed, set att to mNewAtt, and
    // the new params will be effective later when updateConfig
    // called by RkAiqCore

    // if something changed
    if (0 != memcmp(& mCurLinExpAttr, &LinExpAttr, sizeof(Uapi_LinExpAttr_t))) {
        mNewLinExpAttr   = LinExpAttr;
        updateLinExpAttr = true;
        waitSignal();
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getLinExpAttr(Uapi_LinExpAttr_t* pLinExpAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    mCfgMutex.lock();
    Uapi_LinExpAttrV2_t att2;
    rk_aiq_uapi_ae_getLinExpAttr(mAlgoCtx, &att2, false);
    rk_aiq_uapi_ae_convLinExpAttr_v2Tov1(&att2, pLinExpAttr);
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setLinExpAttr(Uapi_LinExpAttrV2_t LinExpAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();
#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setLinExpAttr(mAlgoCtx, &LinExpAttrV2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_LINEXPATTR;
#else
    // check if there is different between att & mCurAtt(sync)/mNewAtt(async)
    // if something changed, set att to mNewAtt, and
    // the new params will be effective later when updateConfig
    // called by RkAiqCore
    bool isChanged = false;
    if (LinExpAttrV2.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewLinExpAttrV2, &LinExpAttrV2, sizeof(LinExpAttrV2)))
        isChanged = true;
    else if (LinExpAttrV2.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurLinExpAttrV2, &LinExpAttrV2, sizeof(LinExpAttrV2)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewLinExpAttrV2   = LinExpAttrV2;
        updateLinExpAttrV2 = true;
        waitSignal(LinExpAttrV2.sync.sync_mode);
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getLinExpAttr(Uapi_LinExpAttrV2_t* pLinExpAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getLinExpAttr(mAlgoCtx, pLinExpAttrV2, false);
    pLinExpAttrV2->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pLinExpAttrV2->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getLinExpAttr(mAlgoCtx, pLinExpAttrV2, false);
        pLinExpAttrV2->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateLinExpAttrV2) {
            memcpy(pLinExpAttrV2, &mNewLinExpAttrV2, sizeof(mNewLinExpAttrV2));
            pLinExpAttrV2->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getLinExpAttr(mAlgoCtx, pLinExpAttrV2, false);
            pLinExpAttrV2->sync.sync_mode = mNewLinExpAttrV2.sync.sync_mode;
            pLinExpAttrV2->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setHdrExpAttr(Uapi_HdrExpAttr_t HdrExpAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();
#ifdef DISABLE_HANDLE_ATTRIB
    Uapi_HdrExpAttrV2_t att2;
    rk_aiq_uapi_ae_convHdrExpAttr_v1Tov2(&HdrExpAttr, &att2);
    rk_aiq_uapi_ae_setHdrExpAttr(mAlgoCtx, &att2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_HDREXPATTR;
#else
    // TODO
    // check if there is different between att & mCurAtt
    // if something changed, set att to mNewAtt, and
    // the new params will be effective later when updateConfig
    // called by RkAiqCore

    // if something changed
    if (0 != memcmp(&mCurHdrExpAttr, &HdrExpAttr, sizeof(Uapi_HdrExpAttr_t))) {
        mNewHdrExpAttr   = HdrExpAttr;
        updateHdrExpAttr = true;
        waitSignal();
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getHdrExpAttr(Uapi_HdrExpAttr_t* pHdrExpAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    mCfgMutex.lock();
    Uapi_HdrExpAttrV2_t att2;
    rk_aiq_uapi_ae_getHdrExpAttr(mAlgoCtx, &att2, false);
    rk_aiq_uapi_ae_convHdrExpAttr_v2Tov1(&att2, pHdrExpAttr);
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setHdrExpAttr(Uapi_HdrExpAttrV2_t HdrExpAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setHdrExpAttr(mAlgoCtx, &HdrExpAttrV2, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_HDREXPATTR;
#else
    bool isChanged = false;
    if (HdrExpAttrV2.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewHdrExpAttrV2, &HdrExpAttrV2, sizeof(HdrExpAttrV2)))
        isChanged = true;
    else if (HdrExpAttrV2.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurHdrExpAttrV2, &HdrExpAttrV2, sizeof(HdrExpAttrV2)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewHdrExpAttrV2   = HdrExpAttrV2;
        updateHdrExpAttrV2 = true;
        waitSignal(HdrExpAttrV2.sync.sync_mode);
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getHdrExpAttr(Uapi_HdrExpAttrV2_t* pHdrExpAttrV2) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getHdrExpAttr(mAlgoCtx, pHdrExpAttrV2, false);
    pHdrExpAttrV2->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pHdrExpAttrV2->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getHdrExpAttr(mAlgoCtx, pHdrExpAttrV2, false);
        pHdrExpAttrV2->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateHdrExpAttrV2) {
            memcpy(pHdrExpAttrV2, &mNewHdrExpAttrV2, sizeof(mNewHdrExpAttrV2));
            pHdrExpAttrV2->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getHdrExpAttr(mAlgoCtx, pHdrExpAttrV2, false);
            pHdrExpAttrV2->sync.sync_mode = mNewHdrExpAttrV2.sync.sync_mode;
            pHdrExpAttrV2->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setLinAeRouteAttr(Uapi_LinAeRouteAttr_t LinAeRouteAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setLinAeRouteAttr(mAlgoCtx, &LinAeRouteAttr, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_LINAEROUTEATTR;
#else
    bool isChanged = false;
    if (LinAeRouteAttr.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewLinAeRouteAttr, &LinAeRouteAttr, sizeof(LinAeRouteAttr)))
        isChanged = true;
    else if (LinAeRouteAttr.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurLinAeRouteAttr, &LinAeRouteAttr, sizeof(LinAeRouteAttr)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewLinAeRouteAttr   = LinAeRouteAttr;
        updateLinAeRouteAttr = true;
        waitSignal(LinAeRouteAttr.sync.sync_mode);
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getLinAeRouteAttr(Uapi_LinAeRouteAttr_t* pLinAeRouteAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getLinAeRouteAttr(mAlgoCtx, pLinAeRouteAttr, false);
    pLinAeRouteAttr->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pLinAeRouteAttr->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getLinAeRouteAttr(mAlgoCtx, pLinAeRouteAttr, false);
        pLinAeRouteAttr->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateLinAeRouteAttr) {
            memcpy(pLinAeRouteAttr, &mNewLinAeRouteAttr, sizeof(mNewLinAeRouteAttr));
            pLinAeRouteAttr->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getLinAeRouteAttr(mAlgoCtx, pLinAeRouteAttr, false);
            pLinAeRouteAttr->sync.sync_mode = mNewLinAeRouteAttr.sync.sync_mode;
            pLinAeRouteAttr->sync.done      = true;
        }
    }
#endif
    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setHdrAeRouteAttr(Uapi_HdrAeRouteAttr_t HdrAeRouteAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setHdrAeRouteAttr(mAlgoCtx, &HdrAeRouteAttr, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_HDRAEROUTEATTR;
#else
    bool isChanged = false;
    if (HdrAeRouteAttr.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewHdrAeRouteAttr, &HdrAeRouteAttr, sizeof(HdrAeRouteAttr)))
        isChanged = true;
    else if (HdrAeRouteAttr.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurHdrAeRouteAttr, &HdrAeRouteAttr, sizeof(HdrAeRouteAttr)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewHdrAeRouteAttr   = HdrAeRouteAttr;
        updateHdrAeRouteAttr = true;
        waitSignal(HdrAeRouteAttr.sync.sync_mode);
    }
#endif
    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getHdrAeRouteAttr(Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getHdrAeRouteAttr(mAlgoCtx, pHdrAeRouteAttr, false);
    pHdrAeRouteAttr->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pHdrAeRouteAttr->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getHdrAeRouteAttr(mAlgoCtx, pHdrAeRouteAttr, false);
        pHdrAeRouteAttr->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateHdrAeRouteAttr) {
            memcpy(pHdrAeRouteAttr, &mNewHdrAeRouteAttr, sizeof(mNewHdrAeRouteAttr));
            pHdrAeRouteAttr->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getHdrAeRouteAttr(mAlgoCtx, pHdrAeRouteAttr, false);
            pHdrAeRouteAttr->sync.sync_mode = mNewHdrAeRouteAttr.sync.sync_mode;
            pHdrAeRouteAttr->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setIrisAttr(Uapi_IrisAttrV2_t IrisAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setIrisAttr(mAlgoCtx, &IrisAttr, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_IRISATTR;
#else
    bool isChanged = false;
    if (IrisAttr.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewIrisAttr, &IrisAttr, sizeof(IrisAttr)))
        isChanged = true;
    else if (IrisAttr.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurIrisAttr, &IrisAttr, sizeof(IrisAttr)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewIrisAttr   = IrisAttr;
        updateIrisAttr = true;
        waitSignal(IrisAttr.sync.sync_mode);
    }
#endif

    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getIrisAttr(Uapi_IrisAttrV2_t* pIrisAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getIrisAttr(mAlgoCtx, pIrisAttr);
    pIrisAttr->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pIrisAttr->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getIrisAttr(mAlgoCtx, pIrisAttr);
        pIrisAttr->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateIrisAttr) {
            memcpy(pIrisAttr, &mNewIrisAttr, sizeof(mNewIrisAttr));
            pIrisAttr->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getIrisAttr(mAlgoCtx, pIrisAttr);
            pIrisAttr->sync.sync_mode = mNewIrisAttr.sync.sync_mode;
            pIrisAttr->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setSyncTestAttr(Uapi_AecSyncTest_t SyncTestAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setSyncTest(mAlgoCtx, &SyncTestAttr, false, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_SYNCTESTATTR;
#else
    bool isChanged = false;
    if (SyncTestAttr.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewAecSyncTestAttr, &SyncTestAttr, sizeof(SyncTestAttr)))
        isChanged = true;
    else if (SyncTestAttr.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurAecSyncTestAttr, &SyncTestAttr, sizeof(SyncTestAttr)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewAecSyncTestAttr = SyncTestAttr;
        updateSyncTestAttr  = true;
        waitSignal(SyncTestAttr.sync.sync_mode);
    }
#endif

    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getSyncTestAttr(Uapi_AecSyncTest_t* pSyncTestAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getSyncTest(mAlgoCtx, pSyncTestAttr, false);
    pSyncTestAttr->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pSyncTestAttr->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getSyncTest(mAlgoCtx, pSyncTestAttr, false);
        pSyncTestAttr->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateSyncTestAttr) {
            memcpy(pSyncTestAttr, &mNewAecSyncTestAttr, sizeof(mNewAecSyncTestAttr));
            pSyncTestAttr->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getSyncTest(mAlgoCtx, pSyncTestAttr, false);
            pSyncTestAttr->sync.sync_mode = mNewAecSyncTestAttr.sync.sync_mode;
            pSyncTestAttr->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setExpWinAttr(Uapi_ExpWin_t ExpWinAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mCfgMutex.lock();

#ifdef DISABLE_HANDLE_ATTRIB
    rk_aiq_uapi_ae_setExpWinAttr(mAlgoCtx, &ExpWinAttr, false);
    AeInstanceConfig_t* pAeInstConfig = (AeInstanceConfig_t*)mAlgoCtx;
    AeConfig_t pAecCfg                = pAeInstConfig->aecCfg;
    pAecCfg->IsReconfig |= UPDATE_EXPWINATTR;
#else
    bool isChanged = false;
    if (ExpWinAttr.sync.sync_mode == RK_AIQ_UAPI_MODE_ASYNC && \
            memcmp(&mNewExpWinAttr, &ExpWinAttr, sizeof(ExpWinAttr)))
        isChanged = true;
    else if (ExpWinAttr.sync.sync_mode != RK_AIQ_UAPI_MODE_ASYNC && \
             memcmp(&mCurExpWinAttr, &ExpWinAttr, sizeof(ExpWinAttr)))
        isChanged = true;

    // if something changed
    if (isChanged) {
        mNewExpWinAttr   = ExpWinAttr;
        updateExpWinAttr = true;
        waitSignal(ExpWinAttr.sync.sync_mode);
    }
#endif

    mCfgMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getExpWinAttr(Uapi_ExpWin_t* pExpWinAttr) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

#ifdef DISABLE_HANDLE_ATTRIB
    mCfgMutex.lock();
    rk_aiq_uapi_ae_getExpWinAttr(mAlgoCtx, pExpWinAttr);
    pExpWinAttr->sync.done = true;
    mCfgMutex.unlock();
#else
    if (pExpWinAttr->sync.sync_mode == RK_AIQ_UAPI_MODE_SYNC) {
        mCfgMutex.lock();
        rk_aiq_uapi_ae_getExpWinAttr(mAlgoCtx, pExpWinAttr);
        pExpWinAttr->sync.done = true;
        mCfgMutex.unlock();
    } else {
        if (updateExpWinAttr) {
            memcpy(pExpWinAttr, &mNewExpWinAttr, sizeof(mNewExpWinAttr));
            pExpWinAttr->sync.done = false;
        } else {
            rk_aiq_uapi_ae_getExpWinAttr(mAlgoCtx, pExpWinAttr);
            pExpWinAttr->sync.sync_mode = mNewExpWinAttr.sync.sync_mode;
            pExpWinAttr->sync.done      = true;
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::queryExpInfo(Uapi_ExpQueryInfo_t* pExpQueryInfo) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_aiq_uapi_ae_queryExpInfo(mAlgoCtx, pExpQueryInfo, false);

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::setLockAeForAf(bool lock_ae) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mLockAebyAfMutex.lock();
    lockaebyaf = lock_ae;
    mLockAebyAfMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::getAfdResForAE(AfdPeakRes_t AfdRes) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    mGetAfdResMutex.lock();
    mAfdRes = AfdRes;
    mGetAfdResMutex.unlock();

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::prepare() {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    ret = RkAiqHandle::prepare();
    RKAIQCORE_CHECK_RET(ret, "ae handle prepare failed");

    RkAiqAlgoConfigAe* ae_config_int = (RkAiqAlgoConfigAe*)mConfig;
    RkAiqCore::RkAiqAlgosGroupShared_t* shared =
        (RkAiqCore::RkAiqAlgosGroupShared_t*)(getGroupShared());
    RkAiqCore::RkAiqAlgosComShared_t* sharedCom = &mAiqCore->mAlogsComSharedParams;

    // TODO config ae common params:

    /*****************AecConfig Sensor Exp related info*****************/
    ae_config_int->LinePeriodsPerField = (float)sharedCom->snsDes.frame_length_lines;
    ae_config_int->PixelPeriodsPerLine = (float)sharedCom->snsDes.line_length_pck;
    ae_config_int->PixelClockFreqMHZ   = (float)sharedCom->snsDes.pixel_clock_freq_mhz;

    /*****************AecConfig pic-info params*****************/
    ae_config_int->RawWidth  = sharedCom->snsDes.isp_acq_width;
    ae_config_int->RawHeight = sharedCom->snsDes.isp_acq_height;
    ae_config_int->nr_switch = sharedCom->snsDes.nr_switch;

    RkAiqAlgoDescription* des = (RkAiqAlgoDescription*)mDes;
    ret                       = des->prepare(mConfig);
    RKAIQCORE_CHECK_RET(ret, "ae algo prepare failed");

    EXIT_ANALYZER_FUNCTION();
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn RkAiqAeHandleInt::preProcess() {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    RkAiqAlgoPreAe* ae_pre_int        = (RkAiqAlgoPreAe*)mPreInParam;
    bool postMsg = true;
    RkAiqCore::RkAiqAlgosGroupShared_t* shared =
        (RkAiqCore::RkAiqAlgosGroupShared_t*)(getGroupShared());
    RkAiqCore::RkAiqAlgosComShared_t* sharedCom = &mAiqCore->mAlogsComSharedParams;

    RkAiqAecStats* xAecStats = nullptr;
    if (shared->aecStatsBuf) {
        xAecStats = (RkAiqAecStats*)shared->aecStatsBuf->map(shared->aecStatsBuf);
        if (!xAecStats) LOGE_AEC("aec stats is null");
    } else {
        LOGW_AEC("the xcamvideobuffer of aec stats is null");
    }
    if ((!xAecStats || !xAecStats->aec_stats_valid) && !sharedCom->init) {
        LOGW("no aec stats, ignore!");
        mPreResShared = NULL;
        mProcResShared = NULL;
        return XCAM_RETURN_BYPASS;
    }

    mPreResShared = new RkAiqAlgoPreResAeIntShared();
    if (!mPreResShared.ptr()) {
        LOGE("new ae mPreOutParam failed, bypass!");
        mProcResShared = NULL;
        return XCAM_RETURN_BYPASS;
    }

    ret = RkAiqHandle::preProcess();
    if (ret < 0) {
        LOGE_ANALYZER("ae handle preProcess failed ret %d", ret);
        mPreResShared = NULL;
        mProcResShared = NULL;
        return ret;
    } else if (ret == XCAM_RETURN_BYPASS) {
        LOGW_ANALYZER("%s:%d bypass !", __func__, __LINE__);
        mPreResShared = NULL;
        mProcResShared = NULL;
        return ret;
    }

    ae_pre_int->aecStatsBuf = shared->aecStatsBuf;


    int algoId = this->getAlgoId();
    if (algoId == 0) {
        AeInstanceConfig_t* pAeInstConfig           = (AeInstanceConfig_t*)mAlgoCtx;

        mGetAfdResMutex.lock();
        pAeInstConfig->aecCfg->AfdRes = mAfdRes;
        mGetAfdResMutex.unlock();
    }

    RkAiqAlgoDescription* des = (RkAiqAlgoDescription*)mDes;
    if (des->pre_process) {
#ifdef DISABLE_HANDLE_ATTRIB
        mCfgMutex.lock();
#endif
        ret = des->pre_process(mPreInParam, (RkAiqAlgoResCom*)(&mPreResShared->result));
#ifdef DISABLE_HANDLE_ATTRIB
        mCfgMutex.unlock();
#endif
        if (ret < 0) {
            LOGE_ANALYZER("ae handle pre_process failed ret %d", ret);
            mPreResShared = NULL;
            mProcResShared = NULL;
            return ret;
        } else if (ret == XCAM_RETURN_BYPASS) {
            LOGW_ANALYZER("%s:%d bypass !", __func__, __LINE__);
            mPreResShared = NULL;
            mProcResShared = NULL;
            return ret;
        }
    }

    if (mPostShared && !mAiqCore->mAlogsComSharedParams.init) {
        SmartPtr<BufferProxy> msg_data = new BufferProxy(mPreResShared);
        msg_data->set_sequence(shared->frameId);
        SmartPtr<XCamMessage> msg =
            new RkAiqCoreVdBufMsg(XCAM_MESSAGE_AE_PRE_RES_OK, shared->frameId, msg_data);
        mAiqCore->post_message(msg);
    }

    EXIT_ANALYZER_FUNCTION();
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn RkAiqAeHandleInt::processing() {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    RkAiqAlgoProcAe* ae_proc_int        = (RkAiqAlgoProcAe*)mProcInParam;
    bool postMsg = true;
    RkAiqCore::RkAiqAlgosGroupShared_t* shared =
        (RkAiqCore::RkAiqAlgosGroupShared_t*)(getGroupShared());
    RkAiqCore::RkAiqAlgosComShared_t* sharedCom = &mAiqCore->mAlogsComSharedParams;

    RkAiqAecStats* xAecStats = nullptr;
    if (shared->aecStatsBuf) {
        xAecStats = (RkAiqAecStats*)shared->aecStatsBuf->map(shared->aecStatsBuf);
        if (!xAecStats) LOGE_AEC("aec stats is null");
    } else {
        LOGW_AEC("the xcamvideobuffer of aec stats is null");
    }
    if ((!xAecStats || !xAecStats->aec_stats_valid) && !sharedCom->init) {
        LOGW("no aec stats, ignore!");
        mPreResShared = NULL;
        mProcResShared = NULL;
        return XCAM_RETURN_BYPASS;
    }

    mProcResShared = new RkAiqAlgoProcResAeIntShared();
    if (!mProcResShared.ptr()) {
        LOGE("new ae mProcOutParam failed, bypass!");
        mPreResShared = NULL;
        return XCAM_RETURN_BYPASS;
    }

    int algoId = this->getAlgoId();
    if (algoId == 0) {
        AeInstanceConfig_t* pAeInstConfig           = (AeInstanceConfig_t*)mAlgoCtx;

        mLockAebyAfMutex.lock();
        pAeInstConfig->lockaebyaf = lockaebyaf;
        mLockAebyAfMutex.unlock();
    }

    ret = RkAiqHandle::processing();
    if (ret < 0) {
        LOGE_ANALYZER("ae handle processing failed ret %d", ret);
        mPreResShared = NULL;
        mProcResShared = NULL;
        return ret;
    } else if (ret == XCAM_RETURN_BYPASS) {
        LOGW_ANALYZER("%s:%d bypass !", __func__, __LINE__);
        mPreResShared = NULL;
        mProcResShared = NULL;
        return ret;
    }

    // TODO config common ae processing params
    ae_proc_int->aecStatsBuf = shared->aecStatsBuf;

    RkAiqAlgoDescription* des = (RkAiqAlgoDescription*)mDes;
    if (des->processing) {
#ifdef DISABLE_HANDLE_ATTRIB
        mCfgMutex.lock();
#endif
        ret = des->processing(mProcInParam, (RkAiqAlgoResCom*)(&mProcResShared->result));
#ifdef DISABLE_HANDLE_ATTRIB
        mCfgMutex.unlock();
#endif
        if (ret < 0) {
            LOGE_ANALYZER("ae algo processing failed ret %d", ret);
            mPreResShared = NULL;
            mProcResShared = NULL;
            return ret;
        } else if (ret == XCAM_RETURN_BYPASS) {
            LOGW_ANALYZER("%s:%d bypass !", __func__, __LINE__);
            mPreResShared = NULL;
            mProcResShared = NULL;
            return ret;
        }
    }

    if (mAiqCore->mAlogsComSharedParams.init) {
        RkAiqCore::RkAiqAlgosGroupShared_t* measGroupshared = nullptr;
        if (mAiqCore->getGroupSharedParams(RK_AIQ_CORE_ANALYZE_MEAS, measGroupshared) !=
                XCAM_RETURN_NO_ERROR) {
            LOGW("get the shared of meas failed");
        }
        if (measGroupshared) {
            measGroupshared->frameId                 = shared->frameId;
        }

        /* Transfer the initial exposure to other algorithm modules */
        for (auto type = RK_AIQ_CORE_ANALYZE_MEAS; type < RK_AIQ_CORE_ANALYZE_MAX; \
                type = (rk_aiq_core_analyze_type_e)(type + 1)) {
            uint64_t grpMask = grpId2GrpMask(type);
            if (!mAiqCore->getGroupSharedParams(grpMask, shared)) {
                if (shared) {
                    shared->curExp.LinearExp = mProcResShared->result.new_ae_exp.LinearExp;
                    memcpy(shared->curExp.HdrExp, mProcResShared->result.new_ae_exp.HdrExp,
                           sizeof(mProcResShared->result.new_ae_exp.HdrExp));
                }
            }

        }
    } else {
        if (mPostShared) {
            SmartPtr<BufferProxy> msg_data = new BufferProxy(mProcResShared);
            msg_data->set_sequence(shared->frameId);
            SmartPtr<XCamMessage> msg =
                new RkAiqCoreVdBufMsg(XCAM_MESSAGE_AE_PROC_RES_OK, shared->frameId, msg_data);
            mAiqCore->post_message(msg);
        }
    }

#if RKAIQ_HAVE_AF
    SmartPtr<RkAiqHandle>* af_handle = mAiqCore->getCurAlgoTypeHandle(RK_AIQ_ALGO_TYPE_AF);
    int algo_id                      = (*af_handle)->getAlgoId();

    if (af_handle) {
        if (algo_id == 0) {
            RkAiqAfHandleInt* af_algo = dynamic_cast<RkAiqAfHandleInt*>(af_handle->ptr());
            RkAiqAlgoProcResAe* ae_res = &mProcResShared->result;

            af_algo->setAeStable(ae_res->ae_proc_res_rk.IsConverged);
        }
    }
#endif

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::postProcess() {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    RkAiqAlgoPostAe* ae_post_int        = (RkAiqAlgoPostAe*)mPostInParam;
    RkAiqAlgoPostResAe* ae_post_res_int = (RkAiqAlgoPostResAe*)mPostOutParam;
    RkAiqCore::RkAiqAlgosGroupShared_t* shared =
        (RkAiqCore::RkAiqAlgosGroupShared_t*)(getGroupShared());
    RkAiqCore::RkAiqAlgosComShared_t* sharedCom = &mAiqCore->mAlogsComSharedParams;

    RkAiqAecStats* xAecStats = nullptr;
    if (shared->aecStatsBuf) {
        xAecStats = (RkAiqAecStats*)shared->aecStatsBuf->map(shared->aecStatsBuf);
        if (!xAecStats) LOGE_AEC("aec stats is null");
    } else {
        LOGW_AEC("the xcamvideobuffer of aec stats is null");
    }
    if ((!xAecStats || !xAecStats->aec_stats_valid) && !sharedCom->init) {
        LOGW("no aec stats, ignore!");
        mPreResShared = NULL;
        mProcResShared = NULL;
        return XCAM_RETURN_BYPASS;
    }
    ret = RkAiqHandle::postProcess();
    if (ret) {
        RKAIQCORE_CHECK_RET(ret, "ae handle postProcess failed");
        mPreResShared = NULL;
        mProcResShared = NULL;
        return ret;
    }

    RkAiqAlgoDescription* des = (RkAiqAlgoDescription*)mDes;
    if (des->post_process)
        ret                       = des->post_process(mPostInParam, mPostOutParam);
    RKAIQCORE_CHECK_RET(ret, "ae algo post_process failed");

    EXIT_ANALYZER_FUNCTION();
    return ret;
}

XCamReturn RkAiqAeHandleInt::genIspResult(RkAiqFullParams* params, RkAiqFullParams* cur_params) {
    ENTER_ANALYZER_FUNCTION();

    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    RkAiqCore::RkAiqAlgosGroupShared_t* shared =
        (RkAiqCore::RkAiqAlgosGroupShared_t*)(getGroupShared());
    RkAiqCore::RkAiqAlgosComShared_t* sharedCom = &mAiqCore->mAlogsComSharedParams;
    if (!mProcResShared.ptr()) {
        params->mExposureParams = NULL;
        params->mAecParams      = cur_params->mAecParams;
        params->mAecParams->data()->frame_id = shared->frameId;
        params->mHistParams     = cur_params->mHistParams;
        params->mHistParams->data()->frame_id = shared->frameId;
        return XCAM_RETURN_NO_ERROR;
    }
    RkAiqAlgoProcResAe* ae_proc                 = &mProcResShared->result;
    RkAiqAlgoPostResAe* ae_post                 = (RkAiqAlgoPostResAe*)mPostOutParam;
    if (!ae_proc) {
        mPreResShared = NULL;
        mProcResShared = NULL;
        params->mExposureParams = NULL;
        params->mAecParams      = cur_params->mAecParams;
        params->mAecParams->data()->frame_id = shared->frameId;
        params->mHistParams     = cur_params->mHistParams;
        params->mHistParams->data()->frame_id = shared->frameId;
        LOGD_ANALYZER("no ae_proc result");
        return XCAM_RETURN_NO_ERROR;
    }

    if (!ae_post) {
        mPreResShared = NULL;
        mProcResShared = NULL;
        params->mExposureParams = NULL;
        params->mAecParams      = cur_params->mAecParams;
        params->mAecParams->data()->frame_id = shared->frameId;
        params->mHistParams     = cur_params->mHistParams;
        params->mHistParams->data()->frame_id = shared->frameId;
        LOGD_ANALYZER("no ae_post result");
        return XCAM_RETURN_NO_ERROR;
    }

    rk_aiq_isp_aec_params_v20_t* aec_param   = params->mAecParams->data().ptr();
    rk_aiq_isp_hist_params_v20_t* hist_param = params->mHistParams->data().ptr();

    SmartPtr<rk_aiq_exposure_params_wrapper_t> exp_param = params->mExposureParams->data();
    SmartPtr<rk_aiq_iris_params_wrapper_t> iris_param    = params->mIrisParams->data();

    int algo_id = this->getAlgoId();

    exp_param->aecExpInfo.LinearExp = ae_proc->new_ae_exp.LinearExp;
    memcpy(exp_param->aecExpInfo.HdrExp, ae_proc->new_ae_exp.HdrExp,
           sizeof(ae_proc->new_ae_exp.HdrExp));
    if (ae_proc->exp_i2c_params.bValid) {
        exp_param->exp_i2c_params = (RKAiqExpI2cParam_t*)malloc(sizeof(RKAiqExpI2cParam_t));
        memcpy(exp_param->exp_i2c_params, &ae_proc->exp_i2c_params, sizeof(RKAiqExpI2cParam_t));
    } else {
        exp_param->exp_i2c_params = NULL;
    }
    exp_param->aecExpInfo.frame_length_lines   = ae_proc->new_ae_exp.frame_length_lines;
    exp_param->aecExpInfo.line_length_pixels   = ae_proc->new_ae_exp.line_length_pixels;
    exp_param->aecExpInfo.pixel_clock_freq_mhz = ae_proc->new_ae_exp.pixel_clock_freq_mhz;
    exp_param->aecExpInfo.Iris.PIris           = ae_proc->new_ae_exp.Iris.PIris;

    // TODO Merge
    // iris_param->IrisType = ae_proc->new_ae_exp.Iris.IrisType;
    iris_param->PIris.step   = ae_proc->new_ae_exp.Iris.PIris.step;
    iris_param->PIris.update = ae_proc->new_ae_exp.Iris.PIris.update;

#if 0
    isp_param->aec_meas = ae_proc->ae_meas;
    isp_param->hist_meas = ae_proc->hist_meas;
#else
    if (sharedCom->init) {
        aec_param->frame_id  = 0;
        hist_param->frame_id = 0;
        exp_param->frame_id  = 0;
    } else {
        aec_param->frame_id  = shared->frameId;
        hist_param->frame_id = shared->frameId;
        exp_param->frame_id  = shared->frameId;
    }

    aec_param->result  = ae_proc->ae_meas;
    hist_param->result = ae_proc->hist_meas;
#endif

    RkAiqAlgoProcResAe* ae_rk = (RkAiqAlgoProcResAe*)ae_proc;
    if (ae_rk->ae_proc_res_rk.exp_set_cnt > 0) {
        int sizes = ae_rk->ae_proc_res_rk.exp_set_cnt * sizeof(RKAiqAecExpInfo_t);
        exp_param->exp_tbl = (RKAiqAecExpInfo_t*)malloc(sizes);
        memcpy(exp_param->exp_tbl, ae_rk->ae_proc_res_rk.exp_set_tbl, sizes);
        exp_param->exp_tbl_size = ae_rk->ae_proc_res_rk.exp_set_cnt;
        LOGD_AEC("frame_id:%d, malloc exp_tbl: %p\n", shared->frameId, exp_param->exp_tbl);
    } else {
        exp_param->exp_tbl_size = 0;
        exp_param->exp_tbl = NULL;
    }
    exp_param->algo_id      = 0;//algo_id;

    if (algo_id == 0) {
        RkAiqAlgoPostResAe* ae_post_rk = (RkAiqAlgoPostResAe*)ae_post;
        // HDC iris control
        iris_param->DCIris.update = ae_post_rk->ae_post_res_rk.DCIris.update;
        iris_param->DCIris.pwmDuty = ae_post_rk->ae_post_res_rk.DCIris.pwmDuty;
        // HDC iris control
        iris_param->HDCIris.update = ae_post_rk->ae_post_res_rk.HDCIris.update;
        iris_param->HDCIris.target = ae_post_rk->ae_post_res_rk.HDCIris.target;
        ae_post_rk->ae_post_res_rk.HDCIris.adc = iris_param->HDCIris.adc;
        ae_post_rk->ae_post_res_rk.HDCIris.zoomPos = iris_param->HDCIris.zoomPos;
    }

    cur_params->mExposureParams = params->mExposureParams;
    cur_params->mAecParams      = params->mAecParams;
    cur_params->mHistParams     = params->mHistParams;

    mPreResShared = NULL;
    mProcResShared = NULL;

    EXIT_ANALYZER_FUNCTION();

    return ret;
}

}  // namespace RkCam
