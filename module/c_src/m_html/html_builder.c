#include "html_builder.h"

#include <string.h>

static HtmlBuilderStatus html_builder_status_from_string_buffer(
    StringBufferStatus status
) {
    switch (status) {
        case STRING_BUFFER_OK:
            return HTML_BUILDER_OK;
        case STRING_BUFFER_ERROR_NULL_ARGUMENT:
            return HTML_BUILDER_ERROR_NULL_ARGUMENT;
        case STRING_BUFFER_ERROR_ALLOCATION_FAILED:
            return HTML_BUILDER_ERROR_ALLOCATION_FAILED;
        default:
            return HTML_BUILDER_ERROR_ALLOCATION_FAILED;
    }
}

static HtmlBuilderStatus html_builder_append_escape_sequence(
    HtmlBuilder* builder,
    char character
) {
    switch (character) {
        case '&':
            return html_builder_append_raw(builder, "&amp;");
        case '<':
            return html_builder_append_raw(builder, "&lt;");
        case '>':
            return html_builder_append_raw(builder, "&gt;");
        case '"':
            return html_builder_append_raw(builder, "&quot;");
        case '\'':
            return html_builder_append_raw(builder, "&#39;");
        default:
            return html_builder_status_from_string_buffer(
                string_buffer_append_char(&builder->buffer, character)
            );
    }
}

HtmlBuilderStatus html_builder_init(HtmlBuilder* builder) {
    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_builder_status_from_string_buffer(
        string_buffer_init(&builder->buffer)
    );
}

void html_builder_free(HtmlBuilder* builder) {
    if (builder == NULL) {
        return;
    }

    string_buffer_free(&builder->buffer);
}

void html_builder_clear(HtmlBuilder* builder) {
    if (builder == NULL) {
        return;
    }

    string_buffer_clear(&builder->buffer);
}

HtmlBuilderStatus html_builder_append_raw(
    HtmlBuilder* builder,
    const char* html
) {
    if (builder == NULL || html == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_builder_status_from_string_buffer(
        string_buffer_append(&builder->buffer, html)
    );
}

HtmlBuilderStatus html_builder_append_raw_n(
    HtmlBuilder* builder,
    const char* html,
    size_t html_length
) {
    if (builder == NULL || html == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_builder_status_from_string_buffer(
        string_buffer_append_n(&builder->buffer, html, html_length)
    );
}

HtmlBuilderStatus html_builder_append_escaped(
    HtmlBuilder* builder,
    const char* text
) {
    size_t index;
    size_t text_length;
    HtmlBuilderStatus status;

    if (builder == NULL || text == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    text_length = strlen(text);

    for (index = 0; index < text_length; index++) {
        status = html_builder_append_escape_sequence(builder, text[index]);
        if (status != HTML_BUILDER_OK) {
            return status;
        }
    }

    return HTML_BUILDER_OK;
}

HtmlBuilderStatus html_builder_open_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_builder_append_raw(builder, "<");
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_builder_append_raw(builder, tag_name);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_builder_append_raw(builder, ">");
}

HtmlBuilderStatus html_builder_close_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_builder_append_raw(builder, "</");
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_builder_append_raw(builder, tag_name);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_builder_append_raw(builder, ">");
}

HtmlBuilderStatus html_builder_append_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL || text_content == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_builder_open_tag(builder, tag_name);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_builder_append_escaped(builder, text_content);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_builder_close_tag(builder, tag_name);
}

const char* html_builder_data(const HtmlBuilder* builder) {
    if (builder == NULL) {
        return NULL;
    }

    return string_buffer_data(&builder->buffer);
}

size_t html_builder_length(const HtmlBuilder* builder) {
    if (builder == NULL) {
        return 0;
    }

    return string_buffer_length(&builder->buffer);
}


HtmlBuilderStatus html_builder_append_newline(HtmlBuilder* builder) {
    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_builder_append_raw(builder, "\n");
}

HtmlBuilderStatus html_builder_append_indent(
    HtmlBuilder* builder,
    size_t indent_level
) {
    size_t index;
    HtmlBuilderStatus status;

    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    for (index = 0; index < indent_level; index++) {
        status = html_builder_append_raw(builder, "    ");
        if (status != HTML_BUILDER_OK) {
            return status;
        }
    }

    return HTML_BUILDER_OK;
}

HtmlBuilderStatus html_builder_append_block_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content,
    size_t indent_level
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL || text_content == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_builder_append_indent(builder, indent_level);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_builder_append_text_element(builder, tag_name, text_content);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_builder_append_newline(builder);
}
