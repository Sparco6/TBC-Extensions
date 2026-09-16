local M={}
MWDT.RegisterModule("unit",M)
MWDT.RegisterTab("unit","Unit / Target",function(p)
 local h=MWDT.MakeLabel(p,"Unit / Target Inspector",15); h:SetPoint("TOPLEFT",8,-8)
 local unit=MWDT.MakeEditBox(p,false); unit:SetPoint("TOPLEFT",8,-34); unit:SetWidth(180); unit:SetHeight(22); unit:SetText("target"); MWDT.AttachEditBoxBG(unit)
 local go=MWDT.MakeButton(p,"Inspect",80,22); go:SetPoint("LEFT",unit,"RIGHT",8,0)
 local out=MWDT.MakeScrollText(p); out:SetPoint("TOPLEFT",8,-66); out:SetPoint("BOTTOMRIGHT",-18,8); M.out=out.editBox
 local function inspect()
  local u=MWDT.Trim(unit:GetText()); if u=="" then u="target" end
  local r={}; local function add(k,v) r[#r+1]=k..": "..tostring(v) end
  add("Exists",UnitExists and UnitExists(u)); add("Name",UnitName and UnitName(u)); add("GUID",UnitGUID and UnitGUID(u)); add("Level",UnitLevel and UnitLevel(u)); add("Class",UnitClass and UnitClass(u)); add("Race",UnitRace and UnitRace(u)); add("Sex",UnitSex and UnitSex(u)); add("Faction",UnitFactionGroup and UnitFactionGroup(u)); add("CreatureType",UnitCreatureType and UnitCreatureType(u)); add("Classification",UnitClassification and UnitClassification(u)); add("Health",UnitHealth and UnitHealth(u)); add("MaxHealth",UnitHealthMax and UnitHealthMax(u)); add("Mana",UnitMana and UnitMana(u)); add("MaxMana",UnitManaMax and UnitManaMax(u)); add("PowerType",UnitPowerType and UnitPowerType(u)); add("Reaction",UnitReaction and UnitReaction("player",u)); add("IsPlayer",UnitIsPlayer and UnitIsPlayer(u)); add("IsDead",UnitIsDead and UnitIsDead(u)); add("IsGhost",UnitIsGhost and UnitIsGhost(u)); add("InCombat",UnitAffectingCombat and UnitAffectingCombat(u)); add("IsFriend",UnitIsFriend and UnitIsFriend("player",u)); add("IsEnemy",UnitIsEnemy and UnitIsEnemy("player",u)); add("CanAttack",UnitCanAttack and UnitCanAttack("player",u)); add("PVP",UnitIsPVP and UnitIsPVP(u)); add("Tapped",UnitIsTapped and UnitIsTapped(u)); add("Connected",UnitIsConnected and UnitIsConnected(u)); add("Visible",UnitIsVisible and UnitIsVisible(u))
  M.out:SetText(table.concat(r,"\n")); M.out:SetCursorPosition(0)
 end
 go:SetScript("OnClick",inspect); M.Inspect=inspect
end)
