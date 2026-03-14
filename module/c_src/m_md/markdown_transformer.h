#ifndef MARKDOWN_TRANSFORMER_H
#define MARKDOWN_TRANSFORMER_H

#include "../m_html/html_builder.h"

typedef enum MarkdownTransformerStatus {
    MARKDOWN_TRANSFORMER_OK = 0,
    MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT = 1,
    MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED = 2,
    MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED = 3
} MarkdownTransformerStatus;


// Transforms Markdown content into an HTML fragment.

// [Parameters]
//   - markdown_content: UTF-8 Markdown input
//   - builder: HTML builder that receives the generated fragment
//
// [Returns]
//   - MARKDOWN_TRANSFORMER_OK on success
//   - error code otherwise

MarkdownTransformerStatus markdown_transformer_transform(
    const char* markdown_content,
    HtmlBuilder* builder
);

#endif // MARKDOWN_TRANSFORMER_H
