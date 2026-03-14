#include "markdown_parser.h"

#include <stdlib.h>
#include <string.h>

struct MarkdownParser {
    char* css_content;
};

static char* mp_duplicate_string(const char* value) {
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

    css_copy = mp_duplicate_string(css_content);
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
    (void)parser;
    (void)markdown_content;

    if (output_html == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    *output_html = NULL;

    if (parser == NULL || markdown_content == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    return MARKDOWN_PARSER_ERROR_RENDER_FAILED;
}


MarkdownParserStatus markdown_parser_to_html_document(
    MarkdownParser* parser,
    const char* markdown_content,
    const char* title,
    char** output_html
) {
    (void)parser;
    (void)markdown_content;
    (void)title;

    if (output_html == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    *output_html = NULL;

    if (parser == NULL || markdown_content == NULL) {
        return MARKDOWN_PARSER_ERROR_NULL_ARGUMENT;
    }

    return MARKDOWN_PARSER_ERROR_RENDER_FAILED;
}


void markdown_parser_free_string(char* value) {
    free(value);
}