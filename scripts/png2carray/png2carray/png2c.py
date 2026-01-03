import argparse
import os
from typing import TextIO
from PIL import Image


def main():
    desc = "Convert a PNG file to a C array definition of bitmap data."
    epilog = "Example: png2c.py input.png"
    parser = argparse.ArgumentParser(prog="png2c.py", description=desc,
                                     epilog=epilog)
    # parser.add_argument('--threshold', '-t', defaut=128, type=int,
    #                    help="Threshold for converting to black and white "
    #                         "(0-255), default=128")
    parser.add_argument('--output', '-o', type=str, default="images.h",
                        help="Filename to write C definitions to.")
    parser.add_argument("input_png", help="Input PNG filename(s) to convert",
                        nargs='+')
    args = parser.parse_args()
    with open(args.output, 'w') as out_file:
        header_guard = args.output.upper().replace('.', '_').replace('-', '_')
        out_file.write(f"#ifndef _{header_guard}_\n")
        out_file.write(f"#define _{header_guard}_\n")
        for png_path in args.input_png:
            output_c_array(out_file, png_path)
        out_file.write(f"#endif  // _{header_guard}_\n")


def output_c_array(stream: TextIO, png_path: str):
    basename = os.path.basename(png_path)
    array_name: str = basename.replace('.', '_').replace('-', '_')

    img = Image.open(png_path)
    width, height = img.size
    mono_img = img.convert('1')  # convert to 1-bit pixels
    byte: int = 0

    stream.write(f"// Bitmap data for {basename} ({width}x{height})\n")
    stream.write(f"const unsigned char {array_name}[] = {{\n")
    line_width: int = 0
    for y in range(height):
        byte = 0
        for x in range(width):
            bit = 1 if mono_img.getpixel((x, y)) == 0 else 0
            # set byte, most signifigant first (MSB)
            byte = (byte << 1) | bit
            if (x % 8) == 7 or x == (width - 1):
                line_width += stream.write(f"0x{byte:02X}, ")
                byte = 0
        if line_width >= 72:
            stream.write("\n")
            line_width = 0
    stream.write("\n};\n\n")


if __name__ == "__main__":
    main()
