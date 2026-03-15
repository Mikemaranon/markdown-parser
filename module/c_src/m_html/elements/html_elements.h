#ifndef HTML_ELEMENTS_H
#define HTML_ELEMENTS_H

#include "../html_builder.h"

HtmlBuilderStatus html_elements_append_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title,
    const char* label
);

HtmlBuilderStatus html_elements_open_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title
);

HtmlBuilderStatus html_elements_close_link(HtmlBuilder* builder);

HtmlBuilderStatus html_elements_append_image(
    HtmlBuilder* builder,
    const char* source,
    const char* alt,
    const char* title
);

#endif // HTML_ELEMENTS_H
