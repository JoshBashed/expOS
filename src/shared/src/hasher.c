#include "shared.h"

shared_Hasher shared_Hasher_init(void) {
    shared_Hasher hasher = {0};
    hasher.state = HASH_FNV_OFFSET;
    return hasher;
}

void shared_Hasher_update(shared_Hasher *hasher, const void *data, usize length) {
    const u8 *bytes = (const u8 *)data;
    for (usize i = 0; i < length; i++) {
        hasher->state ^= (shared_HashValue)bytes[i];
        hasher->state *= HASH_FNV_PRIME;
    }
}

shared_HashValue shared_Hasher_finalize(shared_Hasher *hasher) { return hasher->state; }

void shared_u64_hash(u64 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_i64_hash(i64 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_u32_hash(u32 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_i32_hash(i32 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_u16_hash(u16 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_i16_hash(i16 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_u8_hash(u8 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_i8_hash(i8 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_f32_hash(f32 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_f64_hash(f64 value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_usize_hash(usize value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_isize_hash(isize value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}

void shared_bool_hash(bool value, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, &value, sizeof(value));
}
