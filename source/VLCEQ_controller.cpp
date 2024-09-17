//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#include "VLCEQ_controller.h"
#include "VLCEQ_cids.h"

#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginterfaces/base/ustring.h"
#include "vstgui/vstgui_uidescription.h"

#include "vstgui/vstgui.h"
#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "public.sdk/source/vst/utility/stringconvert.h"

#include <array>

static const std::string kAttrHandleSize              = "handle-size"; //
//                                                    UIViewCreator::kAttrHandleColor
static const std::string kAttrHandleRoundRadius       = "handle-round-radius"; //
static const std::string kAttrHandleMarkColor         = "handle-mark-color"; //
static const std::string kAttrHandleFrameWidth        = "handle-frame-width"; //
static const std::string kAttrHandleFrameColor        = "handle-frame-color"; //

static const std::string kAttrHandleUseGradient       = "handle-use-gradient";
static const std::string kAttrHandleGradientStyle     = "handle-gradient-style";
static const std::string kAttrHandleGradient          = "handle-gradient";
static const std::string kAttrHandleGradientAngle     = "handle-gradient-angle";
static const std::string kAttrHandleRadialCenter      = "handle-radial-center";
static const std::string kAttrHandleRadialRadius      = "handle-radial-radius";

static const std::string kAttrHandleShadowUseGradient       = "handle-shadow-use-gradient";
static const std::string kAttrHandleShadowGradientStyle     = "handle-shadow-gradient-style";
static const std::string kAttrHandleShadowGradient          = "handle-shadow-gradient";
static const std::string kAttrHandleShadowGradientAngle     = "handle-shadow-gradient-angle";
static const std::string kAttrHandleShadowRadialCenter      = "handle-shadow-radial-center";
static const std::string kAttrHandleShadowRadialRadius      = "handle-shadow-radial-radius";

using namespace Steinberg;
namespace VSTGUI {

class SliderHandleCreator : public ViewCreatorAdapter
{
public:
    // register this class with the view factory
    SliderHandleCreator () { UIViewFactory::registerViewCreator (*this); }
    
    // return an unique name here
    IdStringPtr getViewName () const SMTG_OVERRIDE { return "Slider with Handle"; }

    // return the name here from where your custom view inherites.
    // Your view automatically supports the attributes from it.
    IdStringPtr getBaseViewName () const SMTG_OVERRIDE { return UIViewCreator::kCSlider; }

    // create your view here.
    // Note you don't need to apply attributes here as the apply method will be called with this new view
    CView* create (const UIAttributes& attributes, const IUIDescription* description) const SMTG_OVERRIDE
    {
        CRect size(0, 0, 20, 20);
        auto view = new CSliderHandle (size, nullptr, -1, 0, 0, nullptr, nullptr);
        if (description)
        {
            std::list<const string*> gradients;
            description->collectGradientNames (gradients);
            if (!gradients.empty ())
            {
                view->setGradientHandle       (description->getGradient (gradients.front ()->c_str ()));
                view->setGradientHandleShadow (description->getGradient (gradients.front ()->c_str ()));
            }
        }
        return view;
    }

    // apply custom attributes to your view
    bool apply (CView* view, const UIAttributes& attributes, const IUIDescription* description) const SMTG_OVERRIDE
    {
        CSliderHandle* myView = dynamic_cast<CSliderHandle*> (view);
        if (myView == 0)
            return false;
        
        //int32_t value;
        //if (attributes.getIntegerAttribute ("my-custom-attribute", value))
        //    myView->setCustomAttribute (value);

        double d;
        bool b;
        CPoint size;
        CColor color;
        CPoint p;
        
        if (attributes.getDoubleAttribute (kAttrHandleRoundRadius, d))
            myView->setRoundRectRadiusHandle (d);
        if (attributes.getDoubleAttribute (kAttrHandleFrameWidth, d))
            myView->setFrameWidthHandle (d);
        if (attributes.getPointAttribute(kAttrHandleSize, size))
            myView->setSizeHandle(size);
        if (UIViewCreator::stringToColor (attributes.getAttributeValue (UIViewCreator::kAttrHandleColor), color, description))
            myView->setColorHandle (color);
        if (UIViewCreator::stringToColor (attributes.getAttributeValue (kAttrHandleFrameColor), color, description))
            myView->setColorHandleFrame (color);
        if (UIViewCreator::stringToColor (attributes.getAttributeValue (kAttrHandleMarkColor), color, description))
            myView->setColorHandleMark (color);
        
        
        
        if (attributes.getBooleanAttribute (kAttrHandleUseGradient, b))
            myView->setDrawGradientHandle (b);
        if (attributes.getDoubleAttribute (kAttrHandleGradientAngle, d))
            myView->setGradientAngleHandle (d);
        const auto* attr = attributes.getAttributeValue (kAttrHandleGradientStyle);
        if (attr)
        {
            if (*attr == styleStrings() [CSliderHandle::kRadialGradient])
                myView->setGradientStyleHandle (CSliderHandle::kRadialGradient);
            else
                myView->setGradientStyleHandle (CSliderHandle::kLinearGradient);
        }
        if (attributes.getPointAttribute (kAttrHandleRadialCenter, p))
            myView->setRadialCenterHandle (p);
        if (attributes.getDoubleAttribute (kAttrHandleRadialRadius, d))
            myView->setRadialRadiusHandle (d);
        attr = attributes.getAttributeValue (kAttrHandleGradient);
        if (attr)
        {
            CGradient* gradient = description->getGradient (attr->c_str ());
            myView->setGradientHandle (gradient);
        }
        
        
        if (attributes.getBooleanAttribute (kAttrHandleShadowUseGradient, b))
            myView->setDrawGradientHandleShadow (b);
        if (attributes.getDoubleAttribute (kAttrHandleShadowGradientAngle, d))
            myView->setGradientAngleHandleShadow (d);
        attr = attributes.getAttributeValue (kAttrHandleShadowGradientStyle);
        if (attr)
        {
            if (*attr == styleStrings ()[CSliderHandle::kRadialGradient])
                myView->setGradientStyleHandleShadow (CSliderHandle::kRadialGradient);
            else
                myView->setGradientStyleHandleShadow (CSliderHandle::kLinearGradient);
        }
        if (attributes.getPointAttribute (kAttrHandleShadowRadialCenter, p))
            myView->setRadialCenterHandleShadow (p);
        if (attributes.getDoubleAttribute (kAttrHandleShadowRadialRadius, d))
            myView->setRadialRadiusHandleShadow (d);
        attr = attributes.getAttributeValue (kAttrHandleShadowGradient);
        if (attr)
        {
            CGradient* gradient = description->getGradient (attr->c_str ());
            myView->setGradientHandleShadow (gradient);
        }
        
        return true;
    }

    // add your custom attributes to the list
    bool getAttributeNames (StringList& attributeNames) const SMTG_OVERRIDE
    {
        // attributeNames.emplace_back ("my-custom-attribute");
        
        attributeNames.emplace_back (kAttrHandleSize);
        attributeNames.emplace_back (kAttrHandleRoundRadius);
        attributeNames.emplace_back (UIViewCreator::kAttrHandleColor);
        attributeNames.emplace_back (kAttrHandleMarkColor);
        attributeNames.emplace_back (kAttrHandleFrameWidth);
        attributeNames.emplace_back (kAttrHandleFrameColor);
        
        attributeNames.emplace_back (kAttrHandleUseGradient);
        attributeNames.emplace_back (kAttrHandleGradientStyle);
        attributeNames.emplace_back (kAttrHandleGradient);
        attributeNames.emplace_back (kAttrHandleGradientAngle);
        attributeNames.emplace_back (kAttrHandleRadialCenter);
        attributeNames.emplace_back (kAttrHandleRadialRadius);
        
        attributeNames.emplace_back (kAttrHandleShadowUseGradient);
        attributeNames.emplace_back (kAttrHandleShadowGradientStyle);
        attributeNames.emplace_back (kAttrHandleShadowGradient);
        attributeNames.emplace_back (kAttrHandleShadowGradientAngle);
        attributeNames.emplace_back (kAttrHandleShadowRadialCenter);
        attributeNames.emplace_back (kAttrHandleShadowRadialRadius);
        
        
        return true;
    }
    
    // return the type of your custom attributes
    AttrType getAttributeType (const std::string& attributeName) const SMTG_OVERRIDE
    {
        //if (attributeName == "my-custom-attribute")
        //    return kIntegerType;
 
        if (attributeName == kAttrHandleSize)
            return kPointType;
        if (attributeName == UIViewCreator::kAttrHandleColor)
            return kColorType;
        if (attributeName == kAttrHandleRoundRadius)
            return kFloatType;
        if (attributeName == kAttrHandleMarkColor)
            return kColorType;
        if (attributeName == kAttrHandleFrameWidth)
            return kFloatType;
        if (attributeName == kAttrHandleFrameColor)
            return kColorType;
        
        if (attributeName == kAttrHandleUseGradient)
            return kBooleanType;
        if (attributeName == kAttrHandleGradientStyle)
            return kListType;
        if (attributeName == kAttrHandleGradient)
            return kGradientType;
        if (attributeName == kAttrHandleGradientAngle)
            return kFloatType;
        if (attributeName == kAttrHandleRadialCenter)
            return kPointType;
        if (attributeName == kAttrHandleRadialRadius)
            return kFloatType;
        
        if (attributeName == kAttrHandleShadowUseGradient)
            return kBooleanType;
        if (attributeName == kAttrHandleShadowGradientStyle)
            return kListType;
        if (attributeName == kAttrHandleShadowGradient)
            return kGradientType;
        if (attributeName == kAttrHandleShadowGradientAngle)
            return kFloatType;
        if (attributeName == kAttrHandleShadowRadialCenter)
            return kPointType;
        if (attributeName == kAttrHandleShadowRadialRadius)
            return kFloatType;
        
        return kUnknownType;
    }
    
    // return the string value of the custom attributes of the view
    bool getAttributeValue (CView* view, const string& attributeName, string& stringValue, const IUIDescription* desc) const SMTG_OVERRIDE
    {
        CSliderHandle* myView = dynamic_cast<CSliderHandle*> (view);
        if (myView == 0)
            return false;
        
        //if (attributeName == "my-custom-attribute")
        //{
        //    stringValue = UIAttributes::integerToString (myView->getCustomAttribute ());
        //    return true;
        //}

        if (attributeName == kAttrHandleSize)
        {
            stringValue = UIAttributes::pointToString(myView->getSizeHandle());
            return true;
        }
        if (attributeName == UIViewCreator::kAttrHandleColor)
        {
            UIViewCreator::colorToString (myView->getColorHandle (), stringValue, desc);
            return true;
        }
        if (attributeName == kAttrHandleRoundRadius)
        {
            stringValue = UIAttributes::doubleToString (myView->getRoundRectRadiusHandle ());
            return true;
        }
        if (attributeName == kAttrHandleMarkColor)
        {
            UIViewCreator::colorToString (myView->getColorHandleMark (), stringValue, desc);
            return true;
        }
        if (attributeName == kAttrHandleFrameWidth)
        {
            stringValue = UIAttributes::doubleToString (myView->getFrameWidthHandle ());
            return true;
        }
        if (attributeName == kAttrHandleFrameColor)
        {
            UIViewCreator::colorToString (myView->getColorHandleFrame (), stringValue, desc);
            return true;
        }
        
        
        if (attributeName == kAttrHandleUseGradient)
        {
            stringValue = myView->getDrawGradientHandle () ? UIViewCreator::strTrue : UIViewCreator::strFalse;
            return true;
        }
        if (attributeName == kAttrHandleGradientStyle)
        {
            stringValue = styleStrings ()[myView->getGradientStyleHandle ()];
            return true;
        }
        if (attributeName == kAttrHandleGradient)
        {
            CGradient* gradient = myView->getGradientHandle ();
            UTF8StringPtr gradientName = gradient ? desc->lookupGradientName (gradient) : nullptr;
            stringValue = gradientName ? gradientName : "";
            return true;
        }
        if (attributeName == kAttrHandleGradientAngle)
        {
            stringValue = UIAttributes::doubleToString (myView->getGradientAngleHandle ());
            return true;
        }
        if (attributeName == kAttrHandleRadialRadius)
        {
            stringValue = UIAttributes::doubleToString (myView->getRadialRadiusHandle ());
            return true;
        }
        if (attributeName == kAttrHandleRadialCenter)
        {
            stringValue = UIAttributes::pointToString (myView->getRadialCenterHandle ());
            return true;
        }

        
        if (attributeName == kAttrHandleShadowUseGradient)
        {
            stringValue = myView->getDrawGradientHandleShadow () ? UIViewCreator::strTrue : UIViewCreator::strFalse;
            return true;
        }
        if (attributeName == kAttrHandleShadowGradientStyle)
        {
            stringValue = styleStrings ()[myView->getGradientStyleHandleShadow ()];
            return true;
        }
        if (attributeName == kAttrHandleShadowGradient)
        {
            CGradient* gradient = myView->getGradientHandleShadow ();
            UTF8StringPtr gradientName = gradient ? desc->lookupGradientName (gradient) : nullptr;
            stringValue = gradientName ? gradientName : "";
            return true;
        }
        if (attributeName == kAttrHandleShadowGradientAngle)
        {
            stringValue = UIAttributes::doubleToString (myView->getGradientAngleHandleShadow ());
            return true;
        }
        if (attributeName == kAttrHandleShadowRadialRadius)
        {
            stringValue = UIAttributes::doubleToString (myView->getRadialRadiusHandleShadow ());
            return true;
        }
        if (attributeName == kAttrHandleShadowRadialCenter)
        {
            stringValue = UIAttributes::pointToString (myView->getRadialCenterHandleShadow ());
            return true;
        }

        return false;
    }
    //------------------------------------------------------------------------
    bool getPossibleListValues (const string& attributeName, ConstStringPtrList& values) const SMTG_OVERRIDE
    {
        if ( (attributeName == kAttrHandleGradientStyle) || (attributeName == kAttrHandleShadowGradientStyle) )
        {
            for (auto& str : styleStrings ())
                values.emplace_back (&str);
            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------
    bool getAttributeValueRange (const string& attributeName, double& minValue, double& maxValue) const SMTG_OVERRIDE
    {
        if ( (attributeName == kAttrHandleGradientAngle) || (attributeName == kAttrHandleShadowGradientAngle) )
        {
            minValue = 0.;
            maxValue = 360.;
            return true;
        }
        return false;
    }
private:
    using StyleStrings = std::array<string, 2>;
    static StyleStrings& styleStrings ();
};
//------------------------------------------------------------------------
auto SliderHandleCreator::styleStrings () -> StyleStrings&
{
    static StyleStrings strings = {"linear", "radial"};
    return strings;
}

// create a static instance so that it registers itself with the view factory
SliderHandleCreator __gSliderHandleCreator;
}

namespace yg331 {

//------------------------------------------------------------------------
// VLC_EQ_Controller Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::initialize (FUnknown* context)
{
    // Here the Plug-in will be instantiated

    //---do not forget to call parent ------
    tresult result = EditControllerEx1::initialize (context);
    if (result != kResultOk)
    {
        return result;
    }
    
    // create top root unit with kProgramId as id for the programList
    addUnit (new Vst::Unit (STR ("Root"), Vst::kRootUnitId, Vst::kNoParentUnitId, kParamFactoryPreset));
    
    // Here you could register some parameters
    int32 stepCount;
    int32 flags;
    int32 tag;
    Vst::ParamValue defaultVal;
    Vst::ParamValue minPlain;
    Vst::ParamValue maxPlain;
    Vst::ParamValue defaultPlain;
    
    tag          = kParamBypass;
    stepCount    = 1;
    defaultVal   = 0;
    flags        = Vst::ParameterInfo::kIsBypass | Vst::ParameterInfo::kCanAutomate;
    parameters.addParameter(STR16("Bypass"), nullptr, stepCount, defaultVal, flags, tag);
    
    /*
    tag          = kParamFactoryPreset;
    flags        = Vst::ParameterInfo::kIsList | Vst::ParameterInfo::kIsProgramChange;
    auto* paramFactoryPreset = new Vst::StringListParameter(STR16("Factory Presets"), tag, nullptr, flags);
    for (int i = 0; i < NB_PRESETS; i++) paramFactoryPreset->appendString ( USTRING(preset_list[i]) );
    parameters.addParameter (paramFactoryPreset);
    */
    
    // create the program list: here kNumProgs entries
    auto* prgList = new Steinberg::Vst::ProgramList (STR16 ("Factory Presets"), kParamFactoryPreset, Vst::kRootUnitId);
    addProgramList (prgList);
    for (int32 i = 0; i < NB_PRESETS; i++)
    {
        // std::u16string title = STR ("Prog ");
        // title += VST3::toString (i + 1);
        // prgList->addProgram (title.data ());
        prgList->addProgram (USTRING(preset_list[i]));
    }

    //---Program Change parameter---
    Steinberg::Vst::Parameter* prgParam = prgList->getParameter ();

    // by default this program change parameter if automatable we can overwrite this:
    prgParam->getInfo ().flags &= ~Steinberg::Vst::ParameterInfo::kCanAutomate;

    parameters.addParameter (prgParam);
    
    
    tag          = kParamEnable;
    auto* paramEnable = new Vst::StringListParameter(STR16("Enable"), tag);
    paramEnable->appendString (STR16("Enabled")); // first of StringList is default
    paramEnable->appendString (STR16("Disabled"));
    parameters.addParameter (paramEnable);
    
    tag          = kParam2Pass;
    stepCount    = 1;
    defaultVal   = 0;
    flags        = Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsList;
    parameters.addParameter(STR16("2-Pass"), nullptr, stepCount, defaultVal, flags, tag);
    
    tag          = kParamFreqTable;
    auto* paramFrequencyTable = new Vst::StringListParameter(STR16("Freqency Table"), tag);
    paramFrequencyTable->appendString (STR16("VLC")); // first of StringList is default
    paramFrequencyTable->appendString (STR16("ISO"));
    parameters.addParameter (paramFrequencyTable);
    
    minPlain     = minGain;
    maxPlain     = maxGain;
    defaultPlain = dftGain;
    auto* paramGainGlobal = new Vst::RangeParameter(STR16("Gain Global"), kParamGain_Global, STR16("dB"), minPlain, maxPlain, 12.0);
    auto* paramGainBand1  = new Vst::RangeParameter(STR16("Gain Band1"),  kParamGain_Band1,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand2  = new Vst::RangeParameter(STR16("Gain Band2"),  kParamGain_Band2,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand3  = new Vst::RangeParameter(STR16("Gain Band3"),  kParamGain_Band3,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand4  = new Vst::RangeParameter(STR16("Gain Band4"),  kParamGain_Band4,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand5  = new Vst::RangeParameter(STR16("Gain Band5"),  kParamGain_Band5,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand6  = new Vst::RangeParameter(STR16("Gain Band6"),  kParamGain_Band6,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand7  = new Vst::RangeParameter(STR16("Gain Band7"),  kParamGain_Band7,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand8  = new Vst::RangeParameter(STR16("Gain Band8"),  kParamGain_Band8,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand9  = new Vst::RangeParameter(STR16("Gain Band9"),  kParamGain_Band9,  STR16("dB"), minPlain, maxPlain, defaultPlain);
    auto* paramGainBand10 = new Vst::RangeParameter(STR16("Gain Band10"), kParamGain_Band10, STR16("dB"), minPlain, maxPlain, defaultPlain);
    paramGainGlobal->setPrecision(1);
    paramGainBand1-> setPrecision(1);
    paramGainBand2-> setPrecision(1);
    paramGainBand3-> setPrecision(1);
    paramGainBand4-> setPrecision(1);
    paramGainBand5-> setPrecision(1);
    paramGainBand6-> setPrecision(1);
    paramGainBand7-> setPrecision(1);
    paramGainBand8-> setPrecision(1);
    paramGainBand9-> setPrecision(1);
    paramGainBand10->setPrecision(1);
    parameters.addParameter(paramGainGlobal);
    parameters.addParameter(paramGainBand1);
    parameters.addParameter(paramGainBand2);
    parameters.addParameter(paramGainBand3);
    parameters.addParameter(paramGainBand4);
    parameters.addParameter(paramGainBand5);
    parameters.addParameter(paramGainBand6);
    parameters.addParameter(paramGainBand7);
    parameters.addParameter(paramGainBand8);
    parameters.addParameter(paramGainBand9);
    parameters.addParameter(paramGainBand10);
    
    // GUI only parameter
    if (zoomFactors.empty())
    {
        zoomFactors.push_back(ZoomFactor(STR("50%"),  0.50)); // 0/6
        zoomFactors.push_back(ZoomFactor(STR("75%"),  0.75)); // 1/6
        zoomFactors.push_back(ZoomFactor(STR("100%"), 1.00)); // 2/6
        zoomFactors.push_back(ZoomFactor(STR("125%"), 1.25)); // 3/6
        zoomFactors.push_back(ZoomFactor(STR("150%"), 1.50)); // 4/6
        zoomFactors.push_back(ZoomFactor(STR("175%"), 1.75)); // 5/6
        zoomFactors.push_back(ZoomFactor(STR("200%"), 2.00)); // 6/6
    }

    Vst::StringListParameter* zoomParameter = new Vst::StringListParameter(STR("Zoom"), kParamZoom);
    for (ZoomFactorVector::const_iterator it = zoomFactors.begin(), end = zoomFactors.end(); it != end; ++it)
    {
        zoomParameter->appendString(it->title);
    }
    zoomParameter->setNormalized(zoomParameter->toNormalized(2));
    zoomParameter->addDependent(this);
    uiParameters.addParameter(zoomParameter);


    return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::terminate ()
{
    // Here the Plug-in will be de-instantiated, last possibility to remove some memory!
    getParameterObject(kParamZoom)->removeDependent(this);
    
    //---do not forget to call parent ------
    return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::setComponentState (IBStream* state)
{
    // Here you get the state of the component (Processor part)
    if (!state)
        return kResultFalse;

    IBStreamer streamer (state, kLittleEndian);

    int32      savedBypass = 0;
    // ParamValue savedZoom;
    // int32      savedOS;
    int32      savedFactoryPreset = 0;
    int32      savedEnable = 1;
    int32      saved2Pass = 0;
    int32      savedFreqTable = 0;
    ParamValue savedGain_Global = Plain2Norm(12.0,  minGain,  maxGain);
    ParamValue savedGain_Band[EQZ_BANDS_MAX] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    
    if (streamer.readInt32(savedBypass)         == false) return kResultFalse;
    // if (streamer.readDouble(savedZoom)          == false) savedZoom          = 2.0 / 6.0;
    // if (streamer.readInt32(savedOS)             == false) savedOS            = 1;
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

    setParamNormalized(kParamBypass,      savedBypass ? 1 : 0);
    // setParamNormalized(kParamZoom,       savedZoom);
    // setParamNormalized(kParamOS,         ListPlain2Norm(static_cast<ParamValue>(savedOS), overSample_num, ParamValue));
    EditControllerEx1::setParamNormalized(kParamFactoryPreset, ListPlain2Norm(savedFactoryPreset, NB_PRESETS - 1, ParamValue));
    setParamNormalized(kParamEnable,      savedEnable ? 1 : 0);
    setParamNormalized(kParam2Pass,       saved2Pass ? 1 : 0);
    setParamNormalized(kParamFreqTable,   savedFreqTable ? 1 : 0);
    setParamNormalized(kParamGain_Global, savedGain_Global);
    setParamNormalized(kParamGain_Band1,  savedGain_Band[0]);
    setParamNormalized(kParamGain_Band2,  savedGain_Band[1]);
    setParamNormalized(kParamGain_Band3,  savedGain_Band[2]);
    setParamNormalized(kParamGain_Band4,  savedGain_Band[3]);
    setParamNormalized(kParamGain_Band5,  savedGain_Band[4]);
    setParamNormalized(kParamGain_Band6,  savedGain_Band[5]);
    setParamNormalized(kParamGain_Band7,  savedGain_Band[6]);
    setParamNormalized(kParamGain_Band8,  savedGain_Band[7]);
    setParamNormalized(kParamGain_Band9,  savedGain_Band[8]);
    setParamNormalized(kParamGain_Band10, savedGain_Band[9]);
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::setState (IBStream* state)
{
    // Here you get the state of the controller
    if (!state)
        return kResultFalse;

    IBStreamer streamer(state, kLittleEndian);
    
    ParamValue savedZoom = 2.0 / 6.0;
    if (streamer.readDouble(savedZoom)          == false) return kResultFalse;
    pZoom          = savedZoom;
    setParamNormalized(kParamZoom,       savedZoom);
    return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::getState (IBStream* state)
{
    // Here you are asked to deliver the state of the controller (if needed)
    // Note: the real state of your plug-in is saved in the processor
    if (!state)
        return kResultFalse;
    IBStreamer streamer(state, kLittleEndian);
    pZoom = getParamNormalized(kParamZoom);
    if (streamer.writeDouble(pZoom) == false)
           return kResultFalse;
    return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API VLC_EQ_Controller::createView (FIDString name)
{
    // Here the Host wants to open your editor (if you have one)
    if (FIDStringsEqual (name, Vst::ViewType::kEditor))
    {
        // create your editor here and return a IPlugView ptr of it
        auto* view = new VSTGUI::VST3Editor (this, "view", "VLCEQ_editor.uidesc");
        
        std::vector<double> _zoomFactors;
        _zoomFactors.push_back(0.50);
        _zoomFactors.push_back(0.75);
        _zoomFactors.push_back(1.00);
        _zoomFactors.push_back(1.25);
        _zoomFactors.push_back(1.50);
        _zoomFactors.push_back(1.75);
        _zoomFactors.push_back(2.00);
        view->setAllowedZoomFactors(_zoomFactors);
        view->setZoomFactor(1.0);
        view->setIdleRate(1000.0/60.0);

        //VSTGUI::CView::kDirtyCallAlwaysOnMainThread = true;

        return view;
    }
    return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
    // called by host to update your parameters
    tresult result = EditControllerEx1::setParamNormalized (tag, value);
    if (result == kResultOk && tag == kParamFactoryPreset) // preset change
    {
        int32 program = (int32) (getParameterObject(tag)->toPlain(value));
        program = (program >= NB_PRESETS) ? NB_PRESETS - 1 : program;
        
        ParamValue norm_gg = getParameterObject(kParamGain_Global)->toNormalized(eqz_preset_10b[program].f_preamp);
        EditControllerEx1::setParamNormalized (kParamGain_Global, norm_gg);
        
        for (int32 band = 0; band < EQZ_BANDS_MAX; band++)
        {
            ParamValue norm_gb = getParameterObject(kParamGain_Band1 + band)->toNormalized(eqz_preset_10b[program].f_amp[band]);
            EditControllerEx1::setParamNormalized (kParamGain_Band1 + band, norm_gb);
        }
        // if (componentHandler) componentHandler->restartComponent (Steinberg::Vst::kParamValuesChanged);
        // setDirty(true);
    }
    return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
    // called by host to get a string for given normalized value of a specific parameter
    // (without having to set the value!)
    return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VLC_EQ_Controller::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
    // called by host to get a normalized value from a string representation of a specific parameter
    // (without having to set the value!)
    return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}
//------------------------------------------------------------------------
void VLC_EQ_Controller::editorAttached(Vst::EditorView* editor)
{
    editors.push_back(editor);
}

//------------------------------------------------------------------------
void VLC_EQ_Controller::editorRemoved(Vst::EditorView* editor)
{
    editors.erase(std::find(editors.begin(), editors.end(), editor));
}
//------------------------------------------------------------------------
void PLUGIN_API VLC_EQ_Controller::update(FUnknown* changedUnknown, int32 message)
{
    EditControllerEx1::update(changedUnknown, message);

    // GUI Resizing
    // check 'zoomtest' code at
    // https://github.com/steinbergmedia/vstgui/tree/vstgui4_10/vstgui/tests/uidescription%20vst3/source

    Vst::Parameter* param = FCast<Vst::Parameter>(changedUnknown);
    if (!param)
        return;

    if (param->getInfo().id == kParamZoom)
    {
        size_t index = static_cast<size_t> (param->toPlain(param->getNormalized()));

        if (index >= zoomFactors.size())
            return;

        for (EditorVector::const_iterator it = editors.begin(), end = editors.end(); it != end; ++it)
        {
            VSTGUI::VST3Editor* editor = dynamic_cast<VSTGUI::VST3Editor*>(*it);
            if (editor)
                editor->setZoomFactor(zoomFactors[index].factor);
        }
    }
}

//------------------------------------------------------------------------
} // namespace yg331
