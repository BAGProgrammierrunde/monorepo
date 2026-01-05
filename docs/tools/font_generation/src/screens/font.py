from pathlib import Path

from PIL import Image
from rich_pixels import Pixels
from textual.app import ComposeResult
from textual.binding import Binding
from textual.containers import Vertical
from textual.screen import Screen
from textual.widgets import Label, Static, RichLog, Input

from components.file_list import FileList
from components.font_preview import FontPreview


class FontScreen(Screen):
    CSS = """
    Screen { align: center middle; }
    PixelImage { width: 80; height: 20; }
    """
    BINDINGS = [Binding("b", "back", "Zurück"), Binding("escape", "back", "Zurück")]

    def __init__(self) -> None:
        super().__init__()

    def compose(self) -> ComposeResult:
        yield Label("Font", id="title")
        yield Input(placeholder="Text eingeben…")
        yield FontPreview("data/fonts/font_5x7.bmp")

    def action_back(self) -> None:
        self.app.pop_screen()
