#include "html_builder.h"

#include "blocks/html_blocks.h"
#include "core/html_core.h"
#include "elements/html_elements.h"
#include "escape/html_escape.h"
#include "inline/html_inline.h"
#include "tags/html_tags.h"

HtmlBuilderStatus html_builder_init(HtmlBuilder* builder) {
    return html_core_init(builder);
}

void html_builder_free(HtmlBuilder* builder) {
    html_core_free(builder);
}

void html_builder_clear(HtmlBuilder* builder) {
    html_core_clear(builder);
}

HtmlBuilderStatus html_builder_append_raw(
    HtmlBuilder* builder,
    const char* html
) {
    return html_core_append_raw(builder, html);
}

HtmlBuilderStatus html_builder_append_raw_n(
    HtmlBuilder* builder,
    const char* html,
    size_t html_length
) {
    return html_core_append_raw_n(builder, html, html_length);
}

HtmlBuilderStatus html_builder_append_escaped(
    HtmlBuilder* builder,
    const char* text
) {
    return html_escape_append_text(builder, text);
}

HtmlBuilderStatus html_builder_append_newline(HtmlBuilder* builder) {
    return html_core_append_newline(builder);
}

HtmlBuilderStatus html_builder_append_indent(
    HtmlBuilder* builder,
    size_t indent_level
) {
    return html_core_append_indent(builder, indent_level);
}

HtmlBuilderStatus html_builder_open_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_tags_open(builder, tag_name);
}

HtmlBuilderStatus html_builder_close_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_tags_close(builder, tag_name);
}

HtmlBuilderStatus html_builder_open_tag_with_attributes(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    const char* attribute_name_c,
    const char* attribute_value_c
) {
    return html_tags_open_with_attributes(
        builder,
        tag_name,
        attribute_name_a,
        attribute_value_a,
        attribute_name_b,
        attribute_value_b,
        attribute_name_c,
        attribute_value_c
    );
}

HtmlBuilderStatus html_builder_append_void_tag_with_attributes(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    const char* attribute_name_c,
    const char* attribute_value_c
) {
    return html_tags_append_void_with_attributes(
        builder,
        tag_name,
        attribute_name_a,
        attribute_value_a,
        attribute_name_b,
        attribute_value_b,
        attribute_name_c,
        attribute_value_c
    );
}

HtmlBuilderStatus html_builder_append_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content
) {
    return html_tags_append_text_element(
        builder,
        tag_name,
        text_content
    );
}

HtmlBuilderStatus html_builder_open_paragraph(HtmlBuilder* builder) {
    return html_blocks_open_paragraph(builder);
}

HtmlBuilderStatus html_builder_close_paragraph(HtmlBuilder* builder) {
    return html_blocks_close_paragraph(builder);
}

HtmlBuilderStatus html_builder_open_heading(
    HtmlBuilder* builder,
    size_t level
) {
    return html_blocks_open_heading(builder, level);
}

HtmlBuilderStatus html_builder_close_heading(
    HtmlBuilder* builder,
    size_t level
) {
    return html_blocks_close_heading(builder, level);
}

HtmlBuilderStatus html_builder_open_inline_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_inline_open_tag(builder, tag_name);
}

HtmlBuilderStatus html_builder_close_inline_tag(
    HtmlBuilder* builder,
    const char* tag_name
) {
    return html_inline_close_tag(builder, tag_name);
}

HtmlBuilderStatus html_builder_append_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title,
    const char* label
) {
    return html_elements_append_link(builder, href, title, label);
}

HtmlBuilderStatus html_builder_open_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title
) {
    return html_elements_open_link(builder, href, title);
}

HtmlBuilderStatus html_builder_close_link(HtmlBuilder* builder) {
    return html_elements_close_link(builder);
}

HtmlBuilderStatus html_builder_append_image(
    HtmlBuilder* builder,
    const char* source,
    const char* alt,
    const char* title
) {
    return html_elements_append_image(builder, source, alt, title);
}

const char* html_builder_data(const HtmlBuilder* builder) {
    return html_core_data(builder);
}

size_t html_builder_length(const HtmlBuilder* builder) {
    return html_core_length(builder);
}

HtmlBuilderStatus html_builder_append_block_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content,
    size_t indent_level
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL || text_content == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_core_append_indent(builder, indent_level);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_tags_append_text_element(builder, tag_name, text_content);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_core_append_newline(builder);
}