#include "md_inline_styles.h"

#include "../shared/md_shared.h"
#include "md_inline.h"

// Parses and renders inline content wrapped by a specific HTML tag.
MarkdownTransformerStatus md_inline_parse_wrapped_inline(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder,
    const char* tag_name
) {
    MarkdownTransformerStatus status;

    status = md_shared_status_from_html_builder(
        html_builder_open_inline_tag(builder, tag_name)
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

    return md_shared_status_from_html_builder(
        html_builder_close_inline_tag(builder, tag_name)
    );
}

// Parses and renders an inline code span.
MarkdownTransformerStatus md_inline_parse_code_span(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder
) {
    MarkdownTransformerStatus status;

    status = md_shared_status_from_html_builder(
        html_builder_open_inline_tag(builder, "code")
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

    return md_shared_status_from_html_builder(
        html_builder_close_inline_tag(builder, "code")
    );
}