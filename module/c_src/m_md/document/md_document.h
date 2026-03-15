#ifndef MD_DOCUMENT_H
#define MD_DOCUMENT_H

#include "../markdown_transformer.h"

// Transforms a full Markdown document into an HTML fragment.
MarkdownTransformerStatus md_document_transform(
    const char* markdown_content,
    HtmlBuilder* builder
);

#endif // MD_DOCUMENT_H
