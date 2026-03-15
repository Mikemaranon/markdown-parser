#include "html_escape.h"

#include "../core/html_core.h"

#include <string.h>

static HtmlBuilderStatus html_escape_append_sequence(
    HtmlBuilder* builder,
    char character
) {
    switch (character) {
        case '&':
            return html_core_append_raw(builder, "&amp;");
        case '<':
            return html_core_append_raw(builder, "&lt;");
        case '>':
            return html_core_append_raw(builder, "&gt;");
        case '"':
            return html_core_append_raw(builder, "&quot;");
        case '\'':
            return html_core_append_raw(builder, "&#39;");
        default:
            return html_core_append_char(builder, character);
    }
}

HtmlBuilderStatus html_escape_append_text(
    HtmlBuilder* builder,
    const char* text
) {
    if (builder == NULL || text == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_escape_append_text_n(builder, text, strlen(text));
}

HtmlBuilderStatus html_escape_append_text_n(
    HtmlBuilder* builder,
    const char* text,
    size_t text_length
) {
    size_t index;
    HtmlBuilderStatus status;

    if (builder == NULL || text == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    for (index = 0; index < text_length; index++) {
        status = html_escape_append_sequence(builder, text[index]);
        if (status != HTML_BUILDER_OK) {
            return status;
        }
    }

    return HTML_BUILDER_OK;
}

HtmlBuilderStatus html_escape_append_attribute_value(
    HtmlBuilder* builder,
    const char* text
) {
    return html_escape_append_text(builder, text);
}
