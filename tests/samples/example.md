# Markdown Parser Test Suite

This is a simple paragraph with *italic*, _italic alt_, **bold**, __bold alt__, and ***bold italic*** text.

This paragraph also includes ~~strikethrough~~, ++underline++, ==highlight==, ^^superscript^^, &&keyboard&&, and @@inserted@@ syntax.

Inline code works with `printf("hello");` and escaped characters like \*this is not italic\* and \[this is not a link].

## Links and Images

Here is a standard link to [OpenAI](https://openai.com).
Here is a titled link to [GitHub](https://github.com "GitHub Homepage").

Automatic URLs should work too:
https://example.com/test?value=1&other=two

Autolinks should also work:
<https://www.wikipedia.org>

Email autolinks should work:
<john.doe@example.com>

Here is an inline image:
![Sample image](https://example.com/image.png)

And here is an image with title:
![Sample titled image](https://example.com/banner.png "Banner Title")

### Mixed Inline Content

You can combine **bold text with *nested italic*** and also use [links with **inline emphasis**](https://example.com/docs).

Another line with __bold__, `code`, and ==highlighted [text](https://example.com)== inside the same paragraph.

Plain text should remain untouched, including dots, commas, and numbers like 12345.