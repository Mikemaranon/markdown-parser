import ctypes
from ctypes import POINTER, byref, c_char_p, c_int, c_void_p
from pathlib import Path


class MarkdownParserError(Exception):
    """Base exception for Markdown Parser native bridge errors."""


class MarkdownParserAllocationError(MarkdownParserError):
    """Raised when the native library cannot allocate memory."""


class MarkdownParserRenderError(MarkdownParserError):
    """Raised when the native library fails to render Markdown."""


class MarkdownParserNullArgumentError(MarkdownParserError):
    """Raised when the native library receives a null argument."""


class NativeMarkdownParserBridge:
    """
    Low-level bridge between Python and the native Markdown Parser C library.
    """

    MARKDOWN_PARSER_OK = 0
    MARKDOWN_PARSER_ERROR_NULL_ARGUMENT = 1
    MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED = 2
    MARKDOWN_PARSER_ERROR_RENDER_FAILED = 3

    def __init__(self, library_path: str | None = None) -> None:
        self._library = self._load_library(library_path)
        self._configure_native_signatures()

    def create_parser(self) -> c_void_p:
        """
        Creates a native MarkdownParser instance.
        """
        parser_handle = self._library.markdown_parser_create()
        if not parser_handle:
            raise MarkdownParserAllocationError(
                "Failed to create native MarkdownParser instance."
            )

        return parser_handle

    def destroy_parser(self, parser_handle: c_void_p) -> None:
        """
        Destroys a native MarkdownParser instance.
        """
        if parser_handle:
            self._library.markdown_parser_destroy(parser_handle)

    def set_css(self, parser_handle: c_void_p, css_content: str) -> None:
        """
        Stores CSS content inside the native parser context.
        """
        status = self._library.markdown_parser_set_css(
            parser_handle,
            self._encode_string(css_content),
        )
        self._raise_for_status(status)

    def clear_css(self, parser_handle: c_void_p) -> None:
        """
        Clears stored CSS from the native parser context.
        """
        status = self._library.markdown_parser_clear_css(parser_handle)
        self._raise_for_status(status)

    def transform(self, parser_handle: c_void_p, markdown_content: str) -> str:
        """
        Transforms Markdown content into an HTML fragment.
        """
        output_pointer = c_char_p()

        status = self._library.markdown_parser_transform(
            parser_handle,
            self._encode_string(markdown_content),
            byref(output_pointer),
        )
        self._raise_for_status(status)

        return self._consume_native_string(output_pointer)

    def to_html_document(
        self,
        parser_handle: c_void_p,
        markdown_content: str,
        title: str | None = None,
    ) -> str:
        """
        Builds a full HTML document from Markdown content and stored CSS.
        """
        output_pointer = c_char_p()
        native_title = None if title is None else self._encode_string(title)

        status = self._library.markdown_parser_to_html_document(
            parser_handle,
            self._encode_string(markdown_content),
            native_title,
            byref(output_pointer),
        )
        self._raise_for_status(status)

        return self._consume_native_string(output_pointer)

    @staticmethod
    def _load_library(library_path: str | None) -> ctypes.CDLL:
        """
        Loads the native shared library from disk.
        """
        resolved_path = (
            Path(library_path).resolve()
            if library_path is not None
            else NativeMarkdownParserBridge._default_library_path()
        )

        if not resolved_path.exists():
            raise FileNotFoundError(
                f"Native library not found at: {resolved_path}"
            )

        return ctypes.CDLL(str(resolved_path))

    @staticmethod
    def _default_library_path() -> Path:
        """
        Resolves the default path to the native shared library.
        """
        current_file = Path(__file__).resolve()

        return (
            current_file.parent.parent
            / "c_src"
            / "libmarkdown_parser.dylib"
        )

    def _configure_native_signatures(self) -> None:
        """
        Configures ctypes signatures for the native public API.
        """
        self._library.markdown_parser_create.argtypes = []
        self._library.markdown_parser_create.restype = c_void_p

        self._library.markdown_parser_destroy.argtypes = [c_void_p]
        self._library.markdown_parser_destroy.restype = None

        self._library.markdown_parser_set_css.argtypes = [c_void_p, c_char_p]
        self._library.markdown_parser_set_css.restype = c_int

        self._library.markdown_parser_clear_css.argtypes = [c_void_p]
        self._library.markdown_parser_clear_css.restype = c_int

        self._library.markdown_parser_transform.argtypes = [
            c_void_p,
            c_char_p,
            POINTER(c_char_p),
        ]
        self._library.markdown_parser_transform.restype = c_int

        self._library.markdown_parser_to_html_document.argtypes = [
            c_void_p,
            c_char_p,
            c_char_p,
            POINTER(c_char_p),
        ]
        self._library.markdown_parser_to_html_document.restype = c_int

        self._library.markdown_parser_free_string.argtypes = [c_char_p]
        self._library.markdown_parser_free_string.restype = None

    def _consume_native_string(self, native_string: c_char_p) -> str:
        """
        Converts a native allocated string into Python and releases it.
        """
        if not native_string or not native_string.value:
            return ""

        try:
            return native_string.value.decode("utf-8")
        finally:
            self._library.markdown_parser_free_string(native_string)

    @staticmethod
    def _encode_string(value: str) -> bytes:
        """
        Encodes a Python string into UTF-8 bytes for the native API.
        """
        if not isinstance(value, str):
            raise TypeError("Expected a string value.")

        return value.encode("utf-8")

    def _raise_for_status(self, status: int) -> None:
        """
        Maps native status codes to Python exceptions.
        """
        if status == self.MARKDOWN_PARSER_OK:
            return

        if status == self.MARKDOWN_PARSER_ERROR_NULL_ARGUMENT:
            raise MarkdownParserNullArgumentError(
                "Native library reported a null argument error."
            )

        if status == self.MARKDOWN_PARSER_ERROR_ALLOCATION_FAILED:
            raise MarkdownParserAllocationError(
                "Native library reported an allocation failure."
            )

        if status == self.MARKDOWN_PARSER_ERROR_RENDER_FAILED:
            raise MarkdownParserRenderError(
                "Native library reported a render failure."
            )

        raise MarkdownParserError(
            f"Native library returned an unknown status code: {status}"
        )