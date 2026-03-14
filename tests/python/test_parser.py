import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[2]
PYTHON_MODULE_DIR = PROJECT_ROOT / "module" / "python"

sys.path.insert(0, str(PYTHON_MODULE_DIR))

from parser import MarkdownParser


def read_text_file(file_path: Path) -> str:
    """
    Reads a UTF-8 text file and returns its content.
    """
    return file_path.read_text(encoding="utf-8")


def main() -> None:
    tests_dir = PROJECT_ROOT / "tests"
    markdown_file_path = tests_dir / "samples" / "example.md"
    css_file_path = tests_dir / "samples" / "example.css"

    markdown_content = read_text_file(markdown_file_path)
    css_content = read_text_file(css_file_path)

    parser = MarkdownParser()

    try:
        parser.add_css(css_content)

        html_fragment = parser.transform(markdown_content)
        html_document = parser.to_html_document(
            markdown_content,
            title="Markdown Parser Test",
        )

        print("=== Markdown Source File ===")
        print(markdown_file_path)

        print("\n=== HTML Fragment ===")
        print(html_fragment)

        print("\n=== Full HTML Document ===")
        print(html_document)
    finally:
        parser.close()


if __name__ == "__main__":
    main()