//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#pragma once

#include "VLCEQ_shared.h"

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace yg331 {

//------------------------------------------------------------------------
//  VLC_EQ_Processor
//------------------------------------------------------------------------
class VLC_EQ_Processor : public Steinberg::Vst::AudioEffect
{
public:
    VLC_EQ_Processor ();
    ~VLC_EQ_Processor () SMTG_OVERRIDE;

    // Create function
    static Steinberg::FUnknown* createInstance (void* /*context*/) 
    { 
        return (Steinberg::Vst::IAudioProcessor*)new VLC_EQ_Processor; 
    }

    //--- ---------------------------------------------------------------------
    // AudioEffect overrides:
    //--- ---------------------------------------------------------------------
    /** Called at first after constructor */
    Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
    
    /** Called at the end before destructor */
    Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
    
    /** Switch the Plug-in on/off */
    Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

    /** Will be called before any process call */
    Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
    
    /** Asks if a given sample size is supported see SymbolicSampleSizes. */
    Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

    /** Here we go...the process call */
    Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
        
    /** For persistence */
    Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    
    void doPresetChange (ParamValue value) // value is normozed presetParam
    {
        pFactoryPreset = ListNorm2Plain(value, NB_PRESETS - 1, ParamValue); // ListNorm2Plain returns int32
        
        ParamValue plain = eqz_preset_10b[pFactoryPreset].f_preamp;
        pGain_Global = Plain2Norm(plain, minGain, maxGain);
        
        for (int band = 0; band < EQZ_BANDS_MAX; band++)
        {
            ParamValue plain = eqz_preset_10b[pFactoryPreset].f_amp[band];
            pGain_Band[band] = Plain2Norm(plain, minGain, maxGain);
        }
        
        return;
    };

//------------------------------------------------------------------------
protected:
    static inline ParamValue EqzConvertdB( ParamValue db )
    {
        /* Map it to gain,
         * (we do as if the input of iir is /EQZ_IN_FACTOR, but in fact it's the non iir data that is *EQZ_IN_FACTOR)
         * db = 20*log( out / in ) with out = in + amp*iir(i/EQZ_IN_FACTOR)
         * or iir(i) == i for the center freq so
         * db = 20*log( 1 + amp/EQZ_IN_FACTOR )
         * -> amp = EQZ_IN_FACTOR*(10^(db/20) - 1)
         **/

        if     ( db < -20.0 ) db = -20.0;
        else if( db >  20.0 ) db =  20.0;
        
        return EQZ_IN_FACTOR * ( pow( 10.0, db / 20.0 ) - 1.0 );
    }

    void EqzCoeffs (SampleRate    f_rate,
                   ParamValue    f_octave_percent,
                   bool          b_use_vlc_freqs);
    
    template <typename SampleType>
    void processAudio(SampleType** inputs, SampleType** outputs, int32 numChannels, SampleRate sampleRate, int32 sampleFrames);
    
    bool       pBypass = false;
    // ParamValue pZoom; // should be saved at controller state
    // overSample pOS;   // unused
    int32      pFactoryPreset = 0;
    bool       pEnable = false; // StringList is default to '0 : Enabled'
    bool       p2Pass  = false;
    bool       pFreqTable = false;
    ParamValue pGain_Global = Plain2Norm(12.0,  minGain,  maxGain);
    ParamValue pGain_Band[EQZ_BANDS_MAX] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    
    filter_sys_t p_sys;
};

//------------------------------------------------------------------------
} // namespace yg331
