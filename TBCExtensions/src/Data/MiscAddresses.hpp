#pragma once

#include <Windows.h>
#include <cstdint>

// TODO_TBC: All addresses below are WotLK 3.3.5 placeholders.
// You MUST replace every 0x000000 /* TODO_TBC */ with the correct TBC 2.4.3 (build 8606) address.
// See TBC_PORTING_GUIDE.md for instructions on how to find each address.

// Render flags (WotLK: 0xCD774C-0xCD774F)
// TODO_TBC: Find render flags address via IDA - look for bool array near rendering CVars
// WotLK 3.3.5 address was: 0xCD774C
static volatile uint8_t& renderFlags1 = *reinterpret_cast<volatile uint8_t*>(0x000000 /* TODO_TBC */);
static volatile uint8_t& renderFlags2 = *reinterpret_cast<volatile uint8_t*>(0x000001 /* TODO_TBC */);
static volatile uint8_t& renderFlags3 = *reinterpret_cast<volatile uint8_t*>(0x000002 /* TODO_TBC */);
static volatile uint8_t& renderFlags4 = *reinterpret_cast<volatile uint8_t*>(0x000003 /* TODO_TBC */);

// TODO_TBC: Find current map ID global variable
// WotLK 3.3.5 address was: 0xBD088C
// How to find: Search for cross-references to map loading functions, look for int32 global
static int32_t* const g_currentMapID = reinterpret_cast<int32_t*>(0x000000 /* TODO_TBC */);

// TODO_TBC: Find game window HWND pointer
// WotLK 3.3.5 address was: 0xD41620
// How to find: Search for CreateWindowEx call and track where the result is stored
static HWND* const g_window = reinterpret_cast<HWND*>(0x000000 /* TODO_TBC */);

// TODO_TBC: Find action buttons array
// WotLK 3.3.5 address was: 0xC1DED8
// How to find: Search for action bar send packet function, find array it reads from
static uint32_t* g_actionButtonsArray = reinterpret_cast<uint32_t*>(0x000000 /* TODO_TBC */);

// TODO_TBC: Find action bar spell ID array
// WotLK 3.3.5 address was: 0xC1E358
// How to find: Same area as above, parallel array tracking spell IDs
static uint32_t* g_actionBarSpellIDArray = reinterpret_cast<uint32_t*>(0x000000 /* TODO_TBC */);
