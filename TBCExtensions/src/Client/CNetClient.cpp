#include <Client/CDataStore.hpp>
#include <Client/ClientServices.hpp>
#include <Client/CNetClient.hpp>
#include <Data/Enums.hpp>
#include <GameObjects/CGPlayer.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>
#include <SharedDefines.hpp>

int32_t CNetClient::ProcessMessage(void* thisMessage, uint32_t a2, CDataStore* dataStore, uint32_t a4)
{
    // TODO_TBC: Find NetClient::ProcessMessage address
    // WotLK 3.3.5 address was: 0x631FE0
    return reinterpret_cast<int32_t (__thiscall*)(void*, uint32_t, CDataStore*, uint32_t)>(0x000000 /* TODO_TBC */)(thisMessage, a2, dataStore, a4);
}

void CNetClient::SetMessageHandler(void* thisMessage, uint32_t opcode, void* handler, void* param)
{
    // TODO_TBC: Find NetClient::SetMessageHandler address
    // WotLK 3.3.5 address was: 0x631FA0
    reinterpret_cast<void (__thiscall*)(void*, uint32_t, void*, void*)>(0x000000 /* TODO_TBC */)(thisMessage, opcode, handler, param);
}

void CNetClient::Packet_MSG_SET_ACTION_BUTTON(uint32_t slotID, bool p1, bool p2)
{
    // TODO_TBC: Find SendActionButton packet function address
    // WotLK 3.3.5 address was: 0x5AA390
    reinterpret_cast<void (__cdecl*)(uint32_t, bool, bool)>(0x000000 /* TODO_TBC */)(slotID, p1, p2);
}

void __cdecl CNetClient::PacketGroupSpellLaunch(void* handlerParam, uint32_t opcode, uint32_t a2, CDataStore* a3)
{
    // TODO_TBC: Find group spell launch packet handler address
    // WotLK 3.3.5 address was: 0x80FEE0
    reinterpret_cast<void (__cdecl*)(void*, uint32_t, uint32_t, CDataStore*)>(0x000000 /* TODO_TBC */)(handlerParam, opcode, a2, a3);
}

void CNetClient::Apply()
{
    // TODO_TBC: Replace ALL addresses below with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses are provided as reference comments.
    // How to find: Trace the network message dispatcher and handler registration code.

    // InitializePlayerEx hook
    // WotLK 3.3.5: 0x6E8EE2 (call to original InitializePlayer)
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&InitializePlayerEx) - 0x000004 /* TODO_TBC */);

    // ProcessMessage hooks (3 call sites)
    // WotLK 3.3.5: 0x6324CA, 0x714AFC, 0x716A79
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&ProcessMessageEx) - 0x000004 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&ProcessMessageEx) - 0x000004 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&ProcessMessageEx) - 0x000004 /* TODO_TBC */);

    // SetMessageHandler hook
    // WotLK 3.3.5: 0x6B0B9E
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&SetMessageHandlerEx) - 0x000004 /* TODO_TBC */);

    // Group spell launch packet override
    // WotLK 3.3.5: 0x8100E5, 0x8100F5
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&PacketGroupSpellLaunch));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&PacketGroupSpellLaunch));
}

void CNetClient::SetCustomHandlers()
{
    SetMessageHandlerEx(nullptr, 0, SMSG_UPDATE_CUSTOM_COMBAT_RATING, &Packet_SMSG_UPDATE_CUSTOM_COMBAT_RATING, 0);
}

void CNetClient::InitializePlayerEx()
{
    ClientServices::InitializePlayer();
    SetCustomHandlers();
}

void __fastcall CNetClient::ProcessMessageEx(void* _this, uint32_t unused, uint32_t a2, CDataStore* a3, uint32_t a4)
{
    int16_t opcode = 0;
    CDataStore::GetInt16(a3, &opcode);

    if (opcode < NUM_ORIGINAL_MSG_TYPES)
    {
        a3->m_read -= 2;
        CNetClient::ProcessMessage(_this, a2, a3, a4);
    }
    else
    {
        // TODO_TBC: Find custom packet dispatch counter address
        // WotLK 3.3.5 address was: 0xC5D638
        ++*(uint32_t*)0x000000 /* TODO_TBC */;

        auto& packetData = DataContainer::GetInstance().GetPacketHandlerMap();

        for (auto& it : packetData)
        {
            if (opcode < NUM_CUSTOM_MSG_TYPES && it.first == opcode)
            {
                auto& data = it.second;
                reinterpret_cast<void (__cdecl*)(void*, uint32_t, uint32_t, CDataStore*)>(data.m_handler)(data.m_handler, opcode, a2, a3);
            }
            else
                CDataStore::IsRead(a3);
        }
    }
}

void __fastcall CNetClient::SetMessageHandlerEx(void* _this, uint32_t unused, uint32_t opcode, void* handler, void* param)
{
    if (opcode < NUM_ORIGINAL_MSG_TYPES)
        CNetClient::SetMessageHandler(_this, opcode, handler, param);
    else
        DataContainer::GetInstance().AddPacketHandler(opcode, CNetClientCustomPacket(handler, param));
}

void CNetClient::Packet_SMSG_UPDATE_CUSTOM_COMBAT_RATING(void* handlerParam, uint32_t opcode, uint32_t a2, CDataStore* a3)
{
    int8_t  ratingID     = 0;
    int32_t ratingAmount = 0;

    CDataStore::GetInt8(a3, &ratingID);
    CDataStore::GetInt32(a3, &ratingAmount);

    if (ratingID > -1 && ratingID < 7)
    {
        if (ratingAmount > -1)
            DataContainer::GetInstance().SetCustomCombatRating(ratingID, ratingAmount);
        else
            DataContainer::GetInstance().SetCustomCombatRating(ratingID, 0);
    }
}
