local M={}
MWDT.RegisterModule("cvar",M)
MWDT.RegisterTab("cvar","CVars",function(p)
 local h=MWDT.MakeLabel(p,"CVar Browser / Editor",15); h:SetPoint("TOPLEFT",8,-8)
 local search=MWDT.MakeEditBox(p,false); search:SetPoint("TOPLEFT",8,-34); search:SetWidth(280); search:SetHeight(22); MWDT.AttachEditBoxBG(search)
 local name=MWDT.MakeEditBox(p,false); name:SetPoint("TOPLEFT",8,-66); name:SetWidth(230); name:SetHeight(22); MWDT.AttachEditBoxBG(name)
 local val=MWDT.MakeEditBox(p,false); val:SetPoint("LEFT",name,"RIGHT",8,0); val:SetWidth(160); val:SetHeight(22); MWDT.AttachEditBoxBG(val)
 local get=MWDT.MakeButton(p,"Get",65,22); get:SetPoint("LEFT",val,"RIGHT",8,0)
 local set=MWDT.MakeButton(p,"Set",65,22); set:SetPoint("LEFT",get,"RIGHT",6,0)
 local list=MWDT.MakeScrollText(p); list:SetPoint("TOPLEFT",8,-100); list:SetPoint("BOTTOMRIGHT",-18,8)
 M.search=search; M.list=list.editBox; M.name=name; M.val=val
 local function build()
   local q=string.lower(search:GetText() or ""); local rows={}
   for _,n in ipairs(MWDT_KNOWN_CVARS or {}) do if q=="" or string.find(string.lower(n),q,1,true) then
     local ok,v=pcall(GetCVar,n); rows[#rows+1]=n.." = "..(ok and tostring(v) or "<error>")
   end end
   M.list:SetText(table.concat(rows,"\n")); M.list:SetCursorPosition(0)
 end
 M.Build=build
 search:SetScript("OnTextChanged",build)
 get:SetScript("OnClick",function() local n=name:GetText(); local ok,v=pcall(GetCVar,n); val:SetText(ok and tostring(v) or "ERROR") end)
 set:SetScript("OnClick",function() local n=name:GetText(); local v=val:GetText(); local ok,e=pcall(SetCVar,n,v); MWDT.Print(ok and (n.." = "..tostring(GetCVar(n))) or e); build() end)
 build()
end)
M.OnShow=function() if M.Build then M.Build() end end
