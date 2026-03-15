# MARKDOWN PARSER

Official repository for the personal project `Markdown Parser`.

`Markdown Parser` is a modular project built around a native C core that transforms Markdown into HTML, plus a thin Python wrapper that exposes a clean and stable API for real usage.

The current MVP already supports a complete execution cycle:

- native Markdown to HTML transformation in C
- full HTML document generation
- CSS injection into the generated document
- Python integration through a wrapper layer
- shared test fixtures for both C and Python

## Inline syntax scope

The inline parser is responsible only for **inline Markdown expressions** inside already detected block elements such as paragraphs, headings, list items, table cells, and blockquotes.

It does **not** create blocks by itself.

---

## Scope policy

This project follows these rules for inline syntax design:

1. **Native Markdown syntax remains reserved for its original meaning**.
2. **Any HTML-capable feature not covered by native Markdown may be introduced as a dialect extension**.
3. **Dialect extensions must not reuse syntax that could naturally appear as a valid nested Markdown expression**.
4. **All dialect extensions must be explicitly documented**.

This means:

- Native Markdown expressions such as `***text***` must preserve their compositional meaning.
- Features such as underline, mark, subscript, or superscript must use **new delimiters** if they are added as dialect syntax.
- Expressions derived from valid Markdown nesting must never be reassigned to custom meanings.

---

## Supported inline syntax

| Category | Markdown syntax | HTML output | Type | Notes |
|---|---|---|---|---|
| Plain text | `text` | escaped text | Native | Must escape HTML-sensitive characters |
| Escape sequence | `\*`, `\_`, `\[`, `\]` | literal character | Native | Prevents Markdown interpretation |
| Emphasis | `*text*` | `<em>text</em>` | Native | Inline nesting allowed |
| Emphasis | `_text_` | `<em>text</em>` | Native | Standard Markdown behavior |
| Strong | `**text**` | `<strong>text</strong>` | Native | Inline nesting allowed |
| Strong | `__text__` | `<strong>text</strong>` | Native | Must keep original Markdown meaning |
| Strong + emphasis | `***text***` | `<strong><em>text</em></strong>` | Native | Must remain compositional |
| Strong + emphasis | `___text___` | `<strong><em>text</em></strong>` | Native | Must remain compositional |
| Strikethrough | `~~text~~` | `<del>text</del>` | Native / GFM | Inline nesting allowed |
| Inline code | `` `text` `` | `<code>text</code>` | Native | No nested parsing inside |
| Link | `[text](url)` | `<a href="url">text</a>` | Native | Link text may contain inline syntax |
| Link with title | `[text](url "title")` | `<a href="url" title="title">text</a>` | Native | Optional parsing detail |
| Image | `![alt](src)` | `<img src="src" alt="alt">` | Native | `alt` is text content |
| Image with title | `![alt](src "title")` | `<img src="src" alt="alt" title="title">` | Native | Optional parsing detail |
| Autolink | `<https://example.com>` | `<a href="https://example.com">https://example.com</a>` | Native | Standard Markdown behavior |
| Email autolink | `<user@example.com>` | `<a href="mailto:user@example.com">user@example.com</a>` | Native | Standard Markdown behavior |
| Automatic URL | `https://example.com` | `<a href="https://example.com">https://example.com</a>` | Optional native-like | Requires boundary detection |

---

## Dialect extensions

The following expressions are valid candidates for **custom dialect syntax**, because they map naturally to HTML but are not part of native Markdown inline syntax.

Their delimiters must remain **exclusive to the dialect** and must not collide with Markdown-native nesting rules.

| Feature | Proposed syntax | HTML output | Type | Notes |
|---|---|---|---|---|
| Underline | `++text++` | `<u>text</u>` | Dialect | Recommended custom syntax |
| Highlight | `==text==` | `<mark>text</mark>` | Dialect | Clear and readable |
| Superscript | `^^text^^` | `<sup>text</sup>` | Dialect | Avoid single `^` ambiguity |
| Subscript | `~~~text~~~` or another custom form | `<sub>text</sub>` | Dialect | Must avoid collision with strikethrough |
| Keyboard input | `&&text&&` | `<kbd>text</kbd>` | Dialect | Possible extension |
| Inserted text | `@@text@@` | `<ins>text</ins>` | Dialect | Possible extension |

---

## Context-dependent inline syntax

The following expressions are inline in appearance, but they require **external document context** and should not be resolved by a purely local inline function unless reference definitions are already available in the transformer context.

| Category | Markdown syntax | HTML output | Type | Notes |
|---|---|---|---|---|
| Reference link | `[text][id]` | `<a href="...">text</a>` | Native, context-dependent | Requires reference lookup |
| Collapsed reference link | `[text][]` | `<a href="...">text</a>` | Native, context-dependent | Requires reference lookup |
| Shortcut reference link | `[id]` | `<a href="...">id</a>` | Native, context-dependent | Requires reference lookup |
| Reference image | `![alt][id]` | `<img ...>` | Native, context-dependent | Requires reference lookup |

---

## Parsing rules

| Rule | Description |
|---|---|
| Left-to-right scan | The inline parser processes text from left to right |
| HTML escaping | Plain text must always be HTML-escaped |
| Escape precedence | Escaped characters are treated as literals |
| Longest delimiter first | Multi-character delimiters must be checked before shorter ones |
| Literal fallback | Unclosed or invalid expressions must remain as literal text |
| Inline-only responsibility | The inline parser must not create paragraphs, headings, lists, or any block element |
| Code isolation | Content inside inline code is not parsed recursively |
| Controlled nesting | Native inline expressions may nest according to defined precedence rules |
| Safe attributes | Attribute values such as `href`, `src`, `title`, and `alt` must be escaped safely |
| Native syntax preservation | Native Markdown syntax must never be reassigned to custom meanings |
| Dialect isolation | Custom syntax must use delimiters that cannot conflict with valid Markdown nesting |

---

## Explicitly out of scope for the inline parser

| Element | Reason |
|---|---|
| Headings | Block-level parsing responsibility |
| Paragraphs | Block-level parsing responsibility |
| Lists | Block-level parsing responsibility |
| Tables | Block-level parsing responsibility |
| Blockquotes | Block-level parsing responsibility |
| Horizontal rules | Block-level parsing responsibility |
| Fenced code blocks | Block-level parsing responsibility |
| HTML blocks | Block-level parsing responsibility |
| Footnotes | Require document-level structures |
| Task list detection | Belongs to list/block parsing |
| Reference definition declarations | Require document-level storage |

---

## Recommended implementation split

| Responsibility | Suggested internal function |
|---|---|
| Flush plain text | `markdown_transformer_flush_plain_text` |
| Escape handling | `markdown_transformer_parse_escape_sequence` |
| Delimiter matching | `markdown_transformer_match_delimiter` |
| Emphasis parsing | `markdown_transformer_parse_emphasis` |
| Inline code parsing | `markdown_transformer_parse_code_span` |
| Link parsing | `markdown_transformer_parse_link` |
| Image parsing | `markdown_transformer_parse_image` |
| Autolink parsing | `markdown_transformer_parse_autolink` |
| Automatic URL parsing | `markdown_transformer_parse_auto_url` |
| Recursive range parsing | `markdown_transformer_parse_inline_range` |

---

## Final contract

The inline parser should be designed as a **closed-scope component** whose responsibility is:

- parsing self-contained inline Markdown expressions
- preserving native Markdown semantics
- supporting clearly separated dialect extensions
- escaping plain text safely
- appending valid HTML to the builder
- delegating context-dependent features to the transformer layer when external lookup is required