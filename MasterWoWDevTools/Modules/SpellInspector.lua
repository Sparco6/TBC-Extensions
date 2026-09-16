local M={}
MWDT.RegisterModule("spell",M)
MWDT.RegisterTab("spell","Spells",function(p)
 local h=MWDT.MakeLabel(p,"Spell Inspector",15); h:SetPoint("TOPLEFT",8,-8)
 local id=MWDT.MakeEditBox(p,false); id:SetPoint("TOPLEFT",8,-34); id:SetWidth(150); id:SetHeight(22); MWDT.AttachEditBoxBG(id); M.id=id
 local inspect=MWDT.MakeButton(p,"Inspect ID",90,22); inspect:SetPoint("LEFT",id,"RIGHT",8,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local function doid(n)
  n=tonumber(n); if not n then M.out:SetText("Enter numeric spell ID") return end; id:SetText(tostring(n)); local r={"SpellID: "..n}
  if GetSpellInfo then local a,b,c,d,e,f=GetSpellInfo(n); r[#r+1]="Name: "..tostring(a); r[#r+1]="Rank: "..tostring(b); r[#r+1]="Icon: "..tostring(c); r[#r+1]="CastTime: "..tostring(d); r[#r+1]="MinRange: "..tostring(e); r[#r+1]="MaxRange: "..tostring(f) end
  if GetSpellCooldown then local s,d,e=GetSpellCooldown(n); r[#r+1]="CooldownStart: "..tostring(s); r[#r+1]="CooldownDuration: "..tostring(d); r[#r+1]="CooldownEnabled: "..tostring(e) end
  if IsUsableSpell then local u,m=IsUsableSpell(n); r[#r+1]="Usable: "..tostring(u); r[#r+1]="NoMana: "..tostring(m) end
  M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 inspect:SetScript("OnClick",function() doid(id:GetText()) end); M.DoID=doid; MWDT.SpellInspectByID=function(n) if n then doid(n) end end
end)
