#include "markdown_transformer.h"

#include "document/md_document.h"

// Transforms Markdown content into an HTML fragment using the provided builder.
// The builder content is cleared before writing the result.
MarkdownTransformerStatus markdown_transformer_transform(
    const char* markdown_content,
    HtmlBuilder* builder
) {
    if (markdown_content == NULL || builder == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    html_builder_clear(builder);
    return md_document_transform(markdown_content, builder);
}
