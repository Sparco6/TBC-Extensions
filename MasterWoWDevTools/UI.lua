local function MakeButton(parent, text, w, h)
    local b=CreateFrame("Button", nil, parent, "UIPanelButtonTemplate")
    b:SetWidth(w or 90); b:SetHeight(h or 22); b:SetText(text or "Button"); return b
end
MWDT.MakeButton = MakeButton

function MWDT.MakeLabel(parent, text, size)
    local fs=parent:CreateFontString(nil,"OVERLAY","GameFontNormal")
    fs:SetText(text or ""); if size then local f,_,fl=fs:GetFont(); fs:SetFont(f,size,fl) end; return fs
end

function MWDT.MakeEditBox(parent, multiline)
    local eb=CreateFrame("EditBox",nil,parent)
    eb:SetFontObject(ChatFontNormal); eb:SetAutoFocus(false); eb:EnableMouse(true); eb:SetTextInsets(6,6,6,6)
    if multiline then eb:SetMultiLine(true) end
    local bg=CreateFrame("Frame",nil,parent); bg:SetBackdrop({bgFile="Interface\\Tooltips\\UI-Tooltip-Background",edgeFile="Interface\\Tooltips\\UI-Tooltip-Border",tile=true,tileSize=16,edgeSize=12,insets={left=3,right=3,top=3,bottom=3}}); bg:SetBackdropColor(0.05,0.05,0.05,0.95); bg:SetBackdropBorderColor(0.35,0.35,0.35,1); bg:SetFrameLevel(eb:GetFrameLevel()-1)
    eb._bg=bg
    return eb
end

function MWDT.AttachEditBoxBG(eb)
    if eb and eb._bg then eb._bg:ClearAllPoints(); eb._bg:SetPoint("TOPLEFT",eb,"TOPLEFT",-3,3); eb._bg:SetPoint("BOTTOMRIGHT",eb,"BOTTOMRIGHT",3,-3) end
end

function MWDT.MakeScrollText(parent)
    -- TBC 2.4.3-friendly manual scroll frame.  UIPanelScrollFrameTemplate is
    -- inconsistent for dynamically sized multiline edit boxes on old clients,
    -- so keep the scroll range and buttons under our control.
    local holder=CreateFrame("Frame",nil,parent)
    local sf=CreateFrame("ScrollFrame",nil,holder)
    sf:SetPoint("TOPLEFT",0,0)
    sf:SetPoint("BOTTOMRIGHT",-24,0)
    sf:EnableMouseWheel(true)

    local eb=CreateFrame("EditBox",nil,sf)
    eb:SetMultiLine(true); eb:SetAutoFocus(false); eb:SetFontObject(ChatFontNormal)
    eb:SetTextInsets(4,4,4,4); eb:SetWidth(620); eb:SetHeight(32)
    sf:SetScrollChild(eb)

    local up=CreateFrame("Button",nil,holder,"UIPanelScrollUpButtonTemplate")
    up:SetPoint("TOPRIGHT",0,0)
    local down=CreateFrame("Button",nil,holder,"UIPanelScrollDownButtonTemplate")
    down:SetPoint("BOTTOMRIGHT",0,0)

    local function updateRange()
        local viewH=sf:GetHeight() or 0
        local textH=(eb:GetHeight() or 0)
        if eb.GetStringHeight then
            local ok,h=pcall(eb.GetStringHeight,eb)
            if ok and h and h>0 then textH=h+12 end
        end
        if textH < 32 then textH=32 end
        if textH < viewH then textH=viewH end
        eb:SetHeight(textH)
        local maxScroll=math.max(0,textH-viewH)
        sf._maxScroll=maxScroll
        local cur=sf:GetVerticalScroll() or 0
        if cur>maxScroll then sf:SetVerticalScroll(maxScroll) end
        if up.SetEnabled then up:SetEnabled((sf:GetVerticalScroll() or 0)>0) end
        if down.SetEnabled then down:SetEnabled((sf:GetVerticalScroll() or 0)<maxScroll) end
    end

    local function scrollBy(amount)
        updateRange()
        local cur=sf:GetVerticalScroll() or 0
        local maxScroll=sf._maxScroll or 0
        local target=cur+(amount or 0)
        if target<0 then target=0 elseif target>maxScroll then target=maxScroll end
        sf:SetVerticalScroll(target)
        updateRange()
    end

    sf:SetScript("OnMouseWheel",function()
        local delta=arg1 or 0
        scrollBy(delta<0 and 42 or -42)
    end)
    up:SetScript("OnClick",function() scrollBy(-42) end)
    down:SetScript("OnClick",function() scrollBy(42) end)
    eb:SetScript("OnEscapePressed",function() this:ClearFocus() end)
    eb:SetScript("OnTextChanged",function() updateRange() end)
    holder:SetScript("OnShow",function() updateRange() end)

    -- Preserve the old return contract used by all modules.  Anchoring the
    -- returned holder now sizes both the viewport and its scroll controls.
    holder.editBox=eb; holder.scrollFrame=sf; holder.scrollUp=up; holder.scrollDown=down
    holder.UpdateScrollRange=updateRange; holder.ScrollBy=scrollBy
    return holder
end

local frame=CreateFrame("Frame","MasterWoWDevToolsFrame",UIParent)
frame:SetWidth(900); frame:SetHeight(620); frame:SetPoint("CENTER",UIParent,"CENTER",0,0); frame:SetFrameStrata("DIALOG"); frame:SetMovable(true); frame:EnableMouse(true); frame:RegisterForDrag("LeftButton"); frame:SetClampedToScreen(true)
frame:SetBackdrop({bgFile="Interface\\DialogFrame\\UI-DialogBox-Background",edgeFile="Interface\\DialogFrame\\UI-DialogBox-Border",tile=true,tileSize=32,edgeSize=32,insets={left=11,right=12,top=12,bottom=11}})
frame:SetScript("OnDragStart",function() this:StartMoving() end); frame:SetScript("OnDragStop",function() this:StopMovingOrSizing() end)
frame:Hide(); MWDT.frame=frame

local title=MWDT.MakeLabel(frame,"MasterWoW Developer Toolkit  •  TBC 2.4.3",16); title:SetPoint("TOPLEFT",22,-16)
local close=CreateFrame("Button",nil,frame,"UIPanelCloseButton"); close:SetPoint("TOPRIGHT",-6,-6)

local tabBar=CreateFrame("Frame",nil,frame); tabBar:SetPoint("TOPLEFT",18,-44); tabBar:SetWidth(190); tabBar:SetHeight(550)
local content=CreateFrame("Frame",nil,frame); content:SetPoint("TOPLEFT",210,-44); content:SetPoint("BOTTOMRIGHT",-18,18); MWDT.content=content

MWDT.tabButtons={}; MWDT.tabFrames={}
function MWDT.BuildTabs()
    for i,t in ipairs(MWDT.tabs) do
        local b=MakeButton(tabBar,t.title,180,22); b:SetPoint("TOPLEFT",0,-((i-1)*25)); b.tabId=t.id
        b:SetScript("OnClick",function() MWDT.ShowTab(this.tabId) end); MWDT.tabButtons[t.id]=b
        local tf=CreateFrame("Frame",nil,content); tf:SetAllPoints(content); tf:Hide(); MWDT.tabFrames[t.id]=tf
        local ok,err=pcall(t.builder,tf); if not ok then local l=MWDT.MakeLabel(tf,"Module build error: "..tostring(err)); l:SetPoint("TOPLEFT",10,-10) end
    end
end

function MWDT.ShowTab(id)
    if not next(MWDT.tabFrames) then MWDT.BuildTabs() end
    for k,f in pairs(MWDT.tabFrames) do f:Hide() end
    if MWDT.tabFrames[id] then MWDT.tabFrames[id]:Show(); frame:Show(); if MWDT.modules[id] and MWDT.modules[id].OnShow then pcall(MWDT.modules[id].OnShow,MWDT.modules[id]) end end
end
function MWDT.Toggle()
    if frame:IsShown() then frame:Hide() else if not next(MWDT.tabFrames) then MWDT.BuildTabs() end; frame:Show(); if MWDT.tabs[1] then MWDT.ShowTab(MWDT.tabs[1].id) end end
end
