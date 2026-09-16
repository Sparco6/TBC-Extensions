local M={}
MWDT.RegisterModule("console",M)
MWDT.RegisterTab("console","Lua Console",function(p)
 local h=MWDT.MakeLabel(p,"Lua Console / Script Runner",15); h:SetPoint("TOPLEFT",8,-8)
 local input=MWDT.MakeEditBox(p,true); input:SetPoint("TOPLEFT",8,-34); input:SetWidth(640); input:SetHeight(180); MWDT.AttachEditBoxBG(input); M.input=input
 local run=MWDT.MakeButton(p,"Run",80,22); run:SetPoint("TOPLEFT",8,-224)
 local clear=MWDT.MakeButton(p,"Clear",80,22); clear:SetPoint("LEFT",run,"RIGHT",6,0)
 local save=MWDT.MakeButton(p,"Save",80,22); save:SetPoint("LEFT",clear,"RIGHT",6,0)
 local hist=MWDT.MakeButton(p,"History ↑",90,22); hist:SetPoint("LEFT",save,"RIGHT",6,0)
 local outLabel=MWDT.MakeLabel(p,"Output",13); outLabel:SetPoint("TOPLEFT",8,-258)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-278); out:SetPoint("BOTTOMRIGHT",-18,8); M.output=out.editBox; out.editBox:SetText("")
 local function append(s)
   local eb=M.output; local old=eb:GetText() or ""; local text=old..(old~="" and "\n" or "")..tostring(s)
   if string.len(text)>50000 then text=string.sub(text,-50000) end
   eb:SetText(text); eb:SetCursorPosition(string.len(text))
 end
 M.Append=append
 run:SetScript("OnClick",function()
   local code=input:GetText() or ""; if code=="" then return end
   table.insert(MasterWoWDevToolsDB.history,1,code); while table.getn(MasterWoWDevToolsDB.history)>50 do table.remove(MasterWoWDevToolsDB.history) end
   local ok,a,b,c,d,e=MWDT.Eval(code)
   if ok then append("=> "..MWDT.SerializeValue(a).."  "..MWDT.SerializeValue(b).."  "..MWDT.SerializeValue(c).."  "..MWDT.SerializeValue(d).."  "..MWDT.SerializeValue(e)) else append("ERROR: "..tostring(a)) end
 end)
 clear:SetScript("OnClick",function() input:SetText(""); out.editBox:SetText("") end)
 save:SetScript("OnClick",function()
   local code=input:GetText() or ""; if code=="" then return end
   local name="Script "..date("%Y-%m-%d %H:%M:%S"); MasterWoWDevToolsDB.savedScripts[name]=code; append("Saved as: "..name)
 end)
 M.histIndex=0
 hist:SetScript("OnClick",function() M.histIndex=M.histIndex+1; local v=MasterWoWDevToolsDB.history[M.histIndex]; if not v then M.histIndex=1; v=MasterWoWDevToolsDB.history[1] end; if v then input:SetText(v) end end)
 input:SetScript("OnEscapePressed",function() this:ClearFocus() end)
end)
