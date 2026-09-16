local M={}
MWDT.RegisterModule("actions",M)
MWDT.RegisterTab("actions","Actions / Macros",function(p)
 local h=MWDT.MakeLabel(p,"Action Bar / Macro Inspector",15); h:SetPoint("TOPLEFT",8,-8)
 local actions=MWDT.MakeButton(p,"Dump Actions",110,22); actions:SetPoint("TOPLEFT",8,-34)
 local macros=MWDT.MakeButton(p,"Dump Macros",110,22); macros:SetPoint("LEFT",actions,"RIGHT",8,0)
 local binds=MWDT.MakeButton(p,"Bindings",90,22); binds:SetPoint("LEFT",macros,"RIGHT",8,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 actions:SetScript("OnClick",function() local r={}; for i=1,120 do if GetActionInfo then local t,id,sub=GetActionInfo(i); if t then r[#r+1]=i..": "..tostring(t).." id="..tostring(id).." sub="..tostring(sub).." texture="..tostring(GetActionTexture and GetActionTexture(i)) end end end; M.out:SetText(table.concat(r,"\n")) end)
 macros:SetScript("OnClick",function() local r={}; if GetNumMacros then local g,c=GetNumMacros(); r[#r+1]="Global="..g.." Character="..c; for i=1,g+c do local n,ic,b=GetMacroInfo(i); r[#r+1]=i..": "..tostring(n).." icon="..tostring(ic).."\n"..tostring(b) end end; M.out:SetText(table.concat(r,"\n")) end)
 binds:SetScript("OnClick",function() local r={}; if GetNumBindings then for i=1,GetNumBindings() do local cmd,k1,k2=GetBinding(i); r[#r+1]=i..": "..tostring(cmd).." = "..tostring(k1)..", "..tostring(k2) end end; M.out:SetText(table.concat(r,"\n")) end)
end)
