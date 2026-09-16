local M={};MWDT.RegisterModule('nativecatalog',M)
MWDT.RegisterTab('nativecatalog','Native Functions',function(p)
 local title=MWDT.MakeLabel(p,'Native catalog - names are evidence, not permission to call',14);title:SetPoint('TOPLEFT',8,-8)
 local query=MWDT.MakeEditBox(p,false);query:SetPoint('TOPLEFT',8,-38);query:SetWidth(240);query:SetHeight(22);MWDT.AttachEditBoxBG(query)
 local filter=MWDT.MakeEditBox(p,false);filter:SetPoint('LEFT',query,'RIGHT',8,0);filter:SetWidth(110);filter:SetHeight(22);filter:SetText('ALL');MWDT.AttachEditBoxBG(filter)
 local search=MWDT.MakeButton(p,'Search',70,22);search:SetPoint('LEFT',filter,'RIGHT',8,0)
 local stop=MWDT.MakeButton(p,'Stop',60,22);stop:SetPoint('LEFT',search,'RIGHT',8,0)
 local status=MWDT.MakeLabel(p,'Filter: ALL, VERIFIED, OBSERVED, CANDIDATE, UNKNOWN. Search explicitly.',11);status:SetPoint('TOPLEFT',8,-68)
 local output=MWDT.MakeScrollText(p);output:SetPoint('TOPLEFT',8,-94);output:SetPoint('BOTTOMRIGHT',-18,78)
 local prev=MWDT.MakeButton(p,'Previous',80,22);prev:SetPoint('BOTTOMLEFT',8,44)
 local nextPage=MWDT.MakeButton(p,'Next',60,22);nextPage:SetPoint('LEFT',prev,'RIGHT',8,0)
 local address=MWDT.MakeEditBox(p,false);address:SetPoint('LEFT',nextPage,'RIGHT',16,0);address:SetWidth(120);address:SetHeight(22);address:SetText('0x007059B0');MWDT.AttachEditBoxBG(address)
 local note=MWDT.MakeButton(p,'Open Notebook',120,22);note:SetPoint('LEFT',address,'RIGHT',8,0)
 local results,page,index,active={},1,1,false
 local q,kind='', 'ALL'
 local function render()
  local lines={};local first=(page-1)*50+1
  for i=first,math.min(first+49,#results) do
   local r=results[i];local proposed=MWDT_PROPOSED_LABELS and MWDT_PROPOSED_LABELS[r.a]
   lines[#lines+1]=string.format('0x%08X',r.a)..' | '..(r.n~='' and r.n or r.g)..' | '..r.c..' | '..r.s..' / '..r.q
   if proposed then lines[#lines+1]='  Proposed (CANDIDATE/LOW, NOT VERIFIED): '..proposed end
  end
  output.editBox:SetText(table.concat(lines,'\n'));output.editBox:SetCursorPosition(0)
  status:SetText(#results..' matches | Page '..page..' / '..math.max(1,math.ceil(#results/50)))
 end
 search:SetScript('OnClick',function() q=string.lower(query:GetText() or '');kind=string.upper(filter:GetText() or 'ALL');results={};page=1;index=1;active=true;status:SetText('Searching...') end)
 stop:SetScript('OnClick',function() active=false;render() end)
 prev:SetScript('OnClick',function() page=math.max(1,page-1);render() end)
 nextPage:SetScript('OnClick',function() page=math.min(math.max(1,math.ceil(#results/50)),page+1);render() end)
 note:SetScript('OnClick',function() MWDT.ShowTab('research');if MWDT.OpenResearchAddress then MWDT.OpenResearchAddress(address:GetText()) end end)
 local worker=CreateFrame('Frame',nil,p)
 worker:SetScript('OnUpdate',function()
  if not active then return end
  local catalog=MWDT_NATIVE_FUNCTIONS or {}
  local extras=MWDT_PROPOSED_NONFUNCTIONS or {};local total=#catalog+#extras
  for n=1,100 do
   local r=catalog[index] or extras[index-#catalog];if not r then active=false;render();return end
   local proposed=MWDT_PROPOSED_LABELS and MWDT_PROPOSED_LABELS[r.a] or ''
   local hay=string.lower(string.format('0x%08X',r.a)..' '..r.g..' '..r.n..' '..r.c..' '..proposed)
   if (kind=='ALL' or r.s==kind or (kind=='CANDIDATE' and proposed~='')) and string.find(hay,q,1,true) then results[#results+1]=r end
   index=index+1
  end
  status:SetText('Searching '..(index-1)..' / '..total)
 end)
 p:SetScript('OnHide',function() active=false end)
end)
