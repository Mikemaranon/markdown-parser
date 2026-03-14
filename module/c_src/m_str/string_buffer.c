#include "string_buffer.h"

#include <stdlib.h>
#include <string.h>

#define STRING_BUFFER_INITIAL_CAPACITY 64

static size_t string_buffer_max_size(size_t a, size_t b) {
    return (a > b) ? a : b;
}

static StringBufferStatus string_buffer_set_empty(StringBuffer* buffer) {
    if (buffer == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    if (buffer->data != NULL) {
        buffer->data[0] = '\0';
    }

    buffer->length = 0;
    return STRING_BUFFER_OK;
}


StringBufferStatus string_buffer_init(StringBuffer* buffer) {
    if (buffer == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    buffer->data = (char*)malloc(STRING_BUFFER_INITIAL_CAPACITY);
    if (buffer->data == NULL) {
        buffer->length = 0;
        buffer->capacity = 0;
        return STRING_BUFFER_ERROR_ALLOCATION_FAILED;
    }

    buffer->data[0] = '\0';
    buffer->length = 0;
    buffer->capacity = STRING_BUFFER_INITIAL_CAPACITY - 1;

    return STRING_BUFFER_OK;
}


void string_buffer_free(StringBuffer* buffer) {
    if (buffer == NULL) {
        return;
    }

    free(buffer->data);
    buffer->data = NULL;
    buffer->length = 0;
    buffer->capacity = 0;
}


void string_buffer_clear(StringBuffer* buffer) {
    if (buffer == NULL) {
        return;
    }

    string_buffer_set_empty(buffer);
}


StringBufferStatus string_buffer_reserve(
    StringBuffer* buffer,
    size_t required_capacity
) {
    size_t new_capacity;
    size_t allocation_size;
    char* new_data;

    if (buffer == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    if (required_capacity <= buffer->capacity) {
        return STRING_BUFFER_OK;
    }

    new_capacity = string_buffer_max_size(
        buffer->capacity * 2,
        required_capacity
    );

    if (new_capacity == 0) {
        new_capacity = required_capacity;
    }

    allocation_size = new_capacity + 1;
    new_data = (char*)realloc(buffer->data, allocation_size);
    if (new_data == NULL) {
        return STRING_BUFFER_ERROR_ALLOCATION_FAILED;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;

    return STRING_BUFFER_OK;
}


StringBufferStatus string_buffer_append(
    StringBuffer* buffer,
    const char* text
) {
    if (buffer == NULL || text == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    return string_buffer_append_n(buffer, text, strlen(text));
}


StringBufferStatus string_buffer_append_n(
    StringBuffer* buffer,
    const char* text,
    size_t text_length
) {
    size_t required_capacity;
    StringBufferStatus status;

    if (buffer == NULL || text == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    required_capacity = buffer->length + text_length;
    if (required_capacity < buffer->length) {
        return STRING_BUFFER_ERROR_ALLOCATION_FAILED;
    }

    status = string_buffer_reserve(buffer, required_capacity);
    if (status != STRING_BUFFER_OK) {
        return status;
    }

    memcpy(buffer->data + buffer->length, text, text_length);
    buffer->length += text_length;
    buffer->data[buffer->length] = '\0';

    return STRING_BUFFER_OK;
}


StringBufferStatus string_buffer_append_char(
    StringBuffer* buffer,
    char character
) {
    size_t required_capacity;
    StringBufferStatus status;

    if (buffer == NULL) {
        return STRING_BUFFER_ERROR_NULL_ARGUMENT;
    }

    required_capacity = buffer->length + 1;
    if (required_capacity < buffer->length) {
        return STRING_BUFFER_ERROR_ALLOCATION_FAILED;
    }

    status = string_buffer_reserve(buffer, required_capacity);
    if (status != STRING_BUFFER_OK) {
        return status;
    }

    buffer->data[buffer->length] = character;
    buffer->length += 1;
    buffer->data[buffer->length] = '\0';

    return STRING_BUFFER_OK;
}


const char* string_buffer_data(const StringBuffer* buffer) {
    if (buffer == NULL) {
        return NULL;
    }

    return buffer->data;
}


size_t string_buffer_length(const StringBuffer* buffer) {
    if (buffer == NULL) {
        return 0;
    }

    return buffer->length;
}