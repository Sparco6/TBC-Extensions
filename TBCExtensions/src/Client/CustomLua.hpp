#pragma once

#include <SharedDefines.hpp>

struct lua_State;
class Main;

// ============================================================
// TBCExtensions: CustomLua API Backports
// ============================================================
// This file contains backported Lua API functions from WotLK,
// Cataclysm, and MoP, adapted for TBC 2.4.3 (build 8606).
//
// Functions marked STUB return safe default values for addons
// that call them without TBC having the underlying feature.
//
// Functions marked TODO_TBC need address research before use.
// ============================================================

class CustomLua
{
public:
    using SafeRegistrar = int32_t(__cdecl*)(const char*, void*);
    static int32_t LoadScriptFunctionsCustom();
    static void ApplySafeResearchApi();
    static void ApplySafeResearchApiWithRegistrar(SafeRegistrar registrar);
    static bool IsSafeResearchCallback(std::uintptr_t address);
    static bool CaptureClientFingerprint();

private:
    static void AddToFunctionMap(const char* name, void* ptr);
    static void Apply();
    static void RegisterFunctions();
    static int32_t TBCExtGetVersion(lua_State* L);
    static int32_t TBCExtGetClientBuild(lua_State* L);
    static int32_t TBCExtGetModuleBase(lua_State* L);
    static int32_t TBCExtGetLuaState(lua_State* L);
    static int32_t TBCExtGetNativeApiVersion(lua_State* L);
    static int32_t TBCExtGetClientProfile(lua_State* L);
    static int32_t TBCExtGetCoreLuaStatus(lua_State* L);
    static int32_t TBCExtGetCallbackValidatorStatus(lua_State* L);
    static int32_t TBCExtGetNativeTraceStatus(lua_State* L);
    static int32_t TBCExtGetModelHooksStatus(lua_State* L);
    static int32_t TBCExtGetAddressInfo(lua_State* L);
    static int32_t TBCExtIsAddressExecutable(lua_State* L);
    static int32_t TBCExtIsAddressReadable(lua_State* L);
    static int32_t TBCExtCustomDBC(lua_State* L);
    static int32_t TBCExtGetBLPSupport(lua_State* L);
    static int32_t TBCExtInspectBLP(lua_State* L);
    static int32_t TBCExtGetTextureLoaderStatus(lua_State* L);
    static int32_t TBCExtArmBLPObservation(lua_State* L);
    static int32_t TBCExtGetBLPObservationStatus(lua_State* L);
    static int32_t TBCExtArmNativeBLPDXT5(lua_State* L);
    static int32_t TBCExtArmNativeBLPBGRA8(lua_State* L);
    static int32_t TBCExtArmNativeBLPBGRA8Prototype(lua_State* L);
    static int32_t TBCExtGetNativeBLPTraceStatus(lua_State* L);

    friend class Main;

    // ----------------------------------------------------------
    // Original functions (ported from WotLK-Extensions)
    // ----------------------------------------------------------
    static int32_t GetShapeshiftFormID(lua_State* L);
    static int32_t GetSpellDescription(lua_State* L);
    static int32_t GetSpellNameById(lua_State* L);

    static int32_t GetCustomCombatRating(lua_State* L);
    static int32_t GetCustomCombatRatingBonus(lua_State* L);

    static int32_t ConvertCoordsToScreenSpace(lua_State* L);
    static int32_t PortGraveyard(lua_State* L);
    static int32_t FlashGameWindow(lua_State* L);

    // ----------------------------------------------------------
    // Action Bar API
    // ----------------------------------------------------------
    static int32_t FindSpellActionBarSlots(lua_State* L);
    static int32_t ReplaceActionBarSpell(lua_State* L);
    static int32_t SetSpellInActionBarSlot(lua_State* L);

    static int32_t GetActionInfo(lua_State* L);
    static int32_t GetActionTexture(lua_State* L);
    static int32_t GetActionText(lua_State* L);
    static int32_t GetActionCount(lua_State* L);
    static int32_t GetActionCooldown(lua_State* L);
    static int32_t IsAttackAction(lua_State* L);
    static int32_t IsAutoRepeatAction(lua_State* L);
    static int32_t IsCurrentAction(lua_State* L);
    static int32_t IsUsableAction(lua_State* L);
    static int32_t IsConsumableAction(lua_State* L);
    static int32_t IsEquippedAction(lua_State* L);
    static int32_t HasAction(lua_State* L);

    // ----------------------------------------------------------
    // Dev Helper Lua functions
    // ----------------------------------------------------------
    static int32_t ReloadMap(lua_State* L);
    static int32_t ToggleDisplayNormals(lua_State* L);
    static int32_t ToggleGroundEffects(lua_State* L);
    static int32_t ToggleLiquids(lua_State* L);
    static int32_t ToggleM2(lua_State* L);
    static int32_t ToggleTerrain(lua_State* L);
    static int32_t ToggleTerrainCulling(lua_State* L);
    static int32_t ToggleWireframeMode(lua_State* L);
    static int32_t ToggleWMO(lua_State* L);

    // ----------------------------------------------------------
    // Spell API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    static int32_t GetSpellInfo(lua_State* L);
    static int32_t GetSpellTexture(lua_State* L);
    static int32_t GetSpellCooldown(lua_State* L);
    static int32_t GetSpellCharges(lua_State* L);        // STUB - TBC has no charge system
    static int32_t GetSpellCount(lua_State* L);
    static int32_t IsSpellKnown(lua_State* L);
    static int32_t GetSpellLink(lua_State* L);
    static int32_t GetSpellBookItemInfo(lua_State* L);
    static int32_t GetSpellTabInfo(lua_State* L);
    static int32_t GetNumSpellTabs(lua_State* L);
    static int32_t FindSpellBookSlotBySpellID(lua_State* L);
    static int32_t GetSpellPowerCost(lua_State* L);
    static int32_t GetSpellCastTime(lua_State* L);
    static int32_t GetSpellRange(lua_State* L);
    static int32_t IsHarmfulSpell(lua_State* L);
    static int32_t IsHelpfulSpell(lua_State* L);
    static int32_t IsPassiveSpell(lua_State* L);
    static int32_t GetSpellAutocast(lua_State* L);
    static int32_t ToggleSpellAutocast(lua_State* L);

    // ----------------------------------------------------------
    // Talent API Backports
    // ----------------------------------------------------------
    static int32_t GetNumTalents(lua_State* L);
    static int32_t GetTalentInfo(lua_State* L);
    static int32_t GetNumTalentTabs(lua_State* L);
    static int32_t GetTalentTabInfo(lua_State* L);
    static int32_t LearnTalent(lua_State* L);
    static int32_t GetUnspentTalentPoints(lua_State* L);
    static int32_t GetActiveSpecGroup(lua_State* L);     // STUB - returns 1 (no dual spec)
    static int32_t GetNumSpecGroups(lua_State* L);       // STUB - returns 1
    static int32_t GetSpecialization(lua_State* L);
    static int32_t GetSpecializationInfo(lua_State* L);
    static int32_t GetPrimaryTalentTree(lua_State* L);

    // ----------------------------------------------------------
    // Item API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    static int32_t GetItemInfo(lua_State* L);
    static int32_t GetItemInfoInstant(lua_State* L);
    static int32_t GetItemIcon(lua_State* L);
    static int32_t GetItemQualityColor(lua_State* L);
    static int32_t GetItemCount(lua_State* L);
    static int32_t GetItemCooldown(lua_State* L);
    static int32_t GetItemSpell(lua_State* L);
    static int32_t GetItemStats(lua_State* L);
    static int32_t GetItemGem(lua_State* L);             // TBC has gems!
    static int32_t GetItemFamily(lua_State* L);
    static int32_t GetItemClassInfo(lua_State* L);
    static int32_t GetItemSubClassInfo(lua_State* L);
    static int32_t GetItemInventoryType(lua_State* L);
    static int32_t GetItemUniqueness(lua_State* L);
    static int32_t IsEquippableItem(lua_State* L);
    static int32_t IsUsableItem(lua_State* L);
    static int32_t IsConsumableItem(lua_State* L);
    static int32_t IsEquippedItem(lua_State* L);
    static int32_t IsCurrentItem(lua_State* L);
    static int32_t EquipItemByName(lua_State* L);
    static int32_t GetInventoryItemID(lua_State* L);
    static int32_t GetInventoryItemLink(lua_State* L);
    static int32_t GetInventoryItemTexture(lua_State* L);
    static int32_t GetInventoryItemQuality(lua_State* L);
    static int32_t GetInventoryItemDurability(lua_State* L);
    static int32_t GetInventoryItemBroken(lua_State* L);
    static int32_t GetInventoryItemCount(lua_State* L);
    static int32_t GetInventoryItemCooldown(lua_State* L);

    // ----------------------------------------------------------
    // Unit API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    // Identification
    static int32_t UnitGUID(lua_State* L);
    static int32_t UnitExists(lua_State* L);
    static int32_t UnitIsUnit(lua_State* L);
    static int32_t UnitName(lua_State* L);
    static int32_t UnitFullName(lua_State* L);
    static int32_t UnitClass(lua_State* L);
    static int32_t UnitRace(lua_State* L);
    static int32_t UnitSex(lua_State* L);
    static int32_t UnitLevel(lua_State* L);
    static int32_t UnitEffectiveLevel(lua_State* L);
    static int32_t UnitClassification(lua_State* L);
    static int32_t UnitCreatureFamily(lua_State* L);
    static int32_t UnitCreatureType(lua_State* L);

    // Status
    static int32_t UnitHealth(lua_State* L);
    static int32_t UnitHealthMax(lua_State* L);
    static int32_t UnitPower(lua_State* L);
    static int32_t UnitPowerMax(lua_State* L);
    static int32_t UnitPowerType(lua_State* L);
    static int32_t UnitMana(lua_State* L);
    static int32_t UnitManaMax(lua_State* L);
    static int32_t UnitIsDead(lua_State* L);
    static int32_t UnitIsGhost(lua_State* L);
    static int32_t UnitIsDeadOrGhost(lua_State* L);
    static int32_t UnitIsCorpse(lua_State* L);
    static int32_t UnitIsConnected(lua_State* L);
    static int32_t UnitIsPlayer(lua_State* L);
    static int32_t UnitPlayerControlled(lua_State* L);
    static int32_t UnitIsEnemy(lua_State* L);
    static int32_t UnitIsFriend(lua_State* L);
    static int32_t UnitCanAttack(lua_State* L);
    static int32_t UnitCanAssist(lua_State* L);
    static int32_t UnitCanCooperate(lua_State* L);
    static int32_t UnitIsCharmed(lua_State* L);
    static int32_t UnitIsPVP(lua_State* L);
    static int32_t UnitIsPVPFreeForAll(lua_State* L);
    static int32_t UnitIsPVPSanctuary(lua_State* L);

    // Relationships
    static int32_t UnitIsPartyLeader(lua_State* L);
    static int32_t UnitInParty(lua_State* L);
    static int32_t UnitInRaid(lua_State* L);
    static int32_t UnitIsGroupLeader(lua_State* L);
    static int32_t UnitIsGroupAssistant(lua_State* L);
    static int32_t UnitInRange(lua_State* L);
    static int32_t UnitDistanceSquared(lua_State* L);
    static int32_t UnitIsVisible(lua_State* L);
    static int32_t UnitInBattleground(lua_State* L);
    static int32_t UnitInVehicle(lua_State* L);          // STUB - always false

    // Buffs/Debuffs
    static int32_t UnitAura(lua_State* L);
    static int32_t UnitBuff(lua_State* L);
    static int32_t UnitDebuff(lua_State* L);
    static int32_t UnitAuraBySlot(lua_State* L);
    static int32_t GetPlayerBuff(lua_State* L);
    static int32_t GetPlayerBuffName(lua_State* L);
    static int32_t GetPlayerBuffTexture(lua_State* L);
    static int32_t GetPlayerBuffTimeLeft(lua_State* L);

    // Casting
    static int32_t UnitCastingInfo(lua_State* L);
    static int32_t UnitChannelInfo(lua_State* L);
    static int32_t UnitCanBeAttacked(lua_State* L);

    // Stats
    static int32_t UnitStat(lua_State* L);
    static int32_t UnitAttackSpeed(lua_State* L);
    static int32_t UnitAttackPower(lua_State* L);
    static int32_t UnitRangedAttackPower(lua_State* L);
    static int32_t UnitDamage(lua_State* L);
    static int32_t UnitRangedDamage(lua_State* L);
    static int32_t UnitDefense(lua_State* L);
    static int32_t UnitArmor(lua_State* L);
    static int32_t UnitResistance(lua_State* L);

    // ----------------------------------------------------------
    // Quest API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    static int32_t GetQuestLogTitle(lua_State* L);
    static int32_t GetNumQuestLogEntries(lua_State* L);
    static int32_t GetNumQuestLeaderBoards(lua_State* L);
    static int32_t GetQuestLogLeaderBoard(lua_State* L);
    static int32_t GetQuestLogRewardInfo(lua_State* L);
    static int32_t GetNumQuestLogRewards(lua_State* L);
    static int32_t GetNumQuestLogChoices(lua_State* L);
    static int32_t GetQuestLogRewardMoney(lua_State* L);
    static int32_t GetQuestLogRewardSpell(lua_State* L);
    static int32_t GetQuestLogRewardXP(lua_State* L);
    static int32_t GetQuestLink(lua_State* L);
    static int32_t IsQuestFlaggedCompleted(lua_State* L);
    static int32_t GetQuestLogIndexByID(lua_State* L);
    static int32_t C_QuestLog_IsQuestFlaggedCompleted(lua_State* L);
    static int32_t C_QuestLog_GetInfo(lua_State* L);
    static int32_t C_QuestLog_GetNumQuestLogEntries(lua_State* L);

    // ----------------------------------------------------------
    // Map/Position API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    static int32_t GetPlayerMapPosition(lua_State* L);
    static int32_t GetPlayerFacing(lua_State* L);
    static int32_t GetCursorPosition(lua_State* L);
    static int32_t GetCorpseMapPosition(lua_State* L);
    static int32_t GetBindLocation(lua_State* L);
    static int32_t SetMapToCurrentZone(lua_State* L);
    static int32_t C_Map_GetBestMapForUnit(lua_State* L);
    static int32_t C_Map_GetPlayerMapPosition(lua_State* L);
    static int32_t C_Map_GetWorldPosFromMapPos(lua_State* L);
    static int32_t GetZoneText(lua_State* L);
    static int32_t GetSubZoneText(lua_State* L);
    static int32_t GetMinimapZoneText(lua_State* L);
    static int32_t GetRealZoneText(lua_State* L);

    // ----------------------------------------------------------
    // Camera API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t GetCameraZoom(lua_State* L);
    static int32_t SetCameraZoom(lua_State* L);
    static int32_t GetCameraPosition(lua_State* L);
    static int32_t GetCameraFacing(lua_State* L);
    static int32_t CameraOrSelectOrMoveStart(lua_State* L);
    static int32_t CameraOrSelectOrMoveStop(lua_State* L);
    static int32_t MoveViewLeftStart(lua_State* L);
    static int32_t MoveViewRightStart(lua_State* L);
    static int32_t MoveViewUpStart(lua_State* L);
    static int32_t MoveViewDownStart(lua_State* L);
    static int32_t MoveViewLeftStop(lua_State* L);
    static int32_t MoveViewRightStop(lua_State* L);
    static int32_t MoveViewUpStop(lua_State* L);
    static int32_t MoveViewDownStop(lua_State* L);

    // ----------------------------------------------------------
    // System/Utility API Backports (WotLK/Cata/MoP)
    // ----------------------------------------------------------
    static int32_t GetTime(lua_State* L);
    static int32_t GetFramerate(lua_State* L);
    static int32_t GetNetStats(lua_State* L);
    static int32_t GetCVar(lua_State* L);
    static int32_t GetCVarBool(lua_State* L);
    static int32_t SetCVar(lua_State* L);
    static int32_t GetCurrentKeyBoardFocus(lua_State* L);
    static int32_t GetMouseFocus(lua_State* L);
    static int32_t GetLocale(lua_State* L);
    static int32_t GetBuildInfo(lua_State* L);
    static int32_t GetAddOnInfo(lua_State* L);
    static int32_t GetAddOnMetadata(lua_State* L);
    static int32_t GetNumAddOns(lua_State* L);
    static int32_t IsAddOnLoaded(lua_State* L);
    static int32_t IsAddOnLoadOnDemand(lua_State* L);
    static int32_t LoadAddOn(lua_State* L);
    static int32_t EnableAddOn(lua_State* L);
    static int32_t DisableAddOn(lua_State* L);
    static int32_t GetScreenWidth(lua_State* L);
    static int32_t GetScreenHeight(lua_State* L);
    static int32_t GetCursorInfo(lua_State* L);
    static int32_t GetCurrentCursorTexture(lua_State* L);
    static int32_t InCombatLockdown(lua_State* L);
    static int32_t UnitAffectingCombat(lua_State* L);

    // ----------------------------------------------------------
    // Chat/Social API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t SendChatMessage(lua_State* L);
    static int32_t GetNumDisplayChannels(lua_State* L);
    static int32_t GetChannelList(lua_State* L);
    static int32_t GetChannelName(lua_State* L);
    static int32_t C_ChatInfo_SendAddonMessage(lua_State* L);
    static int32_t C_ChatInfo_RegisterAddonMessagePrefix(lua_State* L);
    static int32_t BNConnected(lua_State* L);            // STUB - false (no Battle.net)
    static int32_t BNGetNumFriends(lua_State* L);        // STUB - 0
    static int32_t GetNumFriends(lua_State* L);
    static int32_t GetFriendInfo(lua_State* L);
    static int32_t GetNumGuildMembers(lua_State* L);
    static int32_t GetGuildInfo(lua_State* L);
    static int32_t GetGuildRosterInfo(lua_State* L);

    // ----------------------------------------------------------
    // Inventory/Container API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t GetContainerNumSlots(lua_State* L);
    static int32_t GetContainerItemInfo(lua_State* L);
    static int32_t GetContainerItemID(lua_State* L);
    static int32_t GetContainerItemLink(lua_State* L);
    static int32_t GetContainerNumFreeSlots(lua_State* L);
    static int32_t C_Container_GetContainerNumSlots(lua_State* L);
    static int32_t C_Container_GetContainerItemInfo(lua_State* L);
    static int32_t PickupContainerItem(lua_State* L);
    static int32_t SplitContainerItem(lua_State* L);
    static int32_t UseContainerItem(lua_State* L);

    // ----------------------------------------------------------
    // Tooltip API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t GameTooltip_SetDefaultAnchor(lua_State* L);
    static int32_t GameTooltip_SetUnit(lua_State* L);
    static int32_t GameTooltip_SetSpellByID(lua_State* L);
    static int32_t GameTooltip_SetItemByID(lua_State* L);
    static int32_t GameTooltip_SetHyperlink(lua_State* L);
    static int32_t GameTooltip_AddLine(lua_State* L);
    static int32_t GameTooltip_AddDoubleLine(lua_State* L);
    static int32_t GameTooltip_GetItem(lua_State* L);
    static int32_t GameTooltip_GetSpell(lua_State* L);
    static int32_t GameTooltip_GetUnit(lua_State* L);

    // ----------------------------------------------------------
    // Merchant/Trade API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t GetMerchantNumItems(lua_State* L);
    static int32_t GetMerchantItemInfo(lua_State* L);
    static int32_t GetMerchantItemLink(lua_State* L);
    static int32_t GetMerchantItemID(lua_State* L);
    static int32_t GetMerchantItemCostInfo(lua_State* L);
    static int32_t GetMerchantItemMaxStack(lua_State* L);
    static int32_t BuyMerchantItem(lua_State* L);

    // ----------------------------------------------------------
    // Combat Log API Backports (WotLK/Cata)
    // ----------------------------------------------------------
    static int32_t CombatLogGetCurrentEventInfo(lua_State* L);
    static int32_t GetCombatLogInfo(lua_State* L);

    // ----------------------------------------------------------
    // Compatibility Stubs (features not present in TBC)
    // ----------------------------------------------------------

    // Equipment Sets (WotLK 3.1+)
    static int32_t GetNumEquipmentSets(lua_State* L);     // STUB - returns 0
    static int32_t GetEquipmentSetInfo(lua_State* L);     // STUB - returns nil
    static int32_t UseEquipmentSet(lua_State* L);         // STUB - does nothing

    // Glyphs (WotLK 3.0+)
    static int32_t GetNumGlyphSockets(lua_State* L);      // STUB - returns 0
    static int32_t GetGlyphSocketInfo(lua_State* L);      // STUB - returns nil

    // Dual Spec (WotLK 3.1+)
    static int32_t SetActiveSpecGroup(lua_State* L);      // STUB - does nothing

    // Vehicles (WotLK 3.1+)
    static int32_t CanExitVehicle(lua_State* L);          // STUB - returns false
    static int32_t VehicleExit(lua_State* L);             // STUB - does nothing

    // Achievements (WotLK 3.0+)
    static int32_t GetAchievementInfo(lua_State* L);      // STUB - returns nil
    static int32_t GetNumCompletedAchievements(lua_State* L); // STUB - returns 0

    // Void Storage (Cata+)
    static int32_t IsVoidStorageReady(lua_State* L);      // STUB - returns false

    // Transmogrification (Cata+)
    static int32_t C_Transmog_GetItemInfo(lua_State* L);  // STUB - returns nil
};
