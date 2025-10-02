#include "logging.h"

logging_log_fn current_log_fn = null;

#define FORMAT_SPECIFIER_MAX_LEN 32
#define TYPE_SPECIFIER_MAX_LEN 16

#define LOG_UINT_FN_FOR(identifier, type, buf_size)                                                \
    static void identifier(type value) {                                                           \
        if (value == 0) {                                                                          \
            current_log_fn('0');                                                                   \
            return;                                                                                \
        }                                                                                          \
        u8 buffer[(buf_size)];                                                                     \
        usize pos = 0;                                                                             \
        while (value > 0 && pos < (usize)sizeof(buffer)) {                                         \
            buffer[pos++] = (u8)('0' + (value % 10));                                              \
            value /= 10;                                                                           \
        }                                                                                          \
        while (pos > 0) {                                                                          \
            current_log_fn((char)buffer[--pos]);                                                   \
        }                                                                                          \
    }

#define LOG_INT_FN_FOR(identifier, signed_type, unsigned_type, buf_size)                           \
    static void identifier(signed_type value) {                                                    \
        if (value == 0) {                                                                          \
            current_log_fn('0');                                                                   \
            return;                                                                                \
        }                                                                                          \
        unsigned_type u;                                                                           \
        if (value < 0) {                                                                           \
            current_log_fn('-');                                                                   \
            u = (unsigned_type)(-(value + (signed_type)1)) + (unsigned_type)1;                     \
        } else {                                                                                   \
            u = (unsigned_type)value;                                                              \
        }                                                                                          \
        u8 buffer[(buf_size)];                                                                     \
        usize pos = 0;                                                                             \
        while (u > 0 && pos < (usize)sizeof(buffer)) {                                             \
            buffer[pos++] = (u8)('0' + (u % 10));                                                  \
            u /= 10;                                                                               \
        }                                                                                          \
        while (pos > 0) {                                                                          \
            current_log_fn((char)buffer[--pos]);                                                   \
        }                                                                                          \
    }

#define LOG_UINT_HEX_FN_FOR(identifier, type, buf_size)                                            \
    static void identifier(type value, bool padding) {                                             \
        if (value == 0) {                                                                          \
            current_log_fn('0');                                                                   \
            return;                                                                                \
        }                                                                                          \
        u8 buffer[(buf_size)];                                                                     \
        usize pos = 0;                                                                             \
        const char *hex_chars = "0123456789ABCDEF";                                                \
        while (value > 0 && pos < (usize)sizeof(buffer)) {                                         \
            buffer[pos++] = (u8)hex_chars[value & 0xF];                                            \
            value >>= 4;                                                                           \
        }                                                                                          \
        if (padding) {                                                                             \
            while (pos < (usize)sizeof(buffer)) {                                                  \
                buffer[pos++] = '0';                                                               \
            }                                                                                      \
        }                                                                                          \
        while (pos > 0) {                                                                          \
            current_log_fn((char)buffer[--pos]);                                                   \
        }                                                                                          \
    }
#define LOG_INT_HEX_FN_FOR(identifier, signed_type, unsigned_type, buf_size)                       \
    static void identifier(signed_type value, bool padding) {                                      \
        if (value == 0) {                                                                          \
            current_log_fn('0');                                                                   \
            return;                                                                                \
        }                                                                                          \
        unsigned_type u;                                                                           \
        if (value < 0) {                                                                           \
            current_log_fn('-');                                                                   \
            u = (unsigned_type)(-(value + (signed_type)1)) + (unsigned_type)1;                     \
        } else {                                                                                   \
            u = (unsigned_type)value;                                                              \
        }                                                                                          \
        u8 buffer[(buf_size)];                                                                     \
        usize pos = 0;                                                                             \
        const char *hex_chars = "0123456789ABCDEF";                                                \
        while (u > 0 && pos < (usize)sizeof(buffer)) {                                             \
            buffer[pos++] = (u8)hex_chars[u & 0xF];                                                \
            u >>= 4;                                                                               \
        }                                                                                          \
        if (padding) {                                                                             \
            while (pos < (usize)sizeof(buffer)) {                                                  \
                buffer[pos++] = '0';                                                               \
            }                                                                                      \
        }                                                                                          \
        while (pos > 0) {                                                                          \
            current_log_fn((char)buffer[--pos]);                                                   \
        }                                                                                          \
    }
#define INT_LOG_FN(va_arg, type, options, fn, fn_hex)                                              \
    if (shared_StaticString_eq(options, STATIC_STRING("HEX"))) {                                   \
        fn_hex((type)va_arg(args, type), true);                                                    \
    } else {                                                                                       \
        fn((type)va_arg(args, type));                                                              \
    }

// usize = 64, floor(64 * log10(2)) + 1 = 20
LOG_UINT_FN_FOR(_log_usize, usize, 20)
LOG_INT_FN_FOR(_log_isize, isize, usize, 20)

// usize = 64, floor(64 * log16(2)) + 1 = 16
LOG_UINT_HEX_FN_FOR(_log_usize_hex, usize, 16)
LOG_INT_HEX_FN_FOR(_log_isize_hex, isize, usize, 16)

// u64 = 64, floor(64 * log10(2)) + 1
LOG_UINT_FN_FOR(_log_u64, u64, 20)
LOG_INT_FN_FOR(_log_i64, i64, u64, 20)

// u64 = 64, floor(64 * log16(2)) + 1
LOG_UINT_HEX_FN_FOR(_log_u64_hex, u64, 16)
LOG_INT_HEX_FN_FOR(_log_i64_hex, i64, u64, 16)

// u32 = 32, floor(32 * log10(2)) + 1
LOG_UINT_FN_FOR(_log_u32, u32, 10)
LOG_INT_FN_FOR(_log_i32, i32, u32, 10)

// u32 = 32, floor(32 * log16(2)) + 1
LOG_UINT_HEX_FN_FOR(_log_u32_hex, u32, 8)
LOG_INT_HEX_FN_FOR(_log_i32_hex, i32, u32, 8)

// u16 = 16, floor(16 * log10(2)) + 1
LOG_UINT_FN_FOR(_log_u16, u16, 5)
LOG_INT_FN_FOR(_log_i16, i16, u16, 5)

// u16 = 16, floor(16 * log16(2)) + 1
LOG_UINT_HEX_FN_FOR(_log_u16_hex, u16, 4)
LOG_INT_HEX_FN_FOR(_log_i16_hex, i16, u16, 4)

// u8 = 8, floor(8 * log10(2)) + 1
LOG_UINT_FN_FOR(_log_u8, u8, 3)
LOG_INT_FN_FOR(_log_i8, i8, u8, 3)

// u8 = 8, floor(8 * log16(2)) + 1
LOG_UINT_HEX_FN_FOR(_log_u8_hex, u8, 2)
LOG_INT_HEX_FN_FOR(_log_i8_hex, i8, u8, 2)

void _log_str(shared_StaticString s) {
    for (usize i = 0; i < s.length; i++) {
        current_log_fn(s.str[i]);
    }
}

void _log_bool(bool value) {
    if (value) {
        _log_str(STATIC_STRING("true"));
    } else {
        _log_str(STATIC_STRING("false"));
    }
}

void _log_char(char c) { current_log_fn(c); }

#define HASH_StaticString 15495999123547472670ULL
#define HASH_CString 2938415174267687691ULL
#define HASH_u8 631772817358541784ULL
#define HASH_i8 627981701265227156ULL
#define HASH_u16 5558799945458185151ULL
#define HASH_i16 3092775683940386555ULL
#define HASH_u32 5560783464435099345ULL
#define HASH_i32 3094732814638223685ULL
#define HASH_u64 5563585020063213298ULL
#define HASH_i64 3089943341986682694ULL
#define HASH_usize 15670484296631260865ULL
#define HASH_isize 15594117208746958941ULL
#define HASH_bool 14785269867199075517ULL
#define HASH_char 17483980429552467645ULL

#define SPECIAL_TYPE_LIST(X)                                                                       \
    X(HASH_StaticString, { _log_str(va_arg(args, shared_StaticString)); })                         \
    X(HASH_CString, { _log_str(shared_StaticString_from_cstr(va_arg(args, const char *))); })      \
    X(HASH_u8, {INT_LOG_FN(va_arg, u8, format, _log_u8, _log_u8_hex)})                             \
    X(HASH_i8, {INT_LOG_FN(va_arg, i8, format, _log_i8, _log_i8_hex)})                             \
    X(HASH_u16, {INT_LOG_FN(va_arg, u16, format, _log_u16, _log_u16_hex)})                         \
    X(HASH_i16, {INT_LOG_FN(va_arg, i16, format, _log_i16, _log_i16_hex)})                         \
    X(HASH_u32, {INT_LOG_FN(va_arg, u32, format, _log_u32, _log_u32_hex)})                         \
    X(HASH_i32, {INT_LOG_FN(va_arg, i32, format, _log_i32, _log_i32_hex)})                         \
    X(HASH_u64, {INT_LOG_FN(va_arg, u64, format, _log_u64, _log_u64_hex)})                         \
    X(HASH_i64, {INT_LOG_FN(va_arg, i64, format, _log_i64, _log_i64_hex)})                         \
    X(HASH_usize, {INT_LOG_FN(va_arg, usize, format, _log_usize, _log_usize_hex)})                 \
    X(HASH_isize, {INT_LOG_FN(va_arg, isize, format, _log_isize, _log_isize_hex)})                 \
    X(HASH_bool, { _log_bool(va_arg(args, int)); })                                                \
    X(HASH_char, { _log_char((char)va_arg(args, int)); })

bool _log_special(shared_StaticString specifier, va_list args) {
    if (specifier.length == 0)
        return false;

    u8 type_buf[TYPE_SPECIFIER_MAX_LEN];
    usize type_len = 0;
    while (true) {
        if (type_len >= specifier.length)
            break;
        if (type_len >= TYPE_SPECIFIER_MAX_LEN)
            return false;
        if (specifier.str[type_len] == ':')
            break;
        type_buf[type_len] = specifier.str[type_len];
        type_len++;
    }
    if (type_len == 0)
        return false;
    shared_StaticString type = ((shared_StaticString){(u64)type_len, type_buf});

    u8 format_buf[FORMAT_SPECIFIER_MAX_LEN];
    usize format_len = 0;
    if (type_len < specifier.length) {
        usize j = type_len + 1;
        while (j < specifier.length && format_len < FORMAT_SPECIFIER_MAX_LEN) {
            format_buf[format_len] = specifier.str[j];
            format_len++;
            j++;
        }
    }
    shared_StaticString format = ((shared_StaticString){(u64)format_len, format_buf});

    shared_Hasher hasher = shared_Hasher_init();
    shared_StaticString_hash(type, &hasher);
    shared_HashValue type_hash = shared_Hasher_finalize(&hasher);

    switch (type_hash) {
#define CASE_SPECIAL_TYPE(type_hash_literal, code)                                                 \
    case type_hash_literal: {                                                                      \
        code;                                                                                      \
        return true;                                                                               \
    }
        SPECIAL_TYPE_LIST(CASE_SPECIAL_TYPE)
#undef CASE_SPECIAL_TYPE

    default:
        return false;
    }
}

void _vlog(logging_Level level, shared_StaticString fmt, const char *file, int line, va_list args) {
    if (!current_log_fn)
        return;
    switch (level) {
    case LOGGING_LEVEL_PANIC:
        _log_str(STATIC_STRING("[PANIC] ("));
        break;
    case LOGGING_LEVEL_ERROR:
        _log_str(STATIC_STRING("[ERROR] ("));
        break;
    case LOGGING_LEVEL_WARN:
        _log_str(STATIC_STRING("[WARN ] ("));
        break;
    case LOGGING_LEVEL_INFO:
        _log_str(STATIC_STRING("[INFO ] ("));
        break;
    case LOGGING_LEVEL_DEBUG:
        _log_str(STATIC_STRING("[DEBUG] ("));
        break;
    }

    while (*file) {
        current_log_fn(*file++);
    }
    _log_str(STATIC_STRING(":"));
    _log_i32(line);
    _log_str(STATIC_STRING(") "));

    bool escaped = false;
    for (usize i = 0; i < fmt.length; i++) {
        if (escaped) {
            current_log_fn(fmt.str[i]);
            escaped = false;
            continue;
        }
        if (fmt.str[i] == '\\') {
            escaped = true;
            continue;
        }
        if (fmt.str[i] == '{') {
            // This is not a C-String.
            u8 specifier_buf[FORMAT_SPECIFIER_MAX_LEN];
            usize j = 1;
            usize specifier_len = 0;
            bool failed = false;

            while (true) {
                // Ensure we can read / write
                if ((i + j) >= fmt.length || specifier_len >= FORMAT_SPECIFIER_MAX_LEN) {
                    failed = true;
                    break;
                }
                u8 c = fmt.str[i + j];
                if (c == '}')
                    break;
                specifier_buf[specifier_len++] = c;
                j++;
            }

            if (failed) {
                // Just print the '{' and continue
                current_log_fn('{');
                continue;
            }

            shared_StaticString specifier =
                ((shared_StaticString){(u64)specifier_len, specifier_buf});
            if (!_log_special(specifier, args)) {
                current_log_fn('{');
                continue;
            }
            i += j;
            continue;
        }
        current_log_fn(fmt.str[i]);
    }

    current_log_fn('\n');
}

void logging_log(logging_Level level, shared_StaticString fmt, const char *file, int line, ...) {
    va_list args;
    va_start(args, line);
    _vlog(level, fmt, file, line, args);
    va_end(args);
}

void logging_set_log_fn(logging_log_fn fn) { current_log_fn = fn; }
