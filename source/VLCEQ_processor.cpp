//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#include "VLCEQ_processor.h"
#include "VLCEQ_cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"
#include "public.sdk/source/vst/vsthelpers.h"

using namespace Steinberg;

namespace yg331 {
//------------------------------------------------------------------------
// VLC_EQ_Processor
//------------------------------------------------------------------------
VLC_EQ_Processor::VLC_EQ_Processor ()
{
    //--- set the wanted controller for our processor
    setControllerClass (kVLC_EQ_ControllerUID);
}

//------------------------------------------------------------------------
VLC_EQ_Processor::~VLC_EQ_Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::initialize (FUnknown* context)
{
    // Here the Plug-in will be instantiated
    
    //---always initialize the parent-------
    tresult result = AudioEffect::initialize (context);
    // if everything Ok, continue
    if (result != kResultOk)
    {
        return result;
    }

    //--- create Audio IO ------
    addAudioInput  (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
    addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

    /* If you don't need an event bus, you can remove the next line */
    // addEventInput (STR16 ("Event In"), 1);

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::terminate ()
{
    // Here the Plug-in will be de-instantiated, last possibility to remove some memory!
    
    //---do not forget to call parent ------
    return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::setActive (TBool state)
{
    //--- called when the Plug-in is enable/disable (On/Off) -----
    return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::process (Vst::ProcessData& data)
{
    Vst::IParameterChanges* paramChanges = data.inputParameterChanges;

    if (paramChanges)
    {
        int32 numParamsChanged = paramChanges->getParameterCount();

        for (int32 index = 0; index < numParamsChanged; index++)
        {
            Vst::IParamValueQueue* paramQueue = paramChanges->getParameterData(index);

            if (paramQueue)
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount();

                if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
                    switch (paramQueue->getParameterId()) {
                        case kParamBypass:        pBypass        = (value > 0.5); break;
                        // case kParamZoom:          pZoom          = value; break;
                        // case kParamOS:            pOS            = static_cast<overSample>(ListNorm2Plain(value, overSample_num, ParamValue)); break;
                        case kParamFactoryPreset: doPresetChange(value); break;
                        case kParamEnable:        pEnable        = (value > 0.5); break;
                        case kParam2Pass:         p2Pass         = (value > 0.5); break;
                        case kParamFreqTable:     pFreqTable     = (value > 0.5); break;
                        case kParamGain_Global:   pGain_Global   = value; break;
                        case kParamGain_Band1:    pGain_Band[0]  = value; break;
                        case kParamGain_Band2:    pGain_Band[1]  = value; break;
                        case kParamGain_Band3:    pGain_Band[2]  = value; break;
                        case kParamGain_Band4:    pGain_Band[3]  = value; break;
                        case kParamGain_Band5:    pGain_Band[4]  = value; break;
                        case kParamGain_Band6:    pGain_Band[5]  = value; break;
                        case kParamGain_Band7:    pGain_Band[6]  = value; break;
                        case kParamGain_Band8:    pGain_Band[7]  = value; break;
                        case kParamGain_Band9:    pGain_Band[8]  = value; break;
                        case kParamGain_Band10:   pGain_Band[9]  = value; break;
                        default: break;
                    }
                }
            }
        }
    }

    if (data.numInputs == 0 || data.numOutputs == 0)
    {
        // nothing to do
        return kResultOk;
    }
    
    // (simplification) we suppose in this example that we have the same input channel count than the output
    int32 numChannels = data.inputs[0].numChannels;

    //---get audio buffers----------------
    uint32 sampleFramesSize = Vst::getSampleFramesSizeInBytes(processSetup, data.numSamples);
    void** in  = getChannelBuffersPointer(processSetup, data.inputs[0]);
    void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);
    Vst::SampleRate SampleRate = processSetup.sampleRate;
    
    
    
    // convert normalized to plain
    p_sys.f_gamp = dB2Gain(Norm2Plain(pGain_Global,  minGain,  maxGain));
    for (int32 band = 0; band < p_sys.i_band; band++)
        p_sys.f_amp[band] = EqzConvertdB(Norm2Plain(pGain_Band[band],  minGain,  maxGain));
    p_sys.b_2eqz = p2Pass;
    
    // apply parameter
    EqzCoeffs(SampleRate, 1.0, pFreqTable);

    //---check if silence---------------
    // check if all channel are silent then process silent
    if (data.inputs[0].silenceFlags == Vst::getChannelMask(data.inputs[0].numChannels))
    {
        // mark output silence too (it will help the host to propagate the silence)
        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;

        // the plug-in has to be sure that if it sets the flags silence that the output buffer are
        // clear
        for (int32 i = 0; i < numChannels; i++)
        {
            // do not need to be cleared if the buffers are the same (in this case input buffer are
            // already cleared by the host)
            if (in[i] != out[i])
            {
                memset(out[i], 0, sampleFramesSize);
            }
        }
    }
    else {

        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
        //---in bypass mode outputs should be like inputs-----
        if (pBypass)
        {
            for (int32 channel = 0; channel < numChannels; channel++)
            {
                memcpy (out[channel], in[channel], sampleFramesSize);
            }
        }
        else
        {
            if (data.symbolicSampleSize == Vst::kSample32) {
                processAudio<Vst::Sample32>((Vst::Sample32**)in, (Vst::Sample32**)out, numChannels, SampleRate, data.numSamples);
            }
            else {
                processAudio<Vst::Sample64>((Vst::Sample64**)in, (Vst::Sample64**)out, numChannels, SampleRate, data.numSamples);
            }
        }
    }

    return kResultOk;
}


template <typename SampleType>
void VLC_EQ_Processor::processAudio(SampleType** inputs, SampleType** outputs, int32 numChannels, SampleRate sampleRate, int32 sampleFrames)
{
    for( int i = 0; i < sampleFrames; i++ )
    {
        for( int ch = 0; ch < numChannels; ch++ )
        {
            const Vst::Sample64 x = (Vst::Sample64)inputs[ch][i];
            Vst::Sample64 o = 0.0f;

            for( int band = 0; band < p_sys.i_band; band++ )
            {
                Vst::Sample64 y = p_sys.f_alpha[band] * ( x - p_sys.x[ch][1] ) +
                                  p_sys.f_gamma[band] * p_sys.y[ch][band][0] -
                                  p_sys.f_beta[band]  * p_sys.y[ch][band][1];

                p_sys.y[ch][band][1] = p_sys.y[ch][band][0];
                p_sys.y[ch][band][0] = y;

                o += y * p_sys.f_amp[band];
            }
            p_sys.x[ch][1] = p_sys.x[ch][0];
            p_sys.x[ch][0] = x;

            /* Second filter */
            if( p_sys.b_2eqz )
            {
                const Vst::Sample64 x2 = EQZ_IN_FACTOR * x + o;
                o = 0.0f;
                for( int band = 0; band < p_sys.i_band; band++ )
                {
                    Vst::Sample64 y = p_sys.f_alpha[band] * ( x2 - p_sys.x2[ch][1] ) +
                                      p_sys.f_gamma[band] * p_sys.y2[ch][band][0] -
                                      p_sys.f_beta[band]  * p_sys.y2[ch][band][1];

                    p_sys.y2[ch][band][1] = p_sys.y2[ch][band][0];
                    p_sys.y2[ch][band][0] = y;

                    o += y * p_sys.f_amp[band];
                }
                p_sys.x2[ch][1] = p_sys.x2[ch][0];
                p_sys.x2[ch][0] = x2;

                /* We add source PCM + filtered PCM */
                outputs[ch][i] = (!pEnable) ? (SampleType)(p_sys.f_gamp * p_sys.f_gamp * ( EQZ_IN_FACTOR * x2 + o )) : (SampleType)x;
            }
            else
            {
                /* We add source PCM + filtered PCM */
                outputs[ch][i] = (!pEnable) ? (SampleType)(p_sys.f_gamp * ( EQZ_IN_FACTOR * x + o )) : (SampleType)x;
            }
        }
    }
};

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
    EqzCoeffs( newSetup.sampleRate, 1.0, pFreqTable ); // set coefs

    p_sys.b_2eqz = false;
    p_sys.f_gamp = 1.0;
    for( int32 band = 0; band < p_sys.i_band; band++ )
    {
        p_sys.f_amp[band] = 1.0;
    }

    /* Filter state */
    for( int32 ch = 0; ch < EQZ_CH_MAX; ch++ )
    {
        p_sys.x[ch][0]  =
        p_sys.x[ch][1]  =
        p_sys.x2[ch][0] =
        p_sys.x2[ch][1] = 0.0;

        for( int32 band = 0; band < p_sys.i_band; band++ )
        {
            p_sys.y[ch][band][0]  =
            p_sys.y[ch][band][1]  =
            p_sys.y2[ch][band][0] =
            p_sys.y2[ch][band][1] = 0.0;
        }
    }
    
    //--- called before any processing ----
    return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
    // by default kSample32 is supported
    if (symbolicSampleSize == Vst::kSample32)
        return kResultTrue;

    // disable the following comment if your processing support kSample64
    if (symbolicSampleSize == Vst::kSample64)
        return kResultTrue;

    return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::setState (IBStream* state)
{
    // called when we load a preset, the model has to be reloaded
    if (!state)
        return kResultFalse;

    IBStreamer streamer (state, kLittleEndian);

    int32      savedBypass = 0;
    // ParamValue savedZoom;
    // int32      savedOS;
    int32      savedFactoryPreset = 0;
    int32      savedEnable        = 1;
    int32      saved2Pass         = 0;
    int32      savedFreqTable     = 0;
    ParamValue savedGain_Global   = Plain2Norm(12.0,  minGain,  maxGain);
    ParamValue savedGain_Band[EQZ_BANDS_MAX] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

    if (streamer.readInt32 (savedBypass)        == false) return kResultFalse;
    // if (streamer.readDouble(savedZoom)         == false) savedZoom          = 2.0 / 6.0;
    // if (streamer.readInt32(savedOS)            == false) savedOS            = 1;
    if (streamer.readInt32(savedFactoryPreset)  == false) return kResultFalse;
    if (streamer.readInt32(savedEnable)         == false) return kResultFalse;
    if (streamer.readInt32(saved2Pass)          == false) return kResultFalse;
    if (streamer.readInt32(savedFreqTable)      == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Global)   == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[0])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[1])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[2])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[3])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[4])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[5])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[6])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[7])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[8])  == false) return kResultFalse;
    if (streamer.readDouble(savedGain_Band[9])  == false) return kResultFalse;

    pBypass        = savedBypass > 0;
    // pZoom          = savedZoom;
    // pOS            = static_cast<overSample>(savedOS);
    pFactoryPreset = savedFactoryPreset;
    pEnable        = savedEnable > 0;
    p2Pass         = saved2Pass  > 0;
    pFreqTable     = savedFreqTable > 0;
    pGain_Global   = savedGain_Global;
    pGain_Band[0]  = savedGain_Band[0];
    pGain_Band[1]  = savedGain_Band[1];
    pGain_Band[2]  = savedGain_Band[2];
    pGain_Band[3]  = savedGain_Band[3];
    pGain_Band[4]  = savedGain_Band[4];
    pGain_Band[5]  = savedGain_Band[5];
    pGain_Band[6]  = savedGain_Band[6];
    pGain_Band[7]  = savedGain_Band[7];
    pGain_Band[8]  = savedGain_Band[8];
    pGain_Band[9]  = savedGain_Band[9];
    
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Processor::getState (IBStream* state)
{
    // here we need to save the model
    if (!state)
        return kResultFalse;

    IBStreamer streamer (state, kLittleEndian);

    streamer.writeInt32(pBypass ? 1 : 0);
    // streamer.writeDouble(pZoom);
    // streamer.writeInt32(static_cast<int32>(pOS));
    streamer.writeInt32(pFactoryPreset);
    streamer.writeInt32(pEnable ? 1 : 0);
    streamer.writeInt32(p2Pass ? 1 : 0);
    streamer.writeInt32(pFreqTable ? 1 : 0);
    streamer.writeDouble(pGain_Global);
    streamer.writeDouble(pGain_Band[0]);
    streamer.writeDouble(pGain_Band[1]);
    streamer.writeDouble(pGain_Band[2]);
    streamer.writeDouble(pGain_Band[3]);
    streamer.writeDouble(pGain_Band[4]);
    streamer.writeDouble(pGain_Band[5]);
    streamer.writeDouble(pGain_Band[6]);
    streamer.writeDouble(pGain_Band[7]);
    streamer.writeDouble(pGain_Band[8]);
    streamer.writeDouble(pGain_Band[9]);

    return kResultOk;
}


/* Equalizer coefficient calculation function based on equ-xmms */
void VLC_EQ_Processor::EqzCoeffs (SampleRate    f_rate,
                                  ParamValue    f_octave_percent,
                                  bool          b_use_vlc_freqs)
{
    //ListParameter defaults to fisrt of list, with ParamValue == 0 -> so pFreqTable == false is VLC table ON
    const ParamValue *f_freq_table_10b = !b_use_vlc_freqs
                                       ? f_vlc_frequency_table_10b
                                       : f_iso_frequency_table_10b;
    ParamValue f_nyquist_freq    = 0.5 * f_rate;
    ParamValue f_octave_factor   = pow( 2.0, 0.5 * f_octave_percent );
    ParamValue f_octave_factor_1 = 0.5 * ( f_octave_factor + 1.0 );
    ParamValue f_octave_factor_2 = 0.5 * ( f_octave_factor - 1.0 );
    
    p_sys.i_band = EQZ_BANDS_MAX;

    for( int32 band = 0; band < EQZ_BANDS_MAX; band++ )
    {
        ParamValue f_freq = f_freq_table_10b[band]; //iter every pre-defined freq

        if( f_freq <= f_nyquist_freq )
        {
            ParamValue f_theta_1 = ( 2.0 * M_PI * f_freq ) / f_rate;
            ParamValue f_theta_2 = f_theta_1 / f_octave_factor;
            ParamValue f_sin     = sin( f_theta_2 );
            ParamValue f_sin_prd = sin( f_theta_2 * f_octave_factor_1 )
                                 * sin( f_theta_2 * f_octave_factor_2 );
            ParamValue f_sin_hlf = f_sin * 0.5;
            ParamValue f_den     = f_sin_hlf + f_sin_prd;

            p_sys.f_alpha[band] = f_sin_prd / f_den;
            p_sys.f_beta[band]  = ( f_sin_hlf - f_sin_prd ) / f_den;
            p_sys.f_gamma[band] = f_sin * cos( f_theta_1 ) / f_den;
        }
        else
        {
            /* Any frequency beyond the Nyquist frequency is no good... */
            p_sys.f_alpha[band] =
            p_sys.f_beta[band]  =
            p_sys.f_gamma[band] = 0.0;
        }
    }
}

//------------------------------------------------------------------------
} // namespace yg331
