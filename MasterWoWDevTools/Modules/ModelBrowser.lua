local M={}
MWDT.RegisterModule("models",M)
MWDT.RegisterTab("models","Model Browser",function(p)
 local h=MWDT.MakeLabel(p,"In-game Model / DressUp Browser",15); h:SetPoint("TOPLEFT",8,-8)
 local path=MWDT.MakeEditBox(p,false); path:SetPoint("TOPLEFT",8,-36); path:SetWidth(390); path:SetHeight(22); path:SetText("Character\\Human\\Male\\HumanMale.m2"); MWDT.AttachEditBoxBG(path)
 local load=MWDT.MakeButton(p,"Load Model",90,22); load:SetPoint("LEFT",path,"RIGHT",8,0)
 local player=MWDT.MakeButton(p,"Player",70,22); player:SetPoint("LEFT",load,"RIGHT",6,0)
 local clear=MWDT.MakeButton(p,"Clear",65,22); clear:SetPoint("LEFT",player,"RIGHT",6,0)
 local model=CreateFrame("PlayerModel",nil,p); model:SetPoint("TOPLEFT",8,-72); model:SetWidth(400); model:SetHeight(430); model:SetBackdrop({bgFile="Interface\\Tooltips\\UI-Tooltip-Background",edgeFile="Interface\\Tooltips\\UI-Tooltip-Border",tile=true,tileSize=16,edgeSize=12,insets={left=3,right=3,top=3,bottom=3}}); model:SetBackdropColor(0,0,0,1); M.model=model
 local rotateL=MWDT.MakeButton(p,"Rotate -",75,22); rotateL:SetPoint("TOPLEFT",430,-84)
 local rotateR=MWDT.MakeButton(p,"Rotate +",75,22); rotateR:SetPoint("LEFT",rotateL,"RIGHT",8,0)
 local zoomIn=MWDT.MakeButton(p,"Zoom +",75,22); zoomIn:SetPoint("TOPLEFT",430,-116)
 local zoomOut=MWDT.MakeButton(p,"Zoom -",75,22); zoomOut:SetPoint("LEFT",zoomIn,"RIGHT",8,0)
 local anim=MWDT.MakeEditBox(p,false); anim:SetPoint("TOPLEFT",430,-154); anim:SetWidth(70); anim:SetHeight(22); anim:SetText("0"); MWDT.AttachEditBoxBG(anim)
 local setAnim=MWDT.MakeButton(p,"Animation",85,22); setAnim:SetPoint("LEFT",anim,"RIGHT",8,0)
 local item=MWDT.MakeEditBox(p,false); item:SetPoint("TOPLEFT",430,-194); item:SetWidth(100); item:SetHeight(22); item:SetText("item id"); MWDT.AttachEditBoxBG(item)
 local dress=MWDT.MakeButton(p,"Dress Item",85,22); dress:SetPoint("LEFT",item,"RIGHT",8,0)
 local info=MWDT.MakeScrollText(p); info:SetPoint("TOPLEFT",430,-230); info:SetPoint("BOTTOMRIGHT",-18,8); M.info=info.editBox
 local previous=MWDT.MakeButton(p,"Previous",75,22);previous:SetPoint("TOPLEFT",430,-222)
 local nextKnown=MWDT.MakeButton(p,"Next",75,22);nextKnown:SetPoint("LEFT",previous,"RIGHT",8,0)
 info:ClearAllPoints();info:SetPoint("TOPLEFT",430,-254);info:SetPoint("BOTTOMRIGHT",-18,8)
 local facing=0; local scale=1;local catalogIndex=0
 local function msg(s) M.info:SetText(tostring(s or "")) end
 load:SetScript("OnClick",function()
  local v=path:GetText() or ""; local ok,err=pcall(function() model:SetModel(v) end); msg("SetModel("..v..") => "..tostring(ok)..(err and ("\n"..tostring(err)) or ""))
 end)
 player:SetScript("OnClick",function() local ok,err=pcall(function() model:SetUnit("player") end); msg("SetUnit(player) => "..tostring(ok)..(err and ("\n"..tostring(err)) or "")) end)
 clear:SetScript("OnClick",function() if model.ClearModel then model:ClearModel() end end)
 rotateL:SetScript("OnClick",function() facing=facing-0.2; if model.SetFacing then model:SetFacing(facing) end end)
 rotateR:SetScript("OnClick",function() facing=facing+0.2; if model.SetFacing then model:SetFacing(facing) end end)
 zoomIn:SetScript("OnClick",function() scale=scale*1.15; if model.SetModelScale then model:SetModelScale(scale) end end)
 zoomOut:SetScript("OnClick",function() scale=scale/1.15; if model.SetModelScale then model:SetModelScale(scale) end end)
 setAnim:SetScript("OnClick",function() local n=tonumber(anim:GetText()) or 0; local ok,err=pcall(function() if model.SetSequence then model:SetSequence(n) elseif model.SetAnimation then model:SetAnimation(n) end end); msg("Animation "..n.." => "..tostring(ok)..(err and ("\n"..tostring(err)) or "")) end)
 dress:SetScript("OnClick",function()
  local id=tonumber(item:GetText()); if not id then msg("Enter an item ID.") return end
  local ok,err=pcall(function()
   if model.SetUnit then model:SetUnit("player") end
   if model.TryOn then model:TryOn(id) elseif DressUpItemLink and GetItemInfo then local _,link=GetItemInfo(id); if link then DressUpItemLink(link) end end
  end); msg("Dress item "..id.." => "..tostring(ok)..(err and ("\n"..tostring(err)) or ""))
 end)
 local function selectKnown(step)
  local c=MWDT_MODEL_CATALOG or {};if #c==0 then msg("Model catalog is empty. Generate it from extracted client assets first.") return end
  catalogIndex=catalogIndex+step;if catalogIndex<1 then catalogIndex=#c elseif catalogIndex>#c then catalogIndex=1 end
  local r=c[catalogIndex];path:SetText(r[1]);msg(catalogIndex.." / "..#c.."\n"..r[2].."\nCategory: "..r[3].."\nSource: "..r[4].."\nPress Load Model to preview explicitly.")
 end
 previous:SetScript("OnClick",function()selectKnown(-1)end);nextKnown:SetScript("OnClick",function()selectKnown(1)end)
 msg("Load an existing client .m2 path, show your player, rotate/zoom, or test animation IDs. Invalid paths normally render nothing rather than proving a file exists.")
end)
