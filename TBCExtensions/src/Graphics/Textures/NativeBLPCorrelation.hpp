#pragma once
#include <cstddef>

namespace NativeBLPCorrelation {
enum class Test { DXT5, BGRA8, BGRA8Prototype };
bool CreateHooks();
void SetEnabled(bool enabled);
bool Arm(Test test);
void Describe(char* output, size_t capacity);
}
