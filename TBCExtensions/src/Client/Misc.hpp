#include <SharedDefines.hpp>

class Misc
{
public:
    static void ApplyPatches();
    static void SetYearOffsetMultiplier();

private:
    Misc() = delete;
    ~Misc() = delete;

    // defaults: 49 and 37 - increase by number of custom mods you add
    static inline uint32_t itemModTable[49]    = { 0 };
    static inline uint32_t itemModTableVal[37] = { 0 };

    static inline uint32_t yearOffsetMult = 0;
};
