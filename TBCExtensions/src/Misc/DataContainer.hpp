#pragma once

#include <Client/CNetClient.hpp>
#include <GameObjects/CGPlayer.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

struct CDataStore;

class DataContainer
{
public:
    static DataContainer& GetInstance();

    void AddLuaFunction(const char* name, void* ptr);
    std::unordered_map<const char*, void*>& GetLuaFunctionMap();

    void AddPacketHandler(uint32_t opcode, CNetClientCustomPacket packetData);
    std::unordered_map<uint32_t, CNetClientCustomPacket>& GetPacketHandlerMap();

    std::vector<const char*>& GetSpellVariableData();

    uint32_t* GetRaceTablePtr();
    uint32_t* GetMemoryTablePtr();

    uint32_t GetYearOffsetMultiplier() const;
    void     SetYearOffsetMultiplier();

    int32_t  GetCustomCombatRating(int8_t index) const;
    void     SetCustomCombatRating(int8_t index, int32_t value);

private:
    std::unordered_map<const char*, void*>       m_luaFunctions;
    std::unordered_map<uint32_t, CNetClientCustomPacket> m_packetData;

    std::vector<const char*> m_spellVariables;

    std::vector<uint32_t> m_raceNameTable;
    std::vector<uint32_t> m_memoryTable;

    uint32_t m_yearOffsetMult = 0;

    CGPlayerCustomFields m_playerFields;

    DataContainer();
    DataContainer(const DataContainer&) = delete;
    ~DataContainer();

    DataContainer& operator=(const DataContainer&) = delete;
};
