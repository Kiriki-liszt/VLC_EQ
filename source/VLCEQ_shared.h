//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/base/futils.h"

#include <cmath>
#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#endif
#ifndef M_1_PI
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#endif

namespace yg331 {
//------------------------------------------------------------------------
using ParamValue = Steinberg::Vst::ParamValue;
using SampleRate = Steinberg::Vst::SampleRate;
using int32      = Steinberg::int32;
using uint32     = Steinberg::uint32;

#define EQZ_IN_FACTOR (0.25)
#define EQZ_BANDS_MAX 10
#define EQZ_CH_MAX 32

typedef struct filter_sys
{
    /* Filter static config */
    int32      i_band = EQZ_BANDS_MAX;
    ParamValue f_alpha [EQZ_BANDS_MAX] = {0, };
    ParamValue f_beta  [EQZ_BANDS_MAX] = {0, };
    ParamValue f_gamma [EQZ_BANDS_MAX] = {0, };

    /* Filter dyn config */
    ParamValue f_amp[EQZ_BANDS_MAX]   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};   /* Per band amp */
    ParamValue f_gamp  = 1.0;   /* Global preamp */
    bool       b_2eqz  = false;

    /* Filter state */
    ParamValue x[EQZ_CH_MAX][2] = {0, };
    ParamValue y[EQZ_CH_MAX][128][2] = {0, };

    /* Second filter state */
    ParamValue x2[EQZ_CH_MAX][2] = {0, };
    ParamValue y2[EQZ_CH_MAX][128][2] = {0, };
} filter_sys_t;

typedef struct eqz_config
{
    int32   i_band = EQZ_BANDS_MAX;
    struct
    {
        ParamValue f_frequency = 0.5;
        ParamValue f_alpha = 0.0;
        ParamValue f_beta  = 0.0;
        ParamValue f_gamma = 0.0;
    } band[EQZ_BANDS_MAX];
} eqz_config_t; // UNUSED

typedef enum {
    overSample_1x,
    overSample_2x,
    overSample_4x,
    overSample_8x,
    overSample_num = 3
} overSample;

#define dB2Gain(db)  (pow(10.0, db * 0.05))
#define Gaib2dB(lin) ((lin>0)?(20.0 * log10(lin)):(-100.0))

#define Plain2Norm(v, min, max) ((v - min) / (max - min))
#define Norm2Plain(v, min, max) (v * (max - min) + min)
    
#define LogPlain2Norm(v, min, max) (std::log(v / min) / std::log(max / min))
#define LogNorm2Plain(v, min, max) (min * std::exp(v * std::log(max / min)))

#define ListPlain2Norm(v, step, type) (Steinberg::ToNormalized<type> (v, step))
#define ListNorm2Plain(v, step, type) (Steinberg::FromNormalized<type> (v, step))

static constexpr ParamValue minGain      = -20.0;
static constexpr ParamValue maxGain      =  20.0;
static constexpr ParamValue dftGain      = 0.0;
static constexpr ParamValue nrmGain      = Plain2Norm(dftGain,  minGain,  maxGain);

static const ParamValue f_vlc_frequency_table_10b[EQZ_BANDS_MAX] =
{
    60, 170, 310, 600, 1000, 3000, 6000, 12000, 14000, 16000,
};

static const ParamValue f_iso_frequency_table_10b[EQZ_BANDS_MAX] =
{
    31.25, 62.5, 125, 250, 500, 1000, 2000, 4000, 8000, 16000,
};

#define NB_PRESETS 18
static const char *const preset_list[NB_PRESETS] = {
    "flat", 
    "classical",
    "club", 
    "dance",
    "full bass",
    "full bass treble",
    "full treble",
    "headphones",
    "large hall",
    "live",
    "party",
    "pop",
    "reggae",
    "rock", 
    "ska",
    "soft",
    "softrock",
    "techno"
};

typedef struct eqz_preset
{
    const char psz_name[32];
    int32      i_band = NB_PRESETS;
    ParamValue f_preamp = 12.0;
    ParamValue f_amp[EQZ_BANDS_MAX];
} eqz_preset_t;

static const eqz_preset_t eqz_preset_10b[NB_PRESETS] =
{
    {
        "flat", 10, 12.0,
        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
    },
    {
        "classical", 10, 12.0,
        { -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, -7.2, -7.2, -7.2, -9.6 }
    },
    {
        "club", 10, 6.0,
        { -1.11022e-15, -1.11022e-15, 8.0, 5.6, 5.6, 5.6, 3.2, -1.11022e-15, -1.11022e-15, -1.11022e-15 }
    },
    {
        "dance", 10, 5.0,
        { 9.6, 7.2, 2.4, -1.11022e-15, -1.11022e-15, -5.6, -7.2, -7.2, -1.11022e-15, -1.11022e-15 }
    },
    {
        "full bass", 10, 5.0,
        { -8.0, 9.6, 9.6, 5.6, 1.6, -4.0, -8.0, -10.4, -11.2, -11.2 }
    },
    {
        "full bass treble", 10, 4.0,
        { 7.2, 5.6, -1.11022e-15, -7.2, -4.8, 1.6, 8.0, 11.2, 12.0, 12.0 }
    },
    {
        "full treble", 10, 3.0,
        { -9.6, -9.6, -9.6, -4.0, 2.4, 11.2, 16.0, 16.0, 16.0, 16.8 }
    },
    {
        "headphones", 10, 4.0,
        { 4.8, 11.2, 5.6, -3.2, -2.4, 1.6, 4.8, 9.6, 12.8, 14.4 }
    },
    {
        "large hall", 10, 5.0,
        { 10.4, 10.4, 5.6, 5.6, -1.11022e-15, -4.8, -4.8, -4.8, -1.11022e-15, -1.11022e-15 }
    },
    {
        "live", 10, 7.0,
        { -4.8, -1.11022e-15, 4.0, 5.6, 5.6, 5.6, 4.0, 2.4, 2.4, 2.4 }
    },
    {
        "party", 10, 6.0,
        { 7.2, 7.2, -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, -1.11022e-15, 7.2, 7.2 }
    },
    {
        "pop", 10, 6.0,
        { -1.6, 4.8, 7.2, 8.0, 5.6, -1.11022e-15, -2.4, -2.4, -1.6, -1.6 }
    },
    {
        "reggae", 10, 8.0,
        { -1.11022e-15, -1.11022e-15, -1.11022e-15, -5.6, -1.11022e-15, 6.4, 6.4, -1.11022e-15, -1.11022e-15, -1.11022e-15 }
    },
    {
        "rock", 10, 5.0,
        { 8.0, 4.8, -5.6, -8.0, -3.2, 4.0, 8.8, 11.2, 11.2, 11.2 }
    },
    {
        "ska", 10, 6.0,
        { -2.4, -4.8, -4.0, -1.11022e-15, 4.0, 5.6, 8.8, 9.6, 11.2, 9.6 }
    },
    {
        "soft", 10, 5.0,
        { 4.8, 1.6, -1.11022e-15, -2.4, -1.11022e-15, 4.0, 8.0, 9.6, 11.2, 12.0 }
    },
    {
        "softrock", 10, 7.0,
        { 4.0, 4.0, 2.4, -1.11022e-15, -4.0, -5.6, -3.2, -1.11022e-15, 2.4, 8.8 }
    },
    {
        "techno", 10, 5.0,
        { 8.0, 5.6, -1.11022e-15, -5.6, -4.8, -1.11022e-15, 8.0, 9.6, 9.6, 8.8 }
    },
};
//------------------------------------------------------------------------
} // namespace yg331
