local M={}; MWDT.RegisterModule("native",M)
MWDT.RegisterTab("native","Native / DLL",function(p)
 local h=MWDT.MakeLabel(p,"TBCExtensions / Native Address Inspector",15);h:SetPoint("TOPLEFT",8,-8)
 local status=MWDT.MakeScrollText(p);status:SetPoint("TOPLEFT",8,-42);status:SetPoint("BOTTOMRIGHT",-18,150)
 local addr=MWDT.MakeEditBox(p,false);addr:SetPoint("BOTTOMLEFT",8,104);addr:SetWidth(180);addr:SetHeight(22);addr:SetText("0x007059B0");MWDT.AttachEditBoxBG(addr)
 local check=MWDT.MakeButton(p,"Check Address",100,22);check:SetPoint("LEFT",addr,"RIGHT",8,0)
 local refresh=MWDT.MakeButton(p,"Refresh DLL Status",130,22);refresh:SetPoint("LEFT",check,"RIGHT",8,0)
 local function loaded() return type(TBCExt_GetVersion)=="function" end
 local function safeValue(fn,fallback)
  if type(fn)~="function" then return fallback end
  local ok,value=pcall(fn);if ok then return value end
  return fallback
 end
 local function refreshStatus()
  if not loaded() then status.editBox:SetText("Native Lua API: NOT AVAILABLE\nThe addon cannot determine whether the DLL is mapped until its safe Lua bridge is enabled.\nNormal DevTools remain available.");return end
  local v=safeValue(TBCExt_GetVersion,"unknown");local b=safeValue(TBCExt_GetClientBuild,"unknown");local m=safeValue(TBCExt_GetModuleBase,0);local l=safeValue(TBCExt_GetLuaState,0)
  local api=safeValue(TBCExt_GetNativeApiVersion,1);local profile=safeValue(TBCExt_GetClientProfile,"legacy API; unavailable")
  local core=safeValue(TBCExt_GetCoreLuaStatus,"unknown");local validator=safeValue(TBCExt_GetCallbackValidatorStatus,"unknown")
  local trace=safeValue(TBCExt_GetNativeTraceStatus,"DISABLED");local models=safeValue(TBCExt_GetModelHooksStatus,"NOT CONFIGURED")
  status.editBox:SetText("TBCExtensions: LOADED\nVersion: "..tostring(v).."\nClient build: "..tostring(b).."\nModule base: "..string.format("0x%08X",m or 0).."\nLua state: "..string.format("0x%08X",l or 0).." (LUA_RUNTIME_POINTER; Stable: NO)\nNative API version: "..tostring(api).."\nClient profile: "..tostring(profile).."\nCore Lua offsets: "..tostring(core).."\nCallback validator: "..tostring(validator).."\nNative trace: "..tostring(trace).."\nModel hooks: "..tostring(models))
 end
 local function inspect()
  local n=tonumber(string.gsub(addr:GetText() or "","^0[xX]",""),16);if not n or n<1 or n>4294967295 or n~=math.floor(n) then status.editBox:SetText("Invalid 32-bit hexadecimal address.");return end
  local rec=MWDT_NATIVE_BY_ADDRESS and MWDT_NATIVE_BY_ADDRESS[n];local lines={"Address: "..string.format("0x%08X",n)}
  if loaded() then
   local okr,r=pcall(TBCExt_IsAddressReadable,n);local okx,x=pcall(TBCExt_IsAddressExecutable,n)
   local base=safeValue(TBCExt_GetModuleBase,nil)
   local inModule=type(base)=="number" and n>=base and n<base+0xAC8000
   if type(TBCExt_GetAddressInfo)~='function' then lines[#lines+1]="Type: "..(inModule and "WOW_STATIC_ADDRESS" or "UNKNOWN_POINTER") end
   lines[#lines+1]="Readable: "..(okr and (r and "yes" or "no") or "query failed")
   lines[#lines+1]="Executable: "..(okx and (x and "yes" or "no") or "query failed")
   if inModule and type(TBCExt_GetAddressInfo)~='function' then lines[#lines+1]="Module: Wow.exe\nRVA: "..string.format("0x%08X",n-base) end
  else lines[#lines+1]="Native Lua API unavailable; runtime memory properties cannot be queried." end
  if rec then lines[#lines+1]="Ghidra: "..rec.g.."\nKnown function: "..(rec.n~="" and rec.n or rec.g).."\nCategory: "..rec.c.."\nStatus: "..rec.s.."\nConfidence: "..rec.q end
  if type(TBCExt_GetAddressInfo)=='function' then
   local ok,detail=pcall(TBCExt_GetAddressInfo,n)
   if ok then lines[#lines+1]='OS module metadata:\n'..tostring(detail) end
  end
  lines[#lines+1]="Address is classified only; it is never called.";status.editBox:SetText(table.concat(lines,"\n"))
 end
 refresh:SetScript("OnClick",refreshStatus);check:SetScript("OnClick",inspect);refreshStatus()
end)
