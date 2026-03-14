#ifndef MARKDOWN_PARSER_H
#define MARKDOWN_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// Opaque parser structure.
// The internal implementation is hidden from the public API.
typedef struct MarkdownParser MarkdownParser;

// Status codes returned by public API functions.
typedef enum MarkdownParserStatus {
    MARKDOWN_PARSER_OK = 0,
    MARKDOWN_PARSER_ERROR_NULL_ARGUMENT = 1,
    MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED = 2,
    MARKDOWN_PARSER_ERROR_RENDER_FAILED = 3
} MarkdownParserStatus;


// Creates a new parser instance.

// [Returns]
//      - A valid MarkdownParser pointer on success
//      - NULL on allocation failure

MarkdownParser* markdown_parser_create(void);


// Destroys a parser instance and releases all owned resources.
// Safe to call with NULL.

void markdown_parser_destroy(MarkdownParser* parser);


// Replaces the current CSS content stored in the parser.

// [Parameters]
//      - parser: parser instance
//      - css_content: UTF-8 CSS string
//
// [Returns]
//   - MARKDOWN_PARSER_OK on success
//   - error code otherwise

MarkdownParserStatus markdown_parser_set_css(
    MarkdownParser* parser,
    const char* css_content
);


// Clears the current CSS content stored in the parser.

// [Parameters]
//      - parser: parser instance
//
// [Returns]
//      - MARKDOWN_PARSER_OK on success
//      - error code otherwise

MarkdownParserStatus markdown_parser_clear_css(MarkdownParser* parser);


//Transforms Markdown content into an HTML fragment.
// The output string is allocated by the library and must be released with
// markdown_parser_free_string().

// [Parameters]
//   - parser: parser instance
//   - markdown_content: UTF-8 Markdown input
//   - output_html: output pointer that receives the allocated HTML string
//
// [Returns]
//   - MARKDOWN_PARSER_OK on success
//   - error code otherwise

MarkdownParserStatus markdown_parser_transform(
    MarkdownParser* parser,
    const char* markdown_content,
    char** output_html
);


// Builds a full HTML document from Markdown content and the currently loaded CSS.
// The output string is allocated by the library and must be released with
// markdown_parser_free_string().

// [Parameters]
//   - parser: parser instance
//   - markdown_content: UTF-8 Markdown input
//   - title: UTF-8 document title, can be NULL to use a default title
//   - output_html: output pointer that receives the allocated HTML string
//
// [Returns]
//   - MARKDOWN_PARSER_OK on success
//   - error code otherwise

MarkdownParserStatus markdown_parser_to_html_document(
    MarkdownParser* parser,
    const char* markdown_content,
    const char* title,
    char** output_html
);


// Frees a string allocated and returned by this library.
// Safe to call with NULL.

void markdown_parser_free_string(char* value);

#ifdef __cplusplus
}
#endif

#endif // MARKDOWN_PARSER_H