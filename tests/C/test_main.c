#include "lib/markdown_parser.h"

#include <stdio.h>
#include <stdlib.h>

#define TEST_MARKDOWN_FILE "tests/samples/example.md"
#define TEST_CSS_FILE "tests/samples/example.css"

static void print_status(const char* label, MarkdownParserStatus status) {
    printf("%s: %d\n", label, (int)status);
}

static char* read_text_file(const char* file_path) {
    FILE* file;
    long file_size;
    size_t bytes_read;
    char* buffer;

    if (file_path == NULL) {
        return NULL;
    }

    file = fopen(file_path, "rb");
    if (file == NULL) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    file_size = ftell(file);
    if (file_size < 0) {
        fclose(file);
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    buffer = (char*)malloc((size_t)file_size + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    bytes_read = fread(buffer, 1, (size_t)file_size, file);
    fclose(file);

    if (bytes_read != (size_t)file_size) {
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';
    return buffer;
}

int main(void) {
    MarkdownParser* parser;
    MarkdownParserStatus status;
    char* markdown_content;
    char* css_content;
    char* fragment_html;
    char* document_html;

    markdown_content = read_text_file(TEST_MARKDOWN_FILE);
    if (markdown_content == NULL) {
        printf("Failed to read markdown file: %s\n", TEST_MARKDOWN_FILE);
        return 1;
    }

    css_content = read_text_file(TEST_CSS_FILE);
    if (css_content == NULL) {
        printf("Failed to read CSS file: %s\n", TEST_CSS_FILE);
        free(markdown_content);
        return 1;
    }

    parser = markdown_parser_create();
    if (parser == NULL) {
        printf("Failed to create parser.\n");
        free(markdown_content);
        free(css_content);
        return 1;
    }

    status = markdown_parser_set_css(parser, css_content);
    if (status != MARKDOWN_PARSER_OK) {
        print_status("markdown_parser_set_css failed", status);
        markdown_parser_destroy(parser);
        free(markdown_content);
        free(css_content);
        return 1;
    }

    fragment_html = NULL;
    status = markdown_parser_transform(parser, markdown_content, &fragment_html);
    if (status != MARKDOWN_PARSER_OK) {
        print_status("markdown_parser_transform failed", status);
        markdown_parser_destroy(parser);
        free(markdown_content);
        free(css_content);
        return 1;
    }

    printf("=== Markdown Source File ===\n");
    printf("%s\n", TEST_MARKDOWN_FILE);

    printf("\n=== HTML Fragment ===\n");
    printf("%s\n", fragment_html);
    markdown_parser_free_string(fragment_html);

    document_html = NULL;
    status = markdown_parser_to_html_document(
        parser,
        markdown_content,
        "Markdown Parser Test",
        &document_html
    );
    if (status != MARKDOWN_PARSER_OK) {
        print_status("markdown_parser_to_html_document failed", status);
        markdown_parser_destroy(parser);
        free(markdown_content);
        free(css_content);
        return 1;
    }

    printf("\n=== Full HTML Document ===\n");
    printf("%s\n", document_html);
    markdown_parser_free_string(document_html);

    markdown_parser_destroy(parser);
    free(markdown_content);
    free(css_content);

    return 0;
}
