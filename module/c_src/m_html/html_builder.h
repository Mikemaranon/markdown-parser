#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include <stddef.h>

#include "../m_str/string_buffer.h"

typedef enum HtmlBuilderStatus {
    HTML_BUILDER_OK = 0,
    HTML_BUILDER_ERROR_NULL_ARGUMENT = 1,
    HTML_BUILDER_ERROR_ALLOCATION_FAILED = 2
} HtmlBuilderStatus;

typedef struct HtmlBuilder {
    StringBuffer buffer;
} HtmlBuilder;


// Initializes an HTML builder instance.

// [Parameters]
//   - builder: builder instance to initialize
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_init(HtmlBuilder* builder);


// Releases all memory owned by the builder.
// Safe to call with NULL.

// [Parameters]
//   - builder: builder instance to free

void html_builder_free(HtmlBuilder* builder);


// Clears the builder content but preserves allocated memory when possible.
// Safe to call with NULL.

// [Parameters]
//   - builder: builder instance to clear

void html_builder_clear(HtmlBuilder* builder);


// Appends a raw HTML string without escaping.

// [Parameters]
//   - builder: builder instance
//   - html: UTF-8 HTML string to append
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_raw(
    HtmlBuilder* builder,
    const char* html
);


// Appends a raw character range without escaping.

// [Parameters]
//   - builder: builder instance
//   - html: pointer to the HTML range
//   - html_length: number of bytes to append
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_raw_n(
    HtmlBuilder* builder,
    const char* html,
    size_t html_length
);


// Appends text content with HTML escaping applied.

// [Parameters]
//   - builder: builder instance
//   - text: UTF-8 text to escape and append
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_escaped(
    HtmlBuilder* builder,
    const char* text
);


// Opens an HTML tag without attributes.

// [Parameters]
//   - builder: builder instance
//   - tag_name: UTF-8 tag name
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_open_tag(
    HtmlBuilder* builder,
    const char* tag_name
);


// Closes an HTML tag.

// [Parameters]
//   - builder: builder instance
//   - tag_name: UTF-8 tag name
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_close_tag(
    HtmlBuilder* builder,
    const char* tag_name
);


// Appends a complete HTML element with escaped text content.

// [Parameters]
//   - builder: builder instance
//   - tag_name: UTF-8 tag name
//   - text_content: UTF-8 text content
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content
);


// Returns the internal null-terminated HTML data pointer.
// The returned pointer is owned by the builder and must not be freed directly.

// [Parameters]
//   - builder: builder instance
//
// [Returns]
//   - Internal string pointer on success
//   - NULL if builder is NULL

const char* html_builder_data(const HtmlBuilder* builder);


// Returns the current HTML length, excluding the null terminator.

// [Parameters]
//   - builder: builder instance
//
// [Returns]
//   - Current HTML length
//   - 0 if builder is NULL

size_t html_builder_length(const HtmlBuilder* builder);


// Appends a newline character to the HTML output.

// [Parameters]
//   - builder: builder instance
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_newline(HtmlBuilder* builder);


// Appends indentation using four spaces per indentation level.

// [Parameters]
//   - builder: builder instance
//   - indent_level: indentation depth level
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_indent(
    HtmlBuilder* builder,
    size_t indent_level
);


// Appends a block-level text element with indentation and a trailing newline.
// The text content is HTML-escaped before being written.

// [Parameters]
//   - builder: builder instance
//   - tag_name: UTF-8 tag name
//   - text_content: UTF-8 text content
//   - indent_level: indentation depth level
//
// [Returns]
//   - HTML_BUILDER_OK on success
//   - error code otherwise

HtmlBuilderStatus html_builder_append_block_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content,
    size_t indent_level
);

#endif // HTML_BUILDER_H
