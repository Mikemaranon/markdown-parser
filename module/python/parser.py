from _native import NativeMarkdownParserBridge


class MarkdownParser:
    """
    High-level Python API for the native Markdown Parser library.
    """

    def __init__(self, library_path: str | None = None) -> None:
        self._bridge = NativeMarkdownParserBridge(library_path=library_path)
        self._parser_handle = self._bridge.create_parser()

    def __del__(self) -> None:
        self.close()

    def close(self) -> None:
        """
        Releases the native parser instance.
        """
        if getattr(self, "_parser_handle", None):
            self._bridge.destroy_parser(self._parser_handle)
            self._parser_handle = None

    def add_css(self, css_content: str) -> None:
        """
        Stores CSS content inside the parser context.
        """
        self._ensure_open()
        self._bridge.set_css(self._parser_handle, css_content)

    def clear_css(self) -> None:
        """
        Removes stored CSS from the parser context.
        """
        self._ensure_open()
        self._bridge.clear_css(self._parser_handle)

    def transform(self, markdown_content: str) -> str:
        """
        Transforms Markdown content into an HTML fragment.
        """
        self._ensure_open()
        return self._bridge.transform(self._parser_handle, markdown_content)

    def to_html_document(
        self,
        markdown_content: str,
        title: str | None = None,
    ) -> str:
        """
        Builds a full HTML document from Markdown content and stored CSS.
        """
        self._ensure_open()
        return self._bridge.to_html_document(
            self._parser_handle,
            markdown_content,
            title=title,
        )

    def _ensure_open(self) -> None:
        """
        Ensures the parser instance is still available.
        """
        if self._parser_handle is None:
            raise RuntimeError("MarkdownParser instance is already closed.")