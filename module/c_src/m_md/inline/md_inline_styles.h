#ifndef MD_INLINE_STYLES_H
#define MD_INLINE_STYLES_H

#include "../markdown_transformer.h"

#include <stddef.h>

// Parses a wrapped inline fragment and renders it inside the provided HTML tag.
MarkdownTransformerStatus md_inline_parse_wrapped_inline(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder,
    const char* tag_name
);

// Parses and renders an inline code span.
MarkdownTransformerStatus md_inline_parse_code_span(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder
);

#endif
