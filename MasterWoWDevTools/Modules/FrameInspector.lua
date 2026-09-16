local M={live=false,acc=0}
MWDT.RegisterModule("frames",M)
MWDT.RegisterTab("frames","Frames",function(p)
 local h=MWDT.MakeLabel(p,"Frame Inspector",15); h:SetPoint("TOPLEFT",8,-8)
 local live=MWDT.MakeButton(p,"Live Mouse: OFF",120,22); live:SetPoint("TOPLEFT",8,-34)
 local once=MWDT.MakeButton(p,"Inspect Mouse",110,22); once:SetPoint("LEFT",live,"RIGHT",8,0)
 local enum=MWDT.MakeButton(p,"Enumerate",95,22); enum:SetPoint("LEFT",once,"RIGHT",8,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local function inspect(f)
   if not f then M.out:SetText("No frame under mouse") return end
   local r={}; local function add(k,v) r[#r+1]=k..": "..tostring(v) end
   add("Object",f); add("Name",f.GetName and f:GetName() or "<unnamed>"); add("Type",f.GetObjectType and f:GetObjectType() or "?")
   add("Shown",f.IsShown and f:IsShown()); add("Visible",f.IsVisible and f:IsVisible()); add("Alpha",f.GetAlpha and f:GetAlpha()); add("Scale",f.GetScale and f:GetScale())
   add("Width",f.GetWidth and f:GetWidth()); add("Height",f.GetHeight and f:GetHeight()); add("FrameStrata",f.GetFrameStrata and f:GetFrameStrata()); add("FrameLevel",f.GetFrameLevel and f:GetFrameLevel())
   if f.GetParent then local par=f:GetParent(); add("Parent",par and (par.GetName and par:GetName() or tostring(par)) or "nil") end
   if f.GetNumPoints then for i=1,f:GetNumPoints() do local a,b,c,d,e=f:GetPoint(i); r[#r+1]="Point"..i..": "..tostring(a).." -> "..tostring(b and b:GetName()).." / "..tostring(c).."  x="..tostring(d).." y="..tostring(e) end end
   M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 M.Inspect=inspect
 live:SetScript("OnClick",function() M.live=not M.live; this:SetText("Live Mouse: "..(M.live and "ON" or "OFF")) end)
 once:SetScript("OnClick",function() inspect(GetMouseFocus and GetMouseFocus()) end)
 enum:SetScript("OnClick",function() local rows={}; local n=0; if EnumerateFrames then local f=EnumerateFrames(); while f do n=n+1; rows[#rows+1]=n.."  "..tostring(f.GetName and f:GetName() or f).."  ["..tostring(f.GetObjectType and f:GetObjectType() or "?").."]"; f=EnumerateFrames(f); if n>2000 then break end end end; table.insert(rows,1,"Frames: "..n.."\n"); M.out:SetText(table.concat(rows,"\n")) end)
 p:SetScript("OnUpdate",function() if M.live then M.acc=M.acc+arg1; if M.acc>0.2 then M.acc=0; inspect(GetMouseFocus and GetMouseFocus()) end end end)
end)
