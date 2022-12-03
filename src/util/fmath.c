#include "fmath.h"
#include "../util/util.h"

int ivec3scmp(ivec3s a, ivec3s b) {
    return memcmp(&a, &b, sizeof(ivec3s));
}

s64 ivec3shash(ivec3s v) {
    s64 h = 0;
    for(int i = 0; i < 3; i++) {
        h ^= v.raw[i] + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}
