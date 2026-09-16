#include "NativeBLPCorrelation.hpp"
#include <Windows.h>
#include <MinHook.h>
#include <atomic>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace NativeBLPCorrelation {
namespace {
constexpr uintptr_t kRequest = 0x00457FC0, kCaller = 0x00455EF0, kParser = 0x005B1BA0;
constexpr uintptr_t kTextureClassifier = 0x00454F40;
constexpr uintptr_t kDescriptorConstructor = 0x0059A860, kBackendConsumer = 0x005AAF40;
constexpr uintptr_t kTextureCreate = 0x005AABD0, kTextureUpload = 0x005AACF0, kCopyDispatcher = 0x005B1A60;
constexpr uintptr_t kFormatTable = 0x008C0BB4;
using Request = void* (__cdecl*)(const char*, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
using Caller = int (__fastcall*)(void*, const void*);
using Parser = int (__thiscall*)(void*, const void*);
using TextureClassifier = void* (__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t,
    void*, uintptr_t, uint32_t);
using DescriptorConstructor = int (__thiscall*)(void*, uintptr_t, uintptr_t, uintptr_t, uintptr_t,
    uintptr_t, uintptr_t, uintptr_t, void*, uintptr_t, void**);
using BackendConsumer = uintptr_t (__thiscall*)(void*, void*);
using TextureCreate = uintptr_t (__thiscall*)(void*, void*);
using TextureUpload = uintptr_t (__thiscall*)(void*, void*);
using CopyDispatcher = uintptr_t (__cdecl*)(const void*, uintptr_t, const void*, uint32_t,
    uint32_t, void*, uint32_t, uint32_t);
Request g_request = nullptr;
Caller g_caller = nullptr;
Parser g_parser = nullptr;
TextureClassifier g_textureClassifier = nullptr;
DescriptorConstructor g_descriptorConstructor = nullptr;
BackendConsumer g_backendConsumer = nullptr;
TextureCreate g_textureCreate = nullptr;
TextureUpload g_textureUpload = nullptr;
CopyDispatcher g_copyDispatcher = nullptr;
std::atomic<bool> g_created{false}, g_enabled{false}, g_armed{false};
std::atomic<unsigned> g_generation{0}, g_test{0};
thread_local unsigned t_generation = 0;
thread_local const void* t_candidateSource = nullptr;
thread_local const void* t_activeCallerResource = nullptr;
thread_local bool t_inTextureClassifier = false, t_inDescriptorConstructor = false, t_inBackendConsumer = false;
struct UploadScope { unsigned generation; const void* descriptor; const void* resource; unsigned mip; };
thread_local UploadScope t_uploadScope{};
constexpr size_t kResourceNameCapacity = 0x104; // 0x004577FA writes this inline field at resource +0x0C.
struct CompatibilityAsset {
    const char* normalizedPath;
    uint32_t width;
    uint32_t height;
    uint32_t mipCount;
};
// Developer-only allowlist. Adding an entry does not make it runtime-compatible; it must also pass every metadata gate.
constexpr CompatibilityAsset kCompatibilityAllowlist[] = {
    {"Interface\\MasterWoW\\BLP2Test.blp", 64, 64, 1},
    {"Interface\\MasterWoW\\TBCExt_BGRA8_256_Mips.blp", 256, 256, 9},
    {"Interface\\MasterWoW\\TBCExt_BGRA8_128x64_Mips.blp", 128, 64, 8}
};
std::atomic<unsigned char> g_resourceName[kResourceNameCapacity]{};
std::atomic<bool> g_resourceNameValid{false}, g_resourceNameMatches{false};
std::atomic<unsigned> g_pathAssociatedCalls{0}, g_pathAssociatedThread{0};
std::atomic<unsigned> g_alphaDepth{0}, g_alphaEncoding{0}, g_formatA{0}, g_formatB{0};
std::atomic<unsigned> g_resourceWidth{0}, g_resourceHeight{0};
std::atomic<bool> g_formatObserved{false};
struct Point {
    std::atomic<unsigned> total{0}, armed{0}, matched{0}, result{0}, thread{0};
    std::atomic<uintptr_t> object{0};
};
Point g_req, g_call, g_parse;
std::atomic<uintptr_t> g_source{0};
std::atomic<unsigned> g_encoding{0}, g_width{0}, g_height{0};
std::atomic<unsigned> g_headerMipCount{0};
std::atomic<bool> g_candidateLayoutValid{false};
std::atomic<bool> g_headerSeen{false};
std::atomic<bool> g_gpuAmbiguous{false}, g_constructorSeen{false}, g_backendSeen{false};
std::atomic<unsigned> g_gpuGeneration{0}, g_constructorThread{0}, g_backendThread{0};
std::atomic<uintptr_t> g_gpuResource{0}, g_gpuDescriptor{0}, g_backendContext{0}, g_descriptorResource{0};
std::atomic<bool> g_createSeen{false}, g_uploadSeen{false}, g_copySeen{false};
std::atomic<unsigned> g_createThread{0}, g_uploadThread{0}, g_copyThread{0};
std::atomic<unsigned> g_createInitialSelector{0}, g_createAcceptedSelector{0}, g_createFormat{0};
std::atomic<uintptr_t> g_gpuTexture{0};
std::atomic<unsigned> g_copyMip{0}, g_copyWidth{0}, g_copyHeight{0};
std::atomic<unsigned> g_copySourcePitch{0}, g_copyDestinationPitch{0};
std::atomic<unsigned> g_copyClass2{0}, g_copyClass1{0}, g_copyRows{0}, g_copyBytes{0};
std::atomic<uintptr_t> g_copySource{0}, g_copyDestination{0};
constexpr unsigned kMaxObservedMips = 16;
std::atomic<unsigned> g_observedMipCount{0};
std::array<std::atomic<unsigned>, kMaxObservedMips> g_mipWidths{}, g_mipHeights{},
    g_mipSourcePitches{}, g_mipDestinationPitches{}, g_mipClasses1{}, g_mipClasses2{},
    g_mipRows{}, g_mipBytes{};
std::array<std::atomic<uintptr_t>, kMaxObservedMips> g_mipSources{}, g_mipDestinations{};
std::array<std::atomic<uintptr_t>, kMaxObservedMips> g_mipResources{}, g_mipDescriptors{};
std::array<std::atomic<unsigned>, kMaxObservedMips> g_mipGenerations{}, g_mipThreads{};
std::atomic<unsigned> g_expectedWidth{0}, g_expectedHeight{0}, g_expectedMipCount{0};
std::atomic<uintptr_t> g_previousResource{0}, g_previousDescriptor{0};
std::atomic<unsigned> g_previousTest{0};
enum class PrototypeState : unsigned { NotArmed, Armed, Applied, Rejected };
enum class PrototypeReject : unsigned {
    None, GenerationMismatch, Ambiguous, ResourceMismatch, PathMismatch,
    ParserOrCorrelationNotReady, ThreadMismatch, HeaderOrDimensionsMismatch,
    MemoryInvalid, SelectorMismatch, WriteFailed, ArgumentMismatch,
    ClassificationReentry, MipMetadataInvalid
};
std::atomic<unsigned> g_prototypeState{unsigned(PrototypeState::NotArmed)};
std::atomic<unsigned> g_prototypeGeneration{0}, g_prototypeThread{0};
std::atomic<uintptr_t> g_prototypeResource{0};
std::atomic<unsigned> g_prototypeOriginal1{0}, g_prototypeOriginal2{0};
std::atomic<unsigned> g_prototypeCompat1{0}, g_prototypeCompat2{0};
std::atomic<unsigned> g_prototypeRejectReason{0};
std::atomic<unsigned> g_prototypeGateRequestMatches{0}, g_prototypeGateCallerMatches{0};
std::atomic<unsigned> g_prototypeGateParserMatches{0}, g_prototypeGatePathCandidates{0};
std::atomic<unsigned> g_prototypeGateCallerResult{0}, g_prototypeGateParserResult{0};
std::atomic<bool> g_classifierSeen{false};
std::atomic<unsigned> g_classifierThread{0};
std::atomic<uintptr_t> g_classifierResource{0};
std::atomic<unsigned> g_classifierIncoming1{0}, g_classifierIncoming2{0};
std::atomic<unsigned> g_classifierForwarded1{0}, g_classifierForwarded2{0};

bool Readable(const void* pointer, size_t length) {
    if (!pointer || !length) return false;
    uintptr_t at = reinterpret_cast<uintptr_t>(pointer);
    if (at > UINTPTR_MAX - length) return false;
    const uintptr_t end = at + length;
    while (at < end) {
        MEMORY_BASIC_INFORMATION mbi{};
        if (VirtualQuery(reinterpret_cast<const void*>(at), &mbi, sizeof(mbi)) != sizeof(mbi) ||
            mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
            !(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return false;
        const uintptr_t regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        if (regionEnd <= at) return false;
        at = regionEnd;
    }
    return true;
}
bool Writable(void* pointer, size_t length) {
    if (!pointer || !length) return false;
    uintptr_t at = reinterpret_cast<uintptr_t>(pointer);
    if (at > UINTPTR_MAX - length) return false;
    const uintptr_t end = at + length;
    while (at < end) {
        MEMORY_BASIC_INFORMATION mbi{};
        if (VirtualQuery(reinterpret_cast<const void*>(at), &mbi, sizeof(mbi)) != sizeof(mbi) ||
            mbi.State != MEM_COMMIT || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
            !(mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY |
                PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) return false;
        const uintptr_t regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        if (regionEnd <= at) return false;
        at = regionEnd;
    }
    return true;
}
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
bool Signature(uintptr_t address, const unsigned char* bytes, size_t length) {
    if (!InText(address, length) || !Readable(reinterpret_cast<const void*>(address), length)) return false;
    return memcmp(reinterpret_cast<const void*>(address), bytes, length) == 0;
}
bool Match(const char* path, unsigned test) {
    if (!path) return false;
    char local[260]{};
    if (!Readable(path, 1)) return false;
    __try {
        for (size_t i = 0; i < sizeof(local); ++i) {
            if (!Readable(path + i, 1)) return false;
            const char c = path[i];
            local[i] = c == '/' ? '\\' : c;
            if (!c) break;
            if (i + 1 == sizeof(local)) return false;
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
    if (test == 3) {
        for (const auto& asset : kCompatibilityAllowlist) {
            if (_stricmp(local, asset.normalizedPath) == 0) return true;
            const size_t length = strlen(asset.normalizedPath);
            if (length > 4 && _stricmp(asset.normalizedPath + length - 4, ".blp") == 0 &&
                strlen(local) == length - 4 && _strnicmp(local, asset.normalizedPath, length - 4) == 0) return true;
        }
        return false;
    }
    const char* wanted = test == 1 ? "Interface\\MasterWoW\\TBCExt_BLP2Test_OfflineDXT5" :
        "Interface\\MasterWoW\\BLP2Test";
    if (_stricmp(local, wanted) == 0) return true;
    char withExtension[260]{};
    snprintf(withExtension, sizeof(withExtension), "%s.blp", wanted);
    return _stricmp(local, withExtension) == 0;
}
const CompatibilityAsset* FindCompatibilityAsset(const char* normalizedPath) {
    if (!normalizedPath) return nullptr;
    for (const auto& asset : kCompatibilityAllowlist)
        if (_stricmp(normalizedPath, asset.normalizedPath) == 0) return &asset;
    return nullptr;
}
const char* Wanted(unsigned test) {
    return test == 1 ? "Interface\\MasterWoW\\TBCExt_BLP2Test_OfflineDXT5" :
        (test == 2 || test == 3) ? "Interface\\MasterWoW\\BLP2Test" : "";
}
const char* PrototypeRejectName(unsigned reason) {
    switch (PrototypeReject(reason)) {
    case PrototypeReject::None: return "NONE";
    case PrototypeReject::GenerationMismatch: return "GENERATION_MISMATCH";
    case PrototypeReject::Ambiguous: return "AMBIGUOUS";
    case PrototypeReject::ResourceMismatch: return "RESOURCE_MISMATCH";
    case PrototypeReject::PathMismatch: return "PATH_MISMATCH";
    case PrototypeReject::ParserOrCorrelationNotReady: return "PARSER_OR_CORRELATION_NOT_READY";
    case PrototypeReject::ThreadMismatch: return "THREAD_MISMATCH";
    case PrototypeReject::HeaderOrDimensionsMismatch: return "HEADER_OR_DIMENSIONS_MISMATCH";
    case PrototypeReject::MemoryInvalid: return "MEMORY_INVALID";
    case PrototypeReject::SelectorMismatch: return "SELECTOR_MISMATCH";
    case PrototypeReject::WriteFailed: return "WRITE_FAILED";
    case PrototypeReject::ArgumentMismatch: return "ARGUMENT_MISMATCH";
    case PrototypeReject::ClassificationReentry: return "CLASSIFICATION_REENTRY";
    case PrototypeReject::MipMetadataInvalid: return "MIP_METADATA_INVALID";
    default: return "UNKNOWN_REJECT";
    }
}
bool ResourceName(const void* object, char (&name)[kResourceNameCapacity]) {
    if (!Readable(object, 0x0C + kResourceNameCapacity)) return false;
    __try {
        const auto* bytes = static_cast<const unsigned char*>(object);
        const uintptr_t table = *reinterpret_cast<const uintptr_t*>(bytes);
        if (!Readable(reinterpret_cast<const void*>(table), sizeof(uintptr_t))) return false;
        const uintptr_t destructor = *reinterpret_cast<const uintptr_t*>(table);
        if (!InText(destructor, 1)) return false;
        for (size_t i = 0; i < kResourceNameCapacity; ++i) {
            const unsigned char c = bytes[0x0C + i];
            if (!c) return i != 0;
            if (c < 0x20 || c > 0x7E) return false;
            name[i] = c == '/' ? '\\' : static_cast<char>(c);
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
    return false; // No NUL inside the statically established inline field.
}
void StoreResourceName(const char (&name)[kResourceNameCapacity], unsigned test) {
    g_resourceNameValid.store(false, std::memory_order_release);
    for (size_t i = 0; i < kResourceNameCapacity; ++i)
        g_resourceName[i].store(static_cast<unsigned char>(name[i]), std::memory_order_relaxed);
    g_resourceNameMatches.store(Match(name, test), std::memory_order_relaxed);
    g_resourceNameValid.store(true, std::memory_order_release);
}
void CaptureSource(const void* source) {
    g_source.store(reinterpret_cast<uintptr_t>(source), std::memory_order_relaxed);
    if (!Readable(source, 20)) return;
    __try {
        const auto* bytes = reinterpret_cast<const unsigned char*>(source);
        if (memcmp(bytes, "BLP2", 4) != 0) return;
        g_encoding.store(bytes[8], std::memory_order_relaxed);
        g_alphaDepth.store(bytes[9], std::memory_order_relaxed);
        g_alphaEncoding.store(bytes[10], std::memory_order_relaxed);
        uint32_t width = 0, height = 0;
        memcpy(&width, bytes + 12, 4); memcpy(&height, bytes + 16, 4);
        g_width.store(width, std::memory_order_relaxed);
        g_height.store(height, std::memory_order_relaxed);
        unsigned mipCount = 0;
        bool layoutValid = Readable(source, 148);
        if (layoutValid) {
            const auto* header = reinterpret_cast<const unsigned char*>(source);
            bool ended = false;
            uint32_t mipWidth = width, mipHeight = height;
            for (unsigned i = 0; i < 16; ++i) {
                uint32_t offset = 0, byteCount = 0;
                memcpy(&offset, header + 20 + i * 4, 4);
                memcpy(&byteCount, header + 84 + i * 4, 4);
                if (!offset || !byteCount) {
                    if (offset != byteCount) layoutValid = false;
                    ended = true;
                    continue;
                }
                if (ended || uint64_t(mipWidth) * mipHeight * 4 != byteCount) layoutValid = false;
                ++mipCount;
                mipWidth = (mipWidth > 1) ? mipWidth / 2 : 1;
                mipHeight = (mipHeight > 1) ? mipHeight / 2 : 1;
            }
            if (mipCount > 1 && header[11] == 0) layoutValid = false;
        }
        g_headerMipCount.store(mipCount, std::memory_order_relaxed);
        g_candidateLayoutValid.store(layoutValid && mipCount != 0, std::memory_order_relaxed);
        g_headerSeen.store(true, std::memory_order_release);
    } __except(EXCEPTION_EXECUTE_HANDLER) { /* Observation never changes stock execution. */ }
}
void Count(Point& point) {
    point.total.fetch_add(1, std::memory_order_relaxed);
    if (g_armed.load(std::memory_order_relaxed) || t_generation) point.armed.fetch_add(1, std::memory_order_relaxed);
}
bool Correlated() { return t_generation && t_generation == g_generation.load(std::memory_order_acquire); }

void* __cdecl ObserveRequest(const char* path, uintptr_t a2, uintptr_t a3, uintptr_t a4, uintptr_t a5, uintptr_t a6) {
    Count(g_req);
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const bool matched = g_armed.load(std::memory_order_acquire) &&
        Match(path, g_test.load(std::memory_order_relaxed)) &&
        g_armed.exchange(false, std::memory_order_acq_rel);
    const unsigned previous = t_generation;
    // An unrelated nested request must not inherit its caller's controlled trace ID.
    t_generation = matched ? generation : 0;
    if (matched) {
        g_req.matched.fetch_add(1, std::memory_order_relaxed);
        g_req.thread.store(GetCurrentThreadId(), std::memory_order_relaxed);
    }
    void* result = g_request(path, a2, a3, a4, a5, a6);
    if (matched) {
        g_req.result.store(reinterpret_cast<uintptr_t>(result) != 0, std::memory_order_relaxed);
        g_req.object.store(reinterpret_cast<uintptr_t>(result), std::memory_order_relaxed);
        char name[kResourceNameCapacity]{};
        if (result && ResourceName(result, name)) StoreResourceName(name, g_test.load(std::memory_order_relaxed));
    }
    t_generation = previous;
    return result;
}
int __fastcall ObserveCaller(void* object, const void* source) {
    Count(g_call);
    char name[kResourceNameCapacity]{};
    const unsigned test = g_test.load(std::memory_order_relaxed);
    const bool pathAssociated = g_req.matched.load(std::memory_order_relaxed) &&
        test && ResourceName(object, name) && Match(name, test);
    if (pathAssociated) {
        g_pathAssociatedCalls.fetch_add(1, std::memory_order_relaxed);
        g_pathAssociatedThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
    }
    const unsigned previous = t_generation;
    const void* previousSource = t_candidateSource;
    const void* previousCallerResource = t_activeCallerResource;
    if (pathAssociated) t_generation = g_generation.load(std::memory_order_acquire);
    if (pathAssociated) t_candidateSource = source;
    if (pathAssociated) t_activeCallerResource = object;
    const bool matched = Correlated();
    if (matched) {
        g_call.matched.fetch_add(1, std::memory_order_relaxed);
        g_call.thread.store(GetCurrentThreadId(), std::memory_order_relaxed);
        g_call.object.store(reinterpret_cast<uintptr_t>(object), std::memory_order_relaxed);
        CaptureSource(source);
    }
    const int result = g_caller(object, source);
    if (matched) {
        g_call.result.store(unsigned(result), std::memory_order_relaxed);
        if (result && Readable(object, 0x150)) {
            __try {
                const auto* bytes = static_cast<const unsigned char*>(object);
                g_resourceWidth.store(*reinterpret_cast<const uint32_t*>(bytes + 0x144), std::memory_order_relaxed);
                g_resourceHeight.store(*reinterpret_cast<const uint32_t*>(bytes + 0x148), std::memory_order_relaxed);
                g_formatA.store(*reinterpret_cast<const uint32_t*>(bytes + 0x14C), std::memory_order_relaxed);
                g_formatB.store(*reinterpret_cast<const uint32_t*>(bytes + 0x118), std::memory_order_relaxed);
                g_formatObserved.store(true, std::memory_order_release);
            } __except(EXCEPTION_EXECUTE_HANDLER) { /* UNKNOWN; stock result unchanged. */ }
        }
    }
    t_generation = previous;
    t_candidateSource = previousSource;
    t_activeCallerResource = previousCallerResource;
    return result;
}
int __fastcall ObserveParser(void* object, void*, const void* source) {
    Count(g_parse);
    // 0x00455F1D preserves EDX; 0x00455F3B passes the same source to the direct parser call.
    const bool matched = Correlated() && t_candidateSource && source == t_candidateSource;
    if (matched) {
        g_parse.matched.fetch_add(1, std::memory_order_relaxed);
        g_parse.thread.store(GetCurrentThreadId(), std::memory_order_relaxed);
        g_parse.object.store(reinterpret_cast<uintptr_t>(object), std::memory_order_relaxed);
        CaptureSource(source);
    }
    const int result = g_parser(object, source);
    if (matched) g_parse.result.store(unsigned(result), std::memory_order_relaxed);
    return result;
}
bool ExpectedResourceState(const void* resource, unsigned test) {
    if (!Readable(resource, 0x150)) return false;
    __try {
        const auto* bytes = static_cast<const unsigned char*>(resource);
        const uint32_t width = *reinterpret_cast<const uint32_t*>(bytes + 0x144);
        const uint32_t height = *reinterpret_cast<const uint32_t*>(bytes + 0x148);
        const uint32_t selector1 = *reinterpret_cast<const uint32_t*>(bytes + 0x14C);
        const uint32_t selector2 = *reinterpret_cast<const uint32_t*>(bytes + 0x118);
        return width == 64 && height == 64 &&
            ((test == 1 && selector1 == 7 && selector2 == 7) ||
             (test == 2 && selector1 == 5 && selector2 == 0) ||
             (test == 3 && selector1 == 1 && selector2 == 2));
    } __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
}
void MarkGpuAmbiguous() { g_gpuAmbiguous.store(true, std::memory_order_release); }

bool ApplyControlledPrototype(void* resource, unsigned generation, uint32_t width, uint32_t height,
    uint32_t selector1Argument, uint32_t selector2Argument) {
    if (g_test.load(std::memory_order_acquire) != 3 ||
        g_prototypeState.load(std::memory_order_acquire) != unsigned(PrototypeState::Armed)) return false;
    unsigned reject = 0;
    const unsigned thread = GetCurrentThreadId();
    char name[kResourceNameCapacity]{};
    g_prototypeGateRequestMatches.store(g_req.matched.load(std::memory_order_acquire));
    g_prototypeGateCallerMatches.store(g_call.matched.load(std::memory_order_acquire));
    g_prototypeGateParserMatches.store(g_parse.matched.load(std::memory_order_acquire));
    g_prototypeGatePathCandidates.store(g_pathAssociatedCalls.load(std::memory_order_acquire));
    g_prototypeGateCallerResult.store(g_call.result.load(std::memory_order_acquire));
    g_prototypeGateParserResult.store(g_parse.result.load(std::memory_order_acquire));
    if (generation != g_generation.load(std::memory_order_acquire) ||
        generation != g_prototypeGeneration.load(std::memory_order_acquire)) reject = 1;
    else if (g_gpuAmbiguous.load(std::memory_order_acquire)) reject = 2;
    else if (!resource || reinterpret_cast<uintptr_t>(resource) != g_req.object.load(std::memory_order_acquire) ||
        reinterpret_cast<uintptr_t>(resource) != g_call.object.load(std::memory_order_acquire) ||
        resource != t_activeCallerResource) reject = 3;
    else if (!ResourceName(resource, name) || !Match(name, 3) || !FindCompatibilityAsset(name)) reject = 4;
    else if (g_req.matched.load(std::memory_order_acquire) != 1 ||
        g_call.matched.load(std::memory_order_acquire) != 1 ||
        g_parse.matched.load(std::memory_order_acquire) != 1 ||
        g_pathAssociatedCalls.load(std::memory_order_acquire) != 1 ||
        g_parse.result.load(std::memory_order_acquire) != 1) reject = 5;
    else if (g_req.thread.load(std::memory_order_acquire) != thread ||
        g_call.thread.load(std::memory_order_acquire) != thread ||
        g_parse.thread.load(std::memory_order_acquire) != thread) reject = 6;
    else if (!Readable(resource, 0x150) || !Writable(resource, 0x150)) reject = 8;
    uint32_t selector1 = 0, selector2 = 0, resourceWidth = 0, resourceHeight = 0;
    if (!reject || reject == 7) {
        __try {
            auto* bytes = static_cast<unsigned char*>(resource);
            resourceWidth = *reinterpret_cast<uint32_t*>(bytes + 0x144);
            resourceHeight = *reinterpret_cast<uint32_t*>(bytes + 0x148);
            selector1 = *reinterpret_cast<uint32_t*>(bytes + 0x14C);
            selector2 = *reinterpret_cast<uint32_t*>(bytes + 0x118);
        } __except(EXCEPTION_EXECUTE_HANDLER) { reject = 8; }
    }
    g_resourceWidth.store(resourceWidth); g_resourceHeight.store(resourceHeight);
    g_prototypeOriginal1.store(selector1); g_prototypeOriginal2.store(selector2);
    const CompatibilityAsset* asset = !reject ? FindCompatibilityAsset(name) : nullptr;
    if (!reject && (!asset || !g_headerSeen.load(std::memory_order_acquire) || g_encoding.load() != 3 ||
        g_alphaDepth.load() != 8 || g_alphaEncoding.load() != 0 ||
        !g_candidateLayoutValid.load(std::memory_order_acquire) ||
        g_headerMipCount.load(std::memory_order_acquire) != asset->mipCount ||
        g_width.load() != asset->width || g_height.load() != asset->height ||
        resourceWidth != asset->width || resourceHeight != asset->height ||
        width != asset->width || height != asset->height)) reject = 7;
    if (!reject && (selector1 != 5 || selector2 != 0)) reject = 9;
    if (!reject && (selector1Argument != 5 || selector2Argument != 0)) reject = 11;
    if (!reject) {
        uintptr_t mipTable = 0;
        __try { mipTable = *reinterpret_cast<const uintptr_t*>(static_cast<const unsigned char*>(resource) + 0x120); }
        __except(EXCEPTION_EXECUTE_HANDLER) { mipTable = 0; }
        if (!mipTable || !Readable(reinterpret_cast<const void*>(mipTable), sizeof(uintptr_t))) reject = 13;
    }
    if (reject) {
        g_prototypeRejectReason.store(reject);
        g_prototypeThread.store(thread);
        g_prototypeResource.store(reinterpret_cast<uintptr_t>(resource));
        g_prototypeState.store(unsigned(PrototypeState::Rejected), std::memory_order_release);
        return false;
    }
    bool applied = false;
    __try {
        auto* bytes = static_cast<unsigned char*>(resource);
        *reinterpret_cast<uint32_t*>(bytes + 0x118) = 2;
        *reinterpret_cast<uint32_t*>(bytes + 0x14C) = 1;
        applied = *reinterpret_cast<uint32_t*>(bytes + 0x118) == 2 &&
            *reinterpret_cast<uint32_t*>(bytes + 0x14C) == 1;
    } __except(EXCEPTION_EXECUTE_HANDLER) { applied = false; }
    g_prototypeThread.store(thread); g_prototypeResource.store(reinterpret_cast<uintptr_t>(resource));
    if (!applied) {
        g_prototypeRejectReason.store(10);
        g_prototypeState.store(unsigned(PrototypeState::Rejected), std::memory_order_release);
        MarkGpuAmbiguous();
        return false;
    }
    g_prototypeCompat1.store(1); g_prototypeCompat2.store(2);
    g_formatA.store(1); g_formatB.store(2); g_formatObserved.store(true, std::memory_order_release);
    g_prototypeState.store(unsigned(PrototypeState::Applied), std::memory_order_release);
    return true;
}

void* __cdecl ObserveTextureClassifier(uint32_t width, uint32_t height, uint32_t selector1,
    uint32_t flags, void* resource, uintptr_t callback, uint32_t rawSelector2) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const bool exactControlledScope = g_test.load(std::memory_order_acquire) == 3 &&
        g_prototypeState.load(std::memory_order_acquire) == unsigned(PrototypeState::Armed) &&
        Correlated() && resource && resource == t_activeCallerResource &&
        reinterpret_cast<uintptr_t>(resource) == g_req.object.load(std::memory_order_acquire) &&
        reinterpret_cast<uintptr_t>(resource) == g_call.object.load(std::memory_order_acquire);
    if (!exactControlledScope)
        return g_textureClassifier(width, height, selector1, flags, resource, callback, rawSelector2);
    if (t_inTextureClassifier) {
        g_prototypeRejectReason.store(unsigned(PrototypeReject::ClassificationReentry));
        g_prototypeThread.store(GetCurrentThreadId());
        g_prototypeResource.store(reinterpret_cast<uintptr_t>(resource));
        g_prototypeState.store(unsigned(PrototypeState::Rejected), std::memory_order_release);
        MarkGpuAmbiguous();
        return g_textureClassifier(width, height, selector1, flags, resource, callback, rawSelector2);
    }
    t_inTextureClassifier = true;
    g_classifierSeen.store(true, std::memory_order_release);
    g_classifierThread.store(GetCurrentThreadId());
    g_classifierResource.store(reinterpret_cast<uintptr_t>(resource));
    g_classifierIncoming1.store(selector1); g_classifierIncoming2.store(rawSelector2);
    const bool applied = ApplyControlledPrototype(resource, generation, width, height, selector1, rawSelector2);
    const uint32_t forwarded1 = applied ? 1u : selector1;
    const uint32_t forwarded2 = applied ? 2u : rawSelector2;
    g_classifierForwarded1.store(forwarded1); g_classifierForwarded2.store(forwarded2);
    void* result = g_textureClassifier(width, height, forwarded1, flags, resource, callback, forwarded2);
    t_inTextureClassifier = false;
    return result;
}

int __fastcall ObserveDescriptorConstructor(void* context, void*, uintptr_t a1, uintptr_t a2,
    uintptr_t a3, uintptr_t a4, uintptr_t a5, uintptr_t a6, uintptr_t a7, void* resource,
    uintptr_t callback, void** output) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const unsigned test = g_test.load(std::memory_order_relaxed);
    char name[kResourceNameCapacity]{};
    const bool exactRequestResource = resource &&
        reinterpret_cast<uintptr_t>(resource) == g_req.object.load(std::memory_order_acquire);
    const bool controlledScope = !g_gpuAmbiguous.load(std::memory_order_acquire) && test &&
        g_req.matched.load(std::memory_order_acquire) && (Correlated() || exactRequestResource);
    bool candidate = controlledScope && ResourceName(resource, name) && Match(name, test);
    if (controlledScope && test == 3)
        candidate = g_prototypeState.load(std::memory_order_acquire) == unsigned(PrototypeState::Applied);
    candidate = candidate && ExpectedResourceState(resource, test);
    if (candidate && t_inDescriptorConstructor) MarkGpuAmbiguous();
    const bool observe = candidate && !g_gpuAmbiguous.load(std::memory_order_acquire);
    if (observe) t_inDescriptorConstructor = true;
    const int result = g_descriptorConstructor(context, a1, a2, a3, a4, a5, a6, a7,
        resource, callback, output);
    if (observe) {
        void* descriptor = nullptr;
        bool valid = generation == g_generation.load(std::memory_order_acquire) &&
            Readable(output, sizeof(*output));
        if (valid) {
            __try { descriptor = *output; } __except(EXCEPTION_EXECUTE_HANDLER) { valid = false; }
        }
        if (valid && descriptor && Readable(descriptor, 0x50)) {
            void* storedResource = nullptr;
            __try { storedResource = *reinterpret_cast<void**>(static_cast<unsigned char*>(descriptor) + 0x30); }
            __except(EXCEPTION_EXECUTE_HANDLER) { valid = false; }
            valid = valid && storedResource == resource;
        } else valid = false;
        const uintptr_t prior = g_gpuDescriptor.load(std::memory_order_acquire);
        if ((!valid || (prior && prior != reinterpret_cast<uintptr_t>(descriptor))) &&
            generation == g_generation.load(std::memory_order_acquire)) MarkGpuAmbiguous();
        else {
            g_gpuGeneration.store(generation, std::memory_order_relaxed);
            g_gpuResource.store(reinterpret_cast<uintptr_t>(resource), std::memory_order_relaxed);
            g_gpuDescriptor.store(reinterpret_cast<uintptr_t>(descriptor), std::memory_order_relaxed);
            g_descriptorResource.store(reinterpret_cast<uintptr_t>(resource), std::memory_order_relaxed);
            g_constructorThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
            g_constructorSeen.store(true, std::memory_order_release);
        }
        t_inDescriptorConstructor = false;
    }
    return result;
}

uintptr_t __fastcall ObserveBackendConsumer(void* context, void*, void* descriptor) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const uintptr_t recordedDescriptor = g_gpuDescriptor.load(std::memory_order_acquire);
    const uintptr_t recordedResource = g_gpuResource.load(std::memory_order_acquire);
    bool matched = !g_gpuAmbiguous.load(std::memory_order_acquire) &&
        g_constructorSeen.load(std::memory_order_acquire) &&
        generation == g_gpuGeneration.load(std::memory_order_acquire) &&
        reinterpret_cast<uintptr_t>(descriptor) == recordedDescriptor && Readable(descriptor, 0x50);
    if (matched) {
        void* resource = nullptr;
        __try { resource = *reinterpret_cast<void**>(static_cast<unsigned char*>(descriptor) + 0x30); }
        __except(EXCEPTION_EXECUTE_HANDLER) { matched = false; }
        if (reinterpret_cast<uintptr_t>(resource) != recordedResource ||
            !ExpectedResourceState(resource, g_test.load(std::memory_order_relaxed))) {
            matched = false; MarkGpuAmbiguous();
        }
    }
    if (matched && t_inBackendConsumer) { MarkGpuAmbiguous(); matched = false; }
    if (matched) {
        t_inBackendConsumer = true;
        g_backendContext.store(reinterpret_cast<uintptr_t>(context), std::memory_order_relaxed);
        g_descriptorResource.store(recordedResource, std::memory_order_relaxed);
        g_backendThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
        g_backendSeen.store(true, std::memory_order_release);
    }
    const uintptr_t result = g_backendConsumer(context, descriptor);
    if (matched) {
        t_inBackendConsumer = false;
    }
    return result;
}

bool ExactDescriptor(const void* descriptor, unsigned generation) {
    if (!descriptor || g_gpuAmbiguous.load(std::memory_order_acquire) ||
        generation != g_gpuGeneration.load(std::memory_order_acquire) ||
        reinterpret_cast<uintptr_t>(descriptor) != g_gpuDescriptor.load(std::memory_order_acquire) ||
        !Readable(descriptor, 0x50)) return false;
    __try {
        return *reinterpret_cast<void* const*>(static_cast<const unsigned char*>(descriptor) + 0x30) ==
            reinterpret_cast<void*>(g_gpuResource.load(std::memory_order_acquire));
    } __except(EXCEPTION_EXECUTE_HANDLER) { return false; }
}

uintptr_t __fastcall ObserveTextureCreate(void* context, void*, void* descriptor) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const bool matched = ExactDescriptor(descriptor, generation);
    unsigned initialSelector = 0;
    if (matched) {
        __try { initialSelector = *reinterpret_cast<const uint32_t*>(static_cast<const unsigned char*>(descriptor) + 0x24); }
        __except(EXCEPTION_EXECUTE_HANDLER) { MarkGpuAmbiguous(); }
    }
    const uintptr_t result = g_textureCreate(context, descriptor);
    if (matched && ExactDescriptor(descriptor, generation)) {
        unsigned acceptedSelector = 0, format = 0;
        void* texture = nullptr;
        bool valid = true;
        __try {
            acceptedSelector = *reinterpret_cast<const uint32_t*>(static_cast<const unsigned char*>(descriptor) + 0x24);
            texture = *reinterpret_cast<void* const*>(static_cast<const unsigned char*>(descriptor) + 0x38);
            if (acceptedSelector > 8 || !Readable(reinterpret_cast<const void*>(kFormatTable + acceptedSelector * 4), 4)) valid = false;
            else format = *reinterpret_cast<const uint32_t*>(kFormatTable + acceptedSelector * 4);
        } __except(EXCEPTION_EXECUTE_HANDLER) { valid = false; }
        if (!valid || !texture) MarkGpuAmbiguous();
        else {
            g_createInitialSelector.store(initialSelector, std::memory_order_relaxed);
            g_createAcceptedSelector.store(acceptedSelector, std::memory_order_relaxed);
            g_createFormat.store(format, std::memory_order_relaxed);
            g_gpuTexture.store(reinterpret_cast<uintptr_t>(texture), std::memory_order_relaxed);
            g_createThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
            g_createSeen.store(true, std::memory_order_release);
        }
    }
    return result;
}

uintptr_t __fastcall ObserveTextureUpload(void* context, void*, void* descriptor) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const bool matched = ExactDescriptor(descriptor, generation);
    if (matched && t_uploadScope.descriptor) { MarkGpuAmbiguous(); return g_textureUpload(context, descriptor); }
    if (!matched) return g_textureUpload(context, descriptor);
    t_uploadScope = {generation, descriptor, reinterpret_cast<void*>(g_gpuResource.load(std::memory_order_acquire)), 0};
    g_uploadThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
    g_uploadSeen.store(true, std::memory_order_release);
    const uintptr_t result = g_textureUpload(context, descriptor);
    t_uploadScope = {};
    return result;
}

uintptr_t __cdecl ObserveCopyDispatcher(const void* geometry, uintptr_t mode, const void* source,
    uint32_t sourcePitch, uint32_t class2, void* destination, uint32_t destinationPitch, uint32_t class1) {
    const unsigned generation = g_generation.load(std::memory_order_acquire);
    const unsigned mip = t_uploadScope.mip++;
    const bool matched = t_uploadScope.descriptor && t_uploadScope.generation == generation &&
        ExactDescriptor(t_uploadScope.descriptor, generation) &&
        reinterpret_cast<uintptr_t>(t_uploadScope.resource) == g_gpuResource.load(std::memory_order_acquire);
    if (matched && mip < kMaxObservedMips && Readable(geometry, 8)) {
        uint32_t width = 0, height = 0;
        bool valid = true;
        __try {
            width = *reinterpret_cast<const uint32_t*>(geometry);
            height = *reinterpret_cast<const uint32_t*>(static_cast<const unsigned char*>(geometry) + 4);
        } __except(EXCEPTION_EXECUTE_HANDLER) { valid = false; }
        uint64_t rows = 0, bytes = 0;
        if (valid && class1 == 1 && class2 == 1) { rows = height; bytes = uint64_t(width) * 4u * rows; }
        else if (valid && class1 == class2 && (class1 == 5 || class1 == 7)) {
            rows = height < 4 ? 1 : height / 4;
            bytes = uint64_t(sourcePitch) * rows;
        } else valid = false;
        if (!valid || rows > UINT32_MAX || bytes > UINT32_MAX) MarkGpuAmbiguous();
        else {
            g_mipWidths[mip].store(width); g_mipHeights[mip].store(height);
            g_mipSources[mip].store(reinterpret_cast<uintptr_t>(source));
            g_mipDestinations[mip].store(reinterpret_cast<uintptr_t>(destination));
            g_mipSourcePitches[mip].store(sourcePitch); g_mipDestinationPitches[mip].store(destinationPitch);
            g_mipClasses1[mip].store(class1); g_mipClasses2[mip].store(class2);
            g_mipRows[mip].store(static_cast<unsigned>(rows)); g_mipBytes[mip].store(static_cast<unsigned>(bytes));
            if (g_observedMipCount.load(std::memory_order_relaxed) < mip + 1)
                g_observedMipCount.store(mip + 1, std::memory_order_release);
            if (mip == 0) {
                g_copyMip.store(mip, std::memory_order_relaxed);
                g_copyWidth.store(width, std::memory_order_relaxed); g_copyHeight.store(height, std::memory_order_relaxed);
                g_copySource.store(reinterpret_cast<uintptr_t>(source), std::memory_order_relaxed);
                g_copyDestination.store(reinterpret_cast<uintptr_t>(destination), std::memory_order_relaxed);
                g_copySourcePitch.store(sourcePitch, std::memory_order_relaxed);
                g_copyDestinationPitch.store(destinationPitch, std::memory_order_relaxed);
                g_copyClass2.store(class2, std::memory_order_relaxed); g_copyClass1.store(class1, std::memory_order_relaxed);
                g_copyRows.store(static_cast<unsigned>(rows), std::memory_order_relaxed);
                g_copyBytes.store(static_cast<unsigned>(bytes), std::memory_order_relaxed);
            }
            g_copyThread.store(GetCurrentThreadId(), std::memory_order_relaxed);
            g_copySeen.store(true, std::memory_order_release);
        }
    }
    return g_copyDispatcher(geometry, mode, source, sourcePitch, class2, destination, destinationPitch, class1);
}
void Reset(Point& point) {
    point.armed.store(0); point.matched.store(0); point.result.store(0);
    point.thread.store(0); point.object.store(0);
}
}

bool CreateHooks() {
    static const unsigned char request[] = {0x55,0x8B,0xEC,0x81,0xEC,0x18,0x01,0x00,0x00};
    static const unsigned char caller[] = {0x55,0x8B,0xEC,0x81,0xEC,0xD4,0x04,0x00,0x00};
    static const unsigned char parser[] = {0x55,0x8B,0xEC,0x53,0x56,0x57,0x8B,0xD9};
    static const unsigned char classifier[] = {0x55,0x8B,0xEC,0x83,0xEC,0x4C,0x56,0x57};
    static const unsigned char constructor[] = {0x55,0x8B,0xEC,0x56,0x6A,0x00,0x68,0x84,0x07,0x00,0x00};
    static const unsigned char backend[] = {0x55,0x8B,0xEC,0x57,0x8B,0xF9,0x83,0xBF,0xE4,0x0E,0x00,0x00,0x00};
    static const unsigned char creation[] = {0x55,0x8B,0xEC,0x83,0xEC,0x18,0x53,0x56,0x8B,0x75,0x08,0x57};
    static const unsigned char upload[] = {0x55,0x8B,0xEC,0x83,0xEC,0x44,0x53,0x56,0x8B,0x75,0x08,0x8B,0x46,0x38};
    static const unsigned char copy[] = {0x55,0x8B,0xEC,0x83,0x3D,0xCC,0xAE,0xD2,0x00,0x00,0x75,0x0F};
    if (!Signature(kRequest, request, sizeof(request)) || !Signature(kCaller, caller, sizeof(caller)) ||
        !Signature(kParser, parser, sizeof(parser)) ||
        !Signature(kTextureClassifier, classifier, sizeof(classifier)) ||
        !Signature(kDescriptorConstructor, constructor, sizeof(constructor)) ||
        !Signature(kBackendConsumer, backend, sizeof(backend)) ||
        !Signature(kTextureCreate, creation, sizeof(creation)) ||
        !Signature(kTextureUpload, upload, sizeof(upload)) ||
        !Signature(kCopyDispatcher, copy, sizeof(copy))) return false;
    if (MH_CreateHook(reinterpret_cast<void*>(kRequest), reinterpret_cast<void*>(&ObserveRequest),
        reinterpret_cast<void**>(&g_request)) != MH_OK) return false;
    if (MH_CreateHook(reinterpret_cast<void*>(kCaller), reinterpret_cast<void*>(&ObserveCaller),
        reinterpret_cast<void**>(&g_caller)) != MH_OK) { MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false; }
    if (MH_CreateHook(reinterpret_cast<void*>(kParser), reinterpret_cast<void*>(&ObserveParser),
        reinterpret_cast<void**>(&g_parser)) != MH_OK) {
        MH_RemoveHook(reinterpret_cast<void*>(kCaller)); MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false;
    }
    if (MH_CreateHook(reinterpret_cast<void*>(kTextureClassifier),
        reinterpret_cast<void*>(&ObserveTextureClassifier),
        reinterpret_cast<void**>(&g_textureClassifier)) != MH_OK) {
        MH_RemoveHook(reinterpret_cast<void*>(kParser)); MH_RemoveHook(reinterpret_cast<void*>(kCaller));
        MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false;
    }
    if (MH_CreateHook(reinterpret_cast<void*>(kDescriptorConstructor),
        reinterpret_cast<void*>(&ObserveDescriptorConstructor),
        reinterpret_cast<void**>(&g_descriptorConstructor)) != MH_OK) {
        MH_RemoveHook(reinterpret_cast<void*>(kTextureClassifier)); MH_RemoveHook(reinterpret_cast<void*>(kParser)); MH_RemoveHook(reinterpret_cast<void*>(kCaller));
        MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false;
    }
    if (MH_CreateHook(reinterpret_cast<void*>(kBackendConsumer), reinterpret_cast<void*>(&ObserveBackendConsumer),
        reinterpret_cast<void**>(&g_backendConsumer)) != MH_OK) {
        MH_RemoveHook(reinterpret_cast<void*>(kDescriptorConstructor)); MH_RemoveHook(reinterpret_cast<void*>(kTextureClassifier)); MH_RemoveHook(reinterpret_cast<void*>(kParser));
        MH_RemoveHook(reinterpret_cast<void*>(kCaller)); MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false;
    }
    if (MH_CreateHook(reinterpret_cast<void*>(kTextureCreate), reinterpret_cast<void*>(&ObserveTextureCreate),
        reinterpret_cast<void**>(&g_textureCreate)) != MH_OK ||
        MH_CreateHook(reinterpret_cast<void*>(kTextureUpload), reinterpret_cast<void*>(&ObserveTextureUpload),
        reinterpret_cast<void**>(&g_textureUpload)) != MH_OK ||
        MH_CreateHook(reinterpret_cast<void*>(kCopyDispatcher), reinterpret_cast<void*>(&ObserveCopyDispatcher),
        reinterpret_cast<void**>(&g_copyDispatcher)) != MH_OK) {
        MH_RemoveHook(reinterpret_cast<void*>(kCopyDispatcher)); MH_RemoveHook(reinterpret_cast<void*>(kTextureUpload));
        MH_RemoveHook(reinterpret_cast<void*>(kTextureCreate)); MH_RemoveHook(reinterpret_cast<void*>(kBackendConsumer));
        MH_RemoveHook(reinterpret_cast<void*>(kDescriptorConstructor)); MH_RemoveHook(reinterpret_cast<void*>(kTextureClassifier)); MH_RemoveHook(reinterpret_cast<void*>(kParser));
        MH_RemoveHook(reinterpret_cast<void*>(kCaller)); MH_RemoveHook(reinterpret_cast<void*>(kRequest)); return false;
    }
    g_created.store(true, std::memory_order_release);
    return true;
}
void SetEnabled(bool enabled) { g_enabled.store(enabled && g_created.load(), std::memory_order_release); }
bool Arm(Test test) {
    if (!g_enabled.load(std::memory_order_acquire) || !g_request || !g_caller || !g_parser ||
        !g_textureClassifier || !g_descriptorConstructor || !g_backendConsumer || !g_textureCreate ||
        !g_textureUpload || !g_copyDispatcher) return false;
    g_armed.store(false, std::memory_order_release);
    Reset(g_req); Reset(g_call); Reset(g_parse);
    g_resourceNameValid.store(false); g_resourceNameMatches.store(false);
    g_pathAssociatedCalls.store(0); g_pathAssociatedThread.store(0);
    g_alphaDepth.store(0); g_alphaEncoding.store(0); g_formatA.store(0); g_formatB.store(0);
    g_resourceWidth.store(0); g_resourceHeight.store(0);
    g_formatObserved.store(false);
    g_source.store(0); g_encoding.store(0); g_width.store(0); g_height.store(0); g_headerSeen.store(false);
    g_headerMipCount.store(0); g_candidateLayoutValid.store(false);
    g_gpuAmbiguous.store(false); g_constructorSeen.store(false); g_backendSeen.store(false);
    g_gpuGeneration.store(0); g_constructorThread.store(0); g_backendThread.store(0);
    g_gpuResource.store(0); g_gpuDescriptor.store(0); g_backendContext.store(0); g_descriptorResource.store(0);
    g_createSeen.store(false); g_uploadSeen.store(false); g_copySeen.store(false);
    g_createThread.store(0); g_uploadThread.store(0); g_copyThread.store(0);
    g_createInitialSelector.store(0); g_createAcceptedSelector.store(0); g_createFormat.store(0); g_gpuTexture.store(0);
    g_copyMip.store(0); g_copyWidth.store(0); g_copyHeight.store(0); g_copySourcePitch.store(0);
    g_copyDestinationPitch.store(0); g_copyClass2.store(0); g_copyClass1.store(0); g_copyRows.store(0);
    g_copyBytes.store(0); g_copySource.store(0); g_copyDestination.store(0);
    g_observedMipCount.store(0);
    for (unsigned i = 0; i < kMaxObservedMips; ++i) {
        g_mipWidths[i].store(0); g_mipHeights[i].store(0);
        g_mipSourcePitches[i].store(0); g_mipDestinationPitches[i].store(0);
        g_mipClasses1[i].store(0); g_mipClasses2[i].store(0);
        g_mipRows[i].store(0); g_mipBytes[i].store(0);
        g_mipSources[i].store(0); g_mipDestinations[i].store(0);
    }
    const unsigned testValue = test == Test::DXT5 ? 1u : test == Test::BGRA8 ? 2u : 3u;
    g_test.store(testValue, std::memory_order_relaxed);
    const unsigned generation = g_generation.fetch_add(1, std::memory_order_acq_rel) + 1;
    g_prototypeGeneration.store(testValue == 3 ? generation : 0);
    g_prototypeThread.store(0); g_prototypeResource.store(0);
    g_prototypeOriginal1.store(0); g_prototypeOriginal2.store(0);
    g_prototypeCompat1.store(0); g_prototypeCompat2.store(0); g_prototypeRejectReason.store(0);
    g_prototypeGateRequestMatches.store(0); g_prototypeGateCallerMatches.store(0);
    g_prototypeGateParserMatches.store(0); g_prototypeGatePathCandidates.store(0);
    g_prototypeGateCallerResult.store(0); g_prototypeGateParserResult.store(0);
    g_classifierSeen.store(false); g_classifierThread.store(0); g_classifierResource.store(0);
    g_classifierIncoming1.store(0); g_classifierIncoming2.store(0);
    g_classifierForwarded1.store(0); g_classifierForwarded2.store(0);
    g_prototypeState.store(testValue == 3 ? unsigned(PrototypeState::Armed) : unsigned(PrototypeState::NotArmed));
    g_armed.store(true, std::memory_order_release);
    return true;
}
void Describe(char* output, size_t capacity) {
    if (!output || !capacity) return;
    char resourceName[kResourceNameCapacity]{};
    const bool nameValid = g_resourceNameValid.load(std::memory_order_acquire);
    if (nameValid) for (size_t i = 0; i < kResourceNameCapacity; ++i)
        resourceName[i] = static_cast<char>(g_resourceName[i].load(std::memory_order_relaxed));
    const unsigned format = g_createFormat.load();
    const char* formatName = format == 21 ? "A8R8G8B8" : format == 0x31545844 ? "DXT1" :
        format == 0x33545844 ? "DXT3" : format == 0x35545844 ? "DXT5" : "UNKNOWN";
    const unsigned class1 = g_copyClass1.load(), class2 = g_copyClass2.load();
    const char* copyName = class1 == 1 && class2 == 1 ? "32-BIT ROW" :
        class1 == 5 && class2 == 5 ? "DXT1 BLOCK" : class1 == 7 && class2 == 7 ? "DXT5 BLOCK" : "UNKNOWN";
    snprintf(output, capacity,
        "TRACE %s | GEN %u | %s | REPLACEMENT DISABLED\n"
        "BGRA8 COMPAT PROTOTYPE: %s | generation %u | mutation thread %u\n"
        "PROTOTYPE RESOURCE: 0x%08X | original %u/%u | compat %u/%u\n"
        "PROTOTYPE REJECT: %u (%s) | gate req/caller/parser/path %u/%u/%u/%u | results caller/parser %u/%u\n"
        "CLASSIFY 0x00454F40: %s | thread %u | resource 0x%08X\n"
        "CLASSIFY ARGS: incoming %u/%u | forwarded %u/%u\n"
        "REQUEST: %s | HIT %u | total %u | thread %u\n"
        "RESOURCE: 0x%08X | name +0x0C: %s | path match %s | cache UNKNOWN\n"
        "SOURCE 0x00455EF0: %s | total %u path-candidates %u | thread %u | result %u\n"
        "PARSER 0x005B1BA0: %s | total %u | thread %u | result %u\n"
        "BLP2: %s | enc %u alpha %u/%u | %ux%u | source 0x%08X size UNKNOWN\n"
        "RESOURCE DIMENSIONS: %ux%u\n"
        "SELECTOR #1 (+0x14C): %s %u\n"
        "SELECTOR #2 (+0x118): %s %u\n"
        "GPU CORRELATION: %s | generation %u\n"
        "CONSTRUCTOR 0x0059A860: %s | thread %u\n"
        "BACKEND 0x005AAF40: %s | thread %u | context 0x%08X\n"
        "DESCRIPTOR: 0x%08X | resource +0x30 snapshot 0x%08X | match %s\n"
        "CREATE 0x005AABD0: %s | thread %u | selector %u -> accepted %u\n"
        "GPU FORMAT: %s (0x%08X) | texture 0x%08X | attempts/HRESULT UNKNOWN\n"
        "UPLOAD 0x005AACF0: %s | thread %u | COPY 0x005B1A60: %s | thread %u\n"
        "MIP %u: %ux%u | source 0x%08X pitch %u | locked bits 0x%08X pitch %u\n"
        "COPY CLASS: %u/%u %s | rows %u | bytes %u (DERIVED STATIC)\n"
        "VISUAL EXPECTATION: %s | RUNTIME TRANSCODE DISABLED\n"
        "RELATION: %s",
        g_test.load() == 1 ? "DXT5" : g_test.load() == 2 ? "BGRA8" : g_test.load() == 3 ? "BGRA8 PROTO 1/2" : "NONE",
        g_generation.load(), g_enabled.load() ? (g_armed.load() ? "ARMED" : "ENABLED") : "DISABLED",
        g_prototypeState.load() == unsigned(PrototypeState::Armed) ? "ARMED" :
            g_prototypeState.load() == unsigned(PrototypeState::Applied) ? "APPLIED" :
            g_prototypeState.load() == unsigned(PrototypeState::Rejected) ? "REJECTED" : "NOT ARMED",
        g_prototypeGeneration.load(), g_prototypeThread.load(), unsigned(g_prototypeResource.load()),
        g_prototypeOriginal1.load(), g_prototypeOriginal2.load(),
        g_prototypeCompat1.load(), g_prototypeCompat2.load(), g_prototypeRejectReason.load(),
        PrototypeRejectName(g_prototypeRejectReason.load()),
        g_prototypeGateRequestMatches.load(), g_prototypeGateCallerMatches.load(),
        g_prototypeGateParserMatches.load(), g_prototypeGatePathCandidates.load(),
        g_prototypeGateCallerResult.load(), g_prototypeGateParserResult.load(),
        g_classifierSeen.load() ? "HIT" : "NO HIT", g_classifierThread.load(),
        unsigned(g_classifierResource.load()), g_classifierIncoming1.load(), g_classifierIncoming2.load(),
        g_classifierForwarded1.load(), g_classifierForwarded2.load(),
        Wanted(g_test.load()), g_req.matched.load(), g_req.total.load(), g_req.thread.load(),
        unsigned(g_req.object.load()), nameValid ? resourceName : "UNKNOWN",
        nameValid ? (g_resourceNameMatches.load() ? "YES" : "NO") : "UNKNOWN",
        g_call.matched.load() ? "PATH CANDIDATE" : "NO MATCH", g_call.total.load(),
        g_pathAssociatedCalls.load(), g_call.thread.load(), g_call.result.load(),
        g_parse.matched.load() ? "PATH-CALL HIT" : "NO HIT", g_parse.total.load(),
        g_parse.thread.load(), g_parse.result.load(),
        g_headerSeen.load() ? "SEEN" : "UNKNOWN", g_encoding.load(),
        g_alphaDepth.load(), g_alphaEncoding.load(), g_width.load(), g_height.load(),
        unsigned(g_source.load()), g_resourceWidth.load(), g_resourceHeight.load(),
        g_formatObserved.load() ? "OBSERVED" : "UNKNOWN", g_formatA.load(),
        g_formatObserved.load() ? "OBSERVED" : "UNKNOWN", g_formatB.load(),
        g_gpuAmbiguous.load() ? "AMBIGUOUS" : g_backendSeen.load() ? "VERIFIED" : "NO HIT",
        g_gpuGeneration.load(), g_constructorSeen.load() ? "HIT" : "NO HIT", g_constructorThread.load(),
        g_backendSeen.load() ? "HIT" : "NO HIT", g_backendThread.load(), unsigned(g_backendContext.load()),
        unsigned(g_gpuDescriptor.load()), unsigned(g_descriptorResource.load()),
        g_gpuResource.load() && g_gpuResource.load() == g_descriptorResource.load() ? "YES" : "NO",
        g_createSeen.load() ? "HIT" : "NO HIT", g_createThread.load(),
        g_createInitialSelector.load(), g_createAcceptedSelector.load(), formatName, format,
        unsigned(g_gpuTexture.load()), g_uploadSeen.load() ? "HIT" : "NO HIT", g_uploadThread.load(),
        g_copySeen.load() ? "HIT" : "NO HIT", g_copyThread.load(), g_copyMip.load(),
        g_copyWidth.load(), g_copyHeight.load(), unsigned(g_copySource.load()), g_copySourcePitch.load(),
        unsigned(g_copyDestination.load()), g_copyDestinationPitch.load(), class1, class2, copyName,
        g_copyRows.load(), g_copyBytes.load(),
        g_test.load() == 3 ? "A8R8G8B8 RAW BGRA" : "STOCK CONTROL",
        g_gpuAmbiguous.load() ? "ambiguous; attribution stopped" :
            g_backendSeen.load() ? "exact generation/resource/descriptor equality" :
            "resource path candidate; backend identity not observed");
    size_t used = strnlen(output, capacity);
    if (used < capacity) {
        const unsigned observed = (std::min)(g_observedMipCount.load(std::memory_order_acquire), kMaxObservedMips);
        int wrote = snprintf(output + used, capacity - used, "\nMIP CHAIN: observed %u | header %u | raw-layout %s",
            observed, g_headerMipCount.load(), g_candidateLayoutValid.load() ? "VALID" : "UNKNOWN");
        if (wrote > 0) used += (std::min)(size_t(wrote), capacity - used - 1);
        for (unsigned i = 0; i < observed && used < capacity; ++i) {
            wrote = snprintf(output + used, capacity - used,
                "\nMIP %u DETAIL: %ux%u src 0x%08X/%u dst 0x%08X/%u class %u/%u rows %u bytes %u",
                i, g_mipWidths[i].load(), g_mipHeights[i].load(), unsigned(g_mipSources[i].load()),
                g_mipSourcePitches[i].load(), unsigned(g_mipDestinations[i].load()),
                g_mipDestinationPitches[i].load(), g_mipClasses1[i].load(), g_mipClasses2[i].load(),
                g_mipRows[i].load(), g_mipBytes[i].load());
            if (wrote <= 0) break;
            used += (std::min)(size_t(wrote), capacity - used - 1);
        }
    }
}
}
