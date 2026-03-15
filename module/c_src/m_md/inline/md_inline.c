#include "md_inline.h"

#include "../shared/md_shared.h"
#include "md_inline_links.h"
#include "md_inline_styles.h"

#include <stdlib.h>
#include <string.h>

// Transforms a full inline text string into HTML.
MarkdownTransformerStatus md_inline_transform(
    const char* text,
    HtmlBuilder* builder
) {
    if (text == NULL || builder == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    return md_inline_transform_range(
        text,
        text + strlen(text),
        builder
    );
}

// Transforms an inline character range into HTML.
MarkdownTransformerStatus md_inline_transform_range(
    const char* text_start,
    const char* text_end,
    HtmlBuilder* builder
) {
    const char* cursor;
    const char* plain_start;

    if (text_start == NULL || text_end == NULL || builder == NULL) {
        return MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT;
    }

    cursor = text_start;
    plain_start = text_start;

    while (cursor < text_end) {
        const char* closing_delimiter;
        const char* label_end;
        const char* destination_open;
        const char* destination_close;
        const char* destination_start;
        const char* destination_end;
        const char* title_start;
        const char* title_end;
        const char* auto_url_end;
        MarkdownTransformerStatus status;

        if (*cursor == '\\' && (cursor + 1) < text_end) {
            status = md_shared_flush_plain_text(builder, plain_start, cursor);
            if (status != MARKDOWN_TRANSFORMER_OK) {
                return status;
            }

            status = md_shared_append_escaped_range(builder, cursor + 1, 1);
            if (status != MARKDOWN_TRANSFORMER_OK) {
                return status;
            }

            cursor += 2;
            plain_start = cursor;
            continue;
        }

        if ((cursor + 3) <= text_end && strncmp(cursor, "***", 3) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 3, "***");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_open_tag_with_attrs(
                    builder,
                    "strong",
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_open_tag_with_attrs(
                    builder,
                    "em",
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_transform_range(
                    cursor + 3,
                    closing_delimiter,
                    builder
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_closing_tag(builder, "em");
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_closing_tag(builder, "strong");
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 3;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 3) <= text_end && strncmp(cursor, "___", 3) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 3, "___");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_open_tag_with_attrs(
                    builder,
                    "strong",
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_open_tag_with_attrs(
                    builder,
                    "em",
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    0
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_transform_range(
                    cursor + 3,
                    closing_delimiter,
                    builder
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_closing_tag(builder, "em");
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_shared_append_closing_tag(builder, "strong");
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 3;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "**", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "**");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "strong"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "__", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "__");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "strong"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "~~", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "~~");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "del"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "++", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "++");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "u"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "==", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "==");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "mark"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "^^", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "^^");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "sup"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "&&", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "&&");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "kbd"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 2) <= text_end && strncmp(cursor, "@@", 2) == 0) {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 2, "@@");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 2,
                    closing_delimiter,
                    builder,
                    "ins"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 2;
                plain_start = cursor;
                continue;
            }
        }

        if (*cursor == '*' || *cursor == '_') {
            char delimiter_text[2];

            delimiter_text[0] = *cursor;
            delimiter_text[1] = '\0';

            closing_delimiter = md_shared_find_closing_delimiter(cursor + 1, delimiter_text);

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_wrapped_inline(
                    cursor + 1,
                    closing_delimiter,
                    builder,
                    "em"
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 1;
                plain_start = cursor;
                continue;
            }
        }

        if (*cursor == '`') {
            closing_delimiter = md_shared_find_closing_delimiter(cursor + 1, "`");

            if (closing_delimiter != NULL && closing_delimiter < text_end) {
                status = md_shared_flush_plain_text(builder, plain_start, cursor);
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                status = md_inline_parse_code_span(
                    cursor + 1,
                    closing_delimiter,
                    builder
                );
                if (status != MARKDOWN_TRANSFORMER_OK) {
                    return status;
                }

                cursor = closing_delimiter + 1;
                plain_start = cursor;
                continue;
            }
        }

        if ((cursor + 1) < text_end && cursor[0] == '!' && cursor[1] == '[') {
            label_end = md_shared_find_matching_char(cursor + 2, ']');

            if (
                label_end != NULL &&
                label_end < text_end &&
                (label_end + 1) < text_end &&
                label_end[1] == '('
            ) {
                destination_open = label_end + 2;
                destination_close = md_shared_find_matching_char(destination_open, ')');

                if (
                    destination_close != NULL &&
                    destination_close <= text_end &&
                    md_inline_parse_link_destination(
                        destination_open,
                        destination_close,
                        &destination_start,
                        &destination_end,
                        &title_start,
                        &title_end
                    )
                ) {
                    status = md_shared_flush_plain_text(builder, plain_start, cursor);
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    status = md_inline_parse_image(
                        cursor + 2,
                        label_end,
                        destination_start,
                        destination_end,
                        title_start,
                        title_end,
                        builder
                    );
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    cursor = destination_close + 1;
                    plain_start = cursor;
                    continue;
                }
            }
        }

        if (*cursor == '[') {
            label_end = md_shared_find_matching_char(cursor + 1, ']');

            if (
                label_end != NULL &&
                label_end < text_end &&
                (label_end + 1) < text_end &&
                label_end[1] == '('
            ) {
                destination_open = label_end + 2;
                destination_close = md_shared_find_matching_char(destination_open, ')');

                if (
                    destination_close != NULL &&
                    destination_close <= text_end &&
                    md_inline_parse_link_destination(
                        destination_open,
                        destination_close,
                        &destination_start,
                        &destination_end,
                        &title_start,
                        &title_end
                    )
                ) {
                    status = md_shared_flush_plain_text(builder, plain_start, cursor);
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    status = md_inline_parse_link(
                        cursor + 1,
                        label_end,
                        destination_start,
                        destination_end,
                        title_start,
                        title_end,
                        builder
                    );
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    cursor = destination_close + 1;
                    plain_start = cursor;
                    continue;
                }
            }
        }

        if (*cursor == '<') {
            const char* closing_angle = md_shared_find_matching_char(cursor + 1, '>');

            if (closing_angle != NULL && closing_angle < text_end) {
                int is_autolink = 0;
                const char* probe = cursor + 1;

                if ((closing_angle - probe) > 0 && strncmp(probe, "http://", 7) == 0) {
                    is_autolink = 1;
                } else if ((closing_angle - probe) > 0 && strncmp(probe, "https://", 8) == 0) {
                    is_autolink = 1;
                } else {
                    const char* at_sign = probe;

                    while (at_sign < closing_angle) {
                        if (*at_sign == '@') {
                            is_autolink = 1;
                            break;
                        }

                        if (!md_shared_is_email_char(*at_sign) && *at_sign != '@') {
                            break;
                        }

                        at_sign++;
                    }
                }

                if (is_autolink) {
                    status = md_shared_flush_plain_text(builder, plain_start, cursor);
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    status = md_inline_parse_autolink(
                        cursor + 1,
                        closing_angle,
                        builder
                    );
                    if (status != MARKDOWN_TRANSFORMER_OK) {
                        return status;
                    }

                    cursor = closing_angle + 1;
                    plain_start = cursor;
                    continue;
                }
            }
        }

        if (md_inline_try_parse_auto_url(cursor, text_end, &auto_url_end)) {
            char* href_copy;
            char* text_copy;

            status = md_shared_flush_plain_text(builder, plain_start, cursor);
            if (status != MARKDOWN_TRANSFORMER_OK) {
                return status;
            }

            href_copy = md_shared_copy_range(
                cursor,
                (size_t)(auto_url_end - cursor)
            );
            if (href_copy == NULL) {
                return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
            }

            text_copy = md_shared_copy_range(
                cursor,
                (size_t)(auto_url_end - cursor)
            );
            if (text_copy == NULL) {
                free(href_copy);
                return MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED;
            }

            status = md_shared_append_open_tag_with_attrs(
                builder,
                "a",
                "href",
                href_copy,
                NULL,
                NULL,
                0
            );
            if (status == MARKDOWN_TRANSFORMER_OK) {
                status = md_shared_status_from_html_builder(
                    html_builder_append_escaped(builder, text_copy)
                );
            }
            if (status == MARKDOWN_TRANSFORMER_OK) {
                status = md_shared_append_closing_tag(builder, "a");
            }

            free(href_copy);
            free(text_copy);

            if (status != MARKDOWN_TRANSFORMER_OK) {
                return status;
            }

            cursor = auto_url_end;
            plain_start = cursor;
            continue;
        }

        cursor++;
    }

    return md_shared_flush_plain_text(builder, plain_start, text_end);
}
