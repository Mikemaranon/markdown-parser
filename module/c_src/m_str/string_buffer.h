#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stddef.h>

typedef enum StringBufferStatus {
    STRING_BUFFER_OK = 0,
    STRING_BUFFER_ERROR_NULL_ARGUMENT = 1,
    STRING_BUFFER_ERROR_ALLOCATION_FAILED = 2
} StringBufferStatus;

typedef struct StringBuffer {
    char* data;
    size_t length;
    size_t capacity;
} StringBuffer;


// Initializes a string buffer instance.

// [Parameters]
//   - buffer: buffer instance to initialize
//
// [Returns]
//   - STRING_BUFFER_OK on success
//   - error code otherwise

StringBufferStatus string_buffer_init(StringBuffer* buffer);


// Releases all memory owned by the buffer.
// Safe to call with NULL.

// [Parameters]
//   - buffer: buffer instance to free

void string_buffer_free(StringBuffer* buffer);


// Clears the buffer content but preserves allocated memory when possible.
// Safe to call with NULL.

// [Parameters]
//   - buffer: buffer instance to clear

void string_buffer_clear(StringBuffer* buffer);


// Ensures that the buffer can store at least the requested capacity,
// excluding the null terminator.

// [Parameters]
//   - buffer: buffer instance
//   - required_capacity: minimum writable character capacity
//
// [Returns]
//   - STRING_BUFFER_OK on success
//   - error code otherwise

StringBufferStatus string_buffer_reserve(
    StringBuffer* buffer,
    size_t required_capacity
);


// Appends a null-terminated string to the buffer.

// [Parameters]
//   - buffer: buffer instance
//   - text: UTF-8 string to append
//
// [Returns]
//   - STRING_BUFFER_OK on success
//   - error code otherwise

StringBufferStatus string_buffer_append(
    StringBuffer* buffer,
    const char* text
);


// Appends a raw character range to the buffer.

// [Parameters]
//   - buffer: buffer instance
//   - text: pointer to the character range
//   - text_length: number of bytes to append
//
// [Returns]
//   - STRING_BUFFER_OK on success
//   - error code otherwise

StringBufferStatus string_buffer_append_n(
    StringBuffer* buffer,
    const char* text,
    size_t text_length
);


// Appends a single character to the buffer.

// [Parameters]
//   - buffer: buffer instance
//   - character: character to append
//
// [Returns]
//   - STRING_BUFFER_OK on success
//   - error code otherwise

StringBufferStatus string_buffer_append_char(
    StringBuffer* buffer,
    char character
);


// Returns the internal null-terminated data pointer.
// The returned pointer is owned by the buffer and must not be freed directly.

// [Parameters]
//   - buffer: buffer instance
//
// [Returns]
//   - Internal string pointer on success
//   - NULL if buffer is NULL

const char* string_buffer_data(const StringBuffer* buffer);


// Returns the current string length, excluding the null terminator.

// [Parameters]
//   - buffer: buffer instance
//
// [Returns]
//   - Current string length
//   - 0 if buffer is NULL

size_t string_buffer_length(const StringBuffer* buffer);

#endif // STRING_BUFFER_H