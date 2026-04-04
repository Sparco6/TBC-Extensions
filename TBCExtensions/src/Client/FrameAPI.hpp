#pragma once
// ============================================================
// TBCExtensions: FrameAPI.hpp
// Frame/UI object method backports for TBC 2.4.3 (build 8606)
// ============================================================
// These are metatable methods registered on Frame objects so
// that addons can call them as frame:SetSize(w, h) etc.
//
// All methods follow the standard Lua C function signature:
//   int32_t Method(lua_State* L)
//
// The first argument on the Lua stack (index 1) is always the
// frame/widget userdata ("self").
//
// Methods marked STUB return a safe default value because the
// underlying TBC feature does not exist or addresses have not
// yet been found.
//
// Methods marked TODO_TBC require a memory address in wow.exe
// to be resolved before they will function. See
// TBC_PORTING_GUIDE.md for instructions on finding addresses.
// ============================================================

#include <cstdint>

struct lua_State;

namespace FrameAPI
{
    // --------------------------------------------------------
    // Lifecycle: call once after FrameScript::LoadFunctions()
    // --------------------------------------------------------
    void Initialize();
    void RegisterFrameMethods(lua_State* L);

    // --------------------------------------------------------
    // Introspection helpers
    // --------------------------------------------------------
    void*       GetFrameObject(lua_State* L, int32_t index);
    const char* GetFrameTypeName(void* framePtr);
    bool        IsFrameType(void* framePtr, const char* typeName);

    // ============================================================
    // SIZE AND POSITION METHODS (12 methods)
    // ============================================================
    int32_t SetSize(lua_State* L);          // frame:SetSize(width, height)
    int32_t GetSize(lua_State* L);          // width, height = frame:GetSize()
    int32_t SetWidth(lua_State* L);         // frame:SetWidth(width)
    int32_t SetHeight(lua_State* L);        // frame:SetHeight(height)
    int32_t GetWidth(lua_State* L);         // width = frame:GetWidth()
    int32_t GetHeight(lua_State* L);        // height = frame:GetHeight()
    int32_t GetRect(lua_State* L);          // left, bottom, width, height = frame:GetRect()
    int32_t GetCenter(lua_State* L);        // x, y = frame:GetCenter()
    int32_t GetLeft(lua_State* L);          // x = frame:GetLeft()
    int32_t GetRight(lua_State* L);         // x = frame:GetRight()
    int32_t GetTop(lua_State* L);           // y = frame:GetTop()
    int32_t GetBottom(lua_State* L);        // y = frame:GetBottom()

    // ============================================================
    // POINT/ANCHOR METHODS (5 methods)
    // ============================================================
    int32_t SetPoint(lua_State* L);         // frame:SetPoint(point, relativeTo, relativePoint, x, y)
    int32_t GetPoint(lua_State* L);         // point, relativeTo, relativePoint, x, y = frame:GetPoint(index)
    int32_t SetAllPoints(lua_State* L);     // frame:SetAllPoints(relativeTo)
    int32_t ClearAllPoints(lua_State* L);   // frame:ClearAllPoints()
    int32_t GetNumPoints(lua_State* L);     // count = frame:GetNumPoints()

    // ============================================================
    // BACKDROP METHODS (6 methods) – CRITICAL FOR ADDONS
    // ============================================================
    int32_t SetBackdrop(lua_State* L);              // frame:SetBackdrop(backdropTable)
    int32_t GetBackdrop(lua_State* L);              // backdropTable = frame:GetBackdrop()
    int32_t SetBackdropColor(lua_State* L);         // frame:SetBackdropColor(r, g, b[, a])
    int32_t GetBackdropColor(lua_State* L);         // r, g, b, a = frame:GetBackdropColor()
    int32_t SetBackdropBorderColor(lua_State* L);   // frame:SetBackdropBorderColor(r, g, b[, a])
    int32_t GetBackdropBorderColor(lua_State* L);   // r, g, b, a = frame:GetBackdropBorderColor()

    // ============================================================
    // HIERARCHY METHODS (4 methods)
    // ============================================================
    int32_t SetParent(lua_State* L);        // frame:SetParent(parent)
    int32_t GetParent(lua_State* L);        // parent = frame:GetParent()
    int32_t GetChildren(lua_State* L);      // child1, child2, ... = frame:GetChildren()
    int32_t GetNumChildren(lua_State* L);   // count = frame:GetNumChildren()

    // ============================================================
    // LEVEL AND STRATA METHODS (6 methods)
    // ============================================================
    int32_t SetFrameLevel(lua_State* L);    // frame:SetFrameLevel(level)
    int32_t GetFrameLevel(lua_State* L);    // level = frame:GetFrameLevel()
    int32_t SetFrameStrata(lua_State* L);   // frame:SetFrameStrata(strata)
    int32_t GetFrameStrata(lua_State* L);   // strata = frame:GetFrameStrata()
    int32_t Raise(lua_State* L);            // frame:Raise()
    int32_t Lower(lua_State* L);            // frame:Lower()

    // ============================================================
    // VISIBILITY METHODS (8 methods)
    // ============================================================
    int32_t Show(lua_State* L);                 // frame:Show()
    int32_t Hide(lua_State* L);                 // frame:Hide()
    int32_t IsShown(lua_State* L);              // bool = frame:IsShown()
    int32_t IsVisible(lua_State* L);            // bool = frame:IsVisible()
    int32_t SetShown(lua_State* L);             // frame:SetShown(shown)
    int32_t SetAlpha(lua_State* L);             // frame:SetAlpha(alpha)
    int32_t GetAlpha(lua_State* L);             // alpha = frame:GetAlpha()
    int32_t GetEffectiveAlpha(lua_State* L);    // alpha = frame:GetEffectiveAlpha()

    // ============================================================
    // SCALE METHODS (3 methods)
    // ============================================================
    int32_t SetScale(lua_State* L);             // frame:SetScale(scale)
    int32_t GetScale(lua_State* L);             // scale = frame:GetScale()
    int32_t GetEffectiveScale(lua_State* L);    // scale = frame:GetEffectiveScale()

    // ============================================================
    // BEHAVIOR METHODS (10 methods)
    // ============================================================
    int32_t SetClampedToScreen(lua_State* L);   // frame:SetClampedToScreen(clamped)
    int32_t IsClampedToScreen(lua_State* L);    // bool = frame:IsClampedToScreen()
    int32_t SetMovable(lua_State* L);           // frame:SetMovable(movable)
    int32_t IsMovable(lua_State* L);            // bool = frame:IsMovable()
    int32_t SetResizable(lua_State* L);         // frame:SetResizable(resizable)
    int32_t IsResizable(lua_State* L);          // bool = frame:IsResizable()
    int32_t SetUserPlaced(lua_State* L);        // frame:SetUserPlaced(placed)
    int32_t IsUserPlaced(lua_State* L);         // bool = frame:IsUserPlaced()
    int32_t SetToplevel(lua_State* L);          // frame:SetToplevel(topLevel)
    int32_t IsToplevel(lua_State* L);           // bool = frame:IsToplevel()

    // ============================================================
    // INPUT METHODS (8 methods)
    // ============================================================
    int32_t EnableMouse(lua_State* L);          // frame:EnableMouse(enable)
    int32_t IsMouseEnabled(lua_State* L);       // bool = frame:IsMouseEnabled()
    int32_t EnableMouseWheel(lua_State* L);     // frame:EnableMouseWheel(enable)
    int32_t IsMouseWheelEnabled(lua_State* L);  // bool = frame:IsMouseWheelEnabled()
    int32_t EnableKeyboard(lua_State* L);       // frame:EnableKeyboard(enable)
    int32_t IsKeyboardEnabled(lua_State* L);    // bool = frame:IsKeyboardEnabled()
    int32_t RegisterForDrag(lua_State* L);      // frame:RegisterForDrag("LeftButton", ...)
    int32_t IsMouseOver(lua_State* L);          // bool = frame:IsMouseOver()

    // ============================================================
    // HIT TESTING METHODS (2 methods)
    // ============================================================
    int32_t SetHitRectInsets(lua_State* L);     // frame:SetHitRectInsets(left, right, top, bottom)
    int32_t GetHitRectInsets(lua_State* L);     // left, right, top, bottom = frame:GetHitRectInsets()

    // ============================================================
    // SCRIPT METHODS (4 methods)
    // ============================================================
    int32_t SetScript(lua_State* L);            // frame:SetScript(scriptType, handler)
    int32_t GetScript(lua_State* L);            // handler = frame:GetScript(scriptType)
    int32_t HasScript(lua_State* L);            // bool = frame:HasScript(scriptType)
    int32_t HookScript(lua_State* L);           // frame:HookScript(scriptType, handler)

    // ============================================================
    // REGION METHODS (4 methods)
    // ============================================================
    int32_t CreateTexture(lua_State* L);        // texture = frame:CreateTexture(name, layer)
    int32_t CreateFontString(lua_State* L);     // fontString = frame:CreateFontString(name, layer)
    int32_t GetRegions(lua_State* L);           // region1, ... = frame:GetRegions()
    int32_t GetNumRegions(lua_State* L);        // count = frame:GetNumRegions()

    // ============================================================
    // OBJECT TYPE METHODS (4 methods)
    // ============================================================
    int32_t GetObjectType(lua_State* L);        // type = frame:GetObjectType()
    int32_t IsObjectType(lua_State* L);         // bool = frame:IsObjectType(type)
    int32_t GetName(lua_State* L);              // name = frame:GetName()
    int32_t GetDebugName(lua_State* L);         // name = frame:GetDebugName()

    // ============================================================
    // DRAG AND SIZING METHODS (5 methods)
    // ============================================================
    int32_t StartMoving(lua_State* L);          // frame:StartMoving()
    int32_t StopMovingOrSizing(lua_State* L);   // frame:StopMovingOrSizing()
    int32_t StartSizing(lua_State* L);          // frame:StartSizing(point)
    int32_t SetResizeBounds(lua_State* L);      // frame:SetResizeBounds(minW, minH[, maxW, maxH])
    int32_t GetResizeBounds(lua_State* L);      // minW, minH, maxW, maxH = frame:GetResizeBounds()

    // ============================================================
    // TEXTURE METHODS (14 methods) – For Texture objects
    // ============================================================
    int32_t SetTexture(lua_State* L);           // texture:SetTexture(file)
    int32_t GetTexture(lua_State* L);           // file = texture:GetTexture()
    int32_t SetTexCoord(lua_State* L);          // texture:SetTexCoord(left, right, top, bottom)
    int32_t GetTexCoord(lua_State* L);          // left, right, top, bottom = texture:GetTexCoord()
    int32_t SetVertexColor(lua_State* L);       // texture:SetVertexColor(r, g, b[, a])
    int32_t GetVertexColor(lua_State* L);       // r, g, b, a = texture:GetVertexColor()
    int32_t SetBlendMode(lua_State* L);         // texture:SetBlendMode(mode)
    int32_t GetBlendMode(lua_State* L);         // mode = texture:GetBlendMode()
    int32_t SetDesaturated(lua_State* L);       // texture:SetDesaturated(desaturated)
    int32_t IsDesaturated(lua_State* L);        // bool = texture:IsDesaturated()
    int32_t SetRotation(lua_State* L);          // texture:SetRotation(radians)
    int32_t GetRotation(lua_State* L);          // radians = texture:GetRotation()
    int32_t SetGradient(lua_State* L);          // texture:SetGradient(orient, minR,minG,minB, maxR,maxG,maxB)
    int32_t SetGradientAlpha(lua_State* L);     // texture:SetGradientAlpha(orient, minR,minG,minB,minA, maxR,maxG,maxB,maxA)

    // ============================================================
    // FONTSTRING METHODS (20 methods) – For FontString objects
    // ============================================================
    int32_t SetFont(lua_State* L);              // fontString:SetFont(file, size[, flags])
    int32_t GetFont(lua_State* L);              // file, size, flags = fontString:GetFont()
    int32_t SetText(lua_State* L);              // fontString:SetText(text)
    int32_t GetText(lua_State* L);              // text = fontString:GetText()
    int32_t SetTextColor(lua_State* L);         // fontString:SetTextColor(r, g, b[, a])
    int32_t GetTextColor(lua_State* L);         // r, g, b, a = fontString:GetTextColor()
    int32_t SetJustifyH(lua_State* L);          // fontString:SetJustifyH("LEFT"|"CENTER"|"RIGHT")
    int32_t GetJustifyH(lua_State* L);          // justify = fontString:GetJustifyH()
    int32_t SetJustifyV(lua_State* L);          // fontString:SetJustifyV("TOP"|"MIDDLE"|"BOTTOM")
    int32_t GetJustifyV(lua_State* L);          // justify = fontString:GetJustifyV()
    int32_t SetShadowColor(lua_State* L);       // fontString:SetShadowColor(r, g, b[, a])
    int32_t GetShadowColor(lua_State* L);       // r, g, b, a = fontString:GetShadowColor()
    int32_t SetShadowOffset(lua_State* L);      // fontString:SetShadowOffset(x, y)
    int32_t GetShadowOffset(lua_State* L);      // x, y = fontString:GetShadowOffset()
    int32_t GetStringWidth(lua_State* L);       // width = fontString:GetStringWidth()
    int32_t GetStringHeight(lua_State* L);      // height = fontString:GetStringHeight()
    int32_t SetSpacing(lua_State* L);           // fontString:SetSpacing(spacing)
    int32_t GetSpacing(lua_State* L);           // spacing = fontString:GetSpacing()
    int32_t SetWordWrap(lua_State* L);          // fontString:SetWordWrap(wrap)
    int32_t CanWordWrap(lua_State* L);          // bool = fontString:CanWordWrap()

    // ============================================================
    // BUTTON METHODS (14 methods) – For Button objects
    // ============================================================
    int32_t SetNormalTexture(lua_State* L);     // button:SetNormalTexture(texture)
    int32_t GetNormalTexture(lua_State* L);     // texture = button:GetNormalTexture()
    int32_t SetPushedTexture(lua_State* L);     // button:SetPushedTexture(texture)
    int32_t GetPushedTexture(lua_State* L);     // texture = button:GetPushedTexture()
    int32_t SetHighlightTexture(lua_State* L);  // button:SetHighlightTexture(texture[, blendMode])
    int32_t GetHighlightTexture(lua_State* L);  // texture = button:GetHighlightTexture()
    int32_t SetDisabledTexture(lua_State* L);   // button:SetDisabledTexture(texture)
    int32_t GetDisabledTexture(lua_State* L);   // texture = button:GetDisabledTexture()
    int32_t SetButtonState(lua_State* L);       // button:SetButtonState(state)
    int32_t GetButtonState(lua_State* L);       // state = button:GetButtonState()
    int32_t Enable(lua_State* L);               // button:Enable()
    int32_t Disable(lua_State* L);              // button:Disable()
    int32_t IsEnabled(lua_State* L);            // bool = button:IsEnabled()
    int32_t Click(lua_State* L);                // button:Click([button, down])

    // ============================================================
    // EDITBOX METHODS (14 methods) – For EditBox objects
    // ============================================================
    int32_t SetMaxLetters(lua_State* L);        // editBox:SetMaxLetters(maxLetters)
    int32_t GetMaxLetters(lua_State* L);        // maxLetters = editBox:GetMaxLetters()
    int32_t SetNumeric(lua_State* L);           // editBox:SetNumeric(numeric)
    int32_t IsNumeric(lua_State* L);            // bool = editBox:IsNumeric()
    int32_t SetPassword(lua_State* L);          // editBox:SetPassword(password)
    int32_t IsPassword(lua_State* L);           // bool = editBox:IsPassword()
    int32_t SetAutoFocus(lua_State* L);         // editBox:SetAutoFocus(autoFocus)
    int32_t IsAutoFocus(lua_State* L);          // bool = editBox:IsAutoFocus()
    int32_t SetFocus(lua_State* L);             // editBox:SetFocus()
    int32_t ClearFocus(lua_State* L);           // editBox:ClearFocus()
    int32_t HasFocus(lua_State* L);             // bool = editBox:HasFocus()
    int32_t HighlightText(lua_State* L);        // editBox:HighlightText([start, end])
    int32_t SetCursorPosition(lua_State* L);    // editBox:SetCursorPosition(position)
    int32_t GetCursorPosition(lua_State* L);    // position = editBox:GetCursorPosition()

    // ============================================================
    // SCROLLFRAME METHODS (7 methods) – For ScrollFrame objects
    // ============================================================
    int32_t SetScrollChild(lua_State* L);           // scrollFrame:SetScrollChild(child)
    int32_t GetScrollChild(lua_State* L);           // child = scrollFrame:GetScrollChild()
    int32_t SetHorizontalScroll(lua_State* L);      // scrollFrame:SetHorizontalScroll(offset)
    int32_t GetHorizontalScroll(lua_State* L);      // offset = scrollFrame:GetHorizontalScroll()
    int32_t SetVerticalScroll(lua_State* L);        // scrollFrame:SetVerticalScroll(offset)
    int32_t GetVerticalScroll(lua_State* L);        // offset = scrollFrame:GetVerticalScroll()
    int32_t UpdateScrollChildRect(lua_State* L);    // scrollFrame:UpdateScrollChildRect()

    // ============================================================
    // SLIDER METHODS (10 methods) – For Slider objects
    // ============================================================
    int32_t SetMinMaxValues(lua_State* L);      // slider:SetMinMaxValues(min, max)
    int32_t GetMinMaxValues(lua_State* L);      // min, max = slider:GetMinMaxValues()
    int32_t SetValue(lua_State* L);             // slider:SetValue(value)
    int32_t GetValue(lua_State* L);             // value = slider:GetValue()
    int32_t SetValueStep(lua_State* L);         // slider:SetValueStep(step)
    int32_t GetValueStep(lua_State* L);         // step = slider:GetValueStep()
    int32_t SetObeyStepOnDrag(lua_State* L);    // slider:SetObeyStepOnDrag(obey)
    int32_t GetObeyStepOnDrag(lua_State* L);    // bool = slider:GetObeyStepOnDrag()
    int32_t SetOrientation(lua_State* L);       // slider:SetOrientation(orientation)
    int32_t GetOrientation(lua_State* L);       // orientation = slider:GetOrientation()

    // ============================================================
    // STATUSBAR METHODS (8 methods) – For StatusBar objects
    // ============================================================
    int32_t SetStatusBarTexture(lua_State* L);  // statusBar:SetStatusBarTexture(texture)
    int32_t GetStatusBarTexture(lua_State* L);  // texture = statusBar:GetStatusBarTexture()
    int32_t SetStatusBarColor(lua_State* L);    // statusBar:SetStatusBarColor(r, g, b[, a])
    int32_t GetStatusBarColor(lua_State* L);    // r, g, b, a = statusBar:GetStatusBarColor()
    int32_t SetRotatesTexture(lua_State* L);    // statusBar:SetRotatesTexture(rotates)
    int32_t GetRotatesTexture(lua_State* L);    // bool = statusBar:GetRotatesTexture()
    int32_t SetReverseFill(lua_State* L);       // statusBar:SetReverseFill(reverse)
    int32_t GetReverseFill(lua_State* L);       // bool = statusBar:GetReverseFill()

} // namespace FrameAPI
