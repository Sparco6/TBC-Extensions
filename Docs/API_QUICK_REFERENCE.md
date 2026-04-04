# API Quick Reference

Quick lookup table for all TBC-Extensions Lua API functions.

**Status:** ✅ Works | 🔧 TODO (address needed) | ⚠️ Stub (safe default)

---

## Spell API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `GetSpellInfo(id)` | WotLK | Spell name, rank, icon, cost | 🔧 TODO |
| `GetSpellTexture(id)` | WotLK | Spell icon path | ⚠️ Stub |
| `GetSpellDescription(id)` | WotLK | Parsed spell tooltip text | 🔧 TODO |
| `GetSpellNameById(id)` | Custom | Spell name and rank | 🔧 TODO |
| `GetSpellCooldown(id)` | WotLK | start, duration, enabled | ⚠️ Stub |
| `GetSpellCharges(id)` | MoP | Charge count (N/A in TBC) | ⚠️ Stub |
| `GetSpellCount(id)` | WotLK | Stack count | ⚠️ Stub |
| `IsSpellKnown(id)` | WotLK | Is spell learned | ⚠️ Stub |
| `GetSpellLink(id)` | WotLK | Spell hyperlink | ✅ Works |
| `GetSpellPowerCost(id)` | WotLK | Power cost and type | ⚠️ Stub |
| `GetSpellCastTime(id)` | WotLK | Cast time in ms | ⚠️ Stub |
| `GetSpellRange(id)` | WotLK | Min/max range | ⚠️ Stub |
| `IsHarmfulSpell(id)` | WotLK | Is offensive spell | ⚠️ Stub |
| `IsHelpfulSpell(id)` | WotLK | Is beneficial spell | ⚠️ Stub |
| `IsPassiveSpell(id)` | WotLK | Is passive spell | 🔧 TODO |
| `GetSpellAutocast(id)` | WotLK | Pet autocast state | ⚠️ Stub |
| `ToggleSpellAutocast(id)` | WotLK | Toggle pet autocast | ⚠️ Stub |
| `GetNumSpellTabs()` | WotLK | Spellbook tab count | ⚠️ Stub |
| `GetSpellTabInfo(tab)` | WotLK | Tab name, tex, offset, num | ⚠️ Stub |
| `GetSpellBookItemInfo(slot, type)` | WotLK | Spell in book slot | ⚠️ Stub |
| `FindSpellBookSlotBySpellID(id)` | WotLK | Find spell in book | ⚠️ Stub |

---

## Talent API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `GetNumTalentTabs()` | WotLK | Number of talent trees | ✅ Works |
| `GetTalentTabInfo(tab)` | WotLK | Tree name, icon, points, bg | ⚠️ Stub |
| `GetNumTalents(tab)` | WotLK | Talents in tree | ⚠️ Stub |
| `GetTalentInfo(tab, index)` | WotLK | Talent details | ⚠️ Stub |
| `LearnTalent(tab, index)` | WotLK | Learn a talent | ⚠️ Stub |
| `GetUnspentTalentPoints()` | WotLK | Available points | 🔧 TODO |
| `GetActiveSpecGroup()` | WotLK 3.1+ | Active spec (always 1) | ⚠️ Stub |
| `GetNumSpecGroups()` | WotLK 3.1+ | Spec count (always 1) | ⚠️ Stub |
| `SetActiveSpecGroup(n)` | WotLK 3.1+ | Switch spec (N/A in TBC) | ⚠️ Stub |
| `GetSpecialization()` | Cata | Dominant talent tree | ⚠️ Stub |
| `GetSpecializationInfo(spec)` | Cata | Spec tree info | ⚠️ Stub |
| `GetPrimaryTalentTree()` | Cata | Same as GetSpecialization | ⚠️ Stub |

---

## Item API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `GetItemInfo(id)` | WotLK | Full item info | ⚠️ Stub |
| `GetItemInfoInstant(id)` | WotLK | Item info, no server query | ⚠️ Stub |
| `GetItemIcon(id)` | WotLK | Item icon path | ⚠️ Stub |
| `GetItemQualityColor(quality)` | WotLK | Quality color RGB | ✅ Works |
| `GetItemCount(id)` | WotLK | Count in bags | ⚠️ Stub |
| `GetItemCooldown(id)` | WotLK | Item cooldown | ⚠️ Stub |
| `GetItemSpell(id)` | WotLK | Use spell name + id | ⚠️ Stub |
| `GetItemStats(link, table)` | WotLK | All item stats | ⚠️ Stub |
| `GetItemGem(link, i)` | WotLK | Socket gem info (TBC has gems!) | ⚠️ Stub |
| `GetItemFamily(id)` | WotLK | Bag family | ⚠️ Stub |
| `GetItemClassInfo(class)` | WotLK | Class/subclass name | ⚠️ Stub |
| `GetItemSubClassInfo(class, sub)` | WotLK | Subclass name | ⚠️ Stub |
| `GetItemInventoryType(id)` | WotLK | Equip slot type | ⚠️ Stub |
| `GetItemUniqueness(id)` | WotLK | Unique/unique-equipped | ⚠️ Stub |
| `IsEquippableItem(id)` | WotLK | Can player equip it | ⚠️ Stub |
| `IsUsableItem(id)` | WotLK | Is usable | ⚠️ Stub |
| `IsConsumableItem(id)` | WotLK | Is consumable | ⚠️ Stub |
| `IsEquippedItem(id)` | WotLK | Is currently equipped | ⚠️ Stub |
| `IsCurrentItem(id)` | WotLK | Is active (Hunter aspect) | ⚠️ Stub |
| `EquipItemByName(name)` | WotLK | Equip item by name | ⚠️ Stub |
| `GetInventoryItemID(unit, slot)` | WotLK | Equipped item ID | ⚠️ Stub |
| `GetInventoryItemLink(unit, slot)` | WotLK | Equipped item link | ⚠️ Stub |
| `GetInventoryItemTexture(unit, slot)` | WotLK | Equipped item icon | ⚠️ Stub |
| `GetInventoryItemQuality(unit, slot)` | WotLK | Equipped item quality | ⚠️ Stub |
| `GetInventoryItemDurability(slot)` | WotLK | Current/max durability | ⚠️ Stub |
| `GetInventoryItemBroken(unit, slot)` | WotLK | Is item broken | ⚠️ Stub |
| `GetInventoryItemCount(unit, slot)` | WotLK | Stack size | ⚠️ Stub |
| `GetInventoryItemCooldown(unit, slot)` | WotLK | Equipped item CD | ⚠️ Stub |

---

## Unit API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `UnitGUID(unit)` | WotLK | Unit's GUID | ⚠️ Stub |
| `UnitExists(unit)` | WotLK | Does unit exist | ⚠️ Stub |
| `UnitIsUnit(u1, u2)` | WotLK | Are they the same unit | ⚠️ Stub |
| `UnitName(unit)` | WotLK | Unit name | ⚠️ Stub |
| `UnitFullName(unit)` | WotLK | Name-Server format | ⚠️ Stub |
| `UnitClass(unit)` | WotLK | Class name, token, ID | ⚠️ Stub |
| `UnitRace(unit)` | WotLK | Race name, token, ID | ⚠️ Stub |
| `UnitSex(unit)` | WotLK | Gender (0=unknown, 2=female, 3=male) | ⚠️ Stub |
| `UnitLevel(unit)` | WotLK | Level | ⚠️ Stub |
| `UnitEffectiveLevel(unit)` | WotLK | Effective level | ⚠️ Stub |
| `UnitClassification(unit)` | WotLK | elite/rare/boss etc. | ⚠️ Stub |
| `UnitCreatureFamily(unit)` | WotLK | Pet family name | ⚠️ Stub |
| `UnitCreatureType(unit)` | WotLK | Creature type name | ⚠️ Stub |
| `UnitHealth(unit)` | WotLK | Current HP | ⚠️ Stub |
| `UnitHealthMax(unit)` | WotLK | Max HP | ⚠️ Stub |
| `UnitPower(unit[, type])` | WotLK | Current power | ⚠️ Stub |
| `UnitPowerMax(unit[, type])` | WotLK | Max power | ⚠️ Stub |
| `UnitPowerType(unit)` | WotLK | Power type ID, name, cost | ⚠️ Stub |
| `UnitMana(unit)` | WotLK | Current mana | ⚠️ Stub |
| `UnitManaMax(unit)` | WotLK | Max mana | ⚠️ Stub |
| `UnitIsDead(unit)` | WotLK | Is dead | ⚠️ Stub |
| `UnitIsGhost(unit)` | WotLK | Is ghost | ⚠️ Stub |
| `UnitIsDeadOrGhost(unit)` | WotLK | Is dead or ghost | ⚠️ Stub |
| `UnitIsConnected(unit)` | WotLK | Is player connected | ⚠️ Stub |
| `UnitIsPlayer(unit)` | WotLK | Is a player | ⚠️ Stub |
| `UnitPlayerControlled(unit)` | WotLK | Is player-controlled | ⚠️ Stub |
| `UnitIsEnemy(u1, u2)` | WotLK | Is enemy of u2 | ⚠️ Stub |
| `UnitIsFriend(u1, u2)` | WotLK | Is friend of u2 | ⚠️ Stub |
| `UnitCanAttack(u1, u2)` | WotLK | Can u1 attack u2 | ⚠️ Stub |
| `UnitIsCharmed(unit)` | WotLK | Is charmed | ⚠️ Stub |
| `UnitIsPVP(unit)` | WotLK | PvP flagged | ⚠️ Stub |
| `UnitInVehicle(unit)` | WotLK 3.1+ | In vehicle (N/A in TBC) | ⚠️ Stub |
| `UnitAura(unit, index)` | WotLK | Aura info | ⚠️ Stub |
| `UnitBuff(unit, index)` | WotLK | Buff info | ⚠️ Stub |
| `UnitDebuff(unit, index)` | WotLK | Debuff info | ⚠️ Stub |
| `UnitCastingInfo(unit)` | WotLK | Current cast info | ⚠️ Stub |
| `UnitChannelInfo(unit)` | WotLK | Current channel info | ⚠️ Stub |
| `UnitStat(unit, stat)` | WotLK | Stat value + effective | ⚠️ Stub |
| `UnitAttackSpeed(unit)` | WotLK | Main/off hand speed | ⚠️ Stub |
| `UnitAttackPower(unit)` | WotLK | AP, neg bonuses, positive | ⚠️ Stub |
| `UnitArmor(unit)` | WotLK | Armor value | ⚠️ Stub |
| `UnitResistance(unit, school)` | WotLK | Resistance | ⚠️ Stub |

---

## System/Utility API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `GetTime()` | WotLK | Unix timestamp | ✅ Works |
| `GetBuildInfo()` | WotLK | WoW version info | ✅ Works |
| `GetLocale()` | WotLK | Client locale | ⚠️ Stub |
| `GetFramerate()` | WotLK | Current FPS | ⚠️ Stub |
| `GetNetStats()` | WotLK | Network stats | ⚠️ Stub |
| `GetCVar(name)` | WotLK | CVar value | ⚠️ Stub |
| `GetCVarBool(name)` | WotLK | CVar as boolean | ⚠️ Stub |
| `SetCVar(name, value)` | WotLK | Set CVar | ⚠️ Stub |
| `GetScreenWidth()` | WotLK | Screen width | ⚠️ Stub |
| `GetScreenHeight()` | WotLK | Screen height | ⚠️ Stub |
| `InCombatLockdown()` | WotLK | In combat? | ⚠️ Stub |
| `UnitAffectingCombat(unit)` | WotLK | Unit in combat? | ⚠️ Stub |
| `FlashGameWindow()` | Custom | Flash the WoW window | 🔧 TODO |

---

## Chat/Social API

| Function | Origin | Description | TBC Status |
|----------|--------|-------------|-----------|
| `C_ChatInfo_SendAddonMessage(...)` | Cata | Send addon message | ⚠️ Stub |
| `C_ChatInfo_RegisterAddonMessagePrefix(p)` | Cata | Register prefix | ⚠️ Stub |
| `BNConnected()` | WotLK | Battle.net connected (N/A) | ⚠️ Stub |
| `BNGetNumFriends()` | WotLK | BN friends (N/A) | ⚠️ Stub |

---

## Compatibility Stubs

| Function | Origin | Stub Returns |
|----------|--------|-------------|
| `GetNumEquipmentSets()` | WotLK 3.1+ | 0 |
| `GetEquipmentSetInfo(n)` | WotLK 3.1+ | nil |
| `UseEquipmentSet(n)` | WotLK 3.1+ | (nothing) |
| `GetNumGlyphSockets()` | WotLK 3.0+ | 0 |
| `GetGlyphSocketInfo(n)` | WotLK 3.0+ | nil |
| `UnitInVehicle(unit)` | WotLK 3.1+ | false |
| `CanExitVehicle()` | WotLK 3.1+ | false |
| `VehicleExit()` | WotLK 3.1+ | (nothing) |
| `GetAchievementInfo(id)` | WotLK 3.0+ | nil |
| `GetNumCompletedAchievements()` | WotLK 3.0+ | 0 |
| `IsVoidStorageReady()` | Cata+ | false |
| `C_Transmog_GetItemInfo(id)` | Cata+ | nil |
