#ifndef HTML_ESCAPE_H
#define HTML_ESCAPE_H

#include "../html_builder.h"

#include <stddef.h>

HtmlBuilderStatus html_escape_append_text(
    HtmlBuilder* builder,
    const char* text
);

HtmlBuilderStatus html_escape_append_text_n(
    HtmlBuilder* builder,
    const char* text,
    size_t text_length
);

HtmlBuilderStatus html_escape_append_attribute_value(
    HtmlBuilder* builder,
    const char* text
);

#endif // HTML_ESCAPE_H
