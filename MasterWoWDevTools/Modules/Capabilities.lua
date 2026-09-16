local M={}
MWDT.RegisterModule("capabilities",M)
MWDT.RegisterTab("capabilities","Capabilities",function(p)
 local h=MWDT.MakeLabel(p,"Client Capability Scanner",15); h:SetPoint("TOPLEFT",8,-8)
 local scan=MWDT.MakeButton(p,"Scan",80,22); scan:SetPoint("TOPLEFT",8,-34)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local names={"loadstring","pcall","CreateFrame","EnumerateFrames","GetMouseFocus","GetBuildInfo","GetCVar","SetCVar","GetDungeonDifficulty","SetDungeonDifficulty","ResetInstances","GetItemInfo","GetSpellInfo","GetSpellCooldown","GetActionInfo","GetMacroInfo","GetNumBindings","GetBinding","UnitGUID","UnitBuff","UnitDebuff","GetCursorInfo","GetLootSlotInfo","GetMerchantItemInfo","GetQuestLogTitle","GetAddOnMemoryUsage","UpdateAddOnMemoryUsage","debugprofilestop","DressUpItemLink","ShowUIPanel","HideUIPanel","ReloadUI","GetNetStats","GetFramerate"}
 local function build()
  local r={"Build: "..tostring(GetBuildInfo and GetBuildInfo() or "unknown"),""}
  for _,n in ipairs(names) do local v=_G[n]; r[#r+1]=string.format("%-30s %s  [%s]",n,(v~=nil and "YES" or "NO"),type(v)) end
  local fn,tab,other=0,0,0; for _,v in pairs(_G) do if type(v)=="function" then fn=fn+1 elseif type(v)=="table" then tab=tab+1 else other=other+1 end end
  r[#r+1]="\nRuntime globals: functions="..fn.." tables="..tab.." other="..other
  M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 scan:SetScript("OnClick",build); M.Build=build; build()
end)
M.OnShow=function() if M.Build then M.Build() end end
