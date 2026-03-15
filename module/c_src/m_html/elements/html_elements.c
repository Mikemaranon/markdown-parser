#include "html_elements.h"

#include "../escape/html_escape.h"
#include "../tags/html_tags.h"

HtmlBuilderStatus html_elements_append_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title,
    const char* label
) {
    HtmlBuilderStatus status;

    if (
        builder == NULL ||
        href == NULL ||
        label == NULL
    ) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_tags_open_with_attributes(
        builder,
        "a",
        "href",
        href,
        "title",
        title,
        NULL,
        NULL
    );
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_escape_append_text(builder, label);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_tags_close(builder, "a");
}

HtmlBuilderStatus html_elements_open_link(
    HtmlBuilder* builder,
    const char* href,
    const char* title
) {
    if (builder == NULL || href == NULL) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_tags_open_with_attributes(
        builder,
        "a",
        "href",
        href,
        "title",
        title,
        NULL,
        NULL
    );
}

HtmlBuilderStatus html_elements_close_link(HtmlBuilder* builder) {
    return html_tags_close(builder, "a");
}

HtmlBuilderStatus html_elements_append_image(
    HtmlBuilder* builder,
    const char* source,
    const char* alt,
    const char* title
) {
    if (
        builder == NULL ||
        source == NULL ||
        alt == NULL
    ) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    return html_tags_append_void_with_attributes(
        builder,
        "img",
        "src",
        source,
        "alt",
        alt,
        "title",
        title
    );
}
