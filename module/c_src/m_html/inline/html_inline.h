#ifndef HTML_INLINE_H
#define HTML_INLINE_H

#include "../html_builder.h"

HtmlBuilderStatus html_inline_open_tag(
    HtmlBuilder* builder,
    const char* tag_name
);

HtmlBuilderStatus html_inline_close_tag(
    HtmlBuilder* builder,
    const char* tag_name
);

#endif // HTML_INLINE_H
