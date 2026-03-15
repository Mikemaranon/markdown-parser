#include "md_blocks.h"
#include "md_table.h"

#include "../inline/md_inline.h"
#include "../shared/md_shared.h"

#include <stdlib.h>

// Appends a heading block to the builder.
MarkdownTransformerStatus md_blocks_append_heading(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length,
    size_t heading_level
) {
    char* content_copy;
    HtmlBuilderStatus builder_status;
    MarkdownTransformerStatus status;

    if (builder == NULL || content_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    if (heading_level < 1 || heading_level > 6) {
        return MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED;
    }

    content_copy = md_shared_copy_range(content_start, content_length);
    if (content_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    builder_status = html_builder_append_indent(builder, 1);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_open_heading(builder, heading_level);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    status = md_inline_transform(content_copy, builder);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        free(content_copy);
        return status;
    }

    builder_status = html_builder_close_heading(builder, heading_level);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_newline(builder);
    free(content_copy);

    return md_shared_status_from_html_builder(builder_status);
}

// Appends a paragraph block to the builder.
MarkdownTransformerStatus md_blocks_append_paragraph(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length
) {
    char* content_copy;
    HtmlBuilderStatus builder_status;
    MarkdownTransformerStatus status;

    if (builder == NULL || content_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    content_copy = md_shared_copy_range(content_start, content_length);
    if (content_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    builder_status = html_builder_append_indent(builder, 1);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_open_paragraph(builder);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    status = md_inline_transform(content_copy, builder);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        free(content_copy);
        return status;
    }

    builder_status = html_builder_close_paragraph(builder);
    if (builder_status != HTML_BUILDER_OK) {
        free(content_copy);
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_newline(builder);
    free(content_copy);

    return md_shared_status_from_html_builder(builder_status);
}

// Parses an ATX heading line.
int md_blocks_parse_heading(
    const char* line_start,
    size_t line_length,
    size_t* heading_level,
    const char** content_start,
    size_t* content_length
) {
    size_t index;
    size_t hashes;

    if (
        line_start == NULL ||
        heading_level == NULL ||
        content_start == NULL ||
        content_length == NULL
    ) {
        return 0;
    }

    hashes = 0;
    index = 0;

    while (index < line_length && line_start[index] == '#') {
        hashes++;
        index++;
    }

    if (hashes < 1 || hashes > 6) {
        return 0;
    }

    if (index >= line_length || line_start[index] != ' ') {
        return 0;
    }

    while (index < line_length && line_start[index] == ' ') {
        index++;
    }

    *heading_level = hashes;
    *content_start = line_start + index;
    *content_length = line_length - index;

    return 1;
}

// Flushes the current paragraph buffer into the builder.
MarkdownTransformerStatus md_blocks_flush_paragraph(
    HtmlBuilder* builder,
    StringBuffer* paragraph_buffer
) {
    MarkdownTransformerStatus status;

    if (builder == NULL || paragraph_buffer == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    if (string_buffer_length(paragraph_buffer) == 0) {
        return MARKDOWN_TRANSFORMER_OK;
    }

    status = md_blocks_append_paragraph(
        builder,
        string_buffer_data(paragraph_buffer),
        string_buffer_length(paragraph_buffer)
    );

    string_buffer_clear(paragraph_buffer);
    return status;
}

// Appends a line to the current paragraph buffer.
MarkdownTransformerStatus md_blocks_append_line_to_paragraph(
    StringBuffer* paragraph_buffer,
    const char* line_start,
    size_t line_length
) {
    StringBufferStatus buffer_status;

    if (paragraph_buffer == NULL || line_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    if (string_buffer_length(paragraph_buffer) > 0) {
        buffer_status = string_buffer_append_char(paragraph_buffer, ' ');
        if (buffer_status != STRING_BUFFER_OK) {
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        }
    }

    buffer_status = string_buffer_append_n(paragraph_buffer, line_start, line_length);
    if (buffer_status != STRING_BUFFER_OK) {
        return (buffer_status == STRING_BUFFER_ERROR_NULL_ARGUMENT)
            ? MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT
            : MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    return MARKDOWN_TRANSFORMER_OK;
}

// Appends a raw HTML block line with the standard block indentation.
MarkdownTransformerStatus md_blocks_append_raw_block_line(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length
) {
    HtmlBuilderStatus builder_status;

    if (builder == NULL || content_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    builder_status = html_builder_append_indent(builder, 1);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(
        builder,
        content_start,
        content_length
    );
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_newline(builder);
    return md_shared_status_from_html_builder(builder_status);
}
