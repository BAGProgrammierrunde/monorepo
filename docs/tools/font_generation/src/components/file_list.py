from pathlib import Path

from textual.app import App, ComposeResult
from textual.widgets import ListView, ListItem, Label
from textual.containers import Vertical


class FileList(ListView):
    def load_directory(self, path: Path) -> None:
        self.clear()

        if not path.exists():
            self.append(ListItem(Label("Ordner existiert nicht")))
            return

        entries = sorted(
            path.iterdir(),
            key=lambda p: (p.is_file(), p.name.lower())
        )

        for entry in entries:
            prefix = "📁 " if entry.is_dir() else "📄 "
            self.append(
                ListItem(
                    Label(prefix + entry.name),
                )
            )
