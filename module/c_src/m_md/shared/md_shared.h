#ifndef MD_SHARED_H
#define MD_SHARED_H

#include "../markdown_transformer.h"

#include <stddef.h>

// Maps an HtmlBuilder status to the corresponding Markdown transformer status.
MarkdownTransformerStatus md_shared_status_from_html_builder(
    HtmlBuilderStatus status
);

// Returns whether the provided line contains only whitespace.
int md_shared_is_blank_line(
    const char* line_start,
    size_t line_length
);

// Allocates and returns a null-terminated copy of the provided character range.
char* md_shared_copy_range(
    const char* start,
    size_t length
);

// Returns whether the provided character is valid inside an email autolink.
int md_shared_is_email_char(char character);

// Returns whether the provided character is safe inside an automatic URL.
int md_shared_is_safe_url_char(char character);

// Returns whether the provided character is a valid automatic URL boundary.
int md_shared_is_auto_url_boundary(char character);

// Finds the next unescaped occurrence of the provided delimiter.
const char* md_shared_find_closing_delimiter(
    const char* start,
    const char* delimiter
);

// Finds the next unescaped occurrence of the provided character.
const char* md_shared_find_matching_char(
    const char* start,
    char target
);

// Appends raw content to the builder.
MarkdownTransformerStatus md_shared_append_raw_n(
    HtmlBuilder* builder,
    const char* content,
    size_t content_length
);

// Escapes and appends a character range to the builder.
MarkdownTransformerStatus md_shared_append_escaped_range(
    HtmlBuilder* builder,
    const char* start,
    size_t length
);

// Appends an opening HTML tag with up to two attributes.
MarkdownTransformerStatus md_shared_append_open_tag_with_attrs(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    int self_closing
);

// Appends a closing HTML tag.
MarkdownTransformerStatus md_shared_append_closing_tag(
    HtmlBuilder* builder,
    const char* tag_name
);

// Flushes a plain text range as escaped HTML.
MarkdownTransformerStatus md_shared_flush_plain_text(
    HtmlBuilder* builder,
    const char* start,
    const char* end
);

#endif // MD_SHARED_H
