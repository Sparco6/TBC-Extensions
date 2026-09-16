local M={acc=0, enabled=true}
MWDT.RegisterModule("watch",M)
MWDT.RegisterTab("watch","Watch",function(p)
 local h=MWDT.MakeLabel(p,"Live Expression Watch",15); h:SetPoint("TOPLEFT",8,-8)
 local expr=MWDT.MakeEditBox(p,false); expr:SetPoint("TOPLEFT",8,-34); expr:SetWidth(360); expr:SetHeight(22); expr:SetText('UnitHealth("target")'); MWDT.AttachEditBoxBG(expr)
 local add=MWDT.MakeButton(p,"Add",70,22); add:SetPoint("LEFT",expr,"RIGHT",8,0)
 local clear=MWDT.MakeButton(p,"Clear",70,22); clear:SetPoint("LEFT",add,"RIGHT",6,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 add:SetScript("OnClick",function() local e=expr:GetText(); if e~="" then table.insert(MasterWoWDevToolsDB.watch,e) end end)
 clear:SetScript("OnClick",function() MasterWoWDevToolsDB.watch={} end)
 p:SetScript("OnUpdate",function() M.acc=M.acc+arg1; if M.acc<0.25 then return end; M.acc=0; local r={}
   for i,e in ipairs(MasterWoWDevToolsDB.watch or {}) do local ok,a=MWDT.Eval("return "..e); r[#r+1]=i..". "..e.." = "..(ok and MWDT.SerializeValue(a) or ("ERROR: "..tostring(a))) end
   M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end)
end)
