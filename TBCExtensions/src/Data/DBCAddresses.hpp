#pragma once

#include <Client/DBClient.hpp>

// TODO_TBC: All DBC addresses below are WotLK 3.3.5 placeholders set to 0x000000.
// You MUST replace every TODO_TBC address with the correct TBC 2.4.3 (build 8606) value.
// Use IDA Pro / Ghidra to find these offsets. See TBC_PORTING_GUIDE.md for methodology.

// NOTE: TBC 2.4.3 does NOT have: ZoneLight, Achievement, DungeonEncounter, BarberShop,
//       DanceMove, Currency, ArenaTeam (some), Rune, or Glyph DBCs.
//       These entries are omitted intentionally.

static DBClient* const g_areaTableDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_areaTriggerDB        = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_auctionHouseDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_bankBagSlotPricesDB  = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_battlemasterListDB   = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_charBaseInfoDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_charStartOutfitDB    = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_chrClassesDB         = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_chrRacesDB           = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_cinematicSequencesDB = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_creatureDisplayInfoDB= reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_creatureFamilyDB     = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_creatureTypeDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_durabilityCostsDB    = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_emotesDB             = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_factionDB            = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_factionTemplateDB    = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_gemPropertiesDB      = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_gtCombatRatingsDB    = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_gtChanceToMeleeCritDB= reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_itemDB               = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_itemClassDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_itemSetDB            = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_lightDB              = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_lockDB               = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_mapDB                = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_mapDifficultyDB      = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_npcSoundsDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_powersDB             = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_questSortDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_randPropPointsDB     = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_skillLineDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_skillLineAbilityDB   = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_soundEntriesDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellDB              = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellCastTimesDB     = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellDurationDB      = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellIconDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellItemEnchantmentDB = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellRadiusDB        = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_spellRangeDB         = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_stableSlotPricesDB   = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_talentDB             = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_talentTabDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_taxiNodesDB          = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_taxiPathDB           = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_totemCategoryDB      = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_vehicleDB            = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_weaponImpactSoundsDB = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_worldMapAreaDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_wmoAreaTableDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
static DBClient* const g_worldStateUIDB       = reinterpret_cast<DBClient*>(0x000000 /* TODO_TBC */);
