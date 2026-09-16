local M={}
MWDT.RegisterModule("performance",M)
MWDT.RegisterTab("performance","Performance",function(p)
 local h=MWDT.MakeLabel(p,"Performance / Addon Memory",15); h:SetPoint("TOPLEFT",8,-8)
 local refresh=MWDT.MakeButton(p,"Refresh",80,22); refresh:SetPoint("TOPLEFT",8,-34)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local function build()
  local r={}; if GetFramerate then r[#r+1]="FPS: "..string.format("%.1f",GetFramerate()) end
  if GetNetStats then local down,up,home,world=GetNetStats(); r[#r+1]="Net down/up: "..tostring(down).." / "..tostring(up); r[#r+1]="Latency home/world: "..tostring(home).." / "..tostring(world) end
  if collectgarbage then r[#r+1]="Lua memory KB: "..tostring(math.floor(collectgarbage("count"))) end
  if UpdateAddOnMemoryUsage and GetNumAddOns then UpdateAddOnMemoryUsage(); r[#r+1]="\nADDONS:"; local arr={}; for i=1,GetNumAddOns() do local n=GetAddOnInfo(i); arr[#arr+1]={n=n or ("#"..i),m=GetAddOnMemoryUsage(i) or 0} end; table.sort(arr,function(a,b)return a.m>b.m end); for _,v in ipairs(arr) do r[#r+1]=string.format("%-35s %8.1f KB",v.n,v.m) end end
  M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 refresh:SetScript("OnClick",build); M.Build=build; build()
end)
M.OnShow=function() if M.Build then M.Build() end end
