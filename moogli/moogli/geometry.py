import moogli.core._moogli


class Vec2i(moogli.core._moogli.Vec2i):

    def __init__(self, x=0, y=0):
        super(Vec2i, self).__init__(x, y)

    def __repr__(self):
        return ("Vec2i({0}, {1})").format(self.x(),
                                          self.y())

    def __str__(self):
        return self.__repr__()


class Vec2f(moogli.core._moogli.Vec2f):

    def __init__(self, x=0.0, y=0.0):
        super(Vec2f, self).__init__(x, y)

    def __repr__(self):
        return ("Vec2f({0}, {1})").format(self.x(),
                                          self.y())

    def __str__(self):
        return self.__repr__()


class Vec3i(moogli.core._moogli.Vec3i):
    def __init__(self, x=0, y=0, z=0):
        super(Vec3i, self).__init__(x, y, z)

    def __repr__(self):
        return ("Vec3i({0}, {1}, {2})").format(self.x(),
                                               self.y(),
                                               self.z())

    def __str__(self):
        return self.__repr__()


class Vec3f(moogli.core._moogli.Vec3f):
    def __init__(self, x=0.0, y=0.0, z=0.0):
        super(Vec3f, self).__init__(x, y, z)

    def __repr__(self):
        return ("Vec3f({0}, {1}, {2})").format(self.x(),
                                               self.y(),
                                               self.z())

    def __str__(self):
        return self.__repr__()


class Vec4i(moogli.core._moogli.Vec4i):
    def __init__(self, x=0, y=0, z=0, w=0):
        super(Vec4i, self).__init__(x, y, z, w)

    def __repr__(self):
        return ("Vec4i({0}, {1}, {2}, {3})").format(self.x(),
                                                    self.y(),
                                                    self.z(),
                                                    self.w())

    def __str__(self):
        return self.__repr__()


class Vec4f(moogli.core._moogli.Vec4f):
    def __init__(self, x=0.0, y=0.0, z=0.0, w=0.0):
        super(Vec4f, self).__init__(x, y, z, w)

    def __repr__(self):
        return ("Vec4f({0}, {1}, {2}, {3})").format(self.x(),
                                                    self.y(),
                                                    self.z(),
                                                    self.w())

    def __str__(self):
        return self.__repr__()


class Quat(moogli.core._moogli.Quat):
    def __repr__(self):
        return ("Quat({0}, {1}, {2}, {3})").format(self.x(),
                                                   self.y(),
                                                   self.z(),
                                                   self.w())

    def __str__(self):
        return self.__repr__()

X_AXIS = Vec3f(1.0, 0.0, 0.0)
Y_AXIS = Vec3f(0.0, 1.0, 0.0)
Z_AXIS = Vec3f(0.0, 0.0, 1.0)
