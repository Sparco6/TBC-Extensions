#pragma once

#include <GameObjects/CGUnit.hpp>
#include <SharedDefines.hpp>

// TODO when needed - expand these structs with TBC field layout
struct PlayerBytes0  { uint32_t p; };
struct PlayerBytes1  { uint32_t p; };
struct PlayerBytes2  { uint32_t p; };
struct PlayerFieldBytes  { uint32_t p; };
struct PlayerFieldBytes2 { uint32_t p; };

struct PlayerFields
{
    WoWGUID      m_duelArbiter;
    uint32_t     m_flags;
    uint32_t     m_guildID;
    uint32_t     m_guildRank;
    PlayerBytes0 m_bytes0;
    PlayerBytes1 m_bytes1;
    PlayerBytes2 m_bytes2;
    uint32_t     m_duelTeam;
    uint32_t     m_guildTimestamp;
    uint32_t     m_questLog[100];     // TBC: 25 quests * 4 fields
    uint32_t     m_visibleItems[36];
    uint32_t     m_choosenTitle;
    uint32_t     m_fakeInebriation;
    uint32_t     m_padding0xAF;
    uint32_t     m_invSlot[46];
    uint32_t     m_packSlot[32];
    uint32_t     m_bankSlot[56];
    uint32_t     m_bankBagSlot[14];
    uint32_t     m_vendorBuyBackSlot[24];
    uint32_t     m_keyringSlot[64];
    // TBC: no currency token slots
    uint32_t     m_farsight[2];
    uint32_t     m_knownTitles0[2];
    uint32_t     m_XP;
    uint32_t     m_nextLevelXP;
    uint32_t     m_skillInfo[384];
    uint32_t     m_characterPoints1;
    uint32_t     m_characterPoints2;
    uint32_t     m_trackCreatures;
    uint32_t     m_trackResources;
    float        m_blockPct;
    float        m_dodgePct;
    float        m_parryPct;
    uint32_t     m_expertise;
    uint32_t     m_offhandExpertise;
    float        m_critPct;
    float        m_rangedCritPct;
    float        m_offhandCritPct;
    float        m_spellCritPct[7];
    float        m_shieldBlock;
    float        m_shieldBlockCritPct;
    uint32_t     m_exploredzones[128];
    uint32_t     m_restedXP;
    uint32_t     m_money;
    int32_t      m_damageBonusPos[7];
    int32_t      m_damageBonusNeg[7];
    float        m_damageBonusPct[7];
    uint32_t     m_healingBonusPos;
    float        m_healingTakenBonusPct;
    float        m_healingDoneBonusPct;
    uint32_t     m_targetSpellResistanceMod;
    uint32_t     m_targetPhysicalResistanceMod;
    PlayerFieldBytes m_fieldBytes;
    uint32_t     m_ammoID;
    uint32_t     m_selfResurrectionSpell;
    uint32_t     m_pvpMedals;
    uint32_t     m_buybackPrices[12];
    uint32_t     m_buybackTimestamp[12];
    uint32_t     m_pvpKills;
    uint32_t     m_pvpTodayContrib;
    uint32_t     m_pvpYesterdayContrib;
    uint32_t     m_lifetimeHonorKills;
    PlayerFieldBytes2 m_fieldBytes2;
    int32_t      m_watchedFactionIndex;
    int32_t      m_crWeaponSkill;
    int32_t      m_crDefenseSkill;
    int32_t      m_crDodge;
    int32_t      m_crParry;
    int32_t      m_crBlock;
    int32_t      m_crHitMelee;
    int32_t      m_crHitRanged;
    int32_t      m_crHitSpell;
    int32_t      m_crCritMelee;
    int32_t      m_crCritRanged;
    int32_t      m_crCritSpell;
    int32_t      m_crHitTakenMelee;
    int32_t      m_crHitTakenRanged;
    int32_t      m_crHitTakenSpell;
    int32_t      m_crCritTakenMelee;
    int32_t      m_crCritTakenRanged;
    int32_t      m_crCritTakenSpell;
    int32_t      m_crHasteMelee;
    int32_t      m_crHasteRanged;
    int32_t      m_crHasteSpell;
    int32_t      m_crWeaponSkillMainhand;
    int32_t      m_crWeaponSkillOffhand;
    int32_t      m_crWeaponSkillRanged;
    int32_t      m_crExpertise;
    int32_t      m_crArmorPenetration;
    // TBC: no arena team currency, no rune/glyph fields
    uint32_t     m_honorCurrency;
    uint32_t     m_arenaCurrency;
    uint32_t     m_maxLevel;
    uint32_t     m_dailyQuests[25];
    int32_t      m_petSpellPower;
};

struct CGPlayer : CGUnit
{
    PlayerFields* m_playerData;
    uint32_t      m_padding0x1008[1024];

    static bool IsDeadOrGhost(CGPlayer* thisPlayer);

    static void ApplyPatches();
    static void CharacterCreationRaceCrashfix();
};

struct CGPlayerCustomFields
{
    int32_t m_combatRating[7] = { 0 };
};
