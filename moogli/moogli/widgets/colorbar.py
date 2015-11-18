import moogli
import matplotlib
import matplotlib.cm
import math


class ColorBar(moogli.core._moogli.ColorBar):
    def __init__(self,
                 id,
                 title="Molecule #",
                 text_color=moogli.colors.BLACK,
                 position=moogli.geometry.Vec2f(0.975, 0.5),
                 size=moogli.geometry.Vec2f(0.30, 0.05),
                 text_font="/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-R.ttf",
                 orientation=math.pi / 2.0,
                 text_character_size=20,
                 label_formatting_precision=0,
                 colormap=moogli.colors.MatplotlibColorMap(matplotlib.cm.rainbow),
                 color_resolution=100,
                 scalar_range=moogli.geometry.Vec2f(0.0, 300.0)):
        super(ColorBar, self).__init__(id)
        self._resolution = 50
        self.set_title(title)
        self.set_text_color(text_color)
        self.set_position(moogli.geometry.Vec2f(position.x(), position.y()))
        self.set_size(moogli.geometry.Vec2f(size.x(), size.y()))
        self.set_text_font(text_font)
        self.set_orientation(orientation)
        self.set_text_character_size(text_character_size)
        self.set_colormap(colormap)
        self.set_color_resolution(color_resolution)
        self.set_scalar_range(scalar_range)

    def set_colormap(self, colormap):
        self._colormap = colormap
        self.set_colors()

    def set_color_resolution(self, resolution):
        self._resolution = resolution
        self.set_colors()

    def get_color_resolution(self):
        return self._resolution

    def get_colormap(self):
        return self._colormap

    def set_colors(self):
        colors = [self._colormap(value / (self._resolution - 1.0))
                  for value in range(self._resolution)]
        super(ColorBar, self).set_colors(colors)
