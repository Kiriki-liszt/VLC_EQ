//------------------------------------------------------------------------
// Copyright(c) 2024 yg331.
//------------------------------------------------------------------------

#pragma once

#include "VLCEQ_shared.h"

#include "public.sdk/source/vst/vstparameters.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace VSTGUI {
class CSliderHandle : public CSlider
{
public:
    enum GradientStyle {
        kLinearGradient,
        kRadialGradient
    };
    CSliderHandle (const CRect& size,
                   IControlListener* listener,
                   int32_t  tag,
                   int32_t  iMinPos,
                   int32_t  iMaxPos,
                   CBitmap* handle,
                   CBitmap* background,
                   const CPoint& offset = CPoint (0, 0),
                   const int32_t style  = kLeft | kVertical)
    : CSlider(size, listener, tag, iMinPos, iMaxPos, handle, background, offset, style)
    {
        setWantsIdle(true);
    };
    
    CSliderHandle (const CRect& rect,
                   IControlListener* listener,
                   int32_t  tag,
                   const CPoint& offsetHandle,
                   int32_t  rangeHandle,
                   CBitmap* handle,
                   CBitmap* background,
                   const CPoint& offset = CPoint (0, 0),
                   const int32_t style  = kLeft | kVertical)
    : CSlider(rect, listener, tag, offsetHandle, rangeHandle, handle, background, offset, style)
    {
        setWantsIdle(true);
    };
    
    CSliderHandle (const CSlider& slider)
    : CSlider(slider)
    {
        setWantsIdle(true);
    };
    
    const CPoint& getSizeHandle            () const { return sizeHandle; }
    void    setSizeHandle            (const CPoint& size) { if (size != sizeHandle) { sizeHandle = size; setDirty (); } }
    
    CCoord  getRoundRectRadiusHandle () const { return roundRectRadiusHandle; }
    void    setRoundRectRadiusHandle (CCoord radius) { if (radius != roundRectRadiusHandle) { roundRectRadiusHandle = radius; setDirty (); } }
    
    CCoord  getFrameWidthHandle      () const { return widthHandleFrame; }
    void    setFrameWidthHandle      (CCoord width) { if (width != widthHandleFrame) { widthHandleFrame = width; setDirty (); } }
    
    const CColor& getColorHandle           () const { return colorHandle; }
    void    setColorHandle           (const CColor& color) { if (color != colorHandle) { colorHandle = color; setDirty (); } }
    
    const CColor& getColorHandleFrame      () const { return colorHandleFrame; }
    void    setColorHandleFrame      (const CColor& color) { if (color != colorHandleFrame) { colorHandleFrame = color; setDirty (); } }

    const CColor& getColorHandleMark       () const { return colorHandleMark; }
    void    setColorHandleMark       (const CColor& color) { if (color != colorHandleMark) { colorHandleMark = color; setDirty (); } }
    
    void setDrawGradientHandle  (bool state)           { if (drawGradientHandle  != state) { drawGradientHandle  = state; setDirty (); } }
    void setGradientStyleHandle (GradientStyle style)  { if (gradientStyleHandle != style) { gradientStyleHandle = style; setDirty (); } }
    void setGradientHandle      (CGradient* gradient)  { if (gradientHandle   != gradient) { gradientHandle   = gradient; setDirty (); } }
    void setGradientAngleHandle (double angle)         { if (gradientAngleHandle != angle) { gradientAngleHandle = angle; setDirty (); } }
    void setRadialCenterHandle  (const CPoint& center) { if (radialCenterHandle != center) { radialCenterHandle = center; setDirty (); } }
    void setRadialRadiusHandle  (CCoord radius)        { if (radialRadiusHandle != radius) { radialRadiusHandle = radius; setDirty (); } }
    
    bool          getDrawGradientHandle  () const { return drawGradientHandle; }
    GradientStyle getGradientStyleHandle () const { return gradientStyleHandle; }
    CGradient*    getGradientHandle      () const { return gradientHandle; }
    double        getGradientAngleHandle () const { return gradientAngleHandle; }
    const CPoint& getRadialCenterHandle  () const { return radialCenterHandle; }
    CCoord        getRadialRadiusHandle  () const { return radialRadiusHandle; }
    
    void setDrawGradientHandleShadow  (bool state)           { if (drawGradientHandleShadow  != state) { drawGradientHandleShadow  = state; setDirty (); } }
    void setGradientStyleHandleShadow (GradientStyle style)  { if (gradientStyleHandleShadow != style) { gradientStyleHandleShadow = style; setDirty (); } }
    void setGradientHandleShadow      (CGradient* gradient)  { if (gradientHandleShadow   != gradient) { gradientHandleShadow   = gradient; setDirty (); } }
    void setGradientAngleHandleShadow (double angle)         { if (gradientAngleHandleShadow != angle) { gradientAngleHandleShadow = angle; setDirty (); } }
    void setRadialCenterHandleShadow  (const CPoint& center) { if (radialCenterHandleShadow != center) { radialCenterHandleShadow = center; setDirty (); } }
    void setRadialRadiusHandleShadow  (CCoord radius)        { if (radialRadiusHandleShadow != radius) { radialRadiusHandleShadow = radius; setDirty (); } }
    
    bool          getDrawGradientHandleShadow  () const { return drawGradientHandleShadow; }
    GradientStyle getGradientStyleHandleShadow () const { return gradientStyleHandleShadow; }
    CGradient*    getGradientHandleShadow      () const { return gradientHandleShadow; }
    double        getGradientAngleHandleShadow () const { return gradientAngleHandleShadow; }
    const CPoint& getRadialCenterHandleShadow  () const { return radialCenterHandleShadow; }
    CCoord        getRadialRadiusHandleShadow  () const { return radialRadiusHandleShadow; }

    // overrides
    void onIdle() override {
        invalid();
    };
    void draw (CDrawContext* pContext) override
    {
        CSlider::draw(pContext);
        
        if (getDrawStyle() != 0)
        {
            if (getDrawStyle() & kDrawValue)
            {
                drawHandle (pContext);
            }
        }
        
        setDirty (false);
    };

protected:
    ~CSliderHandle() noexcept override
    {};
    void valueToPoint (CPoint &point) const
    {
        float drawValue   = getValueNormalized ();
        CCoord new_width  = getViewSize().getWidth()  - getSizeHandle().x;
        CCoord new_height = getViewSize().getHeight() - getSizeHandle().y - widthHandleFrame;
        
        if (getDrawStyle() & kDrawValueFromCenter)
        {
            if (getDrawStyle() & kDrawInverted)
                drawValue = 1.f - drawValue;
            
            if (getStyle () & kHorizontal)
            {
                point.x = new_width * drawValue;
                point.y = getViewSize().getHeight() * 0.5;
            }
            else
            {
                point.x = getViewSize().getWidth() * 0.5;
                point.y = new_height * drawValue + getSizeHandle().y * 0.5 + widthHandleFrame * 0.5;
            }
        }
        else
        {
            if ((getDrawStyle() & kDrawInverted) == false)
                drawValue = 1.f - drawValue;
            
            if (getStyle () & kHorizontal)
            {
                point.x = new_width * drawValue;
                point.y = getViewSize().getHeight() * 0.5;
            }
            else
            {
                point.x = getViewSize().getWidth() * 0.5;
                point.y = new_height * drawValue + getSizeHandle().y * 0.5 + widthHandleFrame * 0.5;
            }
        }
    }
    void drawHandle (CDrawContext* pContext) const
    {
        CPoint where;
        valueToPoint (where);
        auto viewPos = getViewSize ().getTopLeft ();
        where.offset (viewPos);
        CRect handleRect (where.x - 0.5, where.y - 0.5, where.x + 0.5, where.y + 0.5);
        handleRect.extend ((getSizeHandle().x - 1.0) * 0.5, (getSizeHandle().y - 1.0) * 0.5); // r.extend (handleLineWidth, handleLineWidth);
        
        pContext->setDrawMode (kAntiAliasing | kNonIntegralMode);
        
        if (drawGradientHandleShadow)
        {
            handleRect.extend(20, 20);
            handleRect.offset(0, 20);
            SharedPointer<CGraphicsPath> path = owned (pContext->createRoundRectGraphicsPath(handleRect, getRoundRectRadiusHandle()));
            if (path)
            {
                if (gradientStyleHandleShadow == kLinearGradient)
                {
                    CPoint colorStartPoint (0, 0);
                    colorStartPoint.x = handleRect.left + handleRect.getWidth () / 2 + cos (radians (gradientAngleHandleShadow-90)) * handleRect.getWidth () / 2;
                    colorStartPoint.y = handleRect.top + handleRect.getHeight () / 2 + sin (radians (gradientAngleHandleShadow-90)) * handleRect.getHeight () / 2;
                    CPoint colorEndPoint (0, handleRect.getHeight ());
                    colorEndPoint.x = handleRect.left + handleRect.getWidth () / 2 + cos (radians (gradientAngleHandleShadow+90)) * handleRect.getWidth () / 2;
                    colorEndPoint.y = handleRect.top + handleRect.getHeight () / 2 + sin (radians (gradientAngleHandleShadow+90)) * handleRect.getHeight () / 2;

                    pContext->fillLinearGradient (path, *gradientHandleShadow, colorStartPoint, colorEndPoint, false);
                }
                else // radial
                {
                    CPoint center (radialCenterHandleShadow);
                    center.x *= handleRect.getWidth ();
                    center.y *= handleRect.getHeight ();
                    center.offset (handleRect.left, handleRect.top);
                    pContext->fillRadialGradient (path, *gradientHandleShadow, center, radialRadiusHandleShadow * std::max (handleRect.getWidth (), handleRect.getHeight ()));
                }
            }
            handleRect.offset(0, -20);
            handleRect.inset(20, 20);
        }
        
        SharedPointer<CGraphicsPath> path = owned (pContext->createRoundRectGraphicsPath(handleRect, getRoundRectRadiusHandle()));
        if (path)
        {
            if (drawGradientHandle)
            {
                if (gradientStyleHandle == kLinearGradient)
                {
                    CPoint colorStartPoint (0, 0);
                    colorStartPoint.x = handleRect.left + handleRect.getWidth () / 2 + cos (radians (gradientAngleHandle-90)) * handleRect.getWidth () / 2;
                    colorStartPoint.y = handleRect.top + handleRect.getHeight () / 2 + sin (radians (gradientAngleHandle-90)) * handleRect.getHeight () / 2;
                    CPoint colorEndPoint (0, handleRect.getHeight ());
                    colorEndPoint.x = handleRect.left + handleRect.getWidth () / 2 + cos (radians (gradientAngleHandle+90)) * handleRect.getWidth () / 2;
                    colorEndPoint.y = handleRect.top + handleRect.getHeight () / 2 + sin (radians (gradientAngleHandle+90)) * handleRect.getHeight () / 2;

                    pContext->fillLinearGradient (path, *gradientHandle, colorStartPoint, colorEndPoint, false);
                }
                else // radial
                {
                    CPoint center (radialCenterHandle);
                    center.x *= handleRect.getWidth ();
                    center.y *= handleRect.getHeight ();
                    center.offset (handleRect.left, handleRect.top);
                    pContext->fillRadialGradient (path, *gradientHandle, center, radialRadiusHandle * std::max (handleRect.getWidth (), handleRect.getHeight ()));
                }
            }
            else {
                pContext->setFillColor (colorHandle);
                pContext->drawGraphicsPath (path, CDrawContext::kPathFilled);
            }
            
            pContext->setFrameColor (colorHandleFrame);
            pContext->setLineWidth (widthHandleFrame);
            pContext->setLineStyle (kLineSolid);
            pContext->drawGraphicsPath (path, CDrawContext::kPathStroked); // Frame
        }
        handleRect.inset(widthHandleFrame * 0.5, 0.0);
        CCoord lineWidth = 2.5;
        pContext->setLineWidth (lineWidth);
        pContext->setFrameColor (colorHandleMark);
        CPoint leftCenter(handleRect.getLeftCenter().x, handleRect.getLeftCenter().y - lineWidth * 0.5);
        CPoint rightCenter(handleRect.getRightCenter().x, handleRect.getRightCenter().y - lineWidth * 0.5);
        pContext->drawLine(leftCenter, rightCenter);
    };
    
    CColor colorHandle;
    CColor colorHandleFrame;
    CColor colorHandleMark;
    CPoint sizeHandle;
    CCoord roundRectRadiusHandle {5.};
    CCoord widthHandleFrame;
    
    bool                     drawGradientHandle {true};
    SharedPointer<CGradient> gradientHandle;
    GradientStyle            gradientStyleHandle {kLinearGradient};
    double                   gradientAngleHandle {0.};
    CCoord                   radialRadiusHandle {1.};
    CPoint                   radialCenterHandle {0.5, 0.5};
    
    bool                     drawGradientHandleShadow {true};
    SharedPointer<CGradient> gradientHandleShadow;
    GradientStyle            gradientStyleHandleShadow {kLinearGradient};
    double                   gradientAngleHandleShadow {0.};
    CCoord                   radialRadiusHandleShadow {1.};
    CPoint                   radialCenterHandleShadow {0.5, 0.5};
};




class CSliderRound : public CSlider
{
public:
    CSliderRound (const CRect& size,
                   IControlListener* listener,
                   int32_t  tag,
                   int32_t  iMinPos,
                   int32_t  iMaxPos,
                   CBitmap* handle,
                   CBitmap* background,
                   const CPoint& offset = CPoint (0, 0),
                   const int32_t style  = kLeft | kVertical)
    : CSlider(size, listener, tag, iMinPos, iMaxPos, handle, background, offset, style)
    {
        setWantsIdle(true);
    };
    
    CSliderRound (const CRect& rect,
                   IControlListener* listener,
                   int32_t  tag,
                   const CPoint& offsetHandle,
                   int32_t  rangeHandle,
                   CBitmap* handle,
                   CBitmap* background,
                   const CPoint& offset = CPoint (0, 0),
                   const int32_t style  = kLeft | kVertical)
    : CSlider(rect, listener, tag, offsetHandle, rangeHandle, handle, background, offset, style)
    {
        setWantsIdle(true);
    };
    
    CSliderRound (const CSlider& slider)
    : CSlider(slider)
    {
        setWantsIdle(true);
    };
    
    CCoord getRoundRectRadius () const { return roundRectRadius; }
    void setRoundRectRadius (CCoord radius)
    {
        if (radius != roundRectRadius)
        {
            roundRectRadius = radius;
            setDirty ();
        }
    };
    

    // overrides
    void onIdle() override {
        invalid();
    };
    void draw (CDrawContext* pContext) override
    {
        CDrawContext* drawContext = pContext;

        // draw background
        if (getDrawBackground ())
        {
            CRect rect (0, 0, getControlSizePrivate ().x, getControlSizePrivate ().y);
            rect.offset (getViewSize ().left, getViewSize ().top);
            getDrawBackground ()->draw (drawContext, rect, getBackgroundOffset ());
        }

        if (getDrawStyle() != 0)
        {
            auto lineWidth = getFrameWidth ();
            if (lineWidth < 0.)
                lineWidth = pContext->getHairlineSize ();
            CRect r (getViewSize ());
            pContext->setDrawMode (kAntiAliasing);
            pContext->setLineStyle (kLineSolid);
            pContext->setLineWidth (lineWidth);
            if (getDrawStyle() & kDrawFrame || getDrawStyle() & kDrawBack)
            {
                pContext->setFrameColor (getFrameColor());
                pContext->setFillColor (getBackColor());
                auto path = owned (pContext->createGraphicsPath ());
                if (path)
                {
                    if (getDrawStyle() & kDrawFrame)
                        r.inset (lineWidth / 2., lineWidth / 2.);
                    path->addRoundRect (r, getRoundRectRadius());
                    if (getDrawStyle() & kDrawBack)
                        pContext->drawGraphicsPath (path, CDrawContext::kPathFilled);
                    if (getDrawStyle() & kDrawFrame)
                        pContext->drawGraphicsPath (path, CDrawContext::kPathStroked);
                }
                else
                {
                    CDrawStyle d = kDrawFilled;
                    if (getDrawStyle() & kDrawFrame && getDrawStyle() & kDrawBack)
                        d = kDrawFilledAndStroked;
                    else if (getDrawStyle() & kDrawFrame)
                        d = kDrawStroked;
                    pContext->drawRect (r, d);
                }
            }
            if (getDrawStyle() & kDrawValue)
            {
                pContext->setDrawMode (kAliasing);
                if (getDrawStyle() & kDrawFrame)
                    r.inset (lineWidth / 2., lineWidth / 2.);
                CCoord full_left = r.left;
                CCoord full_right = r.right;
                CCoord full_top = r.top;
                CCoord full_bottom = r.bottom;
                float drawValue = getValueNormalized ();
                if (getDrawStyle() & kDrawValueFromCenter)
                {
                    if (getDrawStyle() & kDrawInverted)
                        drawValue = 1.f - drawValue;
                    if (getStyle () & kHorizontal)
                    {
                        CCoord width = r.getWidth ();
                        r.right = r.left + r.getWidth () * drawValue;
                        r.left += width / 2.;
                        r.normalize ();
                    }
                    else
                    {
                        CCoord height = r.getHeight ();
                        r.bottom = r.top + r.getHeight () * drawValue;
                        r.top += height / 2.;
                        r.normalize ();
                    }
                }
                else
                {
                    if (getStyle () & kHorizontal)
                    {
                        if (getDrawStyle() & kDrawInverted)
                            r.left = r.right - r.getWidth () * drawValue;
                        else
                            r.right = r.left + r.getWidth () * drawValue;
                    }
                    else
                    {
                        if (getDrawStyle() & kDrawInverted)
                            r.bottom = r.top + r.getHeight () * drawValue;
                        else
                            r.top = r.bottom - r.getHeight () * drawValue;
                    }
                }
                r.normalize ();
                if (r.getWidth () >= 0.5 && r.getHeight () >= 0.5)
                {
                    pContext->setFillColor (getValueColor());
                    auto path = owned (pContext->createGraphicsPath ());
                    if (path)
                    {
                        //path->addRoundRect (r, getRoundRectRadius());
                        if(true)
                        {
                            CCoord radius = getRoundRectRadius();
                            auto size = r;
                            if (radius <= 0.)
                            {
                                path->addRect (size);
                                return;
                            }
                            CRect rect2 (size);
                            rect2.normalize ();
                            const CCoord left = rect2.left;
                            const CCoord right = rect2.right;
                            const CCoord top = rect2.top;
                            const CCoord bottom = rect2.bottom;
                            
                            path->beginSubpath (CPoint (right - radius, top));
                            if (getDrawStyle() & kDrawValueFromCenter)
                            {
                                if (getStyle () & kHorizontal)
                                {
                                    path->addArc (CRect (right - 2.0 * radius, top, right, top + 2.0 * radius), 270., 360., true); // right-top
                                    path->addArc (CRect (right - 2.0 * radius, bottom - 2.0 * radius, right, bottom), 0., 90., true); // right-bottom
                                    path->addArc (CRect (left, bottom - 2.0 * radius, left + 2.0 * radius, bottom), 90., 180., true); // left-bottom
                                    path->addArc (CRect (left, top, left + 2.0 * radius, top + 2.0 * radius), 180., 270., true); // left-top
                                }
                                else
                                {
                                    path->addArc (CRect (right - 2.0 * radius, top, right, top + 2.0 * radius), 270., 360., true); // right-top
                                    path->addArc (CRect (right - 2.0 * radius, bottom - 2.0 * radius, right, bottom), 0., 90., true); // right-bottom
                                    path->addArc (CRect (left, bottom - 2.0 * radius, left + 2.0 * radius, bottom), 90., 180., true); // left-bottom
                                    path->addArc (CRect (left, top, left + 2.0 * radius, top + 2.0 * radius), 180., 270., true); // left-top
                                }
                            }
                            else
                            {
                                if (getStyle () & kHorizontal)
                                {
                                    path->addArc (CRect (right - 2.0 * radius, top, right, top + 2.0 * radius), 270., 360., true); // right-top
                                    path->addArc (CRect (right - 2.0 * radius, bottom - 2.0 * radius, right, bottom), 0., 90., true); // right-bottom
                                    path->addArc (CRect (left, bottom - 2.0 * radius, left + 2.0 * radius, bottom), 90., 180., true); // left-bottom
                                    path->addArc (CRect (left, top, left + 2.0 * radius, top + 2.0 * radius), 180., 270., true); // left-top
                                }
                                else
                                {
                                    CCoord r_t_f = (radius - (top - full_top));
                                    CCoord dx_top = (radius > (top - full_top)) ? radius - sqrt((radius*radius) - (r_t_f*r_t_f)): 0.0;
                                    double theta_top = (radius > (top - full_top)) ? asin(r_t_f/radius) * 180.0 * M_1_PI : 0.0;

                                    CCoord r_b_t = (radius - (bottom - top));
                                    CCoord dx_bottom = (radius > (bottom - top)) ? radius - sqrt((radius*radius) - (r_b_t*r_b_t)): 0.0;
                                    double theta_bottom = (radius > (bottom - top)) ? asin(r_b_t/radius) * 180.0 * M_1_PI : 0.0;
                                    
                                    if (dx_top > 0.0)
                                    {
                                        path->addLine(right - dx_top, top);
                                        path->addArc (CRect (full_right - 2.0 * radius, full_top, full_right, full_top + 2.0 * radius), 360.0 - theta_top, 360.0, true); // right-top
                                    }
                                    else path->addLine(right - dx_bottom, top);
                                    path->addArc (CRect (right - 2.0 * radius, bottom - 2.0 * radius, right, bottom), 0.0 + theta_bottom, 90., true); // right-bottom
                                    path->addArc (CRect (left, bottom - 2.0 * radius, left + 2.0 * radius, bottom), 90., 180.0 - theta_bottom, true); // left-bottom
                                    if (dx_top) 
                                    {
                                        path->addArc (CRect (full_left, full_top, full_left + 2.0 * radius, full_top + 2.0 * radius), 180.0, 180.0 + theta_top, true); // left-top
                                        path->addLine(left  + dx_top, top);
                                    }
                                    else path->addLine(left  + dx_bottom, top);
                                }
                            }
                            path->closeSubpath ();
                        }
                        pContext->drawGraphicsPath (path, CDrawContext::kPathFilled);
                    }
                    else
                        pContext->drawRect (r, kDrawFilled);
                }
            }
        }
        if (getHandle())
        {
            // calc new coords of slider
            CRect rectNew = calculateHandleRect (getValueNormalized ());

            // draw slider at new position
            getHandle()->draw (drawContext, rectNew);
        }

        setDirty (false);
    };

protected:
    ~CSliderRound() noexcept override
    {};
    
    CCoord roundRectRadius {5.};
};
}

namespace yg331 {

//------------------------------------------------------------------------
//  VLC_EQ_Controller
//------------------------------------------------------------------------
class VLC_EQ_Controller : public Steinberg::Vst::EditControllerEx1, public VSTGUI::VST3EditorDelegate
{
public:
//------------------------------------------------------------------------
    VLC_EQ_Controller () = default;
    ~VLC_EQ_Controller () SMTG_OVERRIDE = default;

    // Create function
    static Steinberg::FUnknown* createInstance (void* /*context*/)
    {
        return (Steinberg::Vst::IEditController*)new VLC_EQ_Controller;
    }

    // IPluginBase
    Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;

    // EditController
    Steinberg::tresult PLUGIN_API setComponentState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::IPlugView* PLUGIN_API createView (Steinberg::FIDString name) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API setParamNormalized (Steinberg::Vst::ParamID tag,
                                                      Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getParamStringByValue (Steinberg::Vst::ParamID tag,
                                                         Steinberg::Vst::ParamValue valueNormalized,
                                                         Steinberg::Vst::String128 string) SMTG_OVERRIDE;
    Steinberg::tresult PLUGIN_API getParamValueByString (Steinberg::Vst::ParamID tag,
                                                         Steinberg::Vst::TChar* string,
                                                         Steinberg::Vst::ParamValue& valueNormalized) SMTG_OVERRIDE;
    
    //---from ComponentBase-----
    // EditController
    // Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;
    // Steinberg::tresult PLUGIN_API receiveText(const char* text) SMTG_OVERRIDE;
    void PLUGIN_API update(Steinberg::FUnknown* changedUnknown, Steinberg::int32 message) SMTG_OVERRIDE;
    void editorAttached(Steinberg::Vst::EditorView* editor) SMTG_OVERRIDE; ///< called from EditorView if it was attached to a parent
    void editorRemoved (Steinberg::Vst::EditorView* editor) SMTG_OVERRIDE; ///< called from EditorView if it was removed from a parent

    //------------------------------------------------------------------------
    Steinberg::Vst::Parameter* getParameterObject(Steinberg::Vst::ParamID tag) SMTG_OVERRIDE
    {
        Steinberg::Vst::Parameter* param = EditControllerEx1::getParameterObject(tag);
        if (param == 0)
        {
            param = uiParameters.getParameter(tag);
        }
        return param;
    }
    bool isPrivateParameter (const Steinberg::Vst::ParamID paramID) SMTG_OVERRIDE
    {
        return uiParameters.getParameter (paramID) != 0 ? true : false;
    }

    // make sure that our UI only parameters doesn't call the following three EditController methods: beginEdit, endEdit, performEdit
    //------------------------------------------------------------------------
    Steinberg::tresult beginEdit(Steinberg::Vst::ParamID tag) SMTG_OVERRIDE
    {
        if (EditControllerEx1::getParameterObject(tag))
            return EditControllerEx1::beginEdit(tag);
        return Steinberg::kResultFalse;
    }

    //------------------------------------------------------------------------
    Steinberg::tresult performEdit(Steinberg::Vst::ParamID tag, Steinberg::Vst::ParamValue valueNormalized) SMTG_OVERRIDE
    {
        if (EditControllerEx1::getParameterObject(tag))
            return EditControllerEx1::performEdit(tag, valueNormalized);
        return Steinberg::kResultFalse;
    }

    //------------------------------------------------------------------------
    Steinberg::tresult endEdit(Steinberg::Vst::ParamID tag) SMTG_OVERRIDE
    {
        if (EditControllerEx1::getParameterObject(tag))
            return EditControllerEx1::endEdit(tag);
        return Steinberg::kResultFalse;
    }

    //---Interface---------
    DEFINE_INTERFACES
        // Here you can add more supported VST3 interfaces
        // DEF_INTERFACE (Vst::IXXX)
    END_DEFINE_INTERFACES (EditController)
    DELEGATE_REFCOUNT (EditController)

//------------------------------------------------------------------------
protected:
    // UI only parameter list
    Steinberg::Vst::ParameterContainer uiParameters;

    // editor list
    typedef std::vector<Steinberg::Vst::EditorView*> EditorVector;
    EditorVector editors;
    
    // zoom title-value struct
    struct ZoomFactor
    {
        const Steinberg::tchar* title;
        double factor;

        ZoomFactor(const Steinberg::tchar* title, double factor) : title(title), factor(factor) {}
    };
    typedef std::vector<ZoomFactor> ZoomFactorVector;
    ZoomFactorVector zoomFactors;
    
    ParamValue pZoom;
};

//------------------------------------------------------------------------
} // namespace yg331
