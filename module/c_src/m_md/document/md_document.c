#include "md_document.h"

#include "../blocks/md_blocks.h"
#include "../blocks/md_table.h"
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
    size_t consumed_length;
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
        if (*line_start == '\0') {
            break;
        }

        line_length = 0;
        while (line_start[line_length] != '\0' && line_start[line_length] != '\n') {
            line_length++;
        }

        if (line_length > 0 && line_start[line_length - 1] == '\r') {
            line_length--;
        }

        if (md_shared_is_blank_line(line_start, line_length)) {
            status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
            if (status != MARKDOWN_TRANSFORMER_OK) {
                string_buffer_free(&paragraph_buffer);
                return status;
            }

            cursor = line_start;
            while (*cursor != '\0' && *cursor != '\n') {
                cursor++;
            }
            if (*cursor == '\n') {
                cursor++;
            }

            line_start = cursor;
            continue;
        }

        if (
            md_table_is_row(line_start, line_length)
        ) {
            const char* next_line_start;
            size_t next_line_length;

            next_line_start = line_start;
            while (*next_line_start != '\0' && *next_line_start != '\n') {
                next_line_start++;
            }
            if (*next_line_start == '\n') {
                next_line_start++;
            }

            next_line_length = 0;
            while (
                next_line_start[next_line_length] != '\0' &&
                next_line_start[next_line_length] != '\n'
            ) {
                next_line_length++;
            }

            if (
                next_line_length > 0 &&
                next_line_start[next_line_length - 1] == '\r'
            ) {
                next_line_length--;
            }

            if (md_table_is_separator(next_line_start, next_line_length)) {
                status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }

                status = md_table_transform(
                    builder,
                    line_start,
                    &consumed_length
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }

                line_start += consumed_length;
                continue;
            }
        }

        if (md_blocks_parse_heading(
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

        cursor = line_start;
        while (*cursor != '\0' && *cursor != '\n') {
            cursor++;
        }
        if (*cursor == '\n') {
            cursor++;
        }

        line_start = cursor;
    }

    status = md_blocks_flush_paragraph(builder, &paragraph_buffer);
    string_buffer_free(&paragraph_buffer);

    return status;
}