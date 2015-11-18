import moogli
import math

class Color(moogli.geometry.Vec4f):
    def __init__(self, r=0.0, g=0.0, b=0.0, a=1.0):
        super(Color, self).__init__(r, g, b, a)



def interpolate(mincolor, maxcolor, scalar, minscalar=0.0, maxscalar=1.0):
    value = moogli.utilities.normalize(scalar, minscalar, maxscalar)
    return mincolor + (maxcolor - mincolor) * value


class UniformColorMap():
    def __init__(self, colors):
        self.colors = colors

    def __call__(self, value):
        index = value * (len(self.colors) - 1)
        intpart, fracpart = divmod(index, 1.0)
        intpart = int(intpart)
        color1 = self.colors[intpart]
        if intpart == len(self.colors) - 1:
            color2 = color1
        else:
            color2 = self.colors[intpart + 1]
        return interpolate(color1, color2, fracpart)


class MatplotlibColorMap():
    def __init__(self, colormap):
        self._colormap = colormap

    def set_colormap(self, colormap):
        self._colormap = colormap

    def get_colormap(self):
        return self._colormap

    def __call__(self, scalar):
        c = self._colormap(scalar)
        return Color(r=c[0], g=c[1], b=c[2], a=c[3])

# PINK COLORS
PINK                    = Color(255, 192, 203, 255) / 255.0
LIGHT_PINK              = Color(255, 182, 193, 255) / 255.0
HOT_PINK                = Color(255, 105, 180, 255) / 255.0
DEEP_PINK               = Color(255, 20,  147, 255) / 255.0
PALE_VIOLET_RED         = Color(219, 112, 147, 255) / 255.0
MEDIUM_VIOLET_RED       = Color(199, 21,  133, 255) / 255.0

# RED COLOURS
LIGHT_SALMON            = Color(255, 160, 122, 255) / 255.0
SALMON                  = Color(250, 128, 114, 255) / 255.0
DARK_SALMON             = Color(233, 150, 122, 255) / 255.0
LIGHT_CORAL             = Color(240, 128, 128, 255) / 255.0
INDIAN_RED              = Color(205,  92,  92, 255) / 255.0
CRIMSON                 = Color(220,  20,  60, 255) / 255.0
FIREBRICK               = Color(178,  34,  34, 255) / 255.0
DARK_RED                = Color(139,   0,   0, 255) / 255.0
RED                     = Color(255,   0,   0, 255) / 255.0

#ORANGE COLOURS
ORANGE_RED              = Color(255,  69,   0, 255) / 255.0
TOMATO                  = Color(255,  99,  71, 255) / 255.0
CORAL                   = Color(255, 127,  80, 255) / 255.0
DARK_ORANGE             = Color(255, 140,   0, 255) / 255.0
ORANGE                  = Color(255, 165,   0, 255) / 255.0

#YELLOW COLOURS
YELLOW                  = Color(255, 255,   0, 255) / 255.0
LIGHT_YELLOW            = Color(255, 255, 224, 255) / 255.0
LEMON_CHIFFON           = Color(255, 250, 205, 255) / 255.0
LIGHT_GOLDENROD_YELLOW  = Color(250, 250, 210, 255) / 255.0
PAPAYA_WHIP             = Color(255, 239, 213, 255) / 255.0
MOCCASIN                = Color(255, 228, 181, 255) / 255.0
PEACH_PUFF              = Color(255, 218, 185, 255) / 255.0
PALE_GOLDENROD          = Color(238, 232, 170, 255) / 255.0
KHAKI                   = Color(240, 230, 140, 255) / 255.0
DARK_KHAKI              = Color(189, 183, 107, 255) / 255.0
GOLD                    = Color(255, 215,   0, 255) / 255.0

# BROWN COLOURS
CORN_SILK               = Color(255, 248, 220, 255) / 255.0
BLANCHED_ALMOND         = Color(255, 235, 205, 255) / 255.0
BISQUE                  = Color(255, 228, 196, 255) / 255.0
NAVAJO_WHITE            = Color(255, 222, 173, 255) / 255.0
WHEAT                   = Color(245, 222, 179, 255) / 255.0
BURLY_WOOD              = Color(222, 184, 135, 255) / 255.0
TAN                     = Color(210, 180, 140, 255) / 255.0
ROSY_BROWN              = Color(188, 143, 143, 255) / 255.0
SANDY__BROWN            = Color(244, 164,  96, 255) / 255.0
GOLDENROD               = Color(218, 165,  32, 255) / 255.0
DARK_GOLDENROD          = Color(184, 134,  11, 255) / 255.0
PERU                    = Color(205, 133,  63, 255) / 255.0
CHOCOLATE               = Color(210, 105,  30, 255) / 255.0
SADDLE_BROWN            = Color(139,  69,  19, 255) / 255.0
SIENNA                  = Color(160,  82,  45, 255) / 255.0
BROWN                   = Color(165,  42,  42, 255) / 255.0
MAROON                  = Color(128,   0,   0, 255) / 255.0

# GREEN COLOURS
DARK_OLIVE_GREEN        = Color( 85, 107,  47, 255) / 255.0
OLIVE                   = Color(128, 128,   0, 255) / 255.0
OLIVE_DRAB              = Color(107, 142,  35, 255) / 255.0
YELLOW_GREEN            = Color(154, 205,  50, 255) / 255.0
LIME_GREEN              = Color( 50, 205,  50, 255) / 255.0
LIME                    = Color(  0, 255,   0, 255) / 255.0
LAWN_GREEN              = Color(124, 252,   0, 255) / 255.0
CHARTREUSE              = Color(127, 255,   0, 255) / 255.0
GREEN_YELLOW            = Color(173, 255,  47, 255) / 255.0
SPRING_GREEN            = Color(  0, 255, 127, 255) / 255.0
MEDIUM_SPRING_GREEN     = Color(  0, 250, 154, 255) / 255.0
LIGHT_GREEN             = Color(144, 238, 144, 255) / 255.0
PALE_GREEN              = Color(152, 251, 152, 255) / 255.0
DARK_SEA_GREEN          = Color(143, 188, 143, 255) / 255.0
MEDIUM_SEA_GREEN        = Color( 60, 179, 113, 255) / 255.0
SEA_GREEN               = Color( 46, 139,  87, 255) / 255.0
FOREST_GREEN            = Color( 34, 139,  34, 255) / 255.0
GREEN                   = Color(  0, 128,   0, 255) / 255.0 
DARK_GREEN              = Color(  0, 100,   0, 255) / 255.0

# CYAN COLOURS
MEDIUM_AQUAMARINE       = Color(102, 205, 170, 255) / 255.0
AQUA                    = Color(  0, 255, 255, 255) / 255.0
CYAN                    = Color(  0, 255, 255, 255) / 255.0
LIGHT_CYAN              = Color(224, 255, 255, 255) / 255.0
PALE_TURQUOISE          = Color(175, 238, 238, 255) / 255.0
AQUA_MARINE             = Color(127, 255, 212, 255) / 255.0
TURQUOISE               = Color( 64, 224, 208, 255) / 255.0
MEDIUM_TURQUOISE        = Color( 72, 209, 204, 255) / 255.0
DARK_TURQUOISE          = Color(  0, 206, 209, 255) / 255.0
LIGHT_SEA_GREEN         = Color( 32, 178, 170, 255) / 255.0
CADET_BLUE              = Color( 95, 158, 160, 255) / 255.0
DARK_CYAN               = Color(  0, 139, 139, 255) / 255.0
TEAL                    = Color(  0, 128, 128, 255) / 255.0

# BLUE COLOURS
LIGHT_STEEL_BLUE        = Color(176, 196, 222, 255) / 255.0
POWDER_BLUE             = Color(176, 224, 230, 255) / 255.0
LIGHT_BLUE              = Color(173, 216, 230, 255) / 255.0
SKY_BLUE                = Color(135, 206, 235, 255) / 255.0
LIGHT_SKY_BLUE          = Color(135, 206, 250, 255) / 255.0
DEEP_SKY_BLUE           = Color(  0, 191, 255, 255) / 255.0
DODGER_BLUE             = Color( 30, 144, 255, 255) / 255.0
CORNFLOWER_BLUE         = Color(100, 149, 237, 255) / 255.0
STEEL_BLUE              = Color( 70, 130, 180, 255) / 255.0
ROYAL_BLUE              = Color( 65, 105, 225, 255) / 255.0
BLUE                    = Color(  0,   0, 255, 255) / 255.0
MEDIUM_BLUE             = Color(  0,   0, 205, 255) / 255.0
DARK_BLUE               = Color(  0,   0, 139, 255) / 255.0
NAVY                    = Color(  0,   0, 128, 255) / 255.0
MIDNIGHT_BLUE           = Color( 25,  25, 112, 255) / 255.0

# PURPLE/VIOLET/MAGENTA COLOURS
LAVENDER                = Color(230, 230, 250, 255) / 255.0
THISTLE                 = Color(216, 191, 216, 255) / 255.0
PLUM                    = Color(221, 160, 221, 255) / 255.0
VIOLET                  = Color(238, 130, 238, 255) / 255.0
ORCHID                  = Color(218, 112, 214, 255) / 255.0
FUCHSIA                 = Color(255,   0, 255, 255) / 255.0
MAGENTA                 = Color(255,   0, 255, 255) / 255.0
MEDIUM_ORCHID           = Color(186,  85, 211, 255) / 255.0
MEDIUM_PURPLE           = Color(147, 112, 219, 255) / 255.0
BLUE_VIOLET             = Color(138,  43, 226, 255) / 255.0
DARK_VIOLET             = Color(148,   0, 211, 255) / 255.0
DARK_ORCHID             = Color(153,  50, 204, 255) / 255.0
DARK_MAGENTA            = Color(139,   0, 139, 255) / 255.0
PURPLE                  = Color(128,   0, 128, 255) / 255.0
INDIGO                  = Color( 75,   0, 130, 255) / 255.0
DARK_SLATE_BLUE         = Color( 72,  61, 139, 255) / 255.0
REBECCA_PURPLE          = Color(102,  51, 153, 255) / 255.0
SLATE_BLUE              = Color(106,  90, 205, 255) / 255.0
MEDIUM_SLATE_BLUE       = Color(123, 104, 238, 255) / 255.0

#WHITE COLOURS
WHITE                   = Color(255, 255, 255, 255) / 255.0
SNOW                    = Color(255, 250, 250, 255) / 255.0
HONEYDEW                = Color(240, 255, 240, 255) / 255.0
MINT_CREAM              = Color(245, 255, 250, 255) / 255.0
AZURE                   = Color(240, 255, 255, 255) / 255.0
ALICE_BLUE              = Color(240, 248, 255, 255) / 255.0
GHOST_WHITE             = Color(248, 248, 255, 255) / 255.0
WHITE_SMOKE             = Color(245, 245, 245, 255) / 255.0
SEA_SHELL               = Color(255, 245, 238, 255) / 255.0
BEIGE                   = Color(245, 245, 220, 255) / 255.0
OLD_LACE                = Color(253, 245, 230, 255) / 255.0
FLORAL_WHITE            = Color(255, 250, 240, 255) / 255.0
IVORY                   = Color(255, 255, 240, 255) / 255.0
ANTIQUE_WHITE           = Color(250, 235, 215, 255) / 255.0
LINEN                   = Color(250, 240, 230, 255) / 255.0
LAVENDER_BLUSH          = Color(255, 240, 245, 255) / 255.0
MISTY_ROSE              = Color(255, 228, 225, 255) / 255.0

# GRAY/BLACK COLOURS
GAINSBORO               = Color(220, 220, 220, 255) / 255.0
LIGHT_GREY              = Color(211, 211, 211, 255) / 255.0
SILVER                  = Color(192, 192, 192, 255) / 255.0
DARK_GRAY               = Color(169, 169, 169, 255) / 255.0
GRAY                    = Color(128, 128, 128, 255) / 255.0
DIM_GRAY                = Color(105, 105, 105, 255) / 255.0
LIGHT_SLATE_GRAY        = Color(119, 136, 153, 255) / 255.0
SLATE_GRAY              = Color(112, 128, 144, 255) / 255.0
DARK_SLATE_GRAY         = Color( 47,  79,  79, 255) / 255.0
BLACK                   = Color(  0,   0,   0, 255) / 255.0
