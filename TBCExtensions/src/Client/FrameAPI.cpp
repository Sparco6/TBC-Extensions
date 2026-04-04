// ============================================================
// TBCExtensions: FrameAPI.cpp
// Frame/UI object method backports for TBC 2.4.3 (build 8606)
// ============================================================
// Implements metatable method injection so that Lua addons can
// call modern WoW API methods on frame objects, e.g.:
//   local f = CreateFrame("Frame")
//   f:SetSize(200, 100)       -- backported from WotLK/Cata
//   f:SetBackdrop({...})      -- backported from WotLK/Cata
//
// HOW IT WORKS
// ============
// WoW registers each frame type (Frame, Button, EditBox, etc.)
// as a Lua userdata with a named metatable in the Lua registry.
// After FrameScript::LoadFunctions() has populated those
// metatables, FrameAPI::Initialize() injects our additional
// methods directly into each metatable, making them callable
// via the normal colon syntax.
//
// PORTING NOTES
// =============
// 1. Fill every 0x000000 /* TODO_TBC */ address with the
//    correct TBC 2.4.3 (build 8606) address.
// 2. Methods marked STUB return safe defaults; replace them
//    with real implementations once you have the addresses.
// 3. See TBC_PORTING_GUIDE.md for address-finding instructions.
// ============================================================

#include <Client/FrameAPI.hpp>
#include <Client/FrameScript.hpp>

// ============================================================
// Helper: register one method into a named frame-type metatable
// ============================================================
// Pseudocode equivalent:
//   local mt = debug.getregistry()[typeName]
//   if mt then mt[methodName] = fn end
//
static void RegisterMethod(lua_State* L, const char* typeName,
                            const char* methodName, int32_t (*fn)(lua_State*))
{
    // Push the metatable registered under typeName onto the stack.
    // LUA_REGISTRYINDEX is -10000 in the standard Lua 5.1 build embedded
    // in TBC 2.4.3. If the WoW client ships a modified Lua with a different
    // value this constant must be updated to match.
    FrameScript::GetMetaTable(L, typeName);   // stack: [mt | nil]

    // LUA_TTABLE is 5 in standard Lua 5.1. Only proceed if we got a table.
    static constexpr int32_t LUA_TTABLE_VALUE = 5;
    if (FrameScript::LuaType(L, -1) != LUA_TTABLE_VALUE)
    {
        // Pop nil or unexpected value.
        FrameScript::SetTop(L, FrameScript::GetTop(L, 0) - 1);
        return;
    }

    // Push the C function and store it under methodName in the metatable.
    // Because WoW sets __index = metatable itself for frame types, methods
    // placed directly in the metatable are accessible via colon syntax.
    FrameScript::PushCFunction(L, fn);            // stack: [mt, fn]
    FrameScript::SetField(L, -2, methodName);     // stack: [mt]

    // Pop the metatable.
    FrameScript::SetTop(L, FrameScript::GetTop(L, 0) - 1);
}

// ============================================================
// Macro: register a method into EVERY widget frame type
// ============================================================
#define REG_ALL(name, fn) \
    RegisterMethod(L, "Frame",        (name), (fn)); \
    RegisterMethod(L, "Button",       (name), (fn)); \
    RegisterMethod(L, "CheckButton",  (name), (fn)); \
    RegisterMethod(L, "EditBox",      (name), (fn)); \
    RegisterMethod(L, "ScrollFrame",  (name), (fn)); \
    RegisterMethod(L, "Slider",       (name), (fn)); \
    RegisterMethod(L, "StatusBar",    (name), (fn)); \
    RegisterMethod(L, "GameTooltip",  (name), (fn)); \
    RegisterMethod(L, "MessageFrame", (name), (fn)); \
    RegisterMethod(L, "SimpleHTML",   (name), (fn))

#define REG_FRAME(name, fn)  RegisterMethod(L, "Frame",       (name), (fn))
#define REG_BTN(name, fn)    RegisterMethod(L, "Button",      (name), (fn)); \
                             RegisterMethod(L, "CheckButton", (name), (fn))
#define REG_EDIT(name, fn)   RegisterMethod(L, "EditBox",     (name), (fn))
#define REG_TEX(name, fn)    RegisterMethod(L, "Texture",     (name), (fn))
#define REG_FONT(name, fn)   RegisterMethod(L, "FontString",  (name), (fn))
#define REG_SCROLL(name, fn) RegisterMethod(L, "ScrollFrame", (name), (fn))
#define REG_SLIDER(name, fn) RegisterMethod(L, "Slider",      (name), (fn))
#define REG_SBAR(name, fn)   RegisterMethod(L, "StatusBar",   (name), (fn))

// ============================================================
// FrameAPI::Initialize
// ============================================================
void FrameAPI::Initialize()
{
    // TODO_TBC: FrameScript::GetState() must have the correct address
    // filled in before this function will work. Once it does, this
    // call injects all backported frame methods into the live metatables.
    lua_State* L = FrameScript::GetState();
    if (!L)
        return;

    RegisterFrameMethods(L);
}

// ============================================================
// FrameAPI::RegisterFrameMethods
// ============================================================
void FrameAPI::RegisterFrameMethods(lua_State* L)
{
    // --------------------------------------------------------
    // Size and Position – available on all widget types
    // --------------------------------------------------------
    REG_ALL("SetSize",          &SetSize);
    REG_ALL("GetSize",          &GetSize);
    REG_ALL("SetWidth",         &SetWidth);
    REG_ALL("SetHeight",        &SetHeight);
    REG_ALL("GetWidth",         &GetWidth);
    REG_ALL("GetHeight",        &GetHeight);
    REG_ALL("GetRect",          &GetRect);
    REG_ALL("GetCenter",        &GetCenter);
    REG_ALL("GetLeft",          &GetLeft);
    REG_ALL("GetRight",         &GetRight);
    REG_ALL("GetTop",           &GetTop);
    REG_ALL("GetBottom",        &GetBottom);

    // --------------------------------------------------------
    // Point / Anchor
    // --------------------------------------------------------
    REG_ALL("SetPoint",         &SetPoint);
    REG_ALL("GetPoint",         &GetPoint);
    REG_ALL("SetAllPoints",     &SetAllPoints);
    REG_ALL("ClearAllPoints",   &ClearAllPoints);
    REG_ALL("GetNumPoints",     &GetNumPoints);

    // --------------------------------------------------------
    // Backdrop – Frame, Button, EditBox, ScrollFrame
    // --------------------------------------------------------
    REG_FRAME("SetBackdrop",              &SetBackdrop);
    REG_FRAME("GetBackdrop",              &GetBackdrop);
    REG_FRAME("SetBackdropColor",         &SetBackdropColor);
    REG_FRAME("GetBackdropColor",         &GetBackdropColor);
    REG_FRAME("SetBackdropBorderColor",   &SetBackdropBorderColor);
    REG_FRAME("GetBackdropBorderColor",   &GetBackdropBorderColor);
    REG_BTN  ("SetBackdrop",              &SetBackdrop);
    REG_BTN  ("GetBackdrop",              &GetBackdrop);
    REG_BTN  ("SetBackdropColor",         &SetBackdropColor);
    REG_BTN  ("GetBackdropColor",         &GetBackdropColor);
    REG_BTN  ("SetBackdropBorderColor",   &SetBackdropBorderColor);
    REG_BTN  ("GetBackdropBorderColor",   &GetBackdropBorderColor);
    REG_EDIT ("SetBackdrop",              &SetBackdrop);
    REG_EDIT ("GetBackdrop",              &GetBackdrop);
    REG_EDIT ("SetBackdropColor",         &SetBackdropColor);
    REG_EDIT ("GetBackdropColor",         &GetBackdropColor);
    REG_EDIT ("SetBackdropBorderColor",   &SetBackdropBorderColor);
    REG_EDIT ("GetBackdropBorderColor",   &GetBackdropBorderColor);
    REG_SCROLL("SetBackdrop",             &SetBackdrop);
    REG_SCROLL("GetBackdrop",             &GetBackdrop);
    REG_SCROLL("SetBackdropColor",        &SetBackdropColor);
    REG_SCROLL("GetBackdropColor",        &GetBackdropColor);
    REG_SCROLL("SetBackdropBorderColor",  &SetBackdropBorderColor);
    REG_SCROLL("GetBackdropBorderColor",  &GetBackdropBorderColor);

    // --------------------------------------------------------
    // Hierarchy
    // --------------------------------------------------------
    REG_ALL("SetParent",        &SetParent);
    REG_ALL("GetParent",        &GetParent);
    REG_ALL("GetChildren",      &GetChildren);
    REG_ALL("GetNumChildren",   &GetNumChildren);

    // --------------------------------------------------------
    // Level and Strata
    // --------------------------------------------------------
    REG_ALL("SetFrameLevel",    &SetFrameLevel);
    REG_ALL("GetFrameLevel",    &GetFrameLevel);
    REG_ALL("SetFrameStrata",   &SetFrameStrata);
    REG_ALL("GetFrameStrata",   &GetFrameStrata);
    REG_ALL("Raise",            &Raise);
    REG_ALL("Lower",            &Lower);

    // --------------------------------------------------------
    // Visibility
    // --------------------------------------------------------
    REG_ALL("Show",                 &Show);
    REG_ALL("Hide",                 &Hide);
    REG_ALL("IsShown",              &IsShown);
    REG_ALL("IsVisible",            &IsVisible);
    REG_ALL("SetShown",             &SetShown);
    REG_ALL("SetAlpha",             &SetAlpha);
    REG_ALL("GetAlpha",             &GetAlpha);
    REG_ALL("GetEffectiveAlpha",    &GetEffectiveAlpha);

    // --------------------------------------------------------
    // Scale
    // --------------------------------------------------------
    REG_ALL("SetScale",             &SetScale);
    REG_ALL("GetScale",             &GetScale);
    REG_ALL("GetEffectiveScale",    &GetEffectiveScale);

    // --------------------------------------------------------
    // Behavior
    // --------------------------------------------------------
    REG_ALL("SetClampedToScreen",   &SetClampedToScreen);
    REG_ALL("IsClampedToScreen",    &IsClampedToScreen);
    REG_ALL("SetMovable",           &SetMovable);
    REG_ALL("IsMovable",            &IsMovable);
    REG_ALL("SetResizable",         &SetResizable);
    REG_ALL("IsResizable",          &IsResizable);
    REG_ALL("SetUserPlaced",        &SetUserPlaced);
    REG_ALL("IsUserPlaced",         &IsUserPlaced);
    REG_ALL("SetToplevel",          &SetToplevel);
    REG_ALL("IsToplevel",           &IsToplevel);

    // --------------------------------------------------------
    // Input
    // --------------------------------------------------------
    REG_ALL("EnableMouse",          &EnableMouse);
    REG_ALL("IsMouseEnabled",       &IsMouseEnabled);
    REG_ALL("EnableMouseWheel",     &EnableMouseWheel);
    REG_ALL("IsMouseWheelEnabled",  &IsMouseWheelEnabled);
    REG_ALL("EnableKeyboard",       &EnableKeyboard);
    REG_ALL("IsKeyboardEnabled",    &IsKeyboardEnabled);
    REG_ALL("RegisterForDrag",      &RegisterForDrag);
    REG_ALL("IsMouseOver",          &IsMouseOver);

    // --------------------------------------------------------
    // Hit Testing
    // --------------------------------------------------------
    REG_ALL("SetHitRectInsets",     &SetHitRectInsets);
    REG_ALL("GetHitRectInsets",     &GetHitRectInsets);

    // --------------------------------------------------------
    // Scripts
    // --------------------------------------------------------
    REG_ALL("SetScript",            &SetScript);
    REG_ALL("GetScript",            &GetScript);
    REG_ALL("HasScript",            &HasScript);
    REG_ALL("HookScript",           &HookScript);

    // --------------------------------------------------------
    // Regions
    // --------------------------------------------------------
    REG_ALL("CreateTexture",        &CreateTexture);
    REG_ALL("CreateFontString",     &CreateFontString);
    REG_ALL("GetRegions",           &GetRegions);
    REG_ALL("GetNumRegions",        &GetNumRegions);

    // --------------------------------------------------------
    // Object Type
    // --------------------------------------------------------
    REG_ALL("GetObjectType",        &GetObjectType);
    REG_ALL("IsObjectType",         &IsObjectType);
    REG_ALL("GetName",              &GetName);
    REG_ALL("GetDebugName",         &GetDebugName);

    // --------------------------------------------------------
    // Drag and Sizing
    // --------------------------------------------------------
    REG_ALL("StartMoving",          &StartMoving);
    REG_ALL("StopMovingOrSizing",   &StopMovingOrSizing);
    REG_ALL("StartSizing",          &StartSizing);
    REG_ALL("SetResizeBounds",      &SetResizeBounds);
    REG_ALL("GetResizeBounds",      &GetResizeBounds);

    // --------------------------------------------------------
    // Texture methods – Texture layer only
    // --------------------------------------------------------
    REG_TEX("SetTexture",           &SetTexture);
    REG_TEX("GetTexture",           &GetTexture);
    REG_TEX("SetTexCoord",          &SetTexCoord);
    REG_TEX("GetTexCoord",          &GetTexCoord);
    REG_TEX("SetVertexColor",       &SetVertexColor);
    REG_TEX("GetVertexColor",       &GetVertexColor);
    REG_TEX("SetBlendMode",         &SetBlendMode);
    REG_TEX("GetBlendMode",         &GetBlendMode);
    REG_TEX("SetDesaturated",       &SetDesaturated);
    REG_TEX("IsDesaturated",        &IsDesaturated);
    REG_TEX("SetRotation",          &SetRotation);
    REG_TEX("GetRotation",          &GetRotation);
    REG_TEX("SetGradient",          &SetGradient);
    REG_TEX("SetGradientAlpha",     &SetGradientAlpha);

    // --------------------------------------------------------
    // FontString methods
    // --------------------------------------------------------
    REG_FONT("SetFont",             &SetFont);
    REG_FONT("GetFont",             &GetFont);
    REG_FONT("SetText",             &SetText);
    REG_FONT("GetText",             &GetText);
    REG_FONT("SetTextColor",        &SetTextColor);
    REG_FONT("GetTextColor",        &GetTextColor);
    REG_FONT("SetJustifyH",         &SetJustifyH);
    REG_FONT("GetJustifyH",         &GetJustifyH);
    REG_FONT("SetJustifyV",         &SetJustifyV);
    REG_FONT("GetJustifyV",         &GetJustifyV);
    REG_FONT("SetShadowColor",      &SetShadowColor);
    REG_FONT("GetShadowColor",      &GetShadowColor);
    REG_FONT("SetShadowOffset",     &SetShadowOffset);
    REG_FONT("GetShadowOffset",     &GetShadowOffset);
    REG_FONT("GetStringWidth",      &GetStringWidth);
    REG_FONT("GetStringHeight",     &GetStringHeight);
    REG_FONT("SetSpacing",          &SetSpacing);
    REG_FONT("GetSpacing",          &GetSpacing);
    REG_FONT("SetWordWrap",         &SetWordWrap);
    REG_FONT("CanWordWrap",         &CanWordWrap);

    // --------------------------------------------------------
    // Button methods
    // --------------------------------------------------------
    REG_BTN("SetNormalTexture",     &SetNormalTexture);
    REG_BTN("GetNormalTexture",     &GetNormalTexture);
    REG_BTN("SetPushedTexture",     &SetPushedTexture);
    REG_BTN("GetPushedTexture",     &GetPushedTexture);
    REG_BTN("SetHighlightTexture",  &SetHighlightTexture);
    REG_BTN("GetHighlightTexture",  &GetHighlightTexture);
    REG_BTN("SetDisabledTexture",   &SetDisabledTexture);
    REG_BTN("GetDisabledTexture",   &GetDisabledTexture);
    REG_BTN("SetButtonState",       &SetButtonState);
    REG_BTN("GetButtonState",       &GetButtonState);
    REG_BTN("Enable",               &Enable);
    REG_BTN("Disable",              &Disable);
    REG_BTN("IsEnabled",            &IsEnabled);
    REG_BTN("Click",                &Click);

    // --------------------------------------------------------
    // EditBox methods
    // --------------------------------------------------------
    REG_EDIT("SetMaxLetters",       &SetMaxLetters);
    REG_EDIT("GetMaxLetters",       &GetMaxLetters);
    REG_EDIT("SetNumeric",          &SetNumeric);
    REG_EDIT("IsNumeric",           &IsNumeric);
    REG_EDIT("SetPassword",         &SetPassword);
    REG_EDIT("IsPassword",          &IsPassword);
    REG_EDIT("SetAutoFocus",        &SetAutoFocus);
    REG_EDIT("IsAutoFocus",         &IsAutoFocus);
    REG_EDIT("SetFocus",            &SetFocus);
    REG_EDIT("ClearFocus",          &ClearFocus);
    REG_EDIT("HasFocus",            &HasFocus);
    REG_EDIT("HighlightText",       &HighlightText);
    REG_EDIT("SetCursorPosition",   &SetCursorPosition);
    REG_EDIT("GetCursorPosition",   &GetCursorPosition);

    // --------------------------------------------------------
    // ScrollFrame methods
    // --------------------------------------------------------
    REG_SCROLL("SetScrollChild",        &SetScrollChild);
    REG_SCROLL("GetScrollChild",        &GetScrollChild);
    REG_SCROLL("SetHorizontalScroll",   &SetHorizontalScroll);
    REG_SCROLL("GetHorizontalScroll",   &GetHorizontalScroll);
    REG_SCROLL("SetVerticalScroll",     &SetVerticalScroll);
    REG_SCROLL("GetVerticalScroll",     &GetVerticalScroll);
    REG_SCROLL("UpdateScrollChildRect", &UpdateScrollChildRect);

    // --------------------------------------------------------
    // Slider methods
    // --------------------------------------------------------
    REG_SLIDER("SetMinMaxValues",   &SetMinMaxValues);
    REG_SLIDER("GetMinMaxValues",   &GetMinMaxValues);
    REG_SLIDER("SetValue",          &SetValue);
    REG_SLIDER("GetValue",          &GetValue);
    REG_SLIDER("SetValueStep",      &SetValueStep);
    REG_SLIDER("GetValueStep",      &GetValueStep);
    REG_SLIDER("SetObeyStepOnDrag", &SetObeyStepOnDrag);
    REG_SLIDER("GetObeyStepOnDrag", &GetObeyStepOnDrag);
    REG_SLIDER("SetOrientation",    &SetOrientation);
    REG_SLIDER("GetOrientation",    &GetOrientation);

    // --------------------------------------------------------
    // StatusBar methods
    // --------------------------------------------------------
    REG_SBAR("SetStatusBarTexture", &SetStatusBarTexture);
    REG_SBAR("GetStatusBarTexture", &GetStatusBarTexture);
    REG_SBAR("SetStatusBarColor",   &SetStatusBarColor);
    REG_SBAR("GetStatusBarColor",   &GetStatusBarColor);
    REG_SBAR("SetRotatesTexture",   &SetRotatesTexture);
    REG_SBAR("GetRotatesTexture",   &GetRotatesTexture);
    REG_SBAR("SetReverseFill",      &SetReverseFill);
    REG_SBAR("GetReverseFill",      &GetReverseFill);
}

// Clean up file-local registration macros so they don't pollute other TUs.
#undef REG_ALL
#undef REG_FRAME
#undef REG_BTN
#undef REG_EDIT
#undef REG_TEX
#undef REG_FONT
#undef REG_SCROLL
#undef REG_SLIDER
#undef REG_SBAR

// ============================================================
// Introspection helpers
// ============================================================

void* FrameAPI::GetFrameObject(lua_State* L, int32_t index)
{
    // TODO_TBC: lua_touserdata address
    // WotLK 3.3.5 address was: 0x84E130
    // Returns the raw userdata pointer (the CGFrame* or similar) at stack index.
    return reinterpret_cast<void*(__cdecl*)(lua_State*, int32_t)>(0x000000 /* TODO_TBC */)(L, index);
}

const char* FrameAPI::GetFrameTypeName(void* framePtr)
{
    // TODO_TBC: Find the vtable or type-tag field inside the frame object
    // to retrieve its Lua type name ("Frame", "Button", etc.).
    // WoW frame objects typically store a type string pointer at a known offset.
    (void)framePtr;
    return "Frame"; // STUB
}

bool FrameAPI::IsFrameType(void* framePtr, const char* typeName)
{
    // STUB: compare GetFrameTypeName result once the above is implemented.
    (void)framePtr;
    (void)typeName;
    return false; // STUB
}

// ============================================================
// SIZE AND POSITION METHODS
// ============================================================

int32_t FrameAPI::SetSize(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetWidth / CFrame::SetHeight
    // Arg 1 = self (frame userdata), Arg 2 = width, Arg 3 = height
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetSize(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetWidth / CFrame::GetHeight
    // Returns two numbers: width, height
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 2; // STUB
}

int32_t FrameAPI::SetWidth(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetWidth(width)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::SetHeight(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetHeight(height)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetWidth(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetWidth()
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetHeight(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetHeight()
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetRect(lua_State* L)
{
    // Returns: left, bottom, width, height
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 4; // STUB
}

int32_t FrameAPI::GetCenter(lua_State* L)
{
    // Returns: x, y
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 2; // STUB
}

int32_t FrameAPI::GetLeft(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetRight(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetTop(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetBottom(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

// ============================================================
// POINT / ANCHOR METHODS
// ============================================================

int32_t FrameAPI::SetPoint(lua_State* L)
{
    // TODO_TBC: Call FrameScript__SetPoint or equivalent
    // Arg 2 = point string, Arg 3 = relativeTo, Arg 4 = relativePoint,
    //         Arg 5 = xOfs, Arg 6 = yOfs
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetPoint(lua_State* L)
{
    // TODO_TBC: Returns point, relativeTo, relativePoint, xOfs, yOfs
    // Arg 2 = optional index (default 1)
    FrameScript::PushString(L, "TOPLEFT");
    FrameScript::PushNil(L);
    FrameScript::PushString(L, "TOPLEFT");
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 5; // STUB
}

int32_t FrameAPI::SetAllPoints(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetAllPoints
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::ClearAllPoints(lua_State* L)
{
    // TODO_TBC: Call CFrame::ClearAllPoints
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetNumPoints(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetNumPoints
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

// ============================================================
// BACKDROP METHODS
// ============================================================

int32_t FrameAPI::SetBackdrop(lua_State* L)
{
    // TODO_TBC: Parse backdrop table and call CFrame::SetBackdrop
    // Arg 2 = table with fields: bgFile, edgeFile, tile, tileSize,
    //         edgeSize, insets {left, right, top, bottom}
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetBackdrop(lua_State* L)
{
    // TODO_TBC: Return a table describing the current backdrop.
    // Returns nil if no backdrop is set.
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetBackdropColor(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetBackdropColor(r, g, b, a)
    // Arg 2=r, Arg 3=g, Arg 4=b, Arg 5=a (optional, default 1)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetBackdropColor(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetBackdropColor -> r, g, b, a
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

int32_t FrameAPI::SetBackdropBorderColor(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetBackdropBorderColor(r, g, b, a)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetBackdropBorderColor(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetBackdropBorderColor -> r, g, b, a
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

// ============================================================
// HIERARCHY METHODS
// ============================================================

int32_t FrameAPI::SetParent(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetParent(parentFrame)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetParent(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetParent() and push the parent userdata
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::GetChildren(lua_State* L)
{
    // TODO_TBC: Push all child frame userdata onto the stack
    return 0; // STUB – returns nothing (no children)
}

int32_t FrameAPI::GetNumChildren(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetNumChildren()
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

// ============================================================
// LEVEL AND STRATA METHODS
// ============================================================

int32_t FrameAPI::SetFrameLevel(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetFrameLevel(level)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetFrameLevel(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetFrameLevel()
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

int32_t FrameAPI::SetFrameStrata(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetFrameStrata(strata)
    // Valid strata: "BACKGROUND", "LOW", "MEDIUM", "HIGH",
    //               "DIALOG", "FULLSCREEN", "FULLSCREEN_DIALOG", "TOOLTIP"
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetFrameStrata(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetFrameStrata()
    FrameScript::PushString(L, "MEDIUM");
    return 1; // STUB
}

int32_t FrameAPI::Raise(lua_State* L)
{
    // TODO_TBC: Call CFrame::Raise()
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::Lower(lua_State* L)
{
    // TODO_TBC: Call CFrame::Lower()
    (void)L;
    return 0; // STUB
}

// ============================================================
// VISIBILITY METHODS
// ============================================================

int32_t FrameAPI::Show(lua_State* L)
{
    // TODO_TBC: Call CFrame::Show()
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::Hide(lua_State* L)
{
    // TODO_TBC: Call CFrame::Hide()
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsShown(lua_State* L)
{
    // TODO_TBC: Call CFrame::IsShown() – true if the frame itself is shown
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::IsVisible(lua_State* L)
{
    // TODO_TBC: Call CFrame::IsVisible() – true if shown and all parents shown
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetShown(lua_State* L)
{
    // TODO_TBC: Calls Show() if arg2 is truthy, Hide() otherwise.
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::SetAlpha(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetAlpha(alpha) where alpha is 0.0–1.0
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetAlpha(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetAlpha()
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

int32_t FrameAPI::GetEffectiveAlpha(lua_State* L)
{
    // TODO_TBC: Returns combined alpha including parent chain
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

// ============================================================
// SCALE METHODS
// ============================================================

int32_t FrameAPI::SetScale(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetScale(scale)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetScale(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetScale()
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

int32_t FrameAPI::GetEffectiveScale(lua_State* L)
{
    // TODO_TBC: Returns combined scale including parent chain
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

// ============================================================
// BEHAVIOR METHODS
// ============================================================

int32_t FrameAPI::SetClampedToScreen(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsClampedToScreen(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetMovable(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsMovable(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetResizable(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsResizable(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetUserPlaced(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsUserPlaced(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetToplevel(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsToplevel(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

// ============================================================
// INPUT METHODS
// ============================================================

int32_t FrameAPI::EnableMouse(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsMouseEnabled(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::EnableMouseWheel(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsMouseWheelEnabled(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::EnableKeyboard(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsKeyboardEnabled(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::RegisterForDrag(lua_State* L)
{
    // TODO_TBC: Call CFrame::RegisterForDrag with each button name argument
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsMouseOver(lua_State* L)
{
    // TODO_TBC: Hit-test the current cursor position against the frame rect
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

// ============================================================
// HIT TESTING METHODS
// ============================================================

int32_t FrameAPI::SetHitRectInsets(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetHitRectInsets(left, right, top, bottom)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetHitRectInsets(lua_State* L)
{
    // Returns: left, right, top, bottom
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 4; // STUB
}

// ============================================================
// SCRIPT METHODS
// ============================================================

int32_t FrameAPI::SetScript(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetScript(scriptName, handler)
    // Script names: "OnLoad", "OnUpdate", "OnShow", "OnHide",
    //   "OnClick", "OnEnter", "OnLeave", "OnEvent", "OnValueChanged", etc.
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetScript(lua_State* L)
{
    // TODO_TBC: Return the Lua function registered for the script type
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::HasScript(lua_State* L)
{
    // TODO_TBC: Return true if a handler exists for the given script type
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::HookScript(lua_State* L)
{
    // TODO_TBC: Append a handler after the existing one (SecureHookScript style)
    (void)L;
    return 0; // STUB
}

// ============================================================
// REGION METHODS
// ============================================================

int32_t FrameAPI::CreateTexture(lua_State* L)
{
    // TODO_TBC: Call CFrame::CreateTexture(name, layer) and push result
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::CreateFontString(lua_State* L)
{
    // TODO_TBC: Call CFrame::CreateFontString(name, layer, template) and push result
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::GetRegions(lua_State* L)
{
    // TODO_TBC: Push all region objects onto the stack
    (void)L;
    return 0; // STUB – no regions
}

int32_t FrameAPI::GetNumRegions(lua_State* L)
{
    // TODO_TBC: Call CFrame::GetNumRegions()
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

// ============================================================
// OBJECT TYPE METHODS
// ============================================================

int32_t FrameAPI::GetObjectType(lua_State* L)
{
    // TODO_TBC: Return the Lua type name of the widget
    FrameScript::PushString(L, "Frame");
    return 1; // STUB
}

int32_t FrameAPI::IsObjectType(lua_State* L)
{
    // TODO_TBC: Compare the frame type name against Arg 2
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::GetName(lua_State* L)
{
    // TODO_TBC: Return the frame's registered global name
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::GetDebugName(lua_State* L)
{
    // TODO_TBC: Return a human-readable debug identifier for the frame
    FrameScript::PushString(L, "Unknown");
    return 1; // STUB
}

// ============================================================
// DRAG AND SIZING METHODS
// ============================================================

int32_t FrameAPI::StartMoving(lua_State* L)
{
    // TODO_TBC: Begin tracking cursor movement for frame drag
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::StopMovingOrSizing(lua_State* L)
{
    // TODO_TBC: End drag/resize tracking
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::StartSizing(lua_State* L)
{
    // TODO_TBC: Begin resize tracking from the given anchor point
    // Arg 2 = point: "TOPLEFT", "TOP", "TOPRIGHT", "LEFT", "RIGHT",
    //                "BOTTOMLEFT", "BOTTOM", "BOTTOMRIGHT"
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::SetResizeBounds(lua_State* L)
{
    // TODO_TBC: Call CFrame::SetResizeBounds(minW, minH[, maxW, maxH])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetResizeBounds(lua_State* L)
{
    // Returns: minW, minH, maxW, maxH
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 4; // STUB
}

// ============================================================
// TEXTURE METHODS
// ============================================================

int32_t FrameAPI::SetTexture(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetTexture(filePath or colorR,G,B,A)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetTexture(lua_State* L)
{
    // TODO_TBC: Return the texture file path
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetTexCoord(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetTexCoord(ULx,ULy, LLx,LLy, URx,URy, LRx,LRy)
    //       or  CTexture::SetTexCoord(left, right, top, bottom)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetTexCoord(lua_State* L)
{
    // Returns: ULx, ULy, LLx, LLy, URx, URy, LRx, LRy
    for (int32_t i = 0; i < 8; ++i)
        FrameScript::PushNumber(L, 0.0);
    return 8; // STUB
}

int32_t FrameAPI::SetVertexColor(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetVertexColor(r, g, b[, a])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetVertexColor(lua_State* L)
{
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

int32_t FrameAPI::SetBlendMode(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetBlendMode("BLEND"|"ADD"|"MOD"|"ALPHAKEY")
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetBlendMode(lua_State* L)
{
    FrameScript::PushString(L, "BLEND");
    return 1; // STUB
}

int32_t FrameAPI::SetDesaturated(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetDesaturated(bool)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsDesaturated(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetRotation(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetRotation(radians[, normalizedX, normalizedY])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetRotation(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetGradient(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetGradient(orientation, minR,minG,minB, maxR,maxG,maxB)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::SetGradientAlpha(lua_State* L)
{
    // TODO_TBC: Call CTexture::SetGradientAlpha(orient, minR,minG,minB,minA, maxR,maxG,maxB,maxA)
    (void)L;
    return 0; // STUB
}

// ============================================================
// FONTSTRING METHODS
// ============================================================

int32_t FrameAPI::SetFont(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetFont(filePath, size[, flags])
    // flags: "OUTLINE", "THICKOUTLINE", "MONOCHROME"
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetFont(lua_State* L)
{
    // Returns: filePath, size, flags
    FrameScript::PushString(L, "");
    FrameScript::PushNumber(L, 12.0);
    FrameScript::PushString(L, "");
    return 3; // STUB
}

int32_t FrameAPI::SetText(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetText(text)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetText(lua_State* L)
{
    // TODO_TBC: Call CFontString::GetText()
    FrameScript::PushString(L, "");
    return 1; // STUB
}

int32_t FrameAPI::SetTextColor(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetTextColor(r, g, b[, a])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetTextColor(lua_State* L)
{
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

int32_t FrameAPI::SetJustifyH(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetJustifyH("LEFT"|"CENTER"|"RIGHT")
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetJustifyH(lua_State* L)
{
    FrameScript::PushString(L, "LEFT");
    return 1; // STUB
}

int32_t FrameAPI::SetJustifyV(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetJustifyV("TOP"|"MIDDLE"|"BOTTOM")
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetJustifyV(lua_State* L)
{
    FrameScript::PushString(L, "MIDDLE");
    return 1; // STUB
}

int32_t FrameAPI::SetShadowColor(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetShadowColor(r, g, b[, a])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetShadowColor(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

int32_t FrameAPI::SetShadowOffset(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetShadowOffset(x, y)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetShadowOffset(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 2; // STUB
}

int32_t FrameAPI::GetStringWidth(lua_State* L)
{
    // TODO_TBC: Return actual rendered text width
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::GetStringHeight(lua_State* L)
{
    // TODO_TBC: Return actual rendered text height
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetSpacing(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetSpacing(spacing)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetSpacing(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetWordWrap(lua_State* L)
{
    // TODO_TBC: Call CFontString::SetWordWrap(bool)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::CanWordWrap(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

// ============================================================
// BUTTON METHODS
// ============================================================

int32_t FrameAPI::SetNormalTexture(lua_State* L)
{
    // TODO_TBC: Call CButton::SetNormalTexture(texture or filePath)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetNormalTexture(lua_State* L)
{
    // TODO_TBC: Return the normal texture object
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetPushedTexture(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetPushedTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetHighlightTexture(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetHighlightTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetDisabledTexture(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetDisabledTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetButtonState(lua_State* L)
{
    // TODO_TBC: Arg 2 = "NORMAL"|"PUSHED", Arg 3 = locked (bool)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetButtonState(lua_State* L)
{
    FrameScript::PushString(L, "NORMAL");
    return 1; // STUB
}

int32_t FrameAPI::Enable(lua_State* L)
{
    // TODO_TBC: Call CButton::Enable()
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::Disable(lua_State* L)
{
    // TODO_TBC: Call CButton::Disable()
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsEnabled(lua_State* L)
{
    FrameScript::PushBoolean(L, true);
    return 1; // STUB
}

int32_t FrameAPI::Click(lua_State* L)
{
    // TODO_TBC: Simulate a click: fire OnClick script on the button
    // Arg 2 = optional button name ("LeftButton"), Arg 3 = down (bool)
    (void)L;
    return 0; // STUB
}

// ============================================================
// EDITBOX METHODS
// ============================================================

int32_t FrameAPI::SetMaxLetters(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetMaxLetters(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetNumeric(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsNumeric(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetPassword(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsPassword(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetAutoFocus(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::IsAutoFocus(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetFocus(lua_State* L)
{
    // TODO_TBC: Give keyboard focus to this EditBox
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::ClearFocus(lua_State* L)
{
    // TODO_TBC: Remove keyboard focus from this EditBox
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::HasFocus(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::HighlightText(lua_State* L)
{
    // TODO_TBC: Select text in range [Arg2, Arg3]; no args = select all
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::SetCursorPosition(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetCursorPosition(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

// ============================================================
// SCROLLFRAME METHODS
// ============================================================

int32_t FrameAPI::SetScrollChild(lua_State* L)
{
    // TODO_TBC: Call CScrollFrame::SetScrollChild(childFrame)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetScrollChild(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetHorizontalScroll(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetHorizontalScroll(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetVerticalScroll(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetVerticalScroll(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::UpdateScrollChildRect(lua_State* L)
{
    // TODO_TBC: Recalculate scroll range based on child size
    (void)L;
    return 0; // STUB
}

// ============================================================
// SLIDER METHODS
// ============================================================

int32_t FrameAPI::SetMinMaxValues(lua_State* L)
{
    // TODO_TBC: Call CSlider::SetMinMaxValues(min, max)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetMinMaxValues(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 1.0);
    return 2; // STUB
}

int32_t FrameAPI::SetValue(lua_State* L)
{
    // TODO_TBC: Call CSlider::SetValue(value)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetValue(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1; // STUB
}

int32_t FrameAPI::SetValueStep(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetValueStep(lua_State* L)
{
    FrameScript::PushNumber(L, 1.0);
    return 1; // STUB
}

int32_t FrameAPI::SetObeyStepOnDrag(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetObeyStepOnDrag(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetOrientation(lua_State* L)
{
    // TODO_TBC: Arg 2 = "HORIZONTAL"|"VERTICAL"
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetOrientation(lua_State* L)
{
    FrameScript::PushString(L, "HORIZONTAL");
    return 1; // STUB
}

// ============================================================
// STATUSBAR METHODS
// ============================================================

int32_t FrameAPI::SetStatusBarTexture(lua_State* L)
{
    // TODO_TBC: Call CStatusBar::SetStatusBarTexture(filePath or texture)
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetStatusBarTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1; // STUB
}

int32_t FrameAPI::SetStatusBarColor(lua_State* L)
{
    // TODO_TBC: Call CStatusBar::SetStatusBarColor(r, g, b[, a])
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetStatusBarColor(lua_State* L)
{
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    FrameScript::PushNumber(L, 1.0);
    return 4; // STUB
}

int32_t FrameAPI::SetRotatesTexture(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetRotatesTexture(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}

int32_t FrameAPI::SetReverseFill(lua_State* L)
{
    (void)L;
    return 0; // STUB
}

int32_t FrameAPI::GetReverseFill(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1; // STUB
}
