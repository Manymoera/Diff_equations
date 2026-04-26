#include <cstdint>
#include <cmath>

struct Xorshift64 {
    uint64_t state;

    // Инициализация через random_device для случайного зерна
    Xorshift64(uint64_t seed) : state(seed == 0 ? 88172645463325252ULL : seed) {}

    uint64_t next() {
        state ^= state << 13;
        state ^= state >> 7;
        state ^= state << 17;
        return state;
    }

    // Возвращает число в диапазоне [0, 1)
    double next_double() {
        return (next() >> 11) * (1.0 / 9007199254740992.0);
    }
};