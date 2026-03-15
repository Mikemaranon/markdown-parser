#ifndef HTML_ATTRIBUTES_H
#define HTML_ATTRIBUTES_H

#include "../html_builder.h"

HtmlBuilderStatus html_attributes_append(
    HtmlBuilder* builder,
    const char* attribute_name,
    const char* attribute_value
);

#endif // HTML_ATTRIBUTES_H
