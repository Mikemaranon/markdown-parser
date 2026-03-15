#include "md_document.h"

#include "../blocks/md_blocks.h"
#include "../shared/md_shared.h"

#include "string_buffer.h"

// Transforms a full Markdown document into an HTML fragment.
MarkdownTransformerStatus md_document_transform(
    const char* markdown_content,
    HtmlBuilder* builder
) {
    const char* cursor;
    const char* line_start;
    const char* content_start;
    size_t line_length;
    size_t heading_level;
    StringBuffer paragraph_buffer;
    StringBufferStatus buffer_status;
    MarkdownTransformerStatus status;

    if (markdown_content == NULL || builder == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    buffer_status = string_buffer_init(&paragraph_buffer);
    if (buffer_status != STRING_BUFFER_OK) {
        return (buffer_status == STRING_BUFFER_ERROR_NULL_ARGUMENT)
            ? MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT
            : MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    cursor = markdown_content;
    line_start = markdown_content;

    while (1) {
        if (*cursor == '\n' || *cursor == '\0') {
            line_length = (size_t)(cursor - line_start);

            if (line_length > 0 && line_start[line_length - 1] == '\r') {
                line_length--;
            }

            if (md_shared_is_blank_line(line_start, line_length)) {
                status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }
            } else if (md_blocks_parse_heading(
                line_start,
                line_length,
                &heading_level,
                &content_start,
                &line_length
            )) {
                status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }

                status = md_blocks_append_heading(
                    builder,
                    content_start,
                    line_length,
                    heading_level
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }
            } else {
                status = md_blocks_append_line_to_paragraph(
                    &paragraph_buffer,
                    line_start,
                    line_length
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }
            }

            if (*cursor == '\0') {
                break;
            }

            cursor++;
            line_start = cursor;
            continue;
        }

        cursor++;
    }

    status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
    string_buffer_free(&paragraph_buffer);

    return status;
}
