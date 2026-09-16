local M={};MWDT.RegisterModule('internals',M)
MWDT.RegisterTab('internals','Client Internals',function(p)
 local output=MWDT.MakeScrollText(p);output:SetPoint('TOPLEFT',8,-40);output:SetPoint('BOTTOMRIGHT',-18,40)
 local refresh=MWDT.MakeButton(p,'Refresh',90,22);refresh:SetPoint('TOPLEFT',8,-8)
 local function show()
  local lines={'Client internals: read-only status; no candidate address is read or called.'}
  for _,entry in ipairs({{'Version','TBCExt_GetVersion'},{'Profile','TBCExt_GetClientProfile'},{'Core Lua','TBCExt_GetCoreLuaStatus'},{'Validator','TBCExt_GetCallbackValidatorStatus'},{'Trace','TBCExt_GetNativeTraceStatus'},{'Model hooks','TBCExt_GetModelHooksStatus'}}) do
   local fn=_G[entry[2]];local value='Unavailable'
   if type(fn)=='function' then local ok,v=pcall(fn);if ok then value=tostring(v) end end
   lines[#lines+1]=entry[1]..': '..value
  end
  lines[#lines+1]='\nMap/loading/object-manager reads: NOT CONFIGURED\nFriend labels: research only; see Native Functions.\nA readable/executable page does not establish a callable function.'
  output.editBox:SetText(table.concat(lines,'\n'))
 end
 refresh:SetScript('OnClick',show);show()
end)
