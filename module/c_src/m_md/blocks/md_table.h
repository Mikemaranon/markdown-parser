#ifndef MD_TABLE_H
#define MD_TABLE_H

#include "../markdown_transformer.h"

#include <stddef.h>

// Returns whether the provided line looks like a Markdown table row.
int md_table_is_row(
    const char* line_start,
    size_t line_length
);

// Returns whether the provided line is a valid Markdown table separator row.
int md_table_is_separator(
    const char* line_start,
    size_t line_length
);

// Transforms a Markdown table block into HTML and reports the consumed lines.
MarkdownTransformerStatus md_table_transform(
    HtmlBuilder* builder,
    const char* markdown_start,
    size_t* consumed_length
);

#endif // MD_TABLE_H
