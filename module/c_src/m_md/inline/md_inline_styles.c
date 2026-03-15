#include "md_inline_styles.h"

#include "../shared/md_shared.h"
#include "md_inline.h"

// Parses a wrapped inline fragment and renders it inside the provided HTML tag.
MarkdownTransformerStatus md_inline_parse_wrapped_inline(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder,
    const char* tag_name
) {
    MarkdownTransformerStatus status;

    status = md_shared_append_open_tag_with_attrs(
        builder,
        tag_name,
        NULL,
        NULL,
        NULL,
        NULL,
        0
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_inline_transform_range(
        content_start,
        content_end,
        builder
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    return md_shared_append_closing_tag(builder, tag_name);
}

// Parses and renders an inline code span.
MarkdownTransformerStatus md_inline_parse_code_span(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder
) {
    MarkdownTransformerStatus status;

    status = md_shared_append_open_tag_with_attrs(
        builder,
        "code",
        NULL,
        NULL,
        NULL,
        NULL,
        0
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_shared_append_escaped_range(
        builder,
        content_start,
        (size_t)(content_end - content_start)
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    return md_shared_append_closing_tag(builder, "code");
}
