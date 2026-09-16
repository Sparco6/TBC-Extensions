local M={}
MWDT.RegisterModule("research",M)

local SCAN_BATCH_SIZE=50
local RESULTS_PER_PAGE=75
local MAX_RESULTS_RENDERED=RESULTS_PER_PAGE
local MAX_EXPORT_ROWS=5000
local MAX_OUTPUT_CHARS=240000
local MAX_RESULTS=12000

local function esc(v)
 v=tostring(v or "")
 if string.find(v,'[",\n\r]') then v='"'..string.gsub(v,'"','""')..'"' end
 return v
end

local function preview(v)
 local kind=type(v)
 if kind=="function" then return "<function>" end
 if kind=="table" then
  local raw=tostring(v); local p=string.match(raw,"[Tt]able:%s*([0-9A-Fa-f]+)")
  return p and ("Runtime pointer: 0x"..string.upper(p).." | Pointer type: LUA_RUNTIME_POINTER | Stable: NO") or "<Lua table>"
 end
 if kind=="userdata" then return "<userdata:"..tostring(v)..">" end
 if kind=="thread" then return "<thread>" end
 local s=tostring(v); if string.len(s)>180 then s=string.sub(s,1,177).."..." end
 return s
end

local function getNotes()
 MasterWoWDevToolsDB=MasterWoWDevToolsDB or {}
 MasterWoWDevToolsDB.researchNotes=MasterWoWDevToolsDB.researchNotes or {}
 return MasterWoWDevToolsDB.researchNotes
end

MWDT.RegisterTab("research","Research / Export",function(p)
 local h=MWDT.MakeLabel(p,"Runtime Research / Address Notebook",15); h:SetPoint("TOPLEFT",8,-8)
 local note=MWDT.MakeLabel(p,"Manual, incremental, cancelable scans. This tab never calls native addresses.",11); note:SetPoint("TOPLEFT",8,-28); note:SetWidth(620); note:SetJustifyH("LEFT")
 local search=MWDT.MakeEditBox(p,false); search:SetPoint("TOPLEFT",8,-58); search:SetWidth(190); search:SetHeight(22); MWDT.AttachEditBoxBG(search)
 local scan=MWDT.MakeButton(p,"Scan Runtime",96,22); scan:SetPoint("LEFT",search,"RIGHT",8,0)
 local stop=MWDT.MakeButton(p,"Stop Scan",78,22); stop:SetPoint("LEFT",scan,"RIGHT",6,0)
 local clear=MWDT.MakeButton(p,"Clear",58,22); clear:SetPoint("LEFT",stop,"RIGHT",6,0)
 local export=MWDT.MakeButton(p,"Export Current",102,22); export:SetPoint("LEFT",clear,"RIGHT",6,0)
 local exportAll=MWDT.MakeButton(p,"Export All",72,22); exportAll:SetPoint("LEFT",export,"RIGHT",6,0)
 local progress=MWDT.MakeLabel(p,"Runtime scan: idle",11); progress:SetPoint("TOPLEFT",8,-84); progress:SetWidth(430); progress:SetJustifyH("LEFT")
 local prev=MWDT.MakeButton(p,"<",28,20); prev:SetPoint("TOPRIGHT",p,"TOPRIGHT",-136,-80)
 local pageText=MWDT.MakeLabel(p,"Page 0 / 0",11); pageText:SetPoint("LEFT",prev,"RIGHT",5,0); pageText:SetWidth(70); pageText:SetJustifyH("CENTER")
 local nextPage=MWDT.MakeButton(p,">",28,20); nextPage:SetPoint("LEFT",pageText,"RIGHT",5,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-108); out:SetPoint("BOTTOMRIGHT",-18,90); M.out=out.editBox
 local name=MWDT.MakeEditBox(p,false); name:SetPoint("BOTTOMLEFT",8,54); name:SetWidth(150); name:SetHeight(22); name:SetText("function/global"); MWDT.AttachEditBoxBG(name)
 local addr=MWDT.MakeEditBox(p,false); addr:SetPoint("LEFT",name,"RIGHT",8,0); addr:SetWidth(105); addr:SetHeight(22); addr:SetText("0x00000000"); MWDT.AttachEditBoxBG(addr)
 local category=MWDT.MakeEditBox(p,false); category:SetPoint("LEFT",addr,"RIGHT",8,0); category:SetWidth(105); category:SetHeight(22); category:SetText("Unknown"); MWDT.AttachEditBoxBG(category)
 local add=MWDT.MakeButton(p,"Add Note",72,22); add:SetPoint("LEFT",category,"RIGHT",8,0)
 local notes=MWDT.MakeButton(p,"Show Notes",82,22); notes:SetPoint("LEFT",add,"RIGHT",6,0)
 local exportNotes=MWDT.MakeButton(p,"Export Notes",88,22); exportNotes:SetPoint("LEFT",notes,"RIGHT",6,0)
 local desc=MWDT.MakeEditBox(p,false); desc:SetPoint("BOTTOMLEFT",8,22); desc:SetWidth(410); desc:SetHeight(22); desc:SetText("observation / evidence / confidence"); MWDT.AttachEditBoxBG(desc)
 local nextExport=MWDT.MakeButton(p,"Next Export",100,22);nextExport:SetPoint("LEFT",desc,"RIGHT",8,0)
 local streamExport=MWDT.MakeButton(p,"Export _G",90,22);streamExport:SetPoint("LEFT",nextExport,"RIGHT",8,0)

 local function catalogRecord()
  local raw=string.gsub(addr:GetText() or "","^0[xX]","")
  local value=tonumber(raw,16)
  if not value or value<1 or value>4294967295 or value~=math.floor(value) then return nil,nil end
  return value, value and MWDT_NATIVE_BY_ADDRESS and MWDT_NATIVE_BY_ADDRESS[value]
 end
 local function fillFromCatalog(showResult)
  local value,rec=catalogRecord()
  if not value then if showResult then MWDT.Print("Invalid hexadecimal notebook address.") end return nil end
  addr:SetText(string.format("0x%08X",value))
  if rec then
   name:SetText(rec.n~="" and rec.n or rec.g);category:SetText(rec.c or "Unknown")
   if showResult then MWDT.Print("Catalog match: "..tostring(rec.g).." | "..tostring(rec.s).." | "..tostring(rec.q)) end
  elseif showResult then MWDT.Print("No native catalog record exists at "..string.format("0x%08X",value)..".") end
  return rec
 end

 local state={mode=nil,key=nil,processed=0,results={},page=1,query="",exportIndex=0,exportLines=nil,exportChars=0}
 local function stopWork(message) state.mode=nil; if message then progress:SetText(message) end end
 local function pageCount() if #state.results==0 then return 0 end return math.ceil(#state.results/RESULTS_PER_PAGE) end
 local function renderPage()
  local pages=pageCount()
  if pages==0 then state.page=1; pageText:SetText("Page 0 / 0"); M.out:SetText("No runtime results. Click Scan Runtime to begin."); M.out:SetCursorPosition(0); return end
  if state.page<1 then state.page=1 elseif state.page>pages then state.page=pages end
  local first=(state.page-1)*RESULTS_PER_PAGE+1
  local last=math.min(first+MAX_RESULTS_RENDERED-1,#state.results)
  local lines={"Runtime matches: "..#state.results,"Showing "..first.."-"..last,""}
  for i=first,last do local row=state.results[i]; lines[#lines+1]=row.name.." ["..row.kind.."]  "..row.value end
  M.out:SetText(table.concat(lines,"\n")); M.out:SetCursorPosition(0); pageText:SetText("Page "..state.page.." / "..pages)
 end
 local function beginScan()
  state.resumeExport=nil
  stopWork(); state.key=nil; state.processed=0; state.results={}; state.page=1; state.query=string.lower(search:GetText() or ""); state.mode="scan"
  progress:SetText("Runtime scan: 0 / scanning..."); M.out:SetText("Scanning incrementally. Stop Scan cancels the operation.")
 end
 local function scanBatch()
  local count=0
  while count<SCAN_BATCH_SIZE do
   local k,v=next(_G,state.key); state.key=k
   if k==nil then table.sort(state.results,function(a,b) return a.name<b.name end); stopWork("Runtime scan: "..state.processed.." / "..state.processed.." complete ("..#state.results.." matches)"); renderPage(); return end
   state.processed=state.processed+1; count=count+1
   local ks=tostring(k)
   if (state.query=="" or string.find(string.lower(ks),state.query,1,true)) and #state.results<MAX_RESULTS then state.results[#state.results+1]={name=ks,kind=type(v),value=preview(v)} end
  end
  progress:SetText("Runtime scan: "..state.processed.." / scanning... ("..#state.results.." matches)")
 end

 local header="address,ghidra_name,friendly_name,category,signature,calling_convention,size,status,confidence,danger_level,description,evidence,test_notes,hit_count"
 local function beginExport(allRows,notebook,resumeIndex)
  stopWork(); state.exportLines={header}; state.exportChars=string.len(header)
  state.exportIndex=resumeIndex or ((allRows or notebook) and 1 or ((state.page-1)*RESULTS_PER_PAGE+1));state.exportStart=state.exportIndex;state.resumeExport=nil; state.mode=notebook and "exportNotes" or (allRows and "exportAll" or "exportPage"); progress:SetText("Export: preparing bounded chunk...")
 end
 local function finishChunk(total,notebook)
  local rows=#state.exportLines-1
  state.resumeExport=state.exportIndex<=total and {index=state.exportIndex,notebook=notebook} or nil
  M.out:SetText(table.concat(state.exportLines,"\n"));M.out:HighlightText();M.out:SetFocus()
  stopWork("Export rows "..state.exportStart.."-"..(state.exportIndex-1).." / "..total..(state.resumeExport and "; copy, then Next Export" or "; complete"))
  MWDT.Print("Copy this CSV chunk before Next Export. Runtime exports cover retained results only (cap "..MAX_RESULTS..").")
 end
 local function beginStream(resume)
  stopWork();state.resumeExport=nil;state.exportLines={header};state.exportChars=string.len(header)
  if not resume then state.streamKey=nil;state.streamPending=nil;state.streamCount=0 end
  state.streamStart=state.streamCount+1;state.mode="streamGlobals";progress:SetText("Export _G: scanning incrementally (not a native API list)")
 end
 local function streamBatch()
  local count=0
  while count<SCAN_BATCH_SIZE do
   local line=state.streamPending
   local done=false
   if not line then
    local ok,k,v=pcall(next,_G,state.streamKey)
    if not ok then stopWork("Globals changed during export; restart Export _G.");return end
    state.streamKey=k;done=k==nil
    if not done then line=table.concat({"",esc(k),esc(k),esc(type(v)),"","","","UNKNOWN","LOW","SAFE_READ",esc(preview(v)),"runtime _G stream; origin unknown","","0"},",") end
   end
   if done or state.exportChars+string.len(line)+1>MAX_OUTPUT_CHARS or #state.exportLines-1>=MAX_EXPORT_ROWS then
    state.streamPending=line
    if not done and #state.exportLines==1 then stopWork("One global exceeds the chunk limit; use a filtered scan instead.");return end
    state.resumeExport=not done and {stream=true} or nil
    M.out:SetText(table.concat(state.exportLines,"\n"));M.out:HighlightText();M.out:SetFocus()
    stopWork("_G export rows "..state.streamStart.."-"..state.streamCount..(done and "; complete" or "; copy, then Next Export"))
    return
   end
   state.exportLines[#state.exportLines+1]=line;state.exportChars=state.exportChars+string.len(line)+1
   state.streamPending=nil;state.streamCount=state.streamCount+1;count=count+1
  end
  progress:SetText("_G export: "..state.streamCount.." globals processed; building bounded chunk")
 end
 streamExport:SetScript("OnClick",function() beginStream(false) end)
 nextExport:SetScript("OnClick",function() local r=state.resumeExport;if r and r.stream then beginStream(true) elseif r then beginExport(true,r.notebook,r.index) else MWDT.Print("No remaining export chunk. Start an export first.") end end)
 local function exportBatch()
  if state.mode=="exportNotes" then
   local notebook=getNotes(); local last=#notebook; local count=0
   while state.exportIndex<=last and count<SCAN_BATCH_SIZE do
    local x=notebook[state.exportIndex]
    local line=table.concat({esc(x.address),esc(x.ghidra_name),esc(x.friendly_name or x.name),esc(x.category),esc(x.signature),esc(x.calling_convention),esc(x.size),esc(x.status or "UNKNOWN"),esc(x.confidence or "LOW"),esc(x.danger_level or "INSTRUMENT_ONLY"),esc(x.description),esc(x.evidence),esc(x.test_notes),esc(x.hit_count or 0)},",")
    if state.exportChars+string.len(line)+1>MAX_OUTPUT_CHARS or #state.exportLines-1>=MAX_EXPORT_ROWS then
     if #state.exportLines==1 then stopWork("One note exceeds the export character cap; shorten that note.");return end
     finishChunk(last,true);return
    end
    state.exportLines[#state.exportLines+1]=line; state.exportChars=state.exportChars+string.len(line)+1; state.exportIndex=state.exportIndex+1; count=count+1
   end
   if state.exportIndex>last then finishChunk(last,true) else progress:SetText("Notebook export: "..(state.exportIndex-1).." / "..last) end
   return
  end
  local first=state.mode=="exportPage" and ((state.page-1)*RESULTS_PER_PAGE+1) or 1
  local last=state.mode=="exportPage" and math.min(first+RESULTS_PER_PAGE-1,#state.results) or #state.results
  local count=0
  while state.exportIndex<=last and count<SCAN_BATCH_SIZE do
   local row=state.results[state.exportIndex]
   local line=table.concat({"",esc(row.name),esc(row.name),esc(row.kind),"","","","UNKNOWN","LOW","SAFE_READ",esc(row.value),"runtime _G scan","","0"},",")
   if state.exportChars+string.len(line)+1>MAX_OUTPUT_CHARS or #state.exportLines-1>=MAX_EXPORT_ROWS then finishChunk(last,false);return end
   state.exportLines[#state.exportLines+1]=line; state.exportChars=state.exportChars+string.len(line)+1; state.exportIndex=state.exportIndex+1; count=count+1
  end
  if state.exportIndex>last then
   finishChunk(last,false)
  else progress:SetText("Export: "..math.max(0,state.exportIndex-first).." / "..math.max(0,last-first+1)) end
 end
 local worker=CreateFrame("Frame",nil,p)
 worker:SetScript("OnUpdate",function() if state.mode=="scan" then scanBatch() elseif state.mode=="streamGlobals" then streamBatch() elseif state.mode=="exportPage" or state.mode=="exportAll" or state.mode=="exportNotes" then exportBatch() end end)

 local function showNotes()
  stopWork("Runtime scan: idle")
  local n=getNotes(); local lines={"Saved research notes: "..#n,""}; local chars=0; local first=math.max(1,#n-RESULTS_PER_PAGE+1)
  for i=first,#n do local x=n[i]; local line=i..". "..tostring(x.friendly_name or x.name).." | "..tostring(x.address).." | "..tostring(x.category).." | "..tostring(x.status or "UNKNOWN").." | "..tostring(x.description); chars=chars+string.len(line)+1; if chars>MAX_OUTPUT_CHARS then break end; lines[#lines+1]=line end
  M.out:SetText(table.concat(lines,"\n")); M.out:SetCursorPosition(0)
 end
 scan:SetScript("OnClick",beginScan)
 stop:SetScript("OnClick",function() if state.mode then stopWork("Stopped after "..state.processed.." runtime entries.") end end)
 clear:SetScript("OnClick",function() stopWork("Runtime scan: idle");state.resumeExport=nil; state.results={}; state.page=1; renderPage() end)
 export:SetScript("OnClick",function() if #state.results>0 then beginExport(false) else MWDT.Print("Nothing to export. Scan Runtime first.") end end)
 exportAll:SetScript("OnClick",function() if #state.results>0 then beginExport(true) else MWDT.Print("Nothing to export. Scan Runtime first.") end end)
 exportNotes:SetScript("OnClick",function() if #getNotes()>0 then beginExport(false,true) else MWDT.Print("The research notebook is empty.") end end)
 prev:SetScript("OnClick",function() state.page=state.page-1; renderPage() end); nextPage:SetScript("OnClick",function() state.page=state.page+1; renderPage() end)
 add:SetScript("OnClick",function()
  local value,rec=catalogRecord()
  if not rec then MWDT.Print("Native notes require an exact catalog address. Runtime/heap pointers are not native function records.");return end
  local friendly=name:GetText();if friendly=="" or friendly=="function/global" then friendly=rec.n~="" and rec.n or rec.g end
  local cat=category:GetText();if cat=="" or cat=="Unknown" then cat=rec.c end
  local n=getNotes(); n[#n+1]={address=string.format("0x%08X",value),ghidra_name=rec.g,friendly_name=friendly,name=friendly,category=cat,signature="",calling_convention="",size=rec.z or "",status=rec.s or "UNKNOWN",confidence=rec.q or "LOW",danger_level="INSTRUMENT_ONLY",description=desc:GetText(),evidence="NativeFunctions.lua generated from functions_enriched.csv; manual observation is not verification",source="MasterWoW DevTools",test_notes="",hit_count=0,last_test="",build=GetBuildInfo and tostring(GetBuildInfo()) or "unknown",character=UnitName and UnitName("player") or "",time=date and date("%Y-%m-%d %H:%M:%S") or ""}; showNotes()
 end)
 notes:SetScript("OnClick",showNotes); search:SetScript("OnEnterPressed",function() this:ClearFocus() end);addr:SetScript("OnEnterPressed",function() this:ClearFocus();fillFromCatalog(true) end)
 p:SetScript("OnHide",function() if state.mode then stopWork("Stopped because the Research tab was hidden.") end end)
 MWDT.OpenResearchAddress=function(value) addr:SetText(value or '');fillFromCatalog(true) end
 M.ShowNotes=showNotes; renderPage()
end)
