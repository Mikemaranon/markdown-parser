#ifndef HTML_TAGS_H
#define HTML_TAGS_H

#include "../html_builder.h"

HtmlBuilderStatus html_tags_open(
    HtmlBuilder* builder,
    const char* tag_name
);

HtmlBuilderStatus html_tags_close(
    HtmlBuilder* builder,
    const char* tag_name
);

HtmlBuilderStatus html_tags_open_with_attributes(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    const char* attribute_name_c,
    const char* attribute_value_c
);

HtmlBuilderStatus html_tags_append_void_with_attributes(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    const char* attribute_name_c,
    const char* attribute_value_c
);

HtmlBuilderStatus html_tags_append_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content
);

#endif // HTML_TAGS_H
