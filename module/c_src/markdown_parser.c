#include "lib/markdown_parser.h"

#include "m_html/html_builder.h"
#include "m_md/markdown_transformer.h"

#include <stdlib.h>
#include <string.h>

#define MARKDOWN_PARSER_DEFAULT_TITLE "Document"

struct MarkdownParser {
    char* css_content;
};

static char* markdown_parser_duplicate_string(const char* value) {
    size_t length;
    char* copy;

    if (value == NULL) {
        return NULL;
    }

    length = strlen(value);
    copy = (char*)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, value, length + 1);
    return copy;
}

static MarkdownParserStatus markdown_parser_status_from_html_builder(
    HtmlBuilderStatus status
) {
    switch (status) {
        case HTML_BUILDER_OK:
            return MARKDOWN_PARSER_OK;
        case HTML_BUILDER_ERROR_NULL_ARGUMENT:
            return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
        case HTML_BUILDER_ERROR_ALLOCATION_FAILED:
            return MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED;
        default:
            return MARKDOWN_PARSER_ERROR_RENDER_FAILED;
    }
}

static MarkdownParserStatus markdown_parser_status_from_transformer(
    MarkdownTransformerStatus status
) {
    switch (status) {
        case MARKDOWN_TRANSFORMER_OK:
            return MARKDOWN_PARSER_OK;
        case MARKDOWN_TRANSFORMER_ERROR_NULL_ARGUMENT:
            return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
        case MARKDOWN_TRANSFORMER_ERROR_ALLOCATION_FAILED:
            return MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED;
        case MARKDOWN_TRANSFORMER_ERROR_TRANSFORM_FAILED:
        default:
            return MARKDOWN_PARSER_ERROR_RENDER_FAILED;
    }
}

static MarkdownParserStatus markdown_parser_build_html_fragment(
    const char* markdown_content,
    char** output_html
) {
    HtmlBuilder builder;
    MarkdownTransformerStatus transformer_status;
    HtmlBuilderStatus builder_status;
    char* html_copy;

    if (markdown_content == NULL || output_html == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    *output_html = NULL;

    builder_status = html_builder_init(&builder);
    if (builder_status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(builder_status);
    }

    transformer_status = markdown_transformer_transform(markdown_content, &builder);
    if (transformer_status != MARKDOWN_TRANSFORMER_OK) {
        html_builder_free(&builder);
        return markdown_parser_status_from_transformer(transformer_status);
    }

    html_copy = markdown_parser_duplicate_string(html_builder_data(&builder));
    html_builder_free(&builder);

    if (html_copy == NULL) {
        return MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED;
    }

    *output_html = html_copy;
    return MARKDOWN_PARSER_OK;
}

static MarkdownParserStatus markdown_parser_append_document_start(
    HtmlBuilder* builder,
    const char* title,
    const char* css_content
) {
    HtmlBuilderStatus status;
    const char* document_title;

    if (builder == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    document_title = (title != NULL) ? title : MARKDOWN_PARSER_DEFAULT_TITLE;

    status = html_builder_append_raw(builder, "<!DOCTYPE html>\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    status = html_builder_append_raw(builder, "<html>\n<head>\n<meta charset=\"UTF-8\">\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    status = html_builder_append_raw(builder, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    status = html_builder_append_raw(builder, "<title>");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    status = html_builder_append_escaped(builder, document_title);
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    status = html_builder_append_raw(builder, "</title>\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    if (css_content != NULL && css_content[0] != '\0') {
        status = html_builder_append_raw(builder, "<style>\n");
        if (status != HTML_BUILDER_OK) {
            return markdown_parser_status_from_html_builder(status);
        }

        status = html_builder_append_raw(builder, css_content);
        if (status != HTML_BUILDER_OK) {
            return markdown_parser_status_from_html_builder(status);
        }

        status = html_builder_append_raw(builder, "\n</style>\n");
        if (status != HTML_BUILDER_OK) {
            return markdown_parser_status_from_html_builder(status);
        }
    }

    status = html_builder_append_raw(builder, "</head>\n<body>\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    return MARKDOWN_PARSER_OK;
}

static MarkdownParserStatus markdown_parser_append_document_end(
    HtmlBuilder* builder
) {
    HtmlBuilderStatus status;

    if (builder == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    status = html_builder_append_raw(builder, "</body>\n</html>\n");
    if (status != HTML_BUILDER_OK) {
        return markdown_parser_status_from_html_builder(status);
    }

    return MARKDOWN_PARSER_OK;
}

MarkdownParser* markdown_parser_create(void) {
    MarkdownParser* parser;

    parser = (MarkdownParser*)malloc(sizeof(MarkdownParser));
    if (parser == NULL) {
        return NULL;
    }

    parser->css_content = NULL;
    return parser;
}

void markdown_parser_destroy(MarkdownParser* parser) {
    if (parser == NULL) {
        return;
    }

    free(parser->css_content);
    parser->css_content = NULL;

    free(parser);
}

MarkdownParserStatus markdown_parser_set_css(
    MarkdownParser* parser,
    const char* css_content
) {
    char* css_copy;

    if (parser == NULL || css_content == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    css_copy = markdown_parser_duplicate_string(css_content);
    if (css_copy == NULL) {
        return MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED;
    }

    free(parser->css_content);
    parser->css_content = css_copy;

    return MARKDOWN_PARSER_OK;
}

MarkdownParserStatus markdown_parser_clear_css(MarkdownParser* parser) {
    if (parser == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    free(parser->css_content);
    parser->css_content = NULL;

    return MARKDOWN_PARSER_OK;
}

MarkdownParserStatus markdown_parser_transform(
    MarkdownParser* parser,
    const char* markdown_content,
    char** output_html
) {
    if (parser == NULL || markdown_content == NULL || output_html == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    (void)parser;

    return markdown_parser_build_html_fragment(markdown_content, output_html);
}

MarkdownParserStatus markdown_parser_to_html_document(
    MarkdownParser* parser,
    const char* markdown_content,
    const char* title,
    char** output_html
) {
    HtmlBuilder builder;
    HtmlBuilderStatus builder_status;
    MarkdownParserStatus parser_status;
    char* fragment_html;
    char* document_html;

    if (parser == NULL || markdown_content == NULL || output_html == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    *output_html = NULL;
    fragment_html = NULL;

    parser_status = markdown_parser_build_html_fragment(
        markdown_content,
        &fragment_html
    );
    if (parser_status != MARKDOWN_PARSER_OK) {
        return parser_status;
    }

    builder_status = html_builder_init(&builder);
    if (builder_status != HTML_BUILDER_OK) {
        free(fragment_html);
        return markdown_parser_status_from_html_builder(builder_status);
    }

    parser_status = markdown_parser_append_document_start(
        &builder,
        title,
        parser->css_content
    );
    if (parser_status != MARKDOWN_PARSER_OK) {
        html_builder_free(&builder);
        free(fragment_html);
        return parser_status;
    }

    builder_status = html_builder_append_raw(&builder, fragment_html);
    if (builder_status != HTML_BUILDER_OK) {
        html_builder_free(&builder);
        free(fragment_html);
        return markdown_parser_status_from_html_builder(builder_status);
    }

    parser_status = markdown_parser_append_document_end(&builder);
    if (parser_status != MARKDOWN_PARSER_OK) {
        html_builder_free(&builder);
        free(fragment_html);
        return parser_status;
    }

    document_html = markdown_parser_duplicate_string(html_builder_data(&builder));

    html_builder_free(&builder);
    free(fragment_html);

    if (document_html == NULL) {
        return MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED;
    }

    *output_html = document_html;
    return MARKDOWN_PARSER_OK;
}

void markdown_parser_free_string(char* value) {
    free(value);
}
