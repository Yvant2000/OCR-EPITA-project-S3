from typing import List, Final, Tuple

from PIL import Image
from PIL import ImageFont
from PIL.ImageDraw import Draw

from os.path import join, sep

from matplotlib.font_manager import findSystemFonts

image_dims: Final[Tuple[int, int]] = 28, 28
half_dims: Final[Tuple[int, int]] = image_dims[0]//2, image_dims[1]//2
background_color: Final[str] = 'white'
digit_list: Final[List[str]] = [str(digit) for digit in range(1, 10)]
text_color: Final[Tuple[int, ...]] = 0, 0, 0
font_size: int = 20


def create_data() -> None:
    """
    This function create a data set of images in folder "data" to train the Neural Network.

    :return: None
    """
    fonts: List[str | bytes] = findSystemFonts(fontpaths=None, fontext='ttf')
    for font_path in fonts:  # font_path: str | bytes
        if 'Noto' in font_path:  # Theses fonts usually don't work well
            continue

        font: ImageFont = ImageFont.truetype(font_path, font_size)
        font_name: str = font_path.split(sep)[-1]

        # if not font_name == "SourceSans3-Semibold.otf":
        #     continue

        image: Image = Image.new('RGB', image_dims, color=background_color)
        image.save(join('data', f'{font_name}_0.bmp'))

        for digit in digit_list:  # digit: int
            for deca_x, deca_y, shift_name in (
                    (-3, 0, "left"),
                    (3, 0, "right"),
                    (0, -3, "up"),
                    (0, 3, "down"),
                    (0, 0, "middle"),
            ):
                image = Image.new('RGB', image_dims, color=background_color)
                draw: Draw = Draw(image)
                text_size: Tuple[int, int] = font.getsize(digit)
                text_pos: Tuple[int, int] = (image_dims[0] - text_size[0]) // 2 + deca_x, (image_dims[1] - text_size[1]) // 2 - 3 + deca_y
                draw.text(text_pos, digit, text_color, font=font)
                image = image.point(lambda pixel: pixel > 240 and 255)
                image.save(join('data', f'{font_name}_{shift_name}_{digit}.bmp'))


if __name__ == "__main__":
    create_data()
