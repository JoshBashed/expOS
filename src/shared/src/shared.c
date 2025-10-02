#include "shared.h"
#include <string.h>

shared_StaticString shared_StaticString_from_cstr(const char *cstr) {
    if (!cstr)
        return STATIC_STRING_EMPTY;

    usize len = 0;
    while (cstr[len])
        len++;

    return (shared_StaticString){len, (u8 *)cstr};
}

void shared_StaticString_hash(shared_StaticString s, shared_Hasher *hasher) {
    shared_Hasher_update(hasher, s.str, s.length);
}

bool shared_StaticString_eq(shared_StaticString a, shared_StaticString b) {
    if (a.length != b.length)
        return false;
    for (usize i = 0; i < a.length; i++) {
        if (a.str[i] != b.str[i])
            return false;
    }
    return true;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--)
        *d++ = *s++;
    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = (const unsigned char *)src;
    if (d < s) {
        while (n--)
            *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--)
            *--d = *--s;
    }
    return dest;
}
