AMBIENT
DIFFUSE
SHININESS
SPECULAR


class ColorMap(list):
    def __init__( self
                , filename = None,
                , values   = None
                ):
        if filename is not None:
            self.read(filename)
        elif values is not None:
            map(lambda value: self.add(value), values)

    def read(self, filename):
        extension =
        filename._read_from[extension](filename)

    def _read_from_csv


    def add(value, color):
        self.append((value, color))
