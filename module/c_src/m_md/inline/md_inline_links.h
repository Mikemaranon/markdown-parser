#ifndef MD_INLINE_LINKS_H
#define MD_INLINE_LINKS_H

#include "../markdown_transformer.h"

#include <stddef.h>

// Parses and renders a Markdown link.
MarkdownTransformerStatus md_inline_parse_link(
    const char* label_start,
    const char* label_end,
    const char* destination_start,
    const char* destination_end,
    const char* title_start,
    const char* title_end,
    HtmlBuilder* builder
);

// Parses and renders a Markdown image.
MarkdownTransformerStatus md_inline_parse_image(
    const char* alt_start,
    const char* alt_end,
    const char* source_start,
    const char* source_end,
    const char* title_start,
    const char* title_end,
    HtmlBuilder* builder
);

// Parses and renders an angle-bracket autolink.
MarkdownTransformerStatus md_inline_parse_autolink(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder
);

// Parses a Markdown link destination and optional title.
int md_inline_parse_link_destination(
    const char* start,
    const char* end,
    const char** destination_start,
    const char** destination_end,
    const char** title_start,
    const char** title_end
);

// Tries to parse an automatic URL from the current cursor position.
int md_inline_try_parse_auto_url(
    const char* cursor,
    const char* text_end,
    const char** url_end
);

#endif
