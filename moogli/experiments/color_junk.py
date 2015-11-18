# http://www.easyrgb.com/index.php?X=MATH&H=18#text18
# http://www.w3.org/TR/2011/REC-css3-color-20110607/#ABC

def _check(value, valuetype, minvalue, maxvalue, valuename):
    typeexception = "'{0}' should be of type '{1}'".format( valuename
                                                          , valuetype.__name__
                                                          )
    rangeexception = "{0} <= '{1}' <= {2}".format( minvalue
                                                 , valuename
                                                 , maxvalue
                                                 )
    if not isinstance(value, valuetype)  : raise TypeError(typeexception)
    if minvalue <= value <= maxvalue: return
    raise ValueError(rangeexception)

class Color(int):
    def __init__(self, red, green, blue, alpha):
        _check(hue, float, 0.0, 1.0, "hue")
        _check(saturation, float, 0.0, 1.0, "saturation")
        _check(lightness, float, 0.0, 1.0, "lightness")
        _check(alpha, float, 0.0, 1.0, "alpha")
        self.hue   = hue
        self.saturation = saturation
        self.lightness  = lightness
        self.alpha = alpha

    # def lighter():
    #     pass

    # def darker():
    #     pass

    # def __str__():
    #     pass

    # def show():
    #     pass

    def hsl(self):
        return self.hsla()[0:3]

    def hsla(self):
        return ( self.hue        * 360.0
               , self.saturation * 100.0
               , self.lightness  * 100.0
               , self.alpha
               )

    def rgb(self):
        return self.rgba()[0:3]

    def rgba(self):
        def hue_to_rgb(m1, m2, hue):
            hue = hue + 1.0 if hue < 0.0 else hue - 1.0
            if hue * 6.0 < 1.0 : return m1 + (m2 - m1) * hue * 6.0
            if hue * 2.0 < 1.0 : return m2
            if hue * 3.0 < 2.0 : return m1 + (m2 - m1) * (2.0/3.0 - hue) * 6.0
            return m1

        lightness = lightness / 100.0
        saturation = saturation / 100.0

        if lightness <= 0.5: m2 = lightness * (1.0 + saturation)
        else               : m2 = lightness * (1.0 - saturation) + saturation
        m1 = lightness * 2.0 - m2
        return ( hue_to_rgb(m1, m2, hue + 1.0/3.0) * 255
               , hue_to_rgb(m1, m2, hue          ) * 255
               , hue_to_rgb(m1, m2, hue - 1.0/3.0) * 255
               , self.alpha
               )

    def hex(self):
        return self.hex8()

    def hex8(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex3(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex4(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()

    def hex6(self):
        (r,g,b,a) = self.rgba()
        a = int(a * 255)
        return ("#" + hex(r)[2:] + hex(g)[2:] + hex(b)[2:] + hex(a)[2:]).upper()



    # def cmyk():pass


def RGB(red, green, blue):
    return RGBA(red, green, blue, 1.0)

def RGBA(red, green, blue, alpha):
    _check(red, int, 0, 255, "red")
    _check(green, int, 0, 255, "green")
    _check(blue, int, 0, 255, "blue")
    _check(alpha, float, 0.0, 1.0, "alpha")

    red     = red   / 255.0
    green   = green / 255.0
    blue    = blue  / 255.0
    minval  = min(red, green, blue)
    maxval  = max(red, green, blue)
    delta   = maxval - minval

    lightnes = ( maxval + minval ) / 2.0

    if delta == 0.0:
        hue = 0.0
        saturation = 0.0
    else:
       if lightness < 0.5 : saturation = delta / (maxval + minval)
       else               : saturation = delta / ( 2.0 - maxval - minval )

       deltar = (((maxval - red) / 6.0 ) + (delta /2.0)) / delta
       deltag = (((maxval - green) / 6.0 ) + (delta / 2.0) ) / delta
       deltab = (((maxval - blue) / 6.0 ) + (delta / 2.0) ) / delta

       if   red   == maxval : hue =           deltab - deltag
       elif green == maxval : hue = 1.0/3.0 + deltar - deltab
       elif blue  == maxval : hue = 2.0/3.0 + deltag - deltar

       if hue < 0.0 : hue += 1.0
       if hue > 1.0 : hue -= 1.0

    return Color(hue, saturation, lightness, alpha)

def HEX(hexstring):
    if not hexstring.startswith("#") :
        raise ValueError("'{0}' should start with '#'".format(hexstring))
    if len(hexstring) == 4:
        return RGB( int(hexstring[1], 16) * 17
                  , int(hexstring[2], 16) * 17
                  , int(hexstring[3], 16) * 17
                  )
    elif len(hexstring) == 5:
        return RGBA( int(hexstring[1], 16) * 17
                   , int(hexstring[2], 16) * 17
                   , int(hexstring[3], 16) * 17
                   , int(hexstring[4], 16) / 15.0
                   )
    elif len(hexstring) == 7:
        return RGB( int(hexstring[1:3], 16)
                  , int(hexstring[3:5], 16)
                  , int(hexstring[5:7], 16)
                  )
    elif len(hexstring) == 9:
        return RGBA( int(hexstring[1:3], 16)
                   , int(hexstring[3:5], 16)
                   , int(hexstring[5:7], 16)
                   , int(hexstring[7:9], 16) / 255.0
                   )
    else:
        raise ValueError("Valid formats are #RGB, #RGBA, #RRGGBB, #RRGGBBAA")

def HSLA(hue, saturation, lightness, alpha):
    _check(hue, float, 0.0, 360.0, "hue")
    _check(saturation, float, 0.0, 100.0, "saturation")
    _check(lightness, float, 0.0, 100.0, "lightness")
    _check(alpha, float, 0.0, 1.0, "alpha")

    return Color( hue/360.0
                , saturation/100.0
                , lightness/100.0
                , alpha
                )

def HSL(hue, saturation, lightness):
    return HSLA(hue, saturation, lightness, 1.0)

# def CMYK(cyan, magenta, yellow, key):
#     return RGBA( 255 - cyan
#                , 255 - magenta
#                , 255 - yellow

#                )

# def CMY(cyan, magenta, yellow):
#     return CMYK(cyan, magenta, yellow, 1.0)


# def HSV(hue, saturation, value):
#     pass

