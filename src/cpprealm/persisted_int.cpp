#include <cpprealm/persisted_int.hpp>

namespace realm {
    int64_t persisted<int64_t>::serialize(int64_t v) {
        return v;
    }

    int64_t persisted<int64_t>::deserialize(int64_t v) {
        return v;
    }
}