#pragma once
#include <cstdint>

namespace tyche::utils {
    inline unsigned int strhash(const char* str) {
        uint32_t h = 0;
        for (auto* p = (uint8_t*)str; *p; p++) h = 31 * h + *p;
        return h;
    }
    inline unsigned int strhash(const char* str, std::size_t len) {
        uint32_t h = 0;
        for (std::size_t i = 0; i < len; i++) h = 31 * h + ((uint8_t*)str)[i];
        return h;
    }
    constexpr uint8_t int8t2uint8t(char val) {
        return val > 0 ? val : 256 + val;
    }
    constexpr uint32_t operator"" _hash(const char* str, size_t n) {
        uint32_t h = 0;
        for (int i = 0; i < n; ++i) {
            h = 31 * h + int8t2uint8t(str[i]);
        }
        return h;
    }
}  // namespace tyche::utils