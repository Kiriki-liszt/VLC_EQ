//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace yg331 {
//------------------------------------------------------------------------
static const Steinberg::FUID kVLC_EQ_ProcessorUID  (0x87336965, 0x79705572, 0x99B67BD0, 0x62809F5C);
static const Steinberg::FUID kVLC_EQ_ControllerUID (0x93E453C5, 0x282C5A91, 0xAED87D5E, 0x79F3B875);

#define VLC_EQ_VST3Category "Fx|EQ"

enum {
    kParamBypass = 0,
    kParamZoom,
    kParamOS,
    kParamFactoryPreset,
    kParamEnable, // soft bypass
    kParam2Pass,
    kParamFreqTable,
    kParamGain_Global,
    kParamGain_Band1,
    kParamGain_Band2,
    kParamGain_Band3,
    kParamGain_Band4,
    kParamGain_Band5,
    kParamGain_Band6,
    kParamGain_Band7,
    kParamGain_Band8,
    kParamGain_Band9,
    kParamGain_Band10,
};

//------------------------------------------------------------------------
} // namespace yg331
