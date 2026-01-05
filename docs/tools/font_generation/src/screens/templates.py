from pathlib import Path


from textual.app import ComposeResult
from textual.binding import Binding
from textual.containers import Vertical
from textual.screen import Screen
from textual.widgets import Label, Static

from components.file_list import FileList


class TemplateScreen(Screen):
    CSS = """
    Screen { align: center middle; }
    PixelImage { width: 80; height: 20; }
    """
    BINDINGS = [Binding("b", "back", "Zurück"), Binding("escape", "back", "Zurück")]

    def __init__(self) -> None:
        super().__init__()

    def compose(self) -> ComposeResult:
        yield Label("Font Templates", id="title")
        self.file_list = FileList()
        yield Vertical(self.file_list)

    def on_mount(self) -> None:
        self.current_path = Path("data/templates").resolve()
        self.file_list.load_directory(self.current_path)
        if self.file_list.children:
            self.file_list.index = 0

    def action_back(self) -> None:
        self.app.pop_screen()
