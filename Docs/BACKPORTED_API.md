# Backported API Reference

Complete reference for all Lua functions provided by TBC-Extensions.

> **TBC Status legend:**
> - ✅ **Works** — Fully implemented, needs TODO_TBC addresses filled in
> - ⚠️ **Stub** — Returns safe default, not fully implemented
> - 🔧 **TODO** — Needs address finding before functional

---

## Spell API

### `GetSpellInfo(spellID)`
**Returns:** `name, rank, icon, cost, isFunnel, powerType, castTime, minRange, maxRange`  
**TBC Status:** 🔧 TODO (reads Spell.dbc via GetLocalizedRow)  
**Notes:** Reads directly from DBC. Addresses must be filled in.

### `GetSpellTexture(spellID)`
**Returns:** `texturePath`  
**TBC Status:** ⚠️ Stub (returns nil until SpellIcon.dbc lookup is implemented)

### `GetSpellCooldown(spellID)`
**Returns:** `start, duration, enabled`  
**TBC Status:** ⚠️ Stub (returns 0, 0, 1)

### `GetSpellCharges(spellID)`
**Returns:** `currentCharges, maxCharges, cooldownStart, cooldownDuration`  
**TBC Status:** ⚠️ Stub (TBC has no charge system — always returns nil)

### `GetSpellCount(spellID)`
**Returns:** `count`  
**TBC Status:** ⚠️ Stub (returns 0)

### `IsSpellKnown(spellID)`
**Returns:** `isKnown`  
**TBC Status:** ⚠️ Stub (returns false until spellbook lookup is implemented)

### `GetSpellLink(spellID)`
**Returns:** `spellLink`  
**TBC Status:** ✅ Works (generates `|Hspell:id|h[Spell]|h` format link)

### `GetSpellPowerCost(spellID)`
**Returns:** `cost, powerType`  
**TBC Status:** ⚠️ Stub (returns 0, 0)

### `GetSpellCastTime(spellID)`
**Returns:** `castTime`  
**TBC Status:** ⚠️ Stub (returns 0)

### `GetSpellRange(spellID)`
**Returns:** `minRange, maxRange`  
**TBC Status:** ⚠️ Stub (returns 0, 0)

### `IsPassiveSpell(spellID)`
**Returns:** `isPassive`  
**TBC Status:** 🔧 TODO (reads `SPELL_ATTR0_PASSIVE` from Spell.dbc)

### `IsHarmfulSpell(spellID|bookSlot, bookType)`
**Returns:** `isHarmful`  
**TBC Status:** ⚠️ Stub

### `IsHelpfulSpell(spellID|bookSlot, bookType)`
**Returns:** `isHelpful`  
**TBC Status:** ⚠️ Stub

---

## Talent API

### `GetNumTalentTabs()`
**Returns:** `numTabs`  
**TBC Status:** ✅ Works (always returns 3)

### `GetUnspentTalentPoints()`
**Returns:** `points`  
**TBC Status:** 🔧 TODO (reads `PLAYER_CHARACTER_POINTS1` field)

### `GetActiveSpecGroup()`
**Returns:** `specGroup`  
**TBC Status:** ⚠️ Stub (always returns 1 — no dual spec in TBC)

### `GetNumSpecGroups()`
**Returns:** `numGroups`  
**TBC Status:** ⚠️ Stub (always returns 1)

### `GetSpecialization()`
**Returns:** `specIndex`  
**TBC Status:** ⚠️ Stub (returns 0 until talent tree scanning implemented)

---

## Item API

### `GetItemInfo(itemID|itemName|itemLink)`
**Returns:** `itemName, itemLink, itemRarity, itemLevel, itemMinLevel, itemType, itemSubType, itemStackCount, itemEquipLoc, itemTexture, itemSellPrice`  
**TBC Status:** ⚠️ Stub (returns nil — needs ItemCache lookup implementation)

### `GetItemQualityColor(quality)`
**Returns:** `r, g, b`  
**TBC Status:** ✅ Works (hardcoded quality color table)

### `GetItemCount(itemID[, includeBank[, includeCharges]])`
**Returns:** `count`  
**TBC Status:** ⚠️ Stub (returns 0)

### `GetItemCooldown(itemID)`
**Returns:** `start, duration, enabled`  
**TBC Status:** ⚠️ Stub (returns 0, 0, 1)

### `GetItemGem(slotLink, gemIndex)`
**Returns:** `gemLink`  
**TBC Status:** ⚠️ Stub (TBC does support gems — implementation pending)

---

## Unit API

### `UnitGUID(unit)`
**Returns:** `guid`  
**TBC Status:** ⚠️ Stub (returns nil — needs unit token lookup)

### `UnitName(unit)`
**Returns:** `name`  
**TBC Status:** ⚠️ Stub

### `UnitFullName(unit)`
**Returns:** `name, server`  
**TBC Status:** ⚠️ Stub

### `UnitClass(unit)`
**Returns:** `className, classToken, classID`  
**TBC Status:** ⚠️ Stub

### `UnitHealth(unit)`
**Returns:** `health`  
**TBC Status:** ⚠️ Stub (returns 0 — needs unit field reader)

### `UnitPower(unit[, powerType])`
**Returns:** `power`  
**TBC Status:** ⚠️ Stub

### `UnitAura(unit, index[, filter])`
**Returns:** `name, icon, count, debuffType, duration, expirationTime, source, isStealable, shouldConsolidate, spellId, canApplyAura, isBossDebuff, castByPlayer`  
**TBC Status:** ⚠️ Stub (returns nil)

### `UnitInVehicle(unit)`
**Returns:** `inVehicle`  
**TBC Status:** ⚠️ Stub (always false — TBC has no vehicles)

### `UnitIsDeadOrGhost(unit)`
**Returns:** `isDeadOrGhost`  
**TBC Status:** ⚠️ Stub

---

## Quest API

### `C_QuestLog_IsQuestFlaggedCompleted(questID)`
**Returns:** `isComplete`  
**TBC Status:** ⚠️ Stub (returns false)

### `C_QuestLog_GetNumQuestLogEntries()`
**Returns:** `numEntries`  
**TBC Status:** ⚠️ Stub (delegates to GetNumQuestLogEntries)

---

## Map/Position API

### `GetPlayerFacing()`
**Returns:** `facing`  
**TBC Status:** 🔧 TODO (reads movement info from player object)

### `GetBuildInfo()`
**Returns:** `version, build, date, time`  
**TBC Status:** ✅ Works (returns "2.4.3", "8606", hardcoded)

### `C_Map_GetBestMapForUnit(unit)`
**Returns:** `mapID`  
**TBC Status:** ✅ Works (returns current map ID)

---

## Camera API

### `GetCameraZoom()`
**Returns:** `zoom`  
**TBC Status:** ⚠️ Stub (returns 0)

### `SetCameraZoom(zoom)`
**TBC Status:** ⚠️ Stub

---

## System/Utility API

### `GetTime()`
**Returns:** `time`  
**TBC Status:** ✅ Works (returns `time(nullptr)`)

### `GetBuildInfo()`
**Returns:** `version, build, date, time`  
**TBC Status:** ✅ Works

### `GetLocale()`
**Returns:** `locale`  
**TBC Status:** ⚠️ Stub (returns "enUS")

### `GetScreenWidth()`
**Returns:** `width`  
**TBC Status:** ⚠️ Stub (returns 1024)

### `GetScreenHeight()`
**Returns:** `height`  
**TBC Status:** ⚠️ Stub (returns 768)

### `InCombatLockdown()`
**Returns:** `inCombat`  
**TBC Status:** ⚠️ Stub (returns false)

---

## Chat/Social API

### `C_ChatInfo_SendAddonMessage(prefix, message, chatType[, target])`
**TBC Status:** ⚠️ Stub (does nothing — needs native SendAddonMessage)

### `C_ChatInfo_RegisterAddonMessagePrefix(prefix)`
**Returns:** `success`  
**TBC Status:** ⚠️ Stub (returns true)

### `BNConnected()`
**Returns:** `connected`  
**TBC Status:** ⚠️ Stub (always false — TBC has no Battle.net)

---

## Compatibility Stubs (WotLK+ features)

### Equipment Sets (WotLK 3.1+)
- `GetNumEquipmentSets()` → 0
- `GetEquipmentSetInfo()` → nil
- `UseEquipmentSet()` → does nothing

### Glyphs (WotLK 3.0+)
- `GetNumGlyphSockets()` → 0
- `GetGlyphSocketInfo()` → nil

### Dual Specialization (WotLK 3.1+)
- `GetActiveSpecGroup()` → 1
- `GetNumSpecGroups()` → 1
- `SetActiveSpecGroup()` → does nothing

### Vehicles (WotLK 3.1+)
- `UnitInVehicle()` → false
- `CanExitVehicle()` → false
- `VehicleExit()` → does nothing

### Achievements (WotLK 3.0+)
- `GetAchievementInfo()` → nil
- `GetNumCompletedAchievements()` → 0

### Void Storage (Cataclysm+)
- `IsVoidStorageReady()` → false

### Transmogrification (Cataclysm+)
- `C_Transmog_GetItemInfo()` → nil
