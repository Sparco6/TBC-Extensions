// ============================================================
// TBCExtensions: CustomLua.cpp
// Backported Lua API functions for TBC 2.4.3 (build 8606)
// ============================================================
// Many functions below call TODO_TBC addresses that MUST be
// replaced with the correct TBC 2.4.3 memory addresses before
// use. See TBC_PORTING_GUIDE.md for instructions.
//
// Functions marked STUB return safe default values for addon
// compatibility when a feature does not exist in TBC.
// ============================================================

#include <Client/CDataStore.hpp>
#include <Client/CGChat.hpp>
#include <Client/ClientServices.hpp>
#include <Client/CNetClient.hpp>
#include <Client/CustomLua.hpp>
#include <Client/CVar.hpp>
#include <Client/DBClient.hpp>
#include <Client/FrameScript.hpp>
#include <Client/SpellParser.hpp>
#include <Client/SStr.hpp>
#include <Data/DBCAddresses.hpp>
#include <Data/Enums.hpp>
#include <Data/MiscAddresses.hpp>
#include <GameObjects/CGUnit.hpp>
#include <GameObjects/CGPlayer.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>
#include <WorldData/CWorld.hpp>

#include <PatchConfig.hpp>

// ============================================================
// Core: Apply and RegisterFunctions
// ============================================================

void CustomLua::Apply()
{
    // TODO_TBC: Replace addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5: patch at 0x52AB17, offset base 0x52AB1B
    // How to find: Locate FrameScript__LoadFunctions call chain init
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, (uint32_t)&LoadScriptFunctionsCustom - 0x000004 /* TODO_TBC */);

    RegisterFunctions();
}

int32_t CustomLua::LoadScriptFunctionsCustom()
{
    auto& luaFunctionMap = DataContainer::GetInstance().GetLuaFunctionMap();

    for (auto& it : luaFunctionMap)
    {
        const char* name = it.first;
        void*       ptr  = it.second;
        FrameScript::RegisterFunction(name, ptr);
    }

    return FrameScript::LoadFunctions();
}

void CustomLua::AddToFunctionMap(const char* name, void* ptr)
{
    DataContainer::GetInstance().AddLuaFunction(name, ptr);
}

void CustomLua::RegisterFunctions()
{
    // Original WotLK-ported functions
    AddToFunctionMap("GetShapeshiftFormID",           (void*)&GetShapeshiftFormID);
    AddToFunctionMap("GetSpellDescription",           (void*)&GetSpellDescription);
    AddToFunctionMap("GetSpellNameById",              (void*)&GetSpellNameById);
    AddToFunctionMap("GetCustomCombatRating",         (void*)&GetCustomCombatRating);
    AddToFunctionMap("GetCustomCombatRatingBonus",    (void*)&GetCustomCombatRatingBonus);
    AddToFunctionMap("ConvertCoordsToScreenSpace",    (void*)&ConvertCoordsToScreenSpace);
    AddToFunctionMap("PortGraveyard",                 (void*)&PortGraveyard);
    AddToFunctionMap("FlashGameWindow",               (void*)&FlashGameWindow);

#if ACTIONBAR_LUA
    AddToFunctionMap("FindSpellActionBarSlots",       (void*)&FindSpellActionBarSlots);
    AddToFunctionMap("ReplaceActionBarSpell",         (void*)&ReplaceActionBarSpell);
    AddToFunctionMap("SetSpellInActionBarSlot",       (void*)&SetSpellInActionBarSlot);
    AddToFunctionMap("GetActionInfo",                 (void*)&GetActionInfo);
    AddToFunctionMap("GetActionTexture",              (void*)&GetActionTexture);
    AddToFunctionMap("GetActionText",                 (void*)&GetActionText);
    AddToFunctionMap("GetActionCount",                (void*)&GetActionCount);
    AddToFunctionMap("GetActionCooldown",             (void*)&GetActionCooldown);
    AddToFunctionMap("IsAttackAction",                (void*)&IsAttackAction);
    AddToFunctionMap("IsAutoRepeatAction",            (void*)&IsAutoRepeatAction);
    AddToFunctionMap("IsCurrentAction",               (void*)&IsCurrentAction);
    AddToFunctionMap("IsUsableAction",                (void*)&IsUsableAction);
    AddToFunctionMap("IsConsumableAction",            (void*)&IsConsumableAction);
    AddToFunctionMap("IsEquippedAction",              (void*)&IsEquippedAction);
    AddToFunctionMap("HasAction",                     (void*)&HasAction);
#endif

#if DEVHELPER_LUA
    AddToFunctionMap("ReloadMap",                     (void*)&ReloadMap);
    AddToFunctionMap("ToggleDisplayNormals",          (void*)&ToggleDisplayNormals);
    AddToFunctionMap("ToggleGroundEffects",           (void*)&ToggleGroundEffects);
    AddToFunctionMap("ToggleLiquids",                 (void*)&ToggleLiquids);
    AddToFunctionMap("ToggleM2",                      (void*)&ToggleM2);
    AddToFunctionMap("ToggleTerrain",                 (void*)&ToggleTerrain);
    AddToFunctionMap("ToggleTerrainCulling",          (void*)&ToggleTerrainCulling);
    AddToFunctionMap("ToggleWireframeMode",           (void*)&ToggleWireframeMode);
    AddToFunctionMap("ToggleWMO",                     (void*)&ToggleWMO);
#endif

    // Spell API
    AddToFunctionMap("GetSpellInfo",                  (void*)&GetSpellInfo);
    AddToFunctionMap("GetSpellTexture",               (void*)&GetSpellTexture);
    AddToFunctionMap("GetSpellCooldown",              (void*)&GetSpellCooldown);
    AddToFunctionMap("GetSpellCharges",               (void*)&GetSpellCharges);
    AddToFunctionMap("GetSpellCount",                 (void*)&GetSpellCount);
    AddToFunctionMap("IsSpellKnown",                  (void*)&IsSpellKnown);
    AddToFunctionMap("GetSpellLink",                  (void*)&GetSpellLink);
    AddToFunctionMap("GetSpellBookItemInfo",          (void*)&GetSpellBookItemInfo);
    AddToFunctionMap("GetSpellTabInfo",               (void*)&GetSpellTabInfo);
    AddToFunctionMap("GetNumSpellTabs",               (void*)&GetNumSpellTabs);
    AddToFunctionMap("FindSpellBookSlotBySpellID",    (void*)&FindSpellBookSlotBySpellID);
    AddToFunctionMap("GetSpellPowerCost",             (void*)&GetSpellPowerCost);
    AddToFunctionMap("GetSpellCastTime",              (void*)&GetSpellCastTime);
    AddToFunctionMap("GetSpellRange",                 (void*)&GetSpellRange);
    AddToFunctionMap("IsHarmfulSpell",                (void*)&IsHarmfulSpell);
    AddToFunctionMap("IsHelpfulSpell",                (void*)&IsHelpfulSpell);
    AddToFunctionMap("IsPassiveSpell",                (void*)&IsPassiveSpell);
    AddToFunctionMap("GetSpellAutocast",              (void*)&GetSpellAutocast);
    AddToFunctionMap("ToggleSpellAutocast",           (void*)&ToggleSpellAutocast);

    // Talent API
    AddToFunctionMap("GetNumTalents",                 (void*)&GetNumTalents);
    AddToFunctionMap("GetTalentInfo",                 (void*)&GetTalentInfo);
    AddToFunctionMap("GetNumTalentTabs",              (void*)&GetNumTalentTabs);
    AddToFunctionMap("GetTalentTabInfo",              (void*)&GetTalentTabInfo);
    AddToFunctionMap("LearnTalent",                   (void*)&LearnTalent);
    AddToFunctionMap("GetUnspentTalentPoints",        (void*)&GetUnspentTalentPoints);
    AddToFunctionMap("GetActiveSpecGroup",            (void*)&GetActiveSpecGroup);
    AddToFunctionMap("GetNumSpecGroups",              (void*)&GetNumSpecGroups);
    AddToFunctionMap("GetSpecialization",             (void*)&GetSpecialization);
    AddToFunctionMap("GetSpecializationInfo",         (void*)&GetSpecializationInfo);
    AddToFunctionMap("GetPrimaryTalentTree",          (void*)&GetPrimaryTalentTree);

    // Item API
    AddToFunctionMap("GetItemInfo",                   (void*)&GetItemInfo);
    AddToFunctionMap("GetItemInfoInstant",            (void*)&GetItemInfoInstant);
    AddToFunctionMap("GetItemIcon",                   (void*)&GetItemIcon);
    AddToFunctionMap("GetItemQualityColor",           (void*)&GetItemQualityColor);
    AddToFunctionMap("GetItemCount",                  (void*)&GetItemCount);
    AddToFunctionMap("GetItemCooldown",               (void*)&GetItemCooldown);
    AddToFunctionMap("GetItemSpell",                  (void*)&GetItemSpell);
    AddToFunctionMap("GetItemStats",                  (void*)&GetItemStats);
    AddToFunctionMap("GetItemGem",                    (void*)&GetItemGem);
    AddToFunctionMap("GetItemFamily",                 (void*)&GetItemFamily);
    AddToFunctionMap("GetItemClassInfo",              (void*)&GetItemClassInfo);
    AddToFunctionMap("GetItemSubClassInfo",           (void*)&GetItemSubClassInfo);
    AddToFunctionMap("GetItemInventoryType",          (void*)&GetItemInventoryType);
    AddToFunctionMap("GetItemUniqueness",             (void*)&GetItemUniqueness);
    AddToFunctionMap("IsEquippableItem",              (void*)&IsEquippableItem);
    AddToFunctionMap("IsUsableItem",                  (void*)&IsUsableItem);
    AddToFunctionMap("IsConsumableItem",              (void*)&IsConsumableItem);
    AddToFunctionMap("IsEquippedItem",                (void*)&IsEquippedItem);
    AddToFunctionMap("IsCurrentItem",                 (void*)&IsCurrentItem);
    AddToFunctionMap("EquipItemByName",               (void*)&EquipItemByName);
    AddToFunctionMap("GetInventoryItemID",            (void*)&GetInventoryItemID);
    AddToFunctionMap("GetInventoryItemLink",          (void*)&GetInventoryItemLink);
    AddToFunctionMap("GetInventoryItemTexture",       (void*)&GetInventoryItemTexture);
    AddToFunctionMap("GetInventoryItemQuality",       (void*)&GetInventoryItemQuality);
    AddToFunctionMap("GetInventoryItemDurability",    (void*)&GetInventoryItemDurability);
    AddToFunctionMap("GetInventoryItemBroken",        (void*)&GetInventoryItemBroken);
    AddToFunctionMap("GetInventoryItemCount",         (void*)&GetInventoryItemCount);
    AddToFunctionMap("GetInventoryItemCooldown",      (void*)&GetInventoryItemCooldown);

    // Unit API
    AddToFunctionMap("UnitGUID",                      (void*)&UnitGUID);
    AddToFunctionMap("UnitExists",                    (void*)&UnitExists);
    AddToFunctionMap("UnitIsUnit",                    (void*)&UnitIsUnit);
    AddToFunctionMap("UnitName",                      (void*)&UnitName);
    AddToFunctionMap("UnitFullName",                  (void*)&UnitFullName);
    AddToFunctionMap("UnitClass",                     (void*)&UnitClass);
    AddToFunctionMap("UnitRace",                      (void*)&UnitRace);
    AddToFunctionMap("UnitSex",                       (void*)&UnitSex);
    AddToFunctionMap("UnitLevel",                     (void*)&UnitLevel);
    AddToFunctionMap("UnitEffectiveLevel",            (void*)&UnitEffectiveLevel);
    AddToFunctionMap("UnitClassification",            (void*)&UnitClassification);
    AddToFunctionMap("UnitCreatureFamily",            (void*)&UnitCreatureFamily);
    AddToFunctionMap("UnitCreatureType",              (void*)&UnitCreatureType);
    AddToFunctionMap("UnitHealth",                    (void*)&UnitHealth);
    AddToFunctionMap("UnitHealthMax",                 (void*)&UnitHealthMax);
    AddToFunctionMap("UnitPower",                     (void*)&UnitPower);
    AddToFunctionMap("UnitPowerMax",                  (void*)&UnitPowerMax);
    AddToFunctionMap("UnitPowerType",                 (void*)&UnitPowerType);
    AddToFunctionMap("UnitMana",                      (void*)&UnitMana);
    AddToFunctionMap("UnitManaMax",                   (void*)&UnitManaMax);
    AddToFunctionMap("UnitIsDead",                    (void*)&UnitIsDead);
    AddToFunctionMap("UnitIsGhost",                   (void*)&UnitIsGhost);
    AddToFunctionMap("UnitIsDeadOrGhost",             (void*)&UnitIsDeadOrGhost);
    AddToFunctionMap("UnitIsCorpse",                  (void*)&UnitIsCorpse);
    AddToFunctionMap("UnitIsConnected",               (void*)&UnitIsConnected);
    AddToFunctionMap("UnitIsPlayer",                  (void*)&UnitIsPlayer);
    AddToFunctionMap("UnitPlayerControlled",          (void*)&UnitPlayerControlled);
    AddToFunctionMap("UnitIsEnemy",                   (void*)&UnitIsEnemy);
    AddToFunctionMap("UnitIsFriend",                  (void*)&UnitIsFriend);
    AddToFunctionMap("UnitCanAttack",                 (void*)&UnitCanAttack);
    AddToFunctionMap("UnitCanAssist",                 (void*)&UnitCanAssist);
    AddToFunctionMap("UnitCanCooperate",              (void*)&UnitCanCooperate);
    AddToFunctionMap("UnitIsCharmed",                 (void*)&UnitIsCharmed);
    AddToFunctionMap("UnitIsPVP",                     (void*)&UnitIsPVP);
    AddToFunctionMap("UnitIsPVPFreeForAll",           (void*)&UnitIsPVPFreeForAll);
    AddToFunctionMap("UnitIsPVPSanctuary",            (void*)&UnitIsPVPSanctuary);
    AddToFunctionMap("UnitIsPartyLeader",             (void*)&UnitIsPartyLeader);
    AddToFunctionMap("UnitInParty",                   (void*)&UnitInParty);
    AddToFunctionMap("UnitInRaid",                    (void*)&UnitInRaid);
    AddToFunctionMap("UnitIsGroupLeader",             (void*)&UnitIsGroupLeader);
    AddToFunctionMap("UnitIsGroupAssistant",          (void*)&UnitIsGroupAssistant);
    AddToFunctionMap("UnitInRange",                   (void*)&UnitInRange);
    AddToFunctionMap("UnitDistanceSquared",           (void*)&UnitDistanceSquared);
    AddToFunctionMap("UnitIsVisible",                 (void*)&UnitIsVisible);
    AddToFunctionMap("UnitInBattleground",            (void*)&UnitInBattleground);
    AddToFunctionMap("UnitInVehicle",                 (void*)&UnitInVehicle);
    AddToFunctionMap("UnitAura",                      (void*)&UnitAura);
    AddToFunctionMap("UnitBuff",                      (void*)&UnitBuff);
    AddToFunctionMap("UnitDebuff",                    (void*)&UnitDebuff);
    AddToFunctionMap("UnitAuraBySlot",                (void*)&UnitAuraBySlot);
    AddToFunctionMap("GetPlayerBuff",                 (void*)&GetPlayerBuff);
    AddToFunctionMap("GetPlayerBuffName",             (void*)&GetPlayerBuffName);
    AddToFunctionMap("GetPlayerBuffTexture",          (void*)&GetPlayerBuffTexture);
    AddToFunctionMap("GetPlayerBuffTimeLeft",         (void*)&GetPlayerBuffTimeLeft);
    AddToFunctionMap("UnitCastingInfo",               (void*)&UnitCastingInfo);
    AddToFunctionMap("UnitChannelInfo",               (void*)&UnitChannelInfo);
    AddToFunctionMap("UnitCanBeAttacked",             (void*)&UnitCanBeAttacked);
    AddToFunctionMap("UnitStat",                      (void*)&UnitStat);
    AddToFunctionMap("UnitAttackSpeed",               (void*)&UnitAttackSpeed);
    AddToFunctionMap("UnitAttackPower",               (void*)&UnitAttackPower);
    AddToFunctionMap("UnitRangedAttackPower",         (void*)&UnitRangedAttackPower);
    AddToFunctionMap("UnitDamage",                    (void*)&UnitDamage);
    AddToFunctionMap("UnitRangedDamage",              (void*)&UnitRangedDamage);
    AddToFunctionMap("UnitDefense",                   (void*)&UnitDefense);
    AddToFunctionMap("UnitArmor",                     (void*)&UnitArmor);
    AddToFunctionMap("UnitResistance",                (void*)&UnitResistance);
    AddToFunctionMap("UnitAffectingCombat",           (void*)&UnitAffectingCombat);

    // Quest API
    AddToFunctionMap("GetQuestLogTitle",              (void*)&GetQuestLogTitle);
    AddToFunctionMap("GetNumQuestLogEntries",         (void*)&GetNumQuestLogEntries);
    AddToFunctionMap("GetNumQuestLeaderBoards",       (void*)&GetNumQuestLeaderBoards);
    AddToFunctionMap("GetQuestLogLeaderBoard",        (void*)&GetQuestLogLeaderBoard);
    AddToFunctionMap("GetQuestLogRewardInfo",         (void*)&GetQuestLogRewardInfo);
    AddToFunctionMap("GetNumQuestLogRewards",         (void*)&GetNumQuestLogRewards);
    AddToFunctionMap("GetNumQuestLogChoices",         (void*)&GetNumQuestLogChoices);
    AddToFunctionMap("GetQuestLogRewardMoney",        (void*)&GetQuestLogRewardMoney);
    AddToFunctionMap("GetQuestLogRewardSpell",        (void*)&GetQuestLogRewardSpell);
    AddToFunctionMap("GetQuestLogRewardXP",           (void*)&GetQuestLogRewardXP);
    AddToFunctionMap("GetQuestLink",                  (void*)&GetQuestLink);
    AddToFunctionMap("IsQuestFlaggedCompleted",       (void*)&IsQuestFlaggedCompleted);
    AddToFunctionMap("GetQuestLogIndexByID",          (void*)&GetQuestLogIndexByID);
    AddToFunctionMap("C_QuestLog_IsQuestFlaggedCompleted", (void*)&C_QuestLog_IsQuestFlaggedCompleted);
    AddToFunctionMap("C_QuestLog_GetInfo",            (void*)&C_QuestLog_GetInfo);
    AddToFunctionMap("C_QuestLog_GetNumQuestLogEntries", (void*)&C_QuestLog_GetNumQuestLogEntries);

    // Map/Position API
    AddToFunctionMap("GetPlayerMapPosition",          (void*)&GetPlayerMapPosition);
    AddToFunctionMap("GetPlayerFacing",               (void*)&GetPlayerFacing);
    AddToFunctionMap("GetCursorPosition",             (void*)&GetCursorPosition);
    AddToFunctionMap("GetCorpseMapPosition",          (void*)&GetCorpseMapPosition);
    AddToFunctionMap("GetBindLocation",               (void*)&GetBindLocation);
    AddToFunctionMap("SetMapToCurrentZone",           (void*)&SetMapToCurrentZone);
    AddToFunctionMap("C_Map_GetBestMapForUnit",       (void*)&C_Map_GetBestMapForUnit);
    AddToFunctionMap("C_Map_GetPlayerMapPosition",    (void*)&C_Map_GetPlayerMapPosition);
    AddToFunctionMap("C_Map_GetWorldPosFromMapPos",   (void*)&C_Map_GetWorldPosFromMapPos);
    AddToFunctionMap("GetZoneText",                   (void*)&GetZoneText);
    AddToFunctionMap("GetSubZoneText",                (void*)&GetSubZoneText);
    AddToFunctionMap("GetMinimapZoneText",            (void*)&GetMinimapZoneText);
    AddToFunctionMap("GetRealZoneText",               (void*)&GetRealZoneText);

    // Camera API
    AddToFunctionMap("GetCameraZoom",                 (void*)&GetCameraZoom);
    AddToFunctionMap("SetCameraZoom",                 (void*)&SetCameraZoom);
    AddToFunctionMap("GetCameraPosition",             (void*)&GetCameraPosition);
    AddToFunctionMap("GetCameraFacing",               (void*)&GetCameraFacing);
    AddToFunctionMap("CameraOrSelectOrMoveStart",     (void*)&CameraOrSelectOrMoveStart);
    AddToFunctionMap("CameraOrSelectOrMoveStop",      (void*)&CameraOrSelectOrMoveStop);
    AddToFunctionMap("MoveViewLeftStart",             (void*)&MoveViewLeftStart);
    AddToFunctionMap("MoveViewRightStart",            (void*)&MoveViewRightStart);
    AddToFunctionMap("MoveViewUpStart",               (void*)&MoveViewUpStart);
    AddToFunctionMap("MoveViewDownStart",             (void*)&MoveViewDownStart);
    AddToFunctionMap("MoveViewLeftStop",              (void*)&MoveViewLeftStop);
    AddToFunctionMap("MoveViewRightStop",             (void*)&MoveViewRightStop);
    AddToFunctionMap("MoveViewUpStop",                (void*)&MoveViewUpStop);
    AddToFunctionMap("MoveViewDownStop",              (void*)&MoveViewDownStop);

    // System/Utility API
    AddToFunctionMap("GetTime",                       (void*)&GetTime);
    AddToFunctionMap("GetFramerate",                  (void*)&GetFramerate);
    AddToFunctionMap("GetNetStats",                   (void*)&GetNetStats);
    AddToFunctionMap("GetCVar",                       (void*)&GetCVar);
    AddToFunctionMap("GetCVarBool",                   (void*)&GetCVarBool);
    AddToFunctionMap("SetCVar",                       (void*)&SetCVar);
    AddToFunctionMap("GetCurrentKeyBoardFocus",       (void*)&GetCurrentKeyBoardFocus);
    AddToFunctionMap("GetMouseFocus",                 (void*)&GetMouseFocus);
    AddToFunctionMap("GetLocale",                     (void*)&GetLocale);
    AddToFunctionMap("GetBuildInfo",                  (void*)&GetBuildInfo);
    AddToFunctionMap("GetAddOnInfo",                  (void*)&GetAddOnInfo);
    AddToFunctionMap("GetAddOnMetadata",              (void*)&GetAddOnMetadata);
    AddToFunctionMap("GetNumAddOns",                  (void*)&GetNumAddOns);
    AddToFunctionMap("IsAddOnLoaded",                 (void*)&IsAddOnLoaded);
    AddToFunctionMap("IsAddOnLoadOnDemand",           (void*)&IsAddOnLoadOnDemand);
    AddToFunctionMap("LoadAddOn",                     (void*)&LoadAddOn);
    AddToFunctionMap("EnableAddOn",                   (void*)&EnableAddOn);
    AddToFunctionMap("DisableAddOn",                  (void*)&DisableAddOn);
    AddToFunctionMap("GetScreenWidth",                (void*)&GetScreenWidth);
    AddToFunctionMap("GetScreenHeight",               (void*)&GetScreenHeight);
    AddToFunctionMap("GetCursorInfo",                 (void*)&GetCursorInfo);
    AddToFunctionMap("GetCurrentCursorTexture",       (void*)&GetCurrentCursorTexture);
    AddToFunctionMap("InCombatLockdown",              (void*)&InCombatLockdown);

    // Chat/Social API
    AddToFunctionMap("SendChatMessage",               (void*)&SendChatMessage);
    AddToFunctionMap("GetNumDisplayChannels",         (void*)&GetNumDisplayChannels);
    AddToFunctionMap("GetChannelList",                (void*)&GetChannelList);
    AddToFunctionMap("GetChannelName",                (void*)&GetChannelName);
    AddToFunctionMap("C_ChatInfo_SendAddonMessage",   (void*)&C_ChatInfo_SendAddonMessage);
    AddToFunctionMap("C_ChatInfo_RegisterAddonMessagePrefix", (void*)&C_ChatInfo_RegisterAddonMessagePrefix);
    AddToFunctionMap("BNConnected",                   (void*)&BNConnected);
    AddToFunctionMap("BNGetNumFriends",               (void*)&BNGetNumFriends);
    AddToFunctionMap("GetNumFriends",                 (void*)&GetNumFriends);
    AddToFunctionMap("GetFriendInfo",                 (void*)&GetFriendInfo);
    AddToFunctionMap("GetNumGuildMembers",            (void*)&GetNumGuildMembers);
    AddToFunctionMap("GetGuildInfo",                  (void*)&GetGuildInfo);
    AddToFunctionMap("GetGuildRosterInfo",            (void*)&GetGuildRosterInfo);

    // Inventory/Container API
    AddToFunctionMap("GetContainerNumSlots",          (void*)&GetContainerNumSlots);
    AddToFunctionMap("GetContainerItemInfo",          (void*)&GetContainerItemInfo);
    AddToFunctionMap("GetContainerItemID",            (void*)&GetContainerItemID);
    AddToFunctionMap("GetContainerItemLink",          (void*)&GetContainerItemLink);
    AddToFunctionMap("GetContainerNumFreeSlots",      (void*)&GetContainerNumFreeSlots);
    AddToFunctionMap("C_Container_GetContainerNumSlots", (void*)&C_Container_GetContainerNumSlots);
    AddToFunctionMap("C_Container_GetContainerItemInfo", (void*)&C_Container_GetContainerItemInfo);
    AddToFunctionMap("PickupContainerItem",           (void*)&PickupContainerItem);
    AddToFunctionMap("SplitContainerItem",            (void*)&SplitContainerItem);
    AddToFunctionMap("UseContainerItem",              (void*)&UseContainerItem);

    // Tooltip API
    AddToFunctionMap("GameTooltip_SetDefaultAnchor",  (void*)&GameTooltip_SetDefaultAnchor);
    AddToFunctionMap("GameTooltip_SetUnit",           (void*)&GameTooltip_SetUnit);
    AddToFunctionMap("GameTooltip_SetSpellByID",      (void*)&GameTooltip_SetSpellByID);
    AddToFunctionMap("GameTooltip_SetItemByID",       (void*)&GameTooltip_SetItemByID);
    AddToFunctionMap("GameTooltip_SetHyperlink",      (void*)&GameTooltip_SetHyperlink);
    AddToFunctionMap("GameTooltip_AddLine",           (void*)&GameTooltip_AddLine);
    AddToFunctionMap("GameTooltip_AddDoubleLine",     (void*)&GameTooltip_AddDoubleLine);
    AddToFunctionMap("GameTooltip_GetItem",           (void*)&GameTooltip_GetItem);
    AddToFunctionMap("GameTooltip_GetSpell",          (void*)&GameTooltip_GetSpell);
    AddToFunctionMap("GameTooltip_GetUnit",           (void*)&GameTooltip_GetUnit);

    // Merchant API
    AddToFunctionMap("GetMerchantNumItems",           (void*)&GetMerchantNumItems);
    AddToFunctionMap("GetMerchantItemInfo",           (void*)&GetMerchantItemInfo);
    AddToFunctionMap("GetMerchantItemLink",           (void*)&GetMerchantItemLink);
    AddToFunctionMap("GetMerchantItemID",             (void*)&GetMerchantItemID);
    AddToFunctionMap("GetMerchantItemCostInfo",       (void*)&GetMerchantItemCostInfo);
    AddToFunctionMap("GetMerchantItemMaxStack",       (void*)&GetMerchantItemMaxStack);
    AddToFunctionMap("BuyMerchantItem",               (void*)&BuyMerchantItem);

    // Combat Log API
    AddToFunctionMap("CombatLogGetCurrentEventInfo",  (void*)&CombatLogGetCurrentEventInfo);
    AddToFunctionMap("GetCombatLogInfo",              (void*)&GetCombatLogInfo);

    // Compatibility Stubs
    AddToFunctionMap("GetNumEquipmentSets",           (void*)&GetNumEquipmentSets);
    AddToFunctionMap("GetEquipmentSetInfo",           (void*)&GetEquipmentSetInfo);
    AddToFunctionMap("UseEquipmentSet",               (void*)&UseEquipmentSet);
    AddToFunctionMap("GetNumGlyphSockets",            (void*)&GetNumGlyphSockets);
    AddToFunctionMap("GetGlyphSocketInfo",            (void*)&GetGlyphSocketInfo);
    AddToFunctionMap("SetActiveSpecGroup",            (void*)&SetActiveSpecGroup);
    AddToFunctionMap("CanExitVehicle",                (void*)&CanExitVehicle);
    AddToFunctionMap("VehicleExit",                   (void*)&VehicleExit);
    AddToFunctionMap("GetAchievementInfo",            (void*)&GetAchievementInfo);
    AddToFunctionMap("GetNumCompletedAchievements",   (void*)&GetNumCompletedAchievements);
    AddToFunctionMap("IsVoidStorageReady",            (void*)&IsVoidStorageReady);
    AddToFunctionMap("C_Transmog_GetItemInfo",        (void*)&C_Transmog_GetItemInfo);
}

// ============================================================
// Original functions (ported from WotLK-Extensions)
// ============================================================

int32_t CustomLua::GetShapeshiftFormID(lua_State* L)
{
    WoWGUID activePlayer = ClientServices::GetActivePlayer();

    if (activePlayer)
    {
        CGUnit* activeObjectPtr = reinterpret_cast<CGUnit*>(ClientServices::GetObjectPtr(activePlayer, TYPEMASK_UNIT));
        FrameScript::PushNumber(L, static_cast<double>(CGUnit::GetShapeshiftFormID(activeObjectPtr)));
        return 1;
    }

    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetSpellDescription(lua_State* L)
{
    if (FrameScript::IsNumber(L, 1))
    {
        uint32_t spellId = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
        SpellRow row{};
        char desc[1024] = { 0 };

        if (DBClient::GetLocalizedRow(g_spellDB, spellId, &row))
        {
            SpellParser::ParseText(&row, desc, 1024, 0, 0, 0, 0, 1, 0);
            FrameScript::PushString(L, desc);
            return 1;
        }
    }

    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellNameById(lua_State* L)
{
    if (FrameScript::IsNumber(L, 1))
    {
        uint32_t spellId = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
        SpellRow row{};

        if (DBClient::GetLocalizedRow(g_spellDB, spellId, &row))
        {
            FrameScript::PushString(L, row.m_name_lang);
            FrameScript::PushString(L, row.m_nameSubtext_lang);
            return 2;
        }
    }

    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GetCustomCombatRating(lua_State* L)
{
    if (FrameScript::IsNumber(L, 1))
    {
        int8_t ratingID = static_cast<int8_t>(FrameScript::GetNumber(L, 1));

        if (ratingID > -1 && ratingID < 7)
        {
            FrameScript::PushNumber(L, DataContainer::GetInstance().GetCustomCombatRating(ratingID));
            return 1;
        }
    }

    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetCustomCombatRatingBonus(lua_State* L)
{
    // TODO_TBC: Implement combat rating bonus calculation for TBC formulas
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::ConvertCoordsToScreenSpace(lua_State* L)
{
    float worldX = static_cast<float>(FrameScript::GetNumber(L, 1));
    float worldY = static_cast<float>(FrameScript::GetNumber(L, 2));
    float resX    = 0.f;
    float resY    = 0.f;

    Util::PercToScreenPos(worldX, worldY, &resX, &resY);

    FrameScript::PushNumber(L, resX);
    FrameScript::PushNumber(L, resY);
    return 2;
}

int32_t CustomLua::PortGraveyard(lua_State* L)
{
    // TODO_TBC: Find Graveyard teleport function address
    // WotLK used custom packet CMSG_TELEPORT_GRAVEYARD_REQUEST
    // For TBC: implement via custom packet or server-side handler
    return 0;
}

int32_t CustomLua::FlashGameWindow(lua_State* L)
{
    if (g_window && *g_window)
    {
        FLASHWINFO fi{};
        fi.cbSize    = sizeof(FLASHWINFO);
        fi.hwnd      = *g_window;
        fi.dwFlags   = FLASHW_ALL | FLASHW_TIMERNOFG;
        fi.uCount    = 3;
        fi.dwTimeout = 0;
        FlashWindowEx(&fi);
    }

    return 0;
}

// ============================================================
// Action Bar API
// ============================================================

int32_t CustomLua::FindSpellActionBarSlots(lua_State* L)
{
    uint32_t spellID = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
    uint8_t count = 0;

    for (uint8_t i = 0; i < 144; i++)
    {
        if (g_actionBarSpellIDArray[i] == spellID)
        {
            FrameScript::PushNumber(L, i);
            count++;
        }
    }

    if (!count)
    {
        FrameScript::PushNil(L);
        return 1;
    }

    return count;
}

int32_t CustomLua::ReplaceActionBarSpell(lua_State* L)
{
    uint32_t oldSpellID = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
    uint32_t newSpellID = static_cast<uint32_t>(FrameScript::GetNumber(L, 2));

    for (uint8_t i = 0; i < 144; i++)
    {
        if (g_actionBarSpellIDArray[i] == oldSpellID)
        {
            g_actionBarSpellIDArray[i] = newSpellID;
            CNetClient::Packet_MSG_SET_ACTION_BUTTON(i, true, false);

            for (uint8_t j = i + 72; j < 144; j += 12)
            {
                if (!g_actionButtonsArray[j])
                {
                    g_actionBarSpellIDArray[i] = newSpellID;
                    g_actionButtonsArray[j]    = 1;
                    CNetClient::Packet_MSG_SET_ACTION_BUTTON(j, true, false);
                }
            }
        }
    }

    return 0;
}

int32_t CustomLua::SetSpellInActionBarSlot(lua_State* L)
{
    uint32_t spellID = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
    uint8_t  slotID  = static_cast<uint8_t>(FrameScript::GetNumber(L, 2));

    if (slotID < 144)
    {
        if (!g_actionButtonsArray[slotID])
            g_actionButtonsArray[slotID] = 1;

        g_actionBarSpellIDArray[slotID] = spellID;
        CNetClient::Packet_MSG_SET_ACTION_BUTTON(slotID, true, false);
    }

    return 0;
}

int32_t CustomLua::GetActionInfo(lua_State* L)
{
    // TODO_TBC: Find GetActionInfo Lua function address to call directly,
    // or implement by reading action button data structures.
    // WotLK: native function existed; for TBC find equivalent.
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 3;
}

int32_t CustomLua::GetActionTexture(lua_State* L)
{
    // TODO_TBC: Implement via action button data lookup
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetActionText(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetActionCount(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetActionCooldown(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 3;
}

int32_t CustomLua::IsAttackAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsAutoRepeatAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsCurrentAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsUsableAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    FrameScript::PushBoolean(L, false);
    return 2;
}

int32_t CustomLua::IsConsumableAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsEquippedAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::HasAction(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// ============================================================
// Dev Helper Lua functions
// ============================================================

int32_t CustomLua::ReloadMap(lua_State* L)
{
    uint64_t activePlayer = ClientServices::GetActivePlayer();

    if (activePlayer)
    {
        MapRow* row = nullptr;
        int32_t mapId = *g_currentMapID;
        CGUnit* activeObjectPtr = reinterpret_cast<CGUnit*>(ClientServices::GetObjectPtr(activePlayer, TYPEMASK_UNIT));
        CMovement* moveInfo = activeObjectPtr->m_movementInfo;

        if (mapId > -1)
        {
            row = reinterpret_cast<MapRow*>(DBClient::GetRow(&g_mapDB->m_vtable2, mapId));

            if (row)
            {
                CWorld::UnloadMap();
                CWorld::LoadMap(row->m_directory, &moveInfo->m_position, mapId);
            }
        }
    }

    return 0;
}

int32_t CustomLua::ToggleDisplayNormals(lua_State* L)
{
    renderFlags3 ^= 0x40;
    return 0;
}

int32_t CustomLua::ToggleGroundEffects(lua_State* L)
{
    renderFlags4 ^= 0x80;
    return 0;
}

int32_t CustomLua::ToggleLiquids(lua_State* L)
{
    renderFlags2 ^= 0x20;
    return 0;
}

int32_t CustomLua::ToggleM2(lua_State* L)
{
    renderFlags1 ^= 0x01;
    return 0;
}

int32_t CustomLua::ToggleTerrain(lua_State* L)
{
    renderFlags1 ^= 0x02;
    return 0;
}

int32_t CustomLua::ToggleTerrainCulling(lua_State* L)
{
    renderFlags1 ^= 0x04;
    return 0;
}

int32_t CustomLua::ToggleWireframeMode(lua_State* L)
{
    renderFlags1 ^= 0x08;
    return 0;
}

int32_t CustomLua::ToggleWMO(lua_State* L)
{
    renderFlags1 ^= 0x10;
    return 0;
}

// ============================================================
// Spell API Backports
// ============================================================

int32_t CustomLua::GetSpellInfo(lua_State* L)
{
    // TODO_TBC: Call native GetSpellInfo or read from Spell.dbc
    // Returns: name, rank, icon, cost, isFunnel, powerType, castTime, minRange, maxRange
    if (FrameScript::IsNumber(L, 1))
    {
        uint32_t spellId = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
        SpellRow row{};

        if (DBClient::GetLocalizedRow(g_spellDB, spellId, &row))
        {
            FrameScript::PushString(L, row.m_name_lang ? row.m_name_lang : "");
            FrameScript::PushString(L, row.m_nameSubtext_lang ? row.m_nameSubtext_lang : "");
            FrameScript::PushNil(L);  // icon path - TODO_TBC
            FrameScript::PushNumber(L, row.m_manaCost);
            FrameScript::PushBoolean(L, false);
            FrameScript::PushNumber(L, row.m_powerType);
            FrameScript::PushNumber(L, 0); // cast time - TODO_TBC
            FrameScript::PushNumber(L, 0.f); // min range
            FrameScript::PushNumber(L, 0.f); // max range
            return 9;
        }
    }

    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellTexture(lua_State* L)
{
    // TODO_TBC: Implement via SpellIcon.dbc lookup
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellCooldown(lua_State* L)
{
    // TODO_TBC: Find GetSpellCooldown native function address
    // Returns: start, duration, enabled
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 1);
    return 3;
}

// STUB: TBC has no charge system
int32_t CustomLua::GetSpellCharges(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellCount(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::IsSpellKnown(lua_State* L)
{
    // TODO_TBC: Check player spell book for the spell ID
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::GetSpellLink(lua_State* L)
{
    if (FrameScript::IsNumber(L, 1))
    {
        uint32_t spellId = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
        char buf[128];
        snprintf(buf, sizeof(buf), "|cff71d5ff|Hspell:%u|h[Spell]|h|r", spellId);
        FrameScript::PushString(L, buf);
        return 1;
    }

    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellBookItemInfo(lua_State* L)
{
    // TODO_TBC: Implement full spellbook item info
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GetSpellTabInfo(lua_State* L)
{
    // TODO_TBC: Implement spellbook tab info from Talent/Ability data
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 4;
}

int32_t CustomLua::GetNumSpellTabs(lua_State* L)
{
    // TODO_TBC: Return real spellbook tab count
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::FindSpellBookSlotBySpellID(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSpellPowerCost(lua_State* L)
{
    // TODO_TBC: Implement from Spell.dbc data
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0); // powerType
    return 2;
}

int32_t CustomLua::GetSpellCastTime(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetSpellRange(lua_State* L)
{
    FrameScript::PushNumber(L, 0.f);
    FrameScript::PushNumber(L, 0.f);
    return 2;
}

int32_t CustomLua::IsHarmfulSpell(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsHelpfulSpell(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsPassiveSpell(lua_State* L)
{
    if (FrameScript::IsNumber(L, 1))
    {
        uint32_t spellId = static_cast<uint32_t>(FrameScript::GetNumber(L, 1));
        SpellRow row{};

        if (DBClient::GetLocalizedRow(g_spellDB, spellId, &row))
        {
            bool passive = (row.m_attributes & SPELL_ATTR0_PASSIVE) != 0;
            FrameScript::PushBoolean(L, passive);
            return 1;
        }
    }

    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::GetSpellAutocast(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::ToggleSpellAutocast(lua_State* L)
{
    return 0;
}

// ============================================================
// Talent API Backports
// ============================================================

int32_t CustomLua::GetNumTalents(lua_State* L)
{
    // TODO_TBC: Read from Talent.dbc for the given tab
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetTalentInfo(lua_State* L)
{
    // TODO_TBC: Read talent info from Talent.dbc
    // Returns: name, texture, tier, column, currentRank, maxRank, isExceptional, available
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetNumTalentTabs(lua_State* L)
{
    // TBC: 3 talent trees per class
    FrameScript::PushNumber(L, 3);
    return 1;
}

int32_t CustomLua::GetTalentTabInfo(lua_State* L)
{
    // TODO_TBC: Read from TalentTab.dbc
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 4;
}

int32_t CustomLua::LearnTalent(lua_State* L)
{
    // TODO_TBC: Implement talent learning via packet
    return 0;
}

int32_t CustomLua::GetUnspentTalentPoints(lua_State* L)
{
    // TODO_TBC: Read PLAYER_CHARACTER_POINTS1 field
    WoWGUID activePlayer = ClientServices::GetActivePlayer();

    if (activePlayer)
    {
        CGPlayer* player = reinterpret_cast<CGPlayer*>(ClientServices::GetObjectPtr(activePlayer, TYPEMASK_PLAYER));

        if (player && player->m_playerData)
        {
            FrameScript::PushNumber(L, player->m_playerData->m_characterPoints1);
            return 1;
        }
    }

    FrameScript::PushNumber(L, 0);
    return 1;
}

// STUB: No dual spec in TBC
int32_t CustomLua::GetActiveSpecGroup(lua_State* L)
{
    FrameScript::PushNumber(L, 1);
    return 1;
}

// STUB: No dual spec in TBC
int32_t CustomLua::GetNumSpecGroups(lua_State* L)
{
    FrameScript::PushNumber(L, 1);
    return 1;
}

int32_t CustomLua::GetSpecialization(lua_State* L)
{
    // TODO_TBC: Return the talent tree with the most invested points (1-3)
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetSpecializationInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetPrimaryTalentTree(lua_State* L)
{
    return GetSpecialization(L);
}

// ============================================================
// Item API Backports
// ============================================================

int32_t CustomLua::GetItemInfo(lua_State* L)
{
    // TODO_TBC: Implement GetItemInfo by reading ItemCache
    // Returns: itemName, itemLink, itemRarity, itemLevel, itemMinLevel,
    //          itemType, itemSubType, itemStackCount, itemEquipLoc,
    //          itemTexture, itemSellPrice
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemInfoInstant(lua_State* L)
{
    // TODO_TBC: Return item info without server query
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemIcon(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemQualityColor(lua_State* L)
{
    // Quality color table (R, G, B, hex)
    static const float qualityColors[7][3] =
    {
        { 0.61f, 0.61f, 0.61f }, // Poor
        { 1.00f, 1.00f, 1.00f }, // Common
        { 0.12f, 1.00f, 0.00f }, // Uncommon
        { 0.00f, 0.44f, 0.87f }, // Rare
        { 0.64f, 0.21f, 0.93f }, // Epic
        { 1.00f, 0.50f, 0.00f }, // Legendary
        { 0.90f, 0.80f, 0.50f }, // Artifact
    };

    int32_t quality = 0;

    if (FrameScript::IsNumber(L, 1))
        quality = static_cast<int32_t>(FrameScript::GetNumber(L, 1));

    if (quality < 0 || quality > 6)
        quality = 0;

    FrameScript::PushNumber(L, qualityColors[quality][0]);
    FrameScript::PushNumber(L, qualityColors[quality][1]);
    FrameScript::PushNumber(L, qualityColors[quality][2]);
    return 3;
}

int32_t CustomLua::GetItemCount(lua_State* L)
{
    // TODO_TBC: Count items in bags
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetItemCooldown(lua_State* L)
{
    // Returns: start, duration, enabled
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 1);
    return 3;
}

int32_t CustomLua::GetItemSpell(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GetItemStats(lua_State* L)
{
    // TODO_TBC: Return item stat table
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemGem(lua_State* L)
{
    // TODO_TBC: Implement gem socket info (TBC does have gems!)
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemFamily(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetItemClassInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemSubClassInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetItemInventoryType(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetItemUniqueness(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::IsEquippableItem(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsUsableItem(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    FrameScript::PushBoolean(L, false);
    return 2;
}

int32_t CustomLua::IsConsumableItem(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsEquippedItem(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsCurrentItem(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::EquipItemByName(lua_State* L)
{
    return 0;
}

int32_t CustomLua::GetInventoryItemID(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetInventoryItemLink(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetInventoryItemTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetInventoryItemQuality(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetInventoryItemDurability(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GetInventoryItemBroken(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::GetInventoryItemCount(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetInventoryItemCooldown(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 1);
    return 3;
}

// ============================================================
// Unit API Backports
// ============================================================

int32_t CustomLua::UnitGUID(lua_State* L)
{
    // TODO_TBC: Implement via ClntObjMgr lookup by unit token
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitExists(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsUnit(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitName(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitFullName(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::UnitClass(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0);
    return 3;
}

int32_t CustomLua::UnitRace(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0);
    return 3;
}

int32_t CustomLua::UnitSex(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitLevel(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitEffectiveLevel(lua_State* L)
{
    return UnitLevel(L);
}

int32_t CustomLua::UnitClassification(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitCreatureFamily(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitCreatureType(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitHealth(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitHealthMax(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitPower(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitPowerMax(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitPowerType(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitMana(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitManaMax(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitIsDead(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsGhost(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsDeadOrGhost(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsCorpse(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsConnected(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsPlayer(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitPlayerControlled(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsEnemy(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsFriend(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitCanAttack(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitCanAssist(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitCanCooperate(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsCharmed(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsPVP(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsPVPFreeForAll(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsPVPSanctuary(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsPartyLeader(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitInParty(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitInRaid(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsGroupLeader(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitIsGroupAssistant(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitInRange(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    FrameScript::PushBoolean(L, false);
    return 2;
}

int32_t CustomLua::UnitDistanceSquared(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    return 1;
}

int32_t CustomLua::UnitIsVisible(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitInBattleground(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// STUB: TBC has no vehicles
int32_t CustomLua::UnitInVehicle(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitAura(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitBuff(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitDebuff(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitAuraBySlot(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetPlayerBuff(lua_State* L)
{
    FrameScript::PushNumber(L, -1);
    return 1;
}

int32_t CustomLua::GetPlayerBuffName(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetPlayerBuffTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetPlayerBuffTimeLeft(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitCastingInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitChannelInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::UnitCanBeAttacked(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::UnitStat(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::UnitAttackSpeed(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 2;
}

int32_t CustomLua::UnitAttackPower(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 3;
}

int32_t CustomLua::UnitRangedAttackPower(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 3;
}

int32_t CustomLua::UnitDamage(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0);
    return 5;
}

int32_t CustomLua::UnitRangedDamage(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0);
    FrameScript::PushNumber(L, 0.0);
    return 2;
}

int32_t CustomLua::UnitDefense(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::UnitArmor(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::UnitResistance(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::UnitAffectingCombat(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// ============================================================
// Quest API Backports
// ============================================================

int32_t CustomLua::GetQuestLogTitle(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetNumQuestLogEntries(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::GetNumQuestLeaderBoards(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetQuestLogLeaderBoard(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushBoolean(L, false);
    return 4;
}

int32_t CustomLua::GetQuestLogRewardInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetNumQuestLogRewards(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetNumQuestLogChoices(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetQuestLogRewardMoney(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetQuestLogRewardSpell(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GetQuestLogRewardXP(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetQuestLink(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::IsQuestFlaggedCompleted(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::GetQuestLogIndexByID(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::C_QuestLog_IsQuestFlaggedCompleted(lua_State* L)
{
    return IsQuestFlaggedCompleted(L);
}

int32_t CustomLua::C_QuestLog_GetInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::C_QuestLog_GetNumQuestLogEntries(lua_State* L)
{
    return GetNumQuestLogEntries(L);
}

// ============================================================
// Map/Position API Backports
// ============================================================

int32_t CustomLua::GetPlayerMapPosition(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 2;
}

int32_t CustomLua::GetPlayerFacing(lua_State* L)
{
    WoWGUID activePlayer = ClientServices::GetActivePlayer();

    if (activePlayer)
    {
        CGUnit* unit = reinterpret_cast<CGUnit*>(ClientServices::GetObjectPtr(activePlayer, TYPEMASK_UNIT));

        if (unit && unit->m_movementInfo)
        {
            FrameScript::PushNumber(L, unit->m_movementInfo->m_orientation);
            return 1;
        }
    }

    FrameScript::PushNumber(L, 0.0f);
    return 1;
}

int32_t CustomLua::GetCursorPosition(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 3;
}

int32_t CustomLua::GetCorpseMapPosition(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 2;
}

int32_t CustomLua::GetBindLocation(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 4;
}

int32_t CustomLua::SetMapToCurrentZone(lua_State* L)
{
    return 0;
}

int32_t CustomLua::C_Map_GetBestMapForUnit(lua_State* L)
{
    FrameScript::PushNumber(L, *g_currentMapID > 0 ? *g_currentMapID : 0);
    return 1;
}

int32_t CustomLua::C_Map_GetPlayerMapPosition(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::C_Map_GetWorldPosFromMapPos(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetZoneText(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetSubZoneText(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetMinimapZoneText(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetRealZoneText(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

// ============================================================
// Camera API Backports
// ============================================================

int32_t CustomLua::GetCameraZoom(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    return 1;
}

int32_t CustomLua::SetCameraZoom(lua_State* L)
{
    return 0;
}

int32_t CustomLua::GetCameraPosition(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 3;
}

int32_t CustomLua::GetCameraFacing(lua_State* L)
{
    FrameScript::PushNumber(L, 0.0f);
    FrameScript::PushNumber(L, 0.0f);
    return 2;
}

int32_t CustomLua::CameraOrSelectOrMoveStart(lua_State* L)  { return 0; }
int32_t CustomLua::CameraOrSelectOrMoveStop(lua_State* L)   { return 0; }
int32_t CustomLua::MoveViewLeftStart(lua_State* L)          { return 0; }
int32_t CustomLua::MoveViewRightStart(lua_State* L)         { return 0; }
int32_t CustomLua::MoveViewUpStart(lua_State* L)            { return 0; }
int32_t CustomLua::MoveViewDownStart(lua_State* L)          { return 0; }
int32_t CustomLua::MoveViewLeftStop(lua_State* L)           { return 0; }
int32_t CustomLua::MoveViewRightStop(lua_State* L)          { return 0; }
int32_t CustomLua::MoveViewUpStop(lua_State* L)             { return 0; }
int32_t CustomLua::MoveViewDownStop(lua_State* L)           { return 0; }

// ============================================================
// System/Utility API Backports
// ============================================================

int32_t CustomLua::GetTime(lua_State* L)
{
    FrameScript::PushNumber(L, static_cast<double>(time(nullptr)));
    return 1;
}

int32_t CustomLua::GetFramerate(lua_State* L)
{
    // TODO_TBC: Find FPS global variable
    FrameScript::PushNumber(L, 0.0f);
    return 1;
}

int32_t CustomLua::GetNetStats(lua_State* L)
{
    // Returns: bandwidthIn, bandwidthOut, latencyHome, latencyWorld
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 4;
}

int32_t CustomLua::GetCVar(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetCVarBool(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::SetCVar(lua_State* L)
{
    return 0;
}

int32_t CustomLua::GetCurrentKeyBoardFocus(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetMouseFocus(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetLocale(lua_State* L)
{
    // TODO_TBC: Return actual client locale
    FrameScript::PushString(L, "enUS");
    return 1;
}

int32_t CustomLua::GetBuildInfo(lua_State* L)
{
    // TBC 2.4.3 build info
    FrameScript::PushString(L, "2.4.3");
    FrameScript::PushString(L, "8606");
    FrameScript::PushString(L, "Jan  1 2009");
    FrameScript::PushString(L, "00:00:00");
    return 4;
}

int32_t CustomLua::GetAddOnInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetAddOnMetadata(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetNumAddOns(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::IsAddOnLoaded(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::IsAddOnLoadOnDemand(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

int32_t CustomLua::LoadAddOn(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::EnableAddOn(lua_State* L)  { return 0; }
int32_t CustomLua::DisableAddOn(lua_State* L) { return 0; }

int32_t CustomLua::GetScreenWidth(lua_State* L)
{
    // TODO_TBC: Read from actual screen resolution
    FrameScript::PushNumber(L, 1024);
    return 1;
}

int32_t CustomLua::GetScreenHeight(lua_State* L)
{
    FrameScript::PushNumber(L, 768);
    return 1;
}

int32_t CustomLua::GetCursorInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetCurrentCursorTexture(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::InCombatLockdown(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// ============================================================
// Chat/Social API Backports
// ============================================================

int32_t CustomLua::SendChatMessage(lua_State* L)
{
    // TODO_TBC: Implement SendChatMessage via native Lua or packet
    return 0;
}

int32_t CustomLua::GetNumDisplayChannels(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetChannelList(lua_State* L)
{
    return 0;
}

int32_t CustomLua::GetChannelName(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::C_ChatInfo_SendAddonMessage(lua_State* L)
{
    return 0;
}

int32_t CustomLua::C_ChatInfo_RegisterAddonMessagePrefix(lua_State* L)
{
    FrameScript::PushBoolean(L, true);
    return 1;
}

// STUB: No Battle.net in TBC
int32_t CustomLua::BNConnected(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// STUB: No Battle.net in TBC
int32_t CustomLua::BNGetNumFriends(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetNumFriends(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetFriendInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetNumGuildMembers(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::GetGuildInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetGuildRosterInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

// ============================================================
// Inventory/Container API Backports
// ============================================================

int32_t CustomLua::GetContainerNumSlots(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetContainerItemInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetContainerItemID(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetContainerItemLink(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetContainerNumFreeSlots(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    FrameScript::PushNumber(L, 0);
    return 2;
}

int32_t CustomLua::C_Container_GetContainerNumSlots(lua_State* L)
{
    return GetContainerNumSlots(L);
}

int32_t CustomLua::C_Container_GetContainerItemInfo(lua_State* L)
{
    return GetContainerItemInfo(L);
}

int32_t CustomLua::PickupContainerItem(lua_State* L)  { return 0; }
int32_t CustomLua::SplitContainerItem(lua_State* L)   { return 0; }
int32_t CustomLua::UseContainerItem(lua_State* L)     { return 0; }

// ============================================================
// Tooltip API Backports
// ============================================================

int32_t CustomLua::GameTooltip_SetDefaultAnchor(lua_State* L) { return 0; }
int32_t CustomLua::GameTooltip_SetUnit(lua_State* L)          { return 0; }
int32_t CustomLua::GameTooltip_SetSpellByID(lua_State* L)     { return 0; }
int32_t CustomLua::GameTooltip_SetItemByID(lua_State* L)      { return 0; }
int32_t CustomLua::GameTooltip_SetHyperlink(lua_State* L)     { return 0; }
int32_t CustomLua::GameTooltip_AddLine(lua_State* L)          { return 0; }
int32_t CustomLua::GameTooltip_AddDoubleLine(lua_State* L)    { return 0; }

int32_t CustomLua::GameTooltip_GetItem(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GameTooltip_GetSpell(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

int32_t CustomLua::GameTooltip_GetUnit(lua_State* L)
{
    FrameScript::PushNil(L);
    FrameScript::PushNil(L);
    return 2;
}

// ============================================================
// Merchant/Trade API Backports
// ============================================================

int32_t CustomLua::GetMerchantNumItems(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetMerchantItemInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetMerchantItemLink(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetMerchantItemID(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

int32_t CustomLua::GetMerchantItemCostInfo(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::GetMerchantItemMaxStack(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

int32_t CustomLua::BuyMerchantItem(lua_State* L) { return 0; }

// ============================================================
// Combat Log API Backports
// ============================================================

int32_t CustomLua::CombatLogGetCurrentEventInfo(lua_State* L)
{
    // TODO_TBC: Implement combat log event info via event system
    return 0;
}

int32_t CustomLua::GetCombatLogInfo(lua_State* L)
{
    return 0;
}

// ============================================================
// Compatibility Stubs (features not in TBC)
// ============================================================

// STUB: Equipment Sets not in TBC
int32_t CustomLua::GetNumEquipmentSets(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

// STUB: Equipment Sets not in TBC
int32_t CustomLua::GetEquipmentSetInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

// STUB: Equipment Sets not in TBC
int32_t CustomLua::UseEquipmentSet(lua_State* L) { return 0; }

// STUB: Glyphs not in TBC
int32_t CustomLua::GetNumGlyphSockets(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

// STUB: Glyphs not in TBC
int32_t CustomLua::GetGlyphSocketInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

// STUB: Dual Spec not in TBC
int32_t CustomLua::SetActiveSpecGroup(lua_State* L) { return 0; }

// STUB: Vehicles not in TBC
int32_t CustomLua::CanExitVehicle(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// STUB: Vehicles not in TBC
int32_t CustomLua::VehicleExit(lua_State* L) { return 0; }

// STUB: Achievements not in TBC
int32_t CustomLua::GetAchievementInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}

// STUB: Achievements not in TBC
int32_t CustomLua::GetNumCompletedAchievements(lua_State* L)
{
    FrameScript::PushNumber(L, 0);
    return 1;
}

// STUB: Void Storage not in TBC
int32_t CustomLua::IsVoidStorageReady(lua_State* L)
{
    FrameScript::PushBoolean(L, false);
    return 1;
}

// STUB: Transmog not in TBC
int32_t CustomLua::C_Transmog_GetItemInfo(lua_State* L)
{
    FrameScript::PushNil(L);
    return 1;
}
