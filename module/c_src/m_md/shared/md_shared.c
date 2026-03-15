#include "md_shared.h"

#include <stdlib.h>
#include <string.h>

// Maps an HtmlBuilder status to the corresponding Markdown transformer status.
MarkdownTransformerStatus md_shared_status_from_html_builder(
    HtmlBuilderStatus status
) {
    switch (status) {
        case HTML_BUILDER_OK:
            return MARKDOWN_TRANSFORMER_OK;
        case HTML_BUILDER_ERROR_NULL_ARGUMENT:
            return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
        case HTML_BUILDER_ERROR_ALLOCATION_FAILED:
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        default:
            return MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED;
    }
}

// Returns whether the provided line contains only whitespace.
int md_shared_is_blank_line(
    const char* line_start,
    size_t line_length
) {
    size_t index;

    if (line_start == NULL) {
        return 1;
    }

    for (index = 0; index < line_length; index++) {
        if (line_start[index] != ' ' && line_start[index] != '\t') {
            return 0;
        }
    }

    return 1;
}

// Allocates and returns a null-terminated copy of the provided character range.
char* md_shared_copy_range(
    const char* start,
    size_t length
) {
    char* copy;

    copy = (char*)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    if (length > 0) {
        memcpy(copy, start, length);
    }

    copy[length] = '\0';
    return copy;
}

// Returns whether the provided character is valid inside an email autolink.
int md_shared_is_email_char(char character) {
    return (
        (character >= 'a' && character <= 'z') ||
        (character >= 'A' && character <= 'Z') ||
        (character >= '0' && character <= '9') ||
        character == '.' ||
        character == '_' ||
        character == '%' ||
        character == '+' ||
        character == '-' ||
        character == '@'
    );
}

// Returns whether the provided character is safe inside an automatic URL.
int md_shared_is_safe_url_char(char character) {
    return (
        (character >= 'a' && character <= 'z') ||
        (character >= 'A' && character <= 'Z') ||
        (character >= '0' && character <= '9') ||
        character == ':' ||
        character == '/' ||
        character == '.' ||
        character == '?' ||
        character == '#' ||
        character == '&' ||
        character == '%' ||
        character == '=' ||
        character == '_' ||
        character == '-' ||
        character == '~' ||
        character == '+' ||
        character == '@' ||
        character == '!' ||
        character == '$' ||
        character == '\'' ||
        character == '(' ||
        character == ')' ||
        character == '*' ||
        character == ',' ||
        character == ';'
    );
}

// Returns whether the provided character is a valid automatic URL boundary.
int md_shared_is_auto_url_boundary(char character) {
    return (
        character == ' ' ||
        character == '\t' ||
        character == '\n' ||
        character == '\r' ||
        character == '<' ||
        character == '>' ||
        character == '"' ||
        character == '\''
    );
}

// Finds the next unescaped occurrence of the provided delimiter.
const char* md_shared_find_closing_delimiter(
    const char* start,
    const char* delimiter
) {
    size_t delimiter_length;
    const char* cursor;

    if (start == NULL || delimiter == NULL) {
        return NULL;
    }

    delimiter_length = strlen(delimiter);
    if (delimiter_length == 0) {
        return NULL;
    }

    cursor = start;
    while (*cursor != '\0') {
        if (*cursor == '\\' && *(cursor + 1) != '\0') {
            cursor += 2;
            continue;
        }

        if (strncmp(cursor, delimiter, delimiter_length) == 0) {
            return cursor;
        }

        cursor++;
    }

    return NULL;
}

// Finds the next unescaped occurrence of the provided character.
const char* md_shared_find_matching_char(
    const char* start,
    char target
) {
    const char* cursor;

    if (start == NULL) {
        return NULL;
    }

    cursor = start;
    while (*cursor != '\0') {
        if (*cursor == '\\' && *(cursor + 1) != '\0') {
            cursor += 2;
            continue;
        }

        if (*cursor == target) {
            return cursor;
        }

        cursor++;
    }

    return NULL;
}

// Appends raw content to the builder.
MarkdownTransformerStatus md_shared_append_raw_n(
    HtmlBuilder* builder,
    const char* content,
    size_t content_length
) {
    return md_shared_status_from_html_builder(
        html_builder_append_raw_n(builder, content, content_length)
    );
}

// Escapes and appends a character range to the builder.
MarkdownTransformerStatus md_shared_append_escaped_range(
    HtmlBuilder* builder,
    const char* start,
    size_t length
) {
    char* copy;
    MarkdownTransformerStatus status;

    if (builder == NULL || start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    copy = md_shared_copy_range(start, length);
    if (copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    status = md_shared_status_from_html_builder(
        html_builder_append_escaped(builder, copy)
    );

    free(copy);
    return status;
}

// Flushes a plain text range as escaped HTML.
MarkdownTransformerStatus md_shared_flush_plain_text(
    HtmlBuilder* builder,
    const char* start,
    const char* end
) {
    if (builder == NULL || start == NULL || end == NULL || end < start) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    if (start == end) {
        return MARKDOWN_TRANSFORMER_OK;
    }

    return md_shared_append_escaped_range(
        builder,
        start,
        (size_t)(end - start)
    );
}
