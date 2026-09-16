local M={};MWDT.RegisterModule('dbc',M)
MWDT.RegisterTab('dbc','DBC Inspector',function(p)
 local title=MWDT.MakeLabel(p,'DBC Inspector - native custom tables / separate offline samples',13);title:SetPoint('TOPLEFT',8,-8)
 local prev=MWDT.MakeButton(p,'Previous Table',110,22);prev:SetPoint('TOPLEFT',8,-38)
 local nextTable=MWDT.MakeButton(p,'Next Table',100,22);nextTable:SetPoint('LEFT',prev,'RIGHT',8,0)
 local back=MWDT.MakeButton(p,'Previous Page',110,22);back:SetPoint('LEFT',nextTable,'RIGHT',8,0)
 local forward=MWDT.MakeButton(p,'Next Page',90,22);forward:SetPoint('LEFT',back,'RIGHT',8,0)
 local file=MWDT.MakeEditBox(p,false);file:SetPoint('TOPLEFT',8,-70);file:SetWidth(250);file:SetHeight(22);file:SetText('TBCExt_Demo.dbc');MWDT.AttachEditBoxBG(file)
 local load=MWDT.MakeButton(p,'Load via DLL',105,22);load:SetPoint('LEFT',file,'RIGHT',8,0)
 local info=MWDT.MakeButton(p,'Native Info',95,22);info:SetPoint('LEFT',load,'RIGHT',8,0)
 local unload=MWDT.MakeButton(p,'Unload',70,22);unload:SetPoint('LEFT',info,'RIGHT',8,0)
 local id=MWDT.MakeEditBox(p,false);id:SetPoint('TOPLEFT',8,-102);id:SetWidth(100);id:SetHeight(22);id:SetText('1');MWDT.AttachEditBoxBG(id)
 local row=MWDT.MakeButton(p,'Read ID',85,22);row:SetPoint('LEFT',id,'RIGHT',8,0)
 local col=MWDT.MakeEditBox(p,false);col:SetPoint('LEFT',row,'RIGHT',8,0);col:SetWidth(45);col:SetHeight(22);col:SetText('2');MWDT.AttachEditBoxBG(col)
 local kind=MWDT.MakeEditBox(p,false);kind:SetPoint('LEFT',col,'RIGHT',8,0);kind:SetWidth(85);kind:SetHeight(22);kind:SetText('string');MWDT.AttachEditBoxBG(kind)
 local field=MWDT.MakeButton(p,'Read Field',95,22);field:SetPoint('LEFT',kind,'RIGHT',8,0)
 local hint=MWDT.MakeLabel(p,'ID | column (1-based) | type: string / float / uint32. Native load does not enable morphs.',11);hint:SetPoint('TOPLEFT',8,-132)
 local output=MWDT.MakeScrollText(p);output:SetPoint('TOPLEFT',8,-155);output:SetPoint('BOTTOMRIGHT',-18,12)
 local function native(action,...)
  if type(TBCExt_CustomDBC)~='function' then output.editBox:SetText('Native custom DBC API unavailable. Install DLL 0.1.3 / API 4. Offline sample buttons above still work.');return end
  local ok,text,status=pcall(TBCExt_CustomDBC,action,...)
  local result='Native DLL: '..(ok and tostring(status) or 'Lua error')..'\n'..tostring(text)
  output.editBox:SetText(string.gsub(result,'|','||'));output.editBox:SetCursorPosition(0)
 end
 load:SetScript('OnClick',function()native('load',file:GetText())end)
 info:SetScript('OnClick',function()native('info')end)
 unload:SetScript('OnClick',function()native('unload')end)
 row:SetScript('OnClick',function()native('row',tonumber(id:GetText()))end)
 field:SetScript('OnClick',function()native('field',tonumber(id:GetText()),tonumber(col:GetText()),kind:GetText())end)
 local selected,page=1,1
 local function render()
  local tables=MWDT_DBC_SAMPLES or {};local t=tables[selected]
  if not t then output.editBox:SetText('No offline samples generated. The addon never reads native DBC memory.');return end
  local lines={t.name,'Source: '..t.source,'SHA256: '..t.sha,'File rows: '..t.count..'; included: '..#t.rows..'; fields: '..t.fields,'Numbered fields are raw uint32 values, NOT inferred IDs/floats/string meanings.'}
  local first=(page-1)*20+1
  for i=first,math.min(first+19,#t.rows) do
   local parts={};for j=1,math.min(32,#t.rows[i]) do parts[#parts+1]='f'..j..'='..t.rows[i][j] end
   lines[#lines+1]='Row '..i..': '..table.concat(parts,' | ')
  end
  if t.fields>32 then lines[#lines+1]='Display limited to first 32 fields.' end
  lines[#lines+1]='Page '..page..' / '..math.max(1,math.ceil(#t.rows/20))
  output.editBox:SetText(table.concat(lines,'\n'));output.editBox:SetCursorPosition(0)
 end
 prev:SetScript('OnClick',function() selected=math.max(1,selected-1);page=1;render() end)
 nextTable:SetScript('OnClick',function() selected=math.min(#(MWDT_DBC_SAMPLES or {}),selected+1);page=1;render() end)
 back:SetScript('OnClick',function() page=math.max(1,page-1);render() end)
 forward:SetScript('OnClick',function() local t=(MWDT_DBC_SAMPLES or {})[selected];if t then page=math.min(math.max(1,math.ceil(#t.rows/20)),page+1) end;render() end)
 render()
end)
