#include <Client/SStr.hpp>
#include <Client/WoWTime.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>

void WoWTime::ApplyWoWTimePatches()
{
    // TODO_TBC: Replace ALL addresses below with TBC 2.4.3 (build 8606) equivalents
    // WotLK 3.3.5 addresses are provided as reference.
    // How to find: Search for PackWoWTime / UnpackWoWTime in IDA via their patterns.
    //              Look for time-related code in calendar/auction house/world events logic.

    // Patch PackWoWTimeToDword call
    // WotLK 3.3.5: 0x76CA56 (code), 0x76CA5C (call offset), ref 0x76CA60
    uint8_t byteArray[] = { 0x8B, 0x55, 0x08, 0x50, 0x52, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x08, 0x5D, 0xC3 };
    Util::OverwriteBytesAtAddress(0x000000 /* TODO_TBC */, byteArray, sizeof(byteArray));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&PackWoWTimeToDword) - 0x000000 /* TODO_TBC */);

    // WotLK 3.3.5: 0x4C989C, 0x4C98C2
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&PackTimeDataToDword) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&PackTimeDataToDword) - 0x000000 /* TODO_TBC */);

    // WotLK 3.3.5: 0x76CAD4, 0x76CB19, 0x76CB93
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&UnpackWoWTime) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&UnpackWoWTime) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&UnpackWoWTime) - 0x000000 /* TODO_TBC */);

    // WotLK 3.3.5: 0x76DA89, 0x76DA9F, 0x7E27B7, 0x7E2AC7
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&GetTimeString) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&GetTimeString) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&GetTimeString) - 0x000000 /* TODO_TBC */);
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&GetTimeString) - 0x000000 /* TODO_TBC */);

    // Patch year >= 31 checks
    // WotLK 3.3.5: 0x5B7ACC, 0x5B82C0, 0x5B8F35, 0x5B8F99, 0x5BFF44, 0x5BFF58, etc.
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000001 /* TODO_TBC */), 0x08);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xFF, 0x03);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x7F);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x08);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x03);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x03);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x02);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x03);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x02);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0xEB);
    Util::SetByteAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x08);
    Util::OverwriteBytesAtAddress(reinterpret_cast<void*>(0x000000 /* TODO_TBC */), 0x90, 0x05);
}

void WoWTime::PackTimeDataToDword(uint32_t* packedTime, int32_t minute, int32_t hour, int32_t weekDay, int32_t monthDay, int32_t month, int32_t year, int32_t flags)
{
    uint32_t temp = 0;
    temp += minute & 63;
    temp += (hour & 31) << 6;
    temp += (weekDay & 7) << 11;
    temp += (monthDay & 63) << 14;
    temp += (month & 15) << 20;
    temp += year >= 31 ? 31 << 24 : (year & 31) << 24;
    temp += (flags & 3) << 29;

    *packedTime = temp;
}

void WoWTime::PackWoWTimeToDword(uint32_t* dword, WoWTime* time)
{
    uint32_t temp = 0;
    temp += time->m_minute & 63;
    temp += (time->m_hour & 31) << 6;
    temp += (time->m_weekDay & 7) << 11;
    temp += (time->m_monthDay & 63) << 14;
    temp += (time->m_month & 15) << 20;
    temp += time->m_year >= 31 ? 31 << 24 : (time->m_year & 31) << 24;
    temp += (time->m_flags & 3) << 29;

    *dword = temp;
}

void WoWTime::UnpackWoWTime(uint32_t packedTime, int32_t* minute, int32_t* hour, int32_t* weekDay, int32_t* monthDay, int32_t* month, int32_t* year, int32_t* flags)
{
    if (minute)
    {
        if ((packedTime & 63) == 63)
            *minute = -1;
        else
            *minute = packedTime & 63;
    }

    if (hour)
    {
        if (((packedTime >> 6) & 31) == 31)
            *hour = -1;
        else
            *hour = (packedTime >> 6) & 31;
    }

    if (weekDay)
    {
        if (((packedTime >> 11) & 7) == 7)
            *weekDay = -1;
        else
            *weekDay = (packedTime >> 11) & 7;
    }

    if (monthDay)
    {
        if (((packedTime >> 14) & 63) == 63)
            *monthDay = -1;
        else
            *monthDay = (packedTime >> 14) & 63;
    }

    if (month)
    {
        if (((packedTime >> 20) & 15) == 15)
            *month = -1;
        else
            *month = (packedTime >> 20) & 15;
    }

    if (year)
        *year = (packedTime >> 24) & 31 + DataContainer::GetInstance().GetYearOffsetMultiplier() * 32;

    if (flags)
    {
        if (((packedTime >> 29) & 3) == 3)
            *flags = -1;
        else
            *flags = (packedTime >> 29) & 3;
    }
}

char* WoWTime::GetTimeString(WoWTime* a1, char* a2, uint32_t a3)
{
    // TODO_TBC: Find WoWTime::GetTimeString address for TBC
    // This function formats a time into a string; reconstruct from TBC client
    // WotLK 3.3.5 address was referenced by patching 0x76DA89 etc.
    // For now, provide a minimal implementation
    if (!a1 || !a2 || a3 == 0)
        return a2;

    if (a1->m_flags & 0x2)
        snprintf(a2, a3, "%02d:%02d", a1->m_hour, a1->m_minute);
    else
        snprintf(a2, a3, "%02d:%02d:%02d", a1->m_hour, a1->m_minute, 0);

    return a2;
}
