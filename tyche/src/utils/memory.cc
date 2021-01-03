#include "utils/memory.h"
namespace tyche::utils {
    shared_array<char> from_string(const std::string& str) {
        return make_shared(str.data(), str.length());
    }
    shared_array<char> from_string_with_end(const std::string& str) {
        return make_shared(str.data(), str.length() + 1);
    }
}  // namespace tyche::utils