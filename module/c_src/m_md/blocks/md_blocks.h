#ifndef MD_BLOCKS_H
#define MD_BLOCKS_H

#include "../markdown_transformer.h"
#include "string_buffer.h"

#include <stddef.h>

// Appends a heading block to the builder.
MarkdownTransformerStatus md_blocks_append_heading(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length,
    size_t heading_level
);

// Appends a paragraph block to the builder.
MarkdownTransformerStatus md_blocks_append_paragraph(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length
);

// Parses an ATX heading line.
int md_blocks_parse_heading(
    const char* line_start,
    size_t line_length,
    size_t* heading_level,
    const char** content_start,
    size_t* content_length
);

// Flushes the current paragraph buffer into the builder.
MarkdownTransformerStatus md_blocks_flush_paragraph(
    HtmlBuilder* builder,
    StringBuffer* paragraph_buffer
);

// Appends a line to the current paragraph buffer.
MarkdownTransformerStatus md_blocks_append_line_to_paragraph(
    StringBuffer* paragraph_buffer,
    const char* line_start,
    size_t line_length
);

#endif // MD_BLOCKS_H
