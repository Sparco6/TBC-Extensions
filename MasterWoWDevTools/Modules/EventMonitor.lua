local M={enabled=false,lines={},filter=""}
MWDT.RegisterModule("events",M)
MWDT.RegisterTab("events","Events",function(p)
 local h=MWDT.MakeLabel(p,"Event Monitor",15); h:SetPoint("TOPLEFT",8,-8)
 local toggle=MWDT.MakeButton(p,"Start",80,22); toggle:SetPoint("TOPLEFT",8,-34)
 local clear=MWDT.MakeButton(p,"Clear",80,22); clear:SetPoint("LEFT",toggle,"RIGHT",6,0)
 local filter=MWDT.MakeEditBox(p,false); filter:SetPoint("LEFT",clear,"RIGHT",8,0); filter:SetWidth(220); filter:SetHeight(22); MWDT.AttachEditBoxBG(filter)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local ef=CreateFrame("Frame"); M.frame=ef
 local function reg(on)
   if on then for _,e in ipairs(MWDT_KNOWN_EVENTS or {}) do pcall(ef.RegisterEvent,ef,e) end else ef:UnregisterAllEvents() end
 end
 toggle:SetScript("OnClick",function() M.enabled=not M.enabled; reg(M.enabled); this:SetText(M.enabled and "Stop" or "Start") end)
 clear:SetScript("OnClick",function() M.lines={}; M.out:SetText("") end)
 filter:SetScript("OnTextChanged",function() M.filter=string.lower(this:GetText() or "") end)
 ef:SetScript("OnEvent",function()
   if not M.enabled then return end; if M.filter~="" and not string.find(string.lower(event),M.filter,1,true) then return end
   local vals={}; for i=1,9 do local v=_G["arg"..i]; if v~=nil then vals[#vals+1]="arg"..i.."="..MWDT.SerializeValue(v) end end
   local line=date("%H:%M:%S").."  "..event..(#vals>0 and ("  "..table.concat(vals,"  ")) or "")
   table.insert(M.lines,1,line); while table.getn(M.lines)>400 do table.remove(M.lines) end
   M.out:SetText(table.concat(M.lines,"\n")); M.out:SetCursorPosition(0)
 end)
end)
