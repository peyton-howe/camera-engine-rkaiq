/*
 * Copyright (c) 2021-2022 Rockchip Eletronics Co., Ltd.
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
#ifndef _CAM_HW_ISP32_H_
#define _CAM_HW_ISP32_H_

#include "Isp32Params.h"
#include "isp3x/CamHwIsp3x.h"
#include "xcam_common.h"

namespace RkCam {

class FakeCamHwIsp32;

class CamHwIsp32 : virtual public CamHwIsp3x, virtual public Isp32Params {
    friend class FakeCamHwIsp32;

 public:
    explicit CamHwIsp32();
    virtual ~CamHwIsp32();
    virtual XCamReturn init(const char* sns_ent_name);
    virtual XCamReturn stop();
    XCamReturn setIspParamsSync(uint32_t frameId);

 protected:
    virtual XCamReturn setIspConfig();

 private:
    void gen_full_isp_params(const struct isp32_isp_params_cfg* update_params,
                             struct isp32_isp_params_cfg* full_params,
                             uint64_t* module_en_update_partial,
                             uint64_t* module_cfg_update_partial);
    struct isp32_isp_params_cfg _full_active_isp32_params;
};

}  // namespace RkCam

#endif
