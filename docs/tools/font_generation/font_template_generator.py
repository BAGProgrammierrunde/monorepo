import sys
import numpy
from PIL import Image

grid_width = 20
grid_height = 20
grid_line = 1
grid_padding = 1
character_width = 6
character_height = 6

width = grid_width * (2 * grid_padding + grid_line + character_width) + grid_line
height = grid_height * (2 * grid_padding + grid_line + character_height) + grid_line

background_color = (255, 255, 255)
grid_color = (200, 200, 200)

def draw_line(pixels, h, color):
    for w in range(width):
        pixels[w, h] = color

def draw_tex_row(pixels, h):
    curr_w = 0
    for col in range(grid_width):
        pixels[curr_w, h] = grid_color
        curr_w += 1
        for w in range(2 * grid_padding + character_width):
            pixels[curr_w, h] = background_color
            curr_w += 1
    pixels[curr_w, h] = grid_color
def generate():
    img = Image.new( 'RGB', (width, height), "white") # create a new black image
    pixels = img.load() # create the pixel map

    curr_h = 0
    for h in range(grid_height):
        draw_line(pixels, curr_h, grid_color)
        curr_h += 1
        for h in range(character_height + 2):
            draw_tex_row(pixels, curr_h)
            curr_h += 1
    draw_line(pixels, curr_h, grid_color)

    img.show()
    img.save('font_template_6x6.bmp')

if __name__ == "__main__":
    print('\n')
    print(f'Generating font template ({width}, {height})...')
    generate()