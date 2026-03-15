#include "md_inline_links.h"

#include "../shared/md_shared.h"
#include "md_inline.h"

#include <stdlib.h>
#include <string.h>

// Parses and renders a Markdown link.
MarkdownTransformerStatus md_inline_parse_link(
    const char* label_start,
    const char* label_end,
    const char* destination_start,
    const char* destination_end,
    const char* title_start,
    const char* title_end,
    HtmlBuilder* builder
) {
    char* href_copy;
    char* title_copy;
    MarkdownTransformerStatus status;

    href_copy = md_shared_copy_range(
        destination_start,
        (size_t)(destination_end - destination_start)
    );
    if (href_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    title_copy = NULL;
    if (title_start != NULL && title_end != NULL && title_end > title_start) {
        title_copy = md_shared_copy_range(
            title_start,
            (size_t)(title_end - title_start)
        );
        if (title_copy == NULL) {
            free(href_copy);
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        }
    }

    status = md_shared_append_open_tag_with_attrs(
        builder,
        "a",
        "href",
        href_copy,
        "title",
        title_copy,
        0
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        free(href_copy);
        free(title_copy);
        return status;
    }

    status = md_inline_transform_range(
        label_start,
        label_end,
        builder
    );

    free(href_copy);
    free(title_copy);

    if (status != MARKDOWN_TRANSFORMER_OK) {
        return status;
    }

    return md_shared_append_closing_tag(builder, "a");
}

// Parses and renders a Markdown image.
MarkdownTransformerStatus md_inline_parse_image(
    const char* alt_start,
    const char* alt_end,
    const char* source_start,
    const char* source_end,
    const char* title_start,
    const char* title_end,
    HtmlBuilder* builder
) {
    char* src_copy;
    char* alt_copy;
    char* title_copy;
    MarkdownTransformerStatus status;

    src_copy = md_shared_copy_range(
        source_start,
        (size_t)(source_end - source_start)
    );
    if (src_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    alt_copy = md_shared_copy_range(
        alt_start,
        (size_t)(alt_end - alt_start)
    );
    if (alt_copy == NULL) {
        free(src_copy);
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    title_copy = NULL;
    if (title_start != NULL && title_end != NULL && title_end > title_start) {
        title_copy = md_shared_copy_range(
            title_start,
            (size_t)(title_end - title_start)
        );
        if (title_copy == NULL) {
            free(src_copy);
            free(alt_copy);
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        }
    }

    status = md_shared_append_open_tag_with_attrs(
        builder,
        "img",
        "src",
        src_copy,
        "alt",
        alt_copy,
        0
    );
    if (status == MARKDOWN_TRANSFORMER_OK && title_copy != NULL) {
        status = md_shared_append_raw_n(builder, " ", 1);
        if (status == MARKDOWN_TRANSFORMER_OK) {
            status = md_shared_append_raw_n(builder, "title", 5);
        }
        if (status == MARKDOWN_TRANSFORMER_OK) {
            status = md_shared_append_raw_n(builder, "=\"", 2);
        }
        if (status == MARKDOWN_TRANSFORMER_OK) {
            status = md_shared_status_from_html_builder(
                html_builder_append_escaped(builder, title_copy)
            );
        }
        if (status == MARKDOWN_TRANSFORMER_OK) {
            status = md_shared_append_raw_n(builder, "\"", 1);
        }
        if (status == MARKDOWN_TRANSFORMER_OK) {
            status = md_shared_append_raw_n(builder, ">", 1);
        }
    }

    if (status == MARKDOWN_TRANSFORMER_OK && title_copy == NULL) {
        status = md_shared_append_raw_n(builder, ">", 1);
    }

    free(src_copy);
    free(alt_copy);
    free(title_copy);

    return status;
}

// Parses and renders an angle-bracket autolink.
MarkdownTransformerStatus md_inline_parse_autolink(
    const char* content_start,
    const char* content_end,
    HtmlBuilder* builder
) {
    const char* at_sign;
    char* content_copy;
    char* href_copy;
    MarkdownTransformerStatus status;

    content_copy = md_shared_copy_range(
        content_start,
        (size_t)(content_end - content_start)
    );
    if (content_copy == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
    }

    at_sign = strchr(content_copy, '@');

    if (at_sign != NULL) {
        size_t href_length = strlen("mailto:") + strlen(content_copy);

        href_copy = (char*)malloc(href_length + 1);
        if (href_copy == NULL) {
            free(content_copy);
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        }

        memcpy(href_copy, "mailto:", strlen("mailto:"));
        memcpy(
            href_copy + strlen("mailto:"),
            content_copy,
            strlen(content_copy) + 1
        );
    } else {
        href_copy = md_shared_copy_range(
            content_start,
            (size_t)(content_end - content_start)
        );
        if (href_copy == NULL) {
            free(content_copy);
            return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
        }
    }

    status = md_shared_append_open_tag_with_attrs(
        builder,
        "a",
        "href",
        href_copy,
        NULL,
        NULL,
        0
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        free(content_copy);
        free(href_copy);
        return status;
    }

    status = md_shared_status_from_html_builder(
        html_builder_append_escaped(builder, content_copy)
    );
    if (status != MARKDOWN_TRANSFORMER_OK) {
        free(content_copy);
        free(href_copy);
        return status;
    }

    status = md_shared_append_closing_tag(builder, "a");

    free(content_copy);
    free(href_copy);

    return status;
}

// Parses a Markdown link destination and optional title.
int md_inline_parse_link_destination(
    const char* start,
    const char* end,
    const char** destination_start,
    const char** destination_end,
    const char** title_start,
    const char** title_end
) {
    const char* cursor;
    const char* destination_finish;
    const char* quote_start;
    char quote_char;

    if (
        start == NULL ||
        end == NULL ||
        destination_start == NULL ||
        destination_end == NULL ||
        title_start == NULL ||
        title_end == NULL
    ) {
        return 0;
    }

    cursor = start;
    while (cursor < end && (*cursor == ' ' || *cursor == '\t')) {
        cursor++;
    }

    if (cursor >= end) {
        return 0;
    }

    *destination_start = cursor;

    while (cursor < end && *cursor != ' ' && *cursor != '\t') {
        cursor++;
    }

    destination_finish = cursor;
    if (destination_finish <= *destination_start) {
        return 0;
    }

    while (cursor < end && (*cursor == ' ' || *cursor == '\t')) {
        cursor++;
    }

    *destination_end = destination_finish;
    *title_start = NULL;
    *title_end = NULL;

    if (cursor >= end) {
        return 1;
    }

    if (*cursor != '"' && *cursor != '\'') {
        return 0;
    }

    quote_char = *cursor;
    quote_start = cursor + 1;
    cursor++;

    while (cursor < end) {
        if (*cursor == '\\' && (cursor + 1) < end) {
            cursor += 2;
            continue;
        }

        if (*cursor == quote_char) {
            *title_start = quote_start;
            *title_end = cursor;
            cursor++;

            while (cursor < end && (*cursor == ' ' || *cursor == '\t')) {
                cursor++;
            }

            return cursor == end;
        }

        cursor++;
    }

    return 0;
}

// Tries to parse an automatic URL from the current cursor position.
int md_inline_try_parse_auto_url(
    const char* cursor,
    const char* text_end,
    const char** url_end
) {
    const char* probe;

    if (cursor == NULL || text_end == NULL || url_end == NULL) {
        return 0;
    }

    if (
        (size_t)(text_end - cursor) < 7 ||
        (
            strncmp(cursor, "http://", 7) != 0 &&
            strncmp(cursor, "https://", 8) != 0
        )
    ) {
        return 0;
    }

    probe = cursor;
    while (probe < text_end && md_shared_is_safe_url_char(*probe)) {
        probe++;
    }

    while (
        probe > cursor &&
        (
            probe[-1] == '.' ||
            probe[-1] == ',' ||
            probe[-1] == ';' ||
            probe[-1] == ':' ||
            probe[-1] == ')' ||
            probe[-1] == '!' ||
            probe[-1] == '?'
        )
    ) {
        probe--;
    }

    if (probe <= cursor) {
        return 0;
    }

    if (probe < text_end && !md_shared_is_auto_url_boundary(*probe)) {
        return 0;
    }

    *url_end = probe;
    return 1;
}
