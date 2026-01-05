from textual.app import ComposeResult
from textual.screen import Screen
from textual.widgets import ListItem, ListView, Label

from screens.font import FontScreen
from screens.fonts import FontsScreen
from screens.templates import TemplateScreen


class MenuScreen(Screen):
    def __init__(self) -> None:
        super().__init__()
        self.routes = {
            "font": lambda: FontScreen(),
            "fonts": lambda: FontsScreen(),
            "templates": lambda: TemplateScreen(),
        }

    def compose(self) -> ComposeResult:
        yield Label("""
██████ ▄▄▄  ▄▄  ▄▄ ▄▄▄▄▄▄    ▄████  ▄▄▄▄▄ ▄▄  ▄▄ ▄▄▄▄▄ ▄▄▄▄   ▄▄▄ ▄▄▄▄▄▄ ▄▄▄  ▄▄▄▄  
██▄▄  ██▀██ ███▄██   ██     ██  ▄▄▄ ██▄▄  ███▄██ ██▄▄  ██▄█▄ ██▀██  ██  ██▀██ ██▄█▄ 
██    ▀███▀ ██ ▀██   ██      ▀███▀  ██▄▄▄ ██ ▀██ ██▄▄▄ ██ ██ ██▀██  ██  ▀███▀ ██ ██ 
                                                                                    """)
        yield ListView(
            ListItem(Label("Font"), id="font"),
            ListItem(Label("Fonts"), id="fonts"),
            ListItem(Label("Font Templates"), id="templates"),
            id="menu",
        )

    def on_list_view_selected(self, event: ListView.Selected) -> None:
        item_id = event.item.id  # <- statt event.index
        if item_id and item_id in self.routes:
            self.app.push_screen(self.routes[item_id]())
