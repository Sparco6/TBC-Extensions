local M={}
MWDT.RegisterModule("item",M)
MWDT.RegisterTab("item","Items",function(p)
 local h=MWDT.MakeLabel(p,"Item Inspector",15); h:SetPoint("TOPLEFT",8,-8)
 local id=MWDT.MakeEditBox(p,false); id:SetPoint("TOPLEFT",8,-34); id:SetWidth(150); id:SetHeight(22); MWDT.AttachEditBoxBG(id); M.id=id
 local inspect=MWDT.MakeButton(p,"Inspect ID",90,22); inspect:SetPoint("LEFT",id,"RIGHT",8,0)
 local cursor=MWDT.MakeButton(p,"From Cursor",100,22); cursor:SetPoint("LEFT",inspect,"RIGHT",6,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local function doid(n)
  n=tonumber(n); if not n then M.out:SetText("Enter numeric item ID") return end
  id:SetText(tostring(n)); local r={"ItemID: "..n}
  if GetItemInfo then local a,b,c,d,e,f,g,h,i,j=GetItemInfo(n); r[#r+1]="Name: "..tostring(a); r[#r+1]="Link: "..tostring(b); r[#r+1]="Quality: "..tostring(c); r[#r+1]="ItemLevel: "..tostring(d); r[#r+1]="ReqLevel: "..tostring(e); r[#r+1]="Type: "..tostring(f); r[#r+1]="SubType: "..tostring(g); r[#r+1]="Stack: "..tostring(h); r[#r+1]="EquipLoc: "..tostring(i); r[#r+1]="Texture: "..tostring(j); r[#r+1]="Resolved: "..tostring(a~=nil) end
  M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 M.DoID=doid; inspect:SetScript("OnClick",function() doid(id:GetText()) end)
 cursor:SetScript("OnClick",function() if GetCursorInfo then local t,a,b=GetCursorInfo(); M.out:SetText("Cursor type: "..tostring(t).."\narg1: "..tostring(a).."\narg2: "..tostring(b)); if t=="item" and a then doid(a) end end end)
 MWDT.ItemInspectByID=function(n) if n then doid(n) end end
end)
