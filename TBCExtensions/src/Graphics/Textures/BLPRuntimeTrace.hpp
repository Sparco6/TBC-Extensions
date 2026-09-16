#pragma once
#include <cstddef>

namespace BlpRuntimeTrace {
// Creation is called only after the existing build-8606 fingerprint and MinHook checks.
bool CreateObservationHook();
void SetEnabled(bool enabled);
bool ArmOneShot();
void Describe(char* output, size_t capacity);
}
