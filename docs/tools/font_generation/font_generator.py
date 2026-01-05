# Python script to generate a C-array from a bitmap font
# Note: current status in progress

import sys
import numpy
from PIL import Image

grid_width = 12
grid_height = 11
grid_line = 1
grid_padding = 1
character_width = 5
character_height = 7

width = grid_width * (2 * grid_padding + grid_line + character_width) + grid_line
height = grid_height * (2 * grid_padding + grid_line + character_height) + grid_line

font_color = (0, 0, 0)

font_file = 'data/fonts/font_5x7.bmp'


def pixels_to_c(pixels, x, y):
    bits = []
    for col in range(y, y + character_height):
        for row in range(x, x + character_width):
            bit = 1 if pixels[row, col] == font_color else 0
            bits.append(bit)
    return bits


def bits_to_c(bits):
    bytes_list = []
    current_byte = 0
    bit_count = 0

    for bit in bits:
        current_byte = (current_byte << 1) | (bit & 1)
        bit_count += 1

        if bit_count == 8:
            bytes_list.append(current_byte)
            current_byte = 0
            bit_count = 0

    # Wenn am Ende Bits übrig sind, nach rechts auffüllen (LSB = Padding 0)
    if bit_count > 0:
        current_byte <<= (8 - bit_count)
        bytes_list.append(current_byte)

    # C-Array als hex-formatierte Bytes erzeugen
    # Name z.B. abhängig von x,y:
    array_name = 'font'

    # Formatierung: 12 Bytes pro Zeile, z.B.
    bytes_per_line = 12
    lines = []
    for i in range(0, len(bytes_list), bytes_per_line):
        chunk = bytes_list[i:i + bytes_per_line]
        line = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append("    " + line)

    c_array = "static const unsigned char " + array_name + "[] = {\n"
    c_array += ",\n".join(lines)
    c_array += "\n};\n"

    # Optional: kleines Header-Drumherum (kannst du auch weglassen/anpassen)
    header_guard = array_name.upper()
    header_guard = header_guard.replace(" ", "_")

    header = f"#pragma {header_guard}_H\n#define {header_guard}_H\n\n"
    header += c_array
    header += f"\n#define {array_name}_WIDTH  {character_width}\n"
    header += f"#define {array_name}_HEIGHT {character_height}\n\n"
    header += "#endif\n"

    return header


def generate():
    img = Image.open(font_file)
    pixels = img.load()

    all_bits = []

    for v in range(grid_height):
        for h in range(grid_width):
            x = h * 3 + 2 + h * character_width
            y = v * 3 + 2 + v * character_height
            all_bits.extend(pixels_to_c(pixels, x, y))

    header = bits_to_c(all_bits)
    print(header)


if __name__ == "__main__":
    print('\n')
    print(f'Generating font template ({width}, {height})...')
    generate()
