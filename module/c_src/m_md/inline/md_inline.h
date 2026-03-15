#ifndef MD_INLINE_H
#define MD_INLINE_H

#include "../markdown_transformer.h"

#include <stddef.h>

// Transforms a full inline text string into HTML.
MarkdownTransformerStatus md_inline_transform(
    const char* text,
    HtmlBuilder* builder
);

// Transforms an inline character range into HTML.
MarkdownTransformerStatus md_inline_transform_range(
    const char* text_start,
    const char* text_end,
    HtmlBuilder* builder
);

#endif // MD_INLINE_H
