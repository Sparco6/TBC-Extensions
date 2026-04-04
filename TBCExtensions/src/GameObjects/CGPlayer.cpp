#include <Client/ClientServices.hpp>
#include <Data/DBCAddresses.hpp>
#include <GameObjects/CGPlayer.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>

#include <PatchConfig.hpp>
#include <SharedDefines.hpp>

bool CGPlayer::IsDeadOrGhost(CGPlayer* thisPlayer)
{
    // TODO_TBC: Find CGPlayer::IsDeadOrGhost address
    // WotLK 3.3.5 address was: 0x6DAC10
    return reinterpret_cast<bool (__thiscall*)(CGPlayer*)>(0x000000 /* TODO_TBC */)(thisPlayer);
}

void CGPlayer::ApplyPatches()
{
#if CHARCREATIONRACE_FIX
    CharacterCreationRaceCrashfix();
#endif

#if COMBOPOINT_FIX
    // TODO_TBC: Find combo point fix address
    // WotLK 3.3.5 address was: 0x611707 (patch the conditional jump to unconditional)
    // How to find: Find function that checks if class can gain combo points
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
#endif
}

void CGPlayer::CharacterCreationRaceCrashfix()
{
    // TODO_TBC: Replace ALL addresses below with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses were: 0x4E157D, 0x4E16A3, 0x4E15B5, 0x4E20EE, etc.
    // How to find: Find CharacterCreate screen code that loads race name pointers
    std::vector<uint32_t> patchedAddresses = {
        // TODO_TBC - add TBC 2.4.3 character creation race table patch addresses here
        // 0x000000, 0x000000, ...
    };

    uint32_t* raceNameTablePtr = DataContainer::GetInstance().GetRaceTablePtr();
    uint32_t* memoryTablePtr   = DataContainer::GetInstance().GetMemoryTablePtr();

    for (uint8_t i = 0; i < patchedAddresses.size(); i++)
        Util::OverwriteUInt32AtAddress(patchedAddresses[i], reinterpret_cast<uint32_t>(memoryTablePtr));

    // TODO_TBC: Find race name pointer table patch address
    // WotLK 3.3.5 address was: 0x4CDA43 (stores raceNameTablePtr), 0xB24180 (original table)
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(raceNameTablePtr));
    memcpy(raceNameTablePtr, reinterpret_cast<const void*>(0x000000 /* TODO_TBC */), 0x30);

    for (uint8_t i = 22; i < 32; i++)
        raceNameTablePtr[i] = (uint32_t)&dummy;

    // TODO_TBC: Find character creation race count limit address
    // WotLK 3.3.5 address was: 0x4E0F86
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x40);
}
