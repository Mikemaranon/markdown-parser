#include "html_blocks.h"

#include "../tags/html_tags.h"

static int html_blocks_is_valid_heading_level(size_t level) {
    return level >= 1 && level <= 6;
}

static void html_blocks_build_heading_tag_name(
    size_t level,
    char tag_name[3]
) {
    tag_name[0] = 'h';
    tag_name[1] = (char)('0' + level);
    tag_name[2] = '\0';
}

HtmlBuilderStatus html_blocks_open_paragraph(HtmlBuilder* builder) {
    return html_tags_open(builder, "p");
}

HtmlBuilderStatus html_blocks_close_paragraph(HtmlBuilder* builder) {
    return html_tags_close(builder, "p");
}

HtmlBuilderStatus html_blocks_open_heading(
    HtmlBuilder* builder,
    size_t level
) {
    char tag_name[3];

    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    if (!html_blocks_is_valid_heading_level(level)) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    html_blocks_build_heading_tag_name(level, tag_name);
    return html_tags_open(builder, tag_name);
}

HtmlBuilderStatus html_blocks_close_heading(
    HtmlBuilder* builder,
    size_t level
) {
    char tag_name[3];

    if (builder == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    if (!html_blocks_is_valid_heading_level(level)) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    html_blocks_build_heading_tag_name(level, tag_name);
    return html_tags_close(builder, tag_name);
}
