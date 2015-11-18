import math


class Point:
    def __init__(self, x = 0.0, y = 0.0, z = 0.0):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "({0}, {1}, {2})".format(self.x, self.y, self.z)

    def __add__(self, point):
        return Point( self.x + point.x
                    , self.y + point.y
                    , self.z + point.z
                    )

    def __sub__(self, point):
        return Point( self.x - point.x
                    , self.y - point.y
                    , self.z - point.z
                    )

    def __rmul__(self, scalar):
        return Point( scalar * self.x
                    , scalar * self.y
                    , scalar * self.z
                    )

    def __div__(self, scalar):
        return Point( self.x / scalar
                    , self.y / scalar
                    , self.z / scalar
                    )

    def __xor__(self, point):
        u = self
        v = point
        return Point( u.y * v.z - u.z * v.y
                    , u.z * v.x - u.x * v.z
                    , u.x * v.y - u.y * v.x
                    )

    def __pow__(self, point):
        return ( self.x * point.x
               + self.y * point.y
               + self.z * point.z
               )

    def magnitude(self):
        return math.sqrt( self.x * self.x
                        , self.y * self.y
                        , self.z * self.z
                        )

    def normalize(self):
        m = self.magnitude()
        self.x /= m
        self.y /= m
        self.z /= m
        return m

def point(obj):
    return Point(obj.x, obj.y, obj.z)


def main():
    p1 = Point(1.0, 2.0, 3.0)
    p2 = Point(5.0, 6.0, 7.0)
    scalar = 10.0
    print("{0} + {1} = {2}".format(p1, p2, p1 + p2))
    print("{0} - {1} = {2}".format(p1, p2, p1 - p2))
    print("{0} . {1} = {2}".format(p1, p2, p1 ** p2))
    print("{0} X {1} = {2}".format(p1, p2, p1 ^ p2))
    print("{0} X {1} = {2}".format(scalar, p1, scalar * p1))
    print("{0} / {1} = {2}".format(p1, scalar, p1 / scalar))

if __name__ == "__main__":
    main()




def cylindrical( theta_start = 0.0
               , theta_end = 2 * math.pi
               , height = 1.0
               , radii  = []
               , height_segments = []
               , radial_segments = []
               , top_open       = False
               , bottom_open    = False
               , position       = Point(0.0, 0.0, 0.0)
               , direction      = Point(0.0, 0.0, 1.0)
               ):
    pass


def spherical( theta_start = 0.0
             , theta_end = 2 * math.pi
             , height = 1.0
             , radii  = []
             , height_segments = []
             , radial_segments = []
             , top_open       = False
             , bottom_open    = False
             , position       = Point(0.0, 0.0, 0.0)
             , direction      = Point(0.0, 0.0, 1.0)
             ):
    pass
