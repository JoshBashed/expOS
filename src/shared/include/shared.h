#pragma once

#include <stdint.h>

// Forward declarations
struct _shared_Hasher;
typedef struct _shared_Hasher shared_Hasher;

// Types
typedef uint64_t u64;
void shared_u64_hash(u64 value, shared_Hasher *hasher);

typedef int64_t i64;
void shared_i64_hash(i64 value, shared_Hasher *hasher);

typedef uint32_t u32;
void shared_u32_hash(u32 value, shared_Hasher *hasher);

typedef int32_t i32;
void shared_i32_hash(i32 value, shared_Hasher *hasher);

typedef uint16_t u16;
void shared_u16_hash(u16 value, shared_Hasher *hasher);

typedef int16_t i16;
void shared_i16_hash(i16 value, shared_Hasher *hasher);

typedef uint8_t u8;
void shared_u8_hash(u8 value, shared_Hasher *hasher);

typedef int8_t i8;
void shared_i8_hash(i8 value, shared_Hasher *hasher);

typedef float f32;
void shared_f32_hash(f32 value, shared_Hasher *hasher);

typedef double f64;
void shared_f64_hash(f64 value, shared_Hasher *hasher);

typedef uintptr_t usize;
void shared_usize_hash(usize value, shared_Hasher *hasher);

typedef intptr_t isize;
void shared_isize_hash(isize value, shared_Hasher *hasher);

typedef u8 bool;
void shared_bool_hash(bool value, shared_Hasher *hasher);

// Byte order conversion
// #define bswap16(x) __builtin_bswap16(x)
// #define bswap32(x) __builtin_bswap32(x)
// #define bswap64(x) __builtin_bswap64(x)
static inline u16 bswap16(u16 x) { return __builtin_bswap16(x); }
static inline u32 bswap32(u32 x) { return __builtin_bswap32(x); }
static inline u64 bswap64(u64 x) { return __builtin_bswap64(x); }

// Constants
#define null ((void *)0)
#define true 1
#define false 0

// Hasher
#define HASH_SIZE 64
#if HASH_SIZE == 32
#define HASH_FNV_PRIME 16777619U
#define HASH_FNV_OFFSET 2166136261U
typedef u32 shared_HashValue;
#elif HASH_SIZE == 64
#define HASH_FNV_PRIME 1099511628211ULL
#define HASH_FNV_OFFSET 14695981039346656037ULL
typedef u64 shared_HashValue;
#else
#error "Unsupported HASH_SIZE"
#endif
typedef struct _shared_Hasher {
    shared_HashValue state;
} shared_Hasher;
shared_Hasher shared_Hasher_init(void);
void shared_Hasher_update(shared_Hasher *hasher, const void *data, usize length);
shared_HashValue shared_Hasher_finalize(shared_Hasher *hasher);

// Other

typedef struct _shared_Buffer {
    u64 size;
    void *ptr;
} shared_Buffer;

// String
typedef struct _shared_StaticString {
    u64 length;
    u8 *str;
} shared_StaticString;

shared_StaticString shared_StaticString_from_cstr(const char *cstr);
void shared_StaticString_hash(shared_StaticString s, shared_Hasher *hasher);
bool shared_StaticString_eq(shared_StaticString a, shared_StaticString b);

#define STATIC_STRING(str) ((shared_StaticString){sizeof(str) - 1, (u8 *)(str)})
#define STATIC_STRING_EMPTY ((shared_StaticString){.length = 0, .str = null})
