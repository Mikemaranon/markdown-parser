#include "html_tags.h"

#include "../attrs/html_attributes.h"
#include "../core/html_core.h"
#include "../escape/html_escape.h"

#include <string.h>

static HtmlBuilderStatus html_tags_append_optional_attribute(
    HtmlBuilder* builder,
    const char* attribute_name,
    const char* attribute_value
) {
    if (attribute_name == NULL || attribute_value == NULL) {
        return HTML_BUILDER_OK;
    }

    return html_attributes_append(
        builder,
        attribute_name,
        attribute_value
    );
}

HtmlBuilderStatus html_tags_open(
    HtmlBuilder* builder,
    const char* tag_name
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_core_append_char(builder, '<');
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_core_append_raw_n(builder, tag_name, strlen(tag_name));
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_core_append_char(builder, '>');
}

HtmlBuilderStatus html_tags_close(
    HtmlBuilder* builder,
    const char* tag_name
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_core_append_raw(builder, "</");
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_core_append_raw_n(builder, tag_name, strlen(tag_name));
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_core_append_char(builder, '>');
}

HtmlBuilderStatus html_tags_open_with_attributes(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* attribute_name_a,
    const char* attribute_value_a,
    const char* attribute_name_b,
    const char* attribute_value_b,
    const char* attribute_name_c,
    const char* attribute_value_c
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_core_append_char(builder, '<');
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_core_append_raw_n(builder, tag_name, strlen(tag_name));
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_tags_append_optional_attribute(
        builder,
        attribute_name_a,
        attribute_value_a
    );
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_tags_append_optional_attribute(
        builder,
        attribute_name_b,
        attribute_value_b
    );
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_tags_append_optional_attribute(
        builder,
        attribute_name_c,
        attribute_value_c
    );
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_core_append_char(builder, '>');
}

HtmlBuilderStatus html_tags_append_void_with_attributes(
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

HtmlBuilderStatus html_tags_append_text_element(
    HtmlBuilder* builder,
    const char* tag_name,
    const char* text_content
) {
    HtmlBuilderStatus status;

    if (builder == NULL || tag_name == NULL || text_content == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_tags_open(builder, tag_name);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_escape_append_text(builder, text_content);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_tags_close(builder, tag_name);
}
