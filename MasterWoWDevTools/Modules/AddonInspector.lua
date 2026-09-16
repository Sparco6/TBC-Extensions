local M={}
MWDT.RegisterModule("addons",M)
MWDT.RegisterTab("addons","Addons / Snippets",function(p)
 local h=MWDT.MakeLabel(p,"Addon Inspector / Built-in Script Library",15); h:SetPoint("TOPLEFT",8,-8)
 local addons=MWDT.MakeButton(p,"List Addons",100,22); addons:SetPoint("TOPLEFT",8,-34)
 local snippets=MWDT.MakeButton(p,"List Snippets",110,22); snippets:SetPoint("LEFT",addons,"RIGHT",8,0)
 local run=MWDT.MakeButton(p,"Run #",70,22); run:SetPoint("LEFT",snippets,"RIGHT",8,0)
 local idx=MWDT.MakeEditBox(p,false); idx:SetPoint("LEFT",run,"RIGHT",6,0); idx:SetWidth(50); idx:SetHeight(22); MWDT.AttachEditBoxBG(idx)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 addons:SetScript("OnClick",function() local r={}; for i=1,GetNumAddOns() do local n,title,notes,en,load=GetAddOnInfo(i); r[#r+1]=i..": "..tostring(n).." | "..tostring(title).." | enabled="..tostring(en).." loaded="..tostring(load).."\n  "..tostring(notes) end; M.out:SetText(table.concat(r,"\n")) end)
 snippets:SetScript("OnClick",function() local r={}; for i,s in ipairs(MWDT_SNIPPETS or {}) do r[#r+1]=i..". ["..s.cat.."] "..s.name.."\n   "..s.code end; M.out:SetText(table.concat(r,"\n")) end)
 run:SetScript("OnClick",function() local n=tonumber(idx:GetText()); local s=n and MWDT_SNIPPETS[n]; if not s then MWDT.Print("Invalid snippet index") return end; local ok,a=MWDT.Eval(s.code); MWDT.Print(ok and (s.name.." => "..MWDT.SerializeValue(a)) or a) end)
end)
