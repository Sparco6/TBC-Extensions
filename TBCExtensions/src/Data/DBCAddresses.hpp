#pragma once

#include <Client/DBClient.hpp>

// TODO_TBC: All DBC addresses below still need the correct TBC 2.4.3 global instance addresses.
// The DBC instances are global WowClientDB structs initialized by StaticDBLoadAll (0x00573C90).
// To find each address: disassemble StaticDBLoadAll in IDA/Ghidra and note the `this` pointer
// passed to each WowClientDB::Load__<Name> call — that address is the global instance.
// func.sym Load function addresses for reference:
//   WowClientDB__Load__AreaTable         = 0x005606D0
//   WowClientDB__Load__AreaTrigger       = 0x00560890
//   WowClientDB__Load__AuctionHouse      = 0x00560DD0
//   WowClientDB__Load__BankBagSlotPrices = 0x00560F90
//   WowClientDB__Load__BattlemasterList  = 0x00561150
//   WowClientDB__Load__CharBaseInfo      = 0x00561850
//   WowClientDB__Load__CharStartOutfit   = 0x00561BD0
//   WowClientDB__Load__ChrClasses        = 0x00562490
//   WowClientDB__Load__ChrRaces          = 0x00562650
//   WowClientDB__Load__CinematicSequences= 0x005629D0
//   WowClientDB__Load__CreatureDisplayInfo=0x00562D50
//   WowClientDB__Load__CreatureFamily    = 0x00562F10
//   WowClientDB__Load__CreatureType      = 0x00563610
//   WowClientDB__Load__DurabilityCosts   = 0x00563D10
//   WowClientDB__Load__Emotes            = 0x00564090
//   WowClientDB__Load__Faction           = 0x00564CD0
//   WowClientDB__Load__FactionTemplate   = 0x00564E90
//   WowClientDB__Load__GemProperties     = 0x00565AD0
//   WowClientDB__Load__gtCombatRatings   = 0x00566710
//   WowClientDB__Load__gtChanceToMeleeCrit=0x005668D0
//   WowClientDB__Load__Item              = 0x00567A50
//   WowClientDB__Load__ItemClass         = 0x00567DD0
//   WowClientDB__Load__ItemSet           = 0x00568BD0
//   WowClientDB__Load__Light             = 0x00569F10
//   WowClientDB__Load__Lock              = 0x0056A7D0
//   WowClientDB__Load__Map               = 0x0056AD10
//   WowClientDB__Load__NPCSounds         = 0x0056B250
//   WowClientDB__Load__QuestSort         = 0x0056C210
//   WowClientDB__Load__RandPropPoints    = 0x0056C3D0
//   WowClientDB__Load__SkillLine         = 0x0056CE50
//   WowClientDB__Load__SkillLineAbility  = 0x0056CC90
//   WowClientDB__Load__SoundEntries      = 0x0056D710
//   WowClientDB__Load__Spell             = 0x0056F850
//   WowClientDB__Load__SpellCastTimes    = 0x0056DFD0
//   WowClientDB__Load__SpellDuration     = 0x0056E6D0
//   WowClientDB__Load__SpellIcon         = 0x0056EC10
//   WowClientDB__Load__SpellItemEnchantment=0x0056EDD0
//   WowClientDB__Load__SpellRadius       = 0x0056F4D0
//   WowClientDB__Load__SpellRange        = 0x0056F690
//   WowClientDB__Load__StableSlotPrices  = 0x00570110
//   WowClientDB__Load__Talent            = 0x00570810
//   WowClientDB__Load__TalentTab         = 0x005709D0
//   WowClientDB__Load__TaxiNodes         = 0x00570B90
//   WowClientDB__Load__TaxiPath          = 0x00570F10
//   WowClientDB__Load__TotemCategory     = 0x00571450
//   WowClientDB__Load__WeaponImpactSounds= 0x00572090
//   WowClientDB__Load__WorldMapArea      = 0x005725D0
//   WowClientDB__Load__WMOAreaTable      = 0x00573AD0
//   WowClientDB__Load__WorldStateUI      = 0x00572E90
// NOTE: LfgDungeons (WowClientDB__Load__LfgDungeons = 0x00569810) exists in TBC; MapDifficulty
//       does NOT appear in this build's Load list. Vehicle is NOT present in TBC 2.4.3.

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
