local M={mode="functions"}
MWDT.RegisterModule("browser",M)
MWDT.RegisterTab("browser","API / Globals",function(p)
 local h=MWDT.MakeLabel(p,"Runtime API / Global Browser",15); h:SetPoint("TOPLEFT",8,-8)
 local search=MWDT.MakeEditBox(p,false); search:SetPoint("TOPLEFT",8,-34); search:SetWidth(300); search:SetHeight(22); MWDT.AttachEditBoxBG(search); M.search=search
 local api=MWDT.MakeButton(p,"Functions",90,22); api:SetPoint("LEFT",search,"RIGHT",8,0)
 local all=MWDT.MakeButton(p,"All Globals",90,22); all:SetPoint("LEFT",api,"RIGHT",6,0)
 local refresh=MWDT.MakeButton(p,"Refresh",80,22); refresh:SetPoint("LEFT",all,"RIGHT",6,0)
 local list=MWDT.MakeScrollText(p); list:SetPoint("TOPLEFT",8,-66); list:SetPoint("BOTTOMRIGHT",-18,8); M.list=list.editBox
 local function build()
   local q=string.lower(search:GetText() or ""); local rows={}; local count=0
   for k,v in pairs(_G) do
     if (M.mode=="all" or type(v)=="function") and (q=="" or string.find(string.lower(tostring(k)),q,1,true)) then
       rows[#rows+1]=tostring(k).."  ["..type(v).."]"; count=count+1
     end
   end
   table.sort(rows); table.insert(rows,1,"Matches: "..count.."\n")
   M.list:SetText(table.concat(rows,"\n")); M.list:SetCursorPosition(0)
 end
 M.Build=build
 api:SetScript("OnClick",function() M.mode="functions"; build() end); all:SetScript("OnClick",function() M.mode="all"; build() end); refresh:SetScript("OnClick",build)
 search:SetScript("OnTextChanged",function() build() end); search:SetScript("OnEscapePressed",function() this:ClearFocus() end)
 build()
end)
M.OnShow=function() if M.Build then M.Build() end end
