#include "md_table.h"

#include "md_blocks.h"
#include "../inline/md_inline.h"
#include "../shared/md_shared.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct MdTableCells {
    char** items;
    size_t count;
} MdTableCells;

// Trims leading and trailing whitespace from the provided mutable string.
static void md_table_trim_in_place(
    char* text
) {
    char* start;
    char* end;
    size_t length;

    if (text == NULL) {
        return;
    }

    start = text;
    while (*start != '\0' && isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        text[0] = '\0';
        return;
    }

    end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    if (start != text) {
        length = strlen(start);
        memmove(text, start, length + 1);
    }
}

// Frees all memory owned by the provided cell collection.
static void md_table_free_cells(
    MdTableCells* cells
) {
    size_t index;

    if (cells == NULL) {
        return;
    }

    if (cells->items != NULL) {
        for (index = 0; index < cells->count; index++) {
            free(cells->items[index]);
        }

        free(cells->items);
    }

    cells->items = NULL;
    cells->count = 0;
}

// Splits a Markdown table row into trimmed cell strings.
static MarkdownTransformerStatus md_table_split_row(
    const char* line_start,
    size_t line_length,
    MdTableCells* out_cells
) {
    char* buffer;
    char* content_start;
    char* content_end;
    char* cell_start;
    char* cursor;
    size_t estimated_cells;
    size_t cell_index;

    if (line_start == NULL || out_cells == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    out_cells->items = NULL;
    out_cells->count = 0;

    buffer = md_shared_copy_range(line_start, line_length);
    if (buffer == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    content_start = buffer;
    content_end = buffer + strlen(buffer);

    if (content_start < content_end && *content_start == '|') {
        content_start++;
    }

    if (content_start < content_end && *(content_end - 1) == '|') {
        *(content_end - 1) = '\0';
        content_end--;
    }

    estimated_cells = 1;
    for (cursor = content_start; *cursor != '\0'; cursor++) {
        if (*cursor == '|') {
            estimated_cells++;
        }
    }

    out_cells->items = (char**)calloc(estimated_cells, sizeof(char*));
    if (out_cells->items == NULL) {
        free(buffer);
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    cell_index = 0;
    cell_start = content_start;
    cursor = content_start;

    while (1) {
        if (*cursor == '|' || *cursor == '\0') {
            size_t raw_length;
            char* cell_value;

            raw_length = (size_t)(cursor - cell_start);
            cell_value = md_shared_copy_range(cell_start, raw_length);
            if (cell_value == NULL) {
                free(buffer);
                md_table_free_cells(out_cells);
                return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
            }

            md_table_trim_in_place(cell_value);
            out_cells->items[cell_index] = cell_value;
            cell_index++;

            if (*cursor == '\0') {
                break;
            }

            cell_start = cursor + 1;
        }

        cursor++;
    }

    out_cells->count = cell_index;
    free(buffer);

    return MARKDOWN_TRANSFORMER_OK;
}

// Returns whether the provided separator cell is valid.
static int md_table_is_separator_cell(
    const char* cell
) {
    const char* cursor;
    size_t dash_count;

    if (cell == NULL || *cell == '\0') {
        return 0;
    }

    cursor = cell;
    if (*cursor == ':') {
        cursor++;
    }

    dash_count = 0;
    while (*cursor == '-') {
        dash_count++;
        cursor++;
    }

    if (dash_count < 3) {
        return 0;
    }

    if (*cursor == ':') {
        cursor++;
    }

    return *cursor == '\0';
}

// Returns the length of the current line, excluding a trailing carriage return.
static size_t md_table_get_line_length(
    const char* line_start
) {
    const char* cursor;
    size_t length;

    if (line_start == NULL) {
        return 0;
    }

    cursor = line_start;
    while (*cursor != '\0' && *cursor != '\n') {
        cursor++;
    }

    length = (size_t)(cursor - line_start);
    if (length > 0 && line_start[length - 1] == '\r') {
        length--;
    }

    return length;
}

// Returns a pointer to the start of the next line.
static const char* md_table_next_line(
    const char* line_start
) {
    const char* cursor;

    if (line_start == NULL) {
        return NULL;
    }

    cursor = line_start;
    while (*cursor != '\0' && *cursor != '\n') {
        cursor++;
    }

    if (*cursor == '\n') {
        return cursor + 1;
    }

    return cursor;
}

// Renders an opening or closing block tag line.
static MarkdownTransformerStatus md_table_append_tag_line(
    HtmlBuilder* builder,
    const char* tag_text
) {
    return md_blocks_append_raw_block_line(
        builder,
        tag_text,
        strlen(tag_text)
    );
}

// Renders a single table cell using inline transformation.
static MarkdownTransformerStatus md_table_append_cell(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* content
) {
    HtmlBuilderStatus builder_status;
    MarkdownTransformerStatus status;

    if (builder == NULL || tag_name == NULL || content == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    builder_status = html_builder_append_indent(builder, 1);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(builder, "<", 1);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(
        builder,
        tag_name,
        strlen(tag_name)
    );
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(builder, ">", 1);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    status = md_inline_transform(content, builder);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    builder_status = html_builder_append_raw_n(builder, "</", 2);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(
        builder,
        tag_name,
        strlen(tag_name)
    );
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_raw_n(builder, ">", 1);
    if (builder_status != HTML_BUILDER_OK) {
        return md_shared_status_from_html_builder(builder_status);
    }

    builder_status = html_builder_append_newline(builder);
    return md_shared_status_from_html_builder(builder_status);
}

// Renders a complete table row.
static MarkdownTransformerStatus md_table_render_cells_row(
    HtmlBuilder* builder,
    const MdTableCells* cells,
    const char* cell_tag
) {
    MarkdownTransformerStatus status;
    size_t index;

    status = md_table_append_tag_line(builder, "<tr>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    for (index = 0; index < cells->count; index++) {
        status = md_table_append_cell(
            builder,
            cell_tag,
            cells->items[index]
        );
        if (status != MARKDOWN_TRANSFORMER_OK) {
            return status;
        }
    }

    status = md_table_append_tag_line(builder, "</tr>");
    return status;
}

// Returns whether the provided line looks like a Markdown table row.
int md_table_is_row(
    const char* line_start,
    size_t line_length
) {
    size_t pipe_count;
    size_t index;

    if (line_start == NULL || line_length == 0) {
        return 0;
    }

    pipe_count = 0;
    for (index = 0; index < line_length; index++) {
        if (line_start[index] == '|') {
            pipe_count++;
        }
    }

    return pipe_count >= 2;
}

// Returns whether the provided line is a valid Markdown table separator row.
int md_table_is_separator(
    const char* line_start,
    size_t line_length
) {
    MdTableCells cells;
    MarkdownTransformerStatus status;
    size_t index;

    if (!md_table_is_row(line_start, line_length)) {
        return 0;
    }

    status = md_table_split_row(line_start, line_length, &cells);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return 0;
    }

    if (cells.count == 0) {
        md_table_free_cells(&cells);
        return 0;
    }

    for (index = 0; index < cells.count; index++) {
        if (!md_table_is_separator_cell(cells.items[index])) {
            md_table_free_cells(&cells);
            return 0;
        }
    }

    md_table_free_cells(&cells);
    return 1;
}

// Transforms a Markdown table block into HTML and reports the consumed length.
MarkdownTransformerStatus md_table_transform(
    HtmlBuilder* builder,
    const char* markdown_start,
    size_t* consumed_length
) {
    const char* current_line;
    const char* next_line;
    size_t current_length;
    size_t next_length;
    MdTableCells header_cells;
    MdTableCells row_cells;
    MarkdownTransformerStatus status;

    if (
        builder == NULL ||
        markdown_start == NULL ||
        consumed_length == NULL
    ) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    *consumed_length = 0;

    current_line = markdown_start;
    current_length = md_table_get_line_length(current_line);
    next_line = md_table_next_line(current_line);
    next_length = md_table_get_line_length(next_line);

    if (
        !md_table_is_row(current_line, current_length) ||
        !md_table_is_separator(next_line, next_length)
    ) {
        return MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED;
    }

    status = md_table_split_row(current_line, current_length, &header_cells);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_table_append_tag_line(builder, "<table>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        md_table_free_cells(&header_cells);
        return status;
    }

    status = md_table_append_tag_line(builder, "<thead>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        md_table_free_cells(&header_cells);
        return status;
    }

    status = md_table_render_cells_row(builder, &header_cells, "th");
    md_table_free_cells(&header_cells);
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_table_append_tag_line(builder, "</thead>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_table_append_tag_line(builder, "<tbody>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    current_line = md_table_next_line(next_line);

    while (*current_line != '\0') {
        current_length = md_table_get_line_length(current_line);

        if (
            md_shared_is_blank_line(current_line, current_length) ||
            !md_table_is_row(current_line, current_length) ||
            md_table_is_separator(current_line, current_length)
        ) {
            break;
        }

        status = md_table_split_row(current_line, current_length, &row_cells);
        if (status != MARKDOWN_TRANSFORMER_OK) {
            return status;
        }

        status = md_table_render_cells_row(builder, &row_cells, "td");
        md_table_free_cells(&row_cells);
        if (status != MARKDOWN_TRANSFORMER_OK) {
            return status;
        }

        current_line = md_table_next_line(current_line);
    }

    status = md_table_append_tag_line(builder, "</tbody>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    status = md_table_append_tag_line(builder, "</table>");
    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    *consumed_length = (size_t)(current_line - markdown_start);
    return MARKDOWN_TRANSFORMER_OK;
}
