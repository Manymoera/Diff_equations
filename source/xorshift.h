#include <cstdint>
#include <cmath>

struct Xorshift64 {
    uint64_t state;

    Xorshift64(uint64_t seed) : state(seed == 0 ? 88172645463325252ULL : seed) {}

    uint64_t next() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }

    double next_double() {
        return (next() >> 11) * (1.0 / 9007199254740992.0);
    }
};