#include "markdown_transformer.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static MarkdownTransformerStatus markdown_transformer_status_from_html_builder(
    HtmlBuilderStatus status
) {
    switch (status) {
        case HTML_BUILDER_OK:
            return MARKDOWN_TRANSFORMER_OK;
        case HTML_BUILDER_ERROR_NULL_ARGUMENT:
            return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
        case HTML_BUILDER_ERROR_ALLOCATION_FAILED:
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        default:
            return MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED;
    }
}

static int markdown_transformer_is_blank_line(
    const char* line_start,
    size_t line_length
) {
    size_t index;

    if (line_start == NULL) {
        return 1;
    }

    for (index = 0; index < line_length; index++) {
        if (line_start[index] != ' ' && line_start[index] != '\t') {
            return 0;
        }
    }

    return 1;
}

static char* markdown_transformer_copy_range(
    const char* start,
    size_t length
) {
    char* copy;

    copy = (char*)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    if (length > 0) {
        memcpy(copy, start, length);
    }

    copy[length] = '\0';
    return copy;
}

static MarkdownTransformerStatus markdown_transformer_append_heading(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length,
    size_t heading_level
) {
    char tag_name[4];
    char* content_copy;
    MarkdownTransformerStatus status;

    if (builder == NULL || content_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    if (heading_level < 1 || heading_level > 6) {
        return MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED;
    }

    tag_name[0] = 'h';
    tag_name[1] = (char)('0' + heading_level);
    tag_name[2] = '\0';
    tag_name[3] = '\0';

    content_copy = markdown_transformer_copy_range(content_start, content_length);
    if (content_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    status = markdown_transformer_status_from_html_builder(
        html_builder_append_block_text_element(builder, tag_name, content_copy, 1)
    );

    free(content_copy);
    return status;
}

static MarkdownTransformerStatus markdown_transformer_append_paragraph(
    HtmlBuilder* builder,
    const char* content_start,
    size_t content_length
) {
    char* content_copy;
    MarkdownTransformerStatus status;

    if (builder == NULL || content_start == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    content_copy = markdown_transformer_copy_range(content_start, content_length);
    if (content_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    status = markdown_transformer_status_from_html_builder(
        html_builder_append_block_text_element(builder, "p", content_copy, 1)
    );

    free(content_copy);
    return status;
}

static int markdown_transformer_parse_heading(
    const char* line_start,
    size_t line_length,
    size_t* heading_level,
    const char** content_start,
    size_t* content_length
) {
    size_t index;
    size_t hashes;

    if (line_start == NULL || heading_level == NULL ||
        content_start == NULL || content_length == NULL) {
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

static MarkdownTransformerStatus markdown_transformer_flush_paragraph(
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

    status = markdown_transformer_append_paragraph(
        builder,
        string_buffer_data(paragraph_buffer),
        string_buffer_length(paragraph_buffer)
    );

    string_buffer_clear(paragraph_buffer);
    return status;
}

static MarkdownTransformerStatus markdown_transformer_append_line_to_paragraph(
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

MarkdownTransformerStatus markdown_transformer_transform(
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

    html_builder_clear(builder);

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

            if (markdown_transformer_is_blank_line(line_start, line_length)) {
                status = markdown_transformer_flush_paragraph(builder, &paragraph_buffer);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }
            } else if (markdown_transformer_parse_heading(
                line_start,
                line_length,
                &heading_level,
                &content_start,
                &line_length
            )) {
                status = markdown_transformer_flush_paragraph(builder, &paragraph_buffer);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    string_buffer_free(&paragraph_buffer);
                    return status;
                }

                status = markdown_transformer_append_heading(
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
                status = markdown_transformer_append_line_to_paragraph(
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

    status = markdown_transformer_flush_paragraph(builder, &paragraph_buffer);
    string_buffer_free(&paragraph_buffer);

    return status;
}
