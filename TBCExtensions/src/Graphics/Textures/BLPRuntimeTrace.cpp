#include "BLPRuntimeTrace.hpp"
#include <Windows.h>
#include <MinHook.h>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace BlpRuntimeTrace {
namespace {
constexpr uintptr_t kLoad = 0x005B3170;
using Original = int(__thiscall*)(void*, const char*, int);
Original g_original = nullptr;
std::atomic<bool> g_created{false}, g_armed{false}, g_seen{false};
std::atomic<bool> g_enabled{false};
std::atomic<uintptr_t> g_source{0}, g_retained{0};
std::atomic<unsigned> g_result{0}, g_encoding{0}, g_width{0}, g_height{0}, g_thread{0}, g_size{0};
std::atomic<bool> g_magic{false};
std::atomic<unsigned> g_totalCalls{0}, g_armedCalls{0}, g_pathMatches{0};

bool InText(uintptr_t address, size_t length) {
    const auto* base = reinterpret_cast<const unsigned char*>(GetModuleHandleA(nullptr));
    if (!base || reinterpret_cast<uintptr_t>(base) != 0x00400000) return false;
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE || nt->FileHeader.Machine != IMAGE_FILE_MACHINE_I386) return false;
    const auto* sections = IMAGE_FIRST_SECTION(nt);
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        const auto& s = sections[i];
        if (memcmp(s.Name, ".text", 5) != 0 || !(s.Characteristics & IMAGE_SCN_MEM_EXECUTE)) continue;
        const uintptr_t lo = reinterpret_cast<uintptr_t>(base) + s.VirtualAddress;
        const uintptr_t hi = lo + s.Misc.VirtualSize;
        return address >= lo && address <= hi && length <= hi - address;
    }
    return false;
}
bool MatchesControlled(const char* path) {
    if (!path) return false;
    char local[260] = {};
    __try {
        for (size_t i = 0; i < sizeof(local); ++i) {
            local[i] = path[i];
            if (!local[i]) break;
            if (local[i] == '/') local[i] = '\\';
            if (i + 1 == sizeof(local)) return false;
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
    return _stricmp(local, "Interface\\MasterWoW\\BLP2Test") == 0 ||
        _stricmp(local, "Interface\\MasterWoW\\BLP2Test.blp") == 0;
}
void CaptureHeader(void* object) {
    __try {
        const auto* self = reinterpret_cast<const unsigned char*>(object);
        const uintptr_t retained = *reinterpret_cast<const uintptr_t*>(self + 0x498);
        g_retained.store(retained, std::memory_order_relaxed);
        g_source.store(*reinterpret_cast<const uintptr_t*>(0x00D43120), std::memory_order_relaxed);
        g_size.store(*reinterpret_cast<const uint32_t*>(0x00D4311C), std::memory_order_relaxed);
        if (retained && memcmp(reinterpret_cast<const void*>(retained), "BLP2", 4) == 0) {
            g_magic.store(true, std::memory_order_relaxed);
            const auto* file = reinterpret_cast<const unsigned char*>(retained);
            g_encoding.store(file[8], std::memory_order_relaxed);
            g_width.store(*reinterpret_cast<const uint32_t*>(file + 12), std::memory_order_relaxed);
            g_height.store(*reinterpret_cast<const uint32_t*>(file + 16), std::memory_order_relaxed);
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) { /* Observation must never affect stock loading. */ }
}
int __fastcall Observe(void* object, void*, const char* path, int flags) {
    g_totalCalls.fetch_add(1, std::memory_order_relaxed);
    const bool armed = g_armed.load(std::memory_order_relaxed);
    if (armed) g_armedCalls.fetch_add(1, std::memory_order_relaxed);
    const bool match = armed && MatchesControlled(path);
    if (match) g_pathMatches.fetch_add(1, std::memory_order_relaxed);
    const int result = g_original(object, path, flags);
    if (match && g_armed.exchange(false, std::memory_order_acq_rel)) {
        g_thread.store(GetCurrentThreadId(), std::memory_order_relaxed);
        g_result.store(unsigned(result), std::memory_order_relaxed);
        if (result) CaptureHeader(object);
        g_seen.store(true, std::memory_order_release);
    }
    return result;
}
}
bool CreateObservationHook() {
    static const unsigned char expected[] = {0x55,0x8B,0xEC,0x51,0x56,0x57,0x8B,0x7D,0x08};
    MEMORY_BASIC_INFORMATION mbi{};
    if (!InText(kLoad, sizeof(expected)) ||
        VirtualQuery(reinterpret_cast<const void*>(kLoad), &mbi, sizeof(mbi)) != sizeof(mbi) ||
        mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
        memcmp(reinterpret_cast<const void*>(kLoad), expected, sizeof(expected)) != 0) return false;
    const auto status = MH_CreateHook(reinterpret_cast<void*>(kLoad), reinterpret_cast<void*>(&Observe),
        reinterpret_cast<void**>(&g_original));
    g_created.store(status == MH_OK, std::memory_order_release);
    return status == MH_OK;
}
void SetEnabled(bool enabled) { g_enabled.store(enabled && g_created.load(), std::memory_order_release); }
bool ArmOneShot() {
    if (!g_enabled.load(std::memory_order_acquire) || !g_original) return false;
    g_seen.store(false, std::memory_order_release);
    g_source.store(0); g_retained.store(0); g_result.store(0);
    g_encoding.store(0); g_width.store(0); g_height.store(0); g_thread.store(0); g_size.store(0); g_magic.store(false);
    g_armedCalls.store(0); g_pathMatches.store(0);
    g_armed.store(true, std::memory_order_release);
    return true;
}
void Describe(char* output, size_t capacity) {
    if (!output || !capacity) return;
    snprintf(output, capacity,
        "BLP OBSERVATION HOOK: %s; ONE-SHOT: %s; TOTAL CALLS: %u; ARMED-WINDOW CALLS: %u; "
        "CONTROLLED PATH MATCHES: %u; CONTROLLED REQUEST: %s; 0x005B3170: %s; "
        "0x005B1BA0: %s (inferred from verified success path, not separately hooked); "
        "RESULT: %u; GLOBAL BUFFER: 0x%08X SIZE: %u; PARSER +0x498: 0x%08X; BLP2 MAGIC: %s; "
        "ENCODING: %u; DIMENSIONS: %ux%u; THREAD: %u; RUNTIME REPLACEMENT: DISABLED",
        g_enabled.load() ? "ENABLED" : "DISABLED", g_armed.load() ? "ARMED" : "OFF",
        g_totalCalls.load(), g_armedCalls.load(), g_pathMatches.load(),
        g_seen.load() ? "YES" : "NO", g_seen.load() ? "HIT" : "NO HIT",
        g_seen.load() && g_result.load() ? "REACHED" : "UNKNOWN",
        g_result.load(), unsigned(g_source.load()), g_size.load(), unsigned(g_retained.load()), g_magic.load() ? "YES" : "NO",
        g_encoding.load(), g_width.load(), g_height.load(), g_thread.load());
}
}
