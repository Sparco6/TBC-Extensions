#include <Client/CFormula.hpp>
#include <Client/CGTooltip.hpp>
#include <Misc/DataContainer.hpp>
#include <Misc/Util.hpp>

#include <PatchConfig.hpp>

void CGTooltip::ApplyPatches()
{
#if SPELLDESCRIPTIONEXT_PATCH
    std::vector<const char*>& data = DataContainer::GetInstance().GetSpellVariableData();

    CFormula::FillSpellVariableData();
    // TODO_TBC: Replace all tooltip patch addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses were: 0x576B63, 0x576B7C, 0x578E8B
    // How to find: Trace spell tooltip rendering code that reads variable table
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(data.data()));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, static_cast<uint32_t>(data.size()));
    Util::OverwriteUInt32AtAddress(0x000000 /* TODO_TBC */, reinterpret_cast<uint32_t>(&CFormula::GetVariableValueEx) - 0x000004 /* TODO_TBC */);
#endif

    // TODO_TBC: Replace all tooltip SetSpell addresses with TBC 2.4.3 equivalents
    // WotLK 3.3.5 addresses were: 0x61DDB7 through 0x63136D
    // How to find: Find CGTooltip::SetSpell calls in item/spell tooltip display code
    // Uncomment and fill these addresses after finding them:
    /*
    std::vector<uint32_t> setSpellAddresses =
    {
        // TODO_TBC - add TBC 2.4.3 addresses here
    };

    for (size_t i = 0; i < setSpellAddresses.size(); i++)
        Util::OverwriteUInt32AtAddress(setSpellAddresses[i] - 4, reinterpret_cast<uint32_t>(&SetSpellEx) - setSpellAddresses[i]);
    */
}

int32_t __fastcall CGTooltip::SetSpellEx(CGTooltip* thisTooltip, int32_t unused, int32_t spellId, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, uint32_t* a9, int32_t a10, int32_t a11, int32_t a12, int32_t a13, int32_t a14, int32_t a15, int32_t a16)
{
    // TODO_TBC: Find CGTooltip::SetSpell address
    // WotLK 3.3.5 address was: 0x6238A0
    return reinterpret_cast<int32_t (__thiscall*)(CGTooltip*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t)>(0x000000 /* TODO_TBC */)(thisTooltip, spellId, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
}

void CGTooltip::AddLine(CGTooltip* thisTooltip, char* str1, char* str2, uint32_t color1, uint32_t color2, int32_t a6)
{
    // TODO_TBC: Find CGTooltip::AddLine address
    // WotLK 3.3.5 address was: 0x61FEC0
    reinterpret_cast<void (__thiscall*)(CGTooltip*, char*, char*, uint32_t, uint32_t, int32_t)>(0x000000 /* TODO_TBC */)(thisTooltip, str1, str2, color1, color2, a6);
}

int32_t CGTooltip::AddTalentPrereqs(CGTooltip* thisTooltip, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6)
{
    // TODO_TBC: Find CGTooltip::AddTalentPrereqs address
    // WotLK 3.3.5 address was: 0x6224F0
    return reinterpret_cast<int32_t (__thiscall*)(CGTooltip*, int32_t, int32_t, int32_t, int32_t, int32_t)>(0x000000 /* TODO_TBC */)(thisTooltip, a2, a3, a4, a5, a6);
}

int32_t CGTooltip::ClearTooltip(CGTooltip* thisTooltip)
{
    // TODO_TBC: Find CGTooltip::Clear address
    // WotLK 3.3.5 address was: 0x61C620
    return reinterpret_cast<int32_t (__thiscall*)(CGTooltip*)>(0x000000 /* TODO_TBC */)(thisTooltip);
}

int32_t CSimpleFrame::Hide(void* thisFrame)
{
    // TODO_TBC: Find CSimpleFrame::Hide address
    // WotLK 3.3.5 address was: 0x48F620
    return reinterpret_cast<int32_t (__thiscall*)(void*)>(0x000000 /* TODO_TBC */)(thisFrame);
}
