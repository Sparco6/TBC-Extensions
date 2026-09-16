#pragma once
#include <cstddef>

namespace NativeBLPCorrelation {
enum class Test { DXT5, BGRA8, BGRA8Prototype64, BGRA8Prototype256Mips, BGRA8Prototype128x64Mips };
bool CreateHooks();
void SetEnabled(bool enabled);
bool Arm(Test test);
void Describe(char* output, size_t capacity);
}
