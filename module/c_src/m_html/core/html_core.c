#include "html_core.h"

HtmlBuilderStatus html_core_status_from_string_buffer(
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

HtmlBuilderStatus html_core_init(HtmlBuilder* builder) {
    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_core_status_from_string_buffer(
        string_buffer_init(&builder->buffer)
    );
}

void html_core_free(HtmlBuilder* builder) {
    if (builder == NULL) {
        return;
    }

    string_buffer_free(&builder->buffer);
}

void html_core_clear(HtmlBuilder* builder) {
    if (builder == NULL) {
        return;
    }

    string_buffer_clear(&builder->buffer);
}

HtmlBuilderStatus html_core_append_raw(
    HtmlBuilder* builder,
    const char* html
) {
    if (builder == NULL || html == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_core_status_from_string_buffer(
        string_buffer_append(&builder->buffer, html)
    );
}

HtmlBuilderStatus html_core_append_raw_n(
    HtmlBuilder* builder,
    const char* html,
    size_t html_length
) {
    if (builder == NULL || html == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_core_status_from_string_buffer(
        string_buffer_append_n(&builder->buffer, html, html_length)
    );
}

HtmlBuilderStatus html_core_append_char(
    HtmlBuilder* builder,
    char character
) {
    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_core_status_from_string_buffer(
        string_buffer_append_char(&builder->buffer, character)
    );
}

HtmlBuilderStatus html_core_append_newline(HtmlBuilder* builder) {
    return html_core_append_char(builder, '\n');
}

HtmlBuilderStatus html_core_append_indent(
    HtmlBuilder* builder,
    size_t indent_level
) {
    size_t index;
    HtmlBuilderStatus status;

    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    for (index = 0; index < indent_level; index++) {
        status = html_core_append_raw(builder, "    ");
        if (status != HTML_BUILDER_OK) {
            return status;
        }
    }

    return HTML_BUILDER_OK;
}

const char* html_core_data(const HtmlBuilder* builder) {
    if (builder == NULL) {
        return NULL;
    }

    return string_buffer_data(&builder->buffer);
}

size_t html_core_length(const HtmlBuilder* builder) {
    if (builder == NULL) {
        return 0;
    }

    return string_buffer_length(&builder->buffer);
}
