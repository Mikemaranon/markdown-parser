#ifndef HTML_CORE_H
#define HTML_CORE_H

#include "../html_builder.h"

#include <stddef.h>

HtmlBuilderStatus html_core_init(HtmlBuilder* builder);

void html_core_free(HtmlBuilder* builder);

void html_core_clear(HtmlBuilder* builder);

HtmlBuilderStatus html_core_append_raw(
    HtmlBuilder* builder,
    const char* html
);

HtmlBuilderStatus html_core_append_raw_n(
    HtmlBuilder* builder,
    const char* html,
    size_t html_length
);

HtmlBuilderStatus html_core_append_char(
    HtmlBuilder* builder,
    char character
);

HtmlBuilderStatus html_core_append_newline(HtmlBuilder* builder);

HtmlBuilderStatus html_core_append_indent(
    HtmlBuilder* builder,
    size_t indent_level
);

const char* html_core_data(const HtmlBuilder* builder);

size_t html_core_length(const HtmlBuilder* builder);

HtmlBuilderStatus html_core_status_from_string_buffer(
    StringBufferStatus status
);

#endif // HTML_CORE_H
