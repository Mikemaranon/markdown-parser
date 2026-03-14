# Markdown Parser

Official repository for the personal project `Markdown Parser`.

`Markdown Parser` is a modular project built around a native C core that transforms Markdown into HTML, plus a thin Python wrapper that exposes a clean and stable API for real usage.

The current MVP already supports a complete execution cycle:

- native Markdown to HTML transformation in C
- full HTML document generation
- CSS injection into the generated document
- Python integration through a wrapper layer
- shared test fixtures for both C and Python

## Current architecture

The repository is currently organized around three clear areas:

```text
markdown-parser/
├── module/
│   ├── c_src/
│   │   ├── lib/
│   │   │   └── markdown_parser.h
│   │   ├── m_str/
│   │   │   ├── string_buffer.c
│   │   │   └── string_buffer.h
│   │   ├── m_html/
│   │   │   ├── html_builder.c
│   │   │   └── html_builder.h
│   │   ├── m_md/
│   │   │   ├── markdown_transformer.c
│   │   │   └── markdown_transformer.h
│   │   ├── markdown_parser.c
│   │   ├── libmarkdown_parser.dylib
│   │   └── libmarkdown_parser.so
│   └── python/
│       ├── _native.py
│       └── parser.py
└── tests/
    ├── c/
    ├── python/
    └── samples/
```

## Project layers

### C core

The native library is the real engine of the project.

Its responsibilities are:

- manage parser state
- store CSS content in the parser context
- transform Markdown into HTML fragments
- generate full HTML documents
- keep the internal implementation modular and extensible

### Python wrapper

The Python layer does not reimplement parsing logic.

Its job is only to:

- load the native shared library
- expose a friendly public API
- convert values between Python and C
- manage native memory returned by the C library

### Shared tests

The repository uses shared fixtures under `tests/samples/` so both C and Python can validate the same real Markdown and CSS files.

## Public Python API

The Python API is intentionally small and stable.

The exposed object provides these methods:

- `add_css(css_content)`
- `clear_css()`
- `transform(markdown_content)`
- `to_html_document(markdown_content, title=None)`
- `close()`

## Native C API

The C public API is exposed through `module/c_src/lib/markdown_parser.h`.

Main functions:

- `markdown_parser_create()`
- `markdown_parser_destroy()`
- `markdown_parser_set_css()`
- `markdown_parser_clear_css()`
- `markdown_parser_transform()`
- `markdown_parser_to_html_document()`
- `markdown_parser_free_string()`

## Current Markdown support

The current MVP supports:

- headings from `#` to `######`
- paragraphs
- blank-line paragraph separation
- HTML escaping for text content

## Build the native library

Example build command for macOS:

```bash
clang -dynamiclib \
  -o module/c_src/libmarkdown_parser.dylib \
  module/c_src/markdown_parser.c \
  module/c_src/m_str/string_buffer.c \
  module/c_src/m_html/html_builder.c \
  module/c_src/m_md/markdown_transformer.c \
  -I module/c_src/lib \
  -I module/c_src/m_str \
  -I module/c_src/m_html \
  -I module/c_src/m_md
```

## Run the tests

### C test

Compile from the repository root:

```bash
clang \
  -o tests/c/markdown_parser_test \
  tests/c/test_main.c \
  module/c_src/markdown_parser.c \
  module/c_src/m_str/string_buffer.c \
  module/c_src/m_html/html_builder.c \
  module/c_src/m_md/markdown_transformer.c \
  -I module/c_src \
  -I module/c_src/lib \
  -I module/c_src/m_str \
  -I module/c_src/m_html \
  -I module/c_src/m_md
```

Then run:

```bash
./tests/c/markdown_parser_test
```

### Python test

Run from the repository root:

```bash
python3 tests/python/test_parser.py
```

## Example Python usage

```python
from parser import MarkdownParser

markdown_content = """# Main Title

This is a paragraph.
"""

css_content = """body { font-family: Arial, sans-serif; }
h1 { color: #1f4b99; }
"""

parser = MarkdownParser()

try:
    parser.add_css(css_content)

    html_fragment = parser.transform(markdown_content)
    html_document = parser.to_html_document(
        markdown_content,
        title="Example Document",
    )

    print(html_fragment)
    print(html_document)
finally:
    parser.close()
```

## Design goals

This project is being developed with these goals in mind:

- modular C architecture
- minimal dependencies
- clean public API
- future extensibility
- stable Python usage over an evolving native core

## Roadmap

Planned next steps include:

- inline formatting support
- stronger HTML formatting control
- lists
- tables
- links
- code blocks
- Linux shared library support
- packaging improvements for Python usage

## Status

The project already has a functional MVP with:

- working native C parser
- working Python wrapper
- shared test inputs
- real end-to-end execution

The next phase is focused on expanding Markdown support while keeping the public API stable.
