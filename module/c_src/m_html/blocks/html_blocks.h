#ifndef HTML_BLOCKS_H
#define HTML_BLOCKS_H

#include "../html_builder.h"

#include <stddef.h>

HtmlBuilderStatus html_blocks_open_paragraph(HtmlBuilder* builder);

HtmlBuilderStatus html_blocks_close_paragraph(HtmlBuilder* builder);

HtmlBuilderStatus html_blocks_open_heading(
    HtmlBuilder* builder,
    size_t level
);

HtmlBuilderStatus html_blocks_close_heading(
    HtmlBuilder* builder,
    size_t level
);

#endif // HTML_BLOCKS_H
