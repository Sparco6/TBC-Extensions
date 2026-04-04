#include <Misc/DataContainer.hpp>

#include <ctime>

DataContainer::DataContainer()
{
    m_raceNameTable.resize(32, 0);
    m_memoryTable.resize(64, 0);
}

DataContainer::~DataContainer()
{
}

DataContainer& DataContainer::GetInstance()
{
    static DataContainer instance;
    return instance;
}

void DataContainer::AddLuaFunction(const char* name, void* ptr)
{
    m_luaFunctions.insert(std::make_pair(name, ptr));
}

std::unordered_map<const char*, void*>& DataContainer::GetLuaFunctionMap()
{
    return m_luaFunctions;
}

void DataContainer::AddPacketHandler(uint32_t opcode, CNetClientCustomPacket packetData)
{
    m_packetData.insert(std::make_pair(opcode, packetData));
}

std::unordered_map<uint32_t, CNetClientCustomPacket>& DataContainer::GetPacketHandlerMap()
{
    return m_packetData;
}

std::vector<const char*>& DataContainer::GetSpellVariableData()
{
    return m_spellVariables;
}

uint32_t* DataContainer::GetRaceTablePtr()
{
    return m_raceNameTable.data();
}

uint32_t* DataContainer::GetMemoryTablePtr()
{
    return m_memoryTable.data();
}

uint32_t DataContainer::GetYearOffsetMultiplier() const
{
    return m_yearOffsetMult;
}

void DataContainer::SetYearOffsetMultiplier()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    m_yearOffsetMult = static_cast<uint32_t>((ltm->tm_year - 100) / 32);
}

int32_t DataContainer::GetCustomCombatRating(int8_t index) const
{
    return m_playerFields.m_combatRating[index];
}

void DataContainer::SetCustomCombatRating(int8_t index, int32_t value)
{
    m_playerFields.m_combatRating[index] = value;
}
