from textual.app import App

from screens.menu import MenuScreen


class MenuApp(App):
    CSS = """
    Screen { align: center middle; }
    ListView { width: 83; height: 10; }
    """

    def on_mount(self) -> None:
        self.push_screen(MenuScreen())


if __name__ == "__main__":
    MenuApp().run()
