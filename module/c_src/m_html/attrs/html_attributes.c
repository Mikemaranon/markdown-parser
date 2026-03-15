#include "html_attributes.h"

#include "../core/html_core.h"
#include "../escape/html_escape.h"

#include <string.h>

HtmlBuilderStatus html_attributes_append(
    HtmlBuilder* builder,
    const char* attribute_name,
    const char* attribute_value
) {
    HtmlBuilderStatus status;

    if (
        builder == NULL ||
        attribute_name == NULL ||
        attribute_value == NULL
    ) {
        return HTML_BUILDER_ERROR_NULL_ARGUMENT;
    }

    status = html_core_append_char(builder, ' ');
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_core_append_raw_n(
        builder,
        attribute_name,
        strlen(attribute_name)
    );
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_core_append_raw(builder, "=\"");
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    status = html_escape_append_attribute_value(builder, attribute_value);
    if (status != HTML_BUILDER_OK) {
        return status;
    }

    return html_core_append_char(builder, '"');
}