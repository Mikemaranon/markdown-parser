#include "html_inline.h"

#include "../tags/html_tags.h"

HtmlBuilderStatus html_inline_open_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_tags_open(builder, tag_name);
}

HtmlBuilderStatus html_inline_close_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_tags_close(builder, tag_name);
}
