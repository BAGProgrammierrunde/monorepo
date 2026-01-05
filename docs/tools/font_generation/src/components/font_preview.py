from PIL import Image
from rich_pixels import Pixels
from textual.widgets import Static


class FontPreview(Static):
    def __init__(self, path: str) -> None:
        super().__init__()
        self.path = path

    def on_mount(self) -> None:
        self.render_image()

    def on_resize(self) -> None:
        self.render_image()

    def render_image(self) -> None:
        img = Image.open(self.path).convert("RGB")
        self.update(Pixels.from_image(img))
