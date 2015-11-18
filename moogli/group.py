import collections


class Group():

    def __init__(self, id):
        self._id = id
        self.groups = collections.OrderedDict()
        self.shapes = collections.OrderedDict()

    def id(self):
        return self._id

    def attach_shape(self, shape):
        # element.groups.add(element)
        if shape.id() not in self.shapes.keys():
            self.shapes[shape.id()] = shape
            #shape.groups[self.id()] = self

    def detach_shape(self, shape):
        # element.groups.remove(self)
        if isinstance(shape, str):
            shape = self.shapes[shape.id()]
        self.shapes.pop(shape.id())
        # shape.groups.pop(self.id())

    def attach_shapes(self, shapes):
        map(self.attach_shape, shapes)

    def attach_group(self, group):
        self.groups[group.id()] = group

    def detach_group(self, group):
        if isinstance(group, str):
            group = self.groups[group.id()]
        self.groups.pop(group.id())

    def hide(self):
        pass

    def show(self):
        pass

    def outline(self):
        pass

    def enable_outline_mode(self):
        pass

    def disable_outline_mode(self):
        pass

    def enable_wireframe_mode(self):
        pass

    def disable_wireframe_mode(self):
        pass

    def set_outline(self):
        pass

    def set_outline_color(self):
        pass

    def set_outline_width(self):
        pass

    def set_wireframe_line_width(self):
        pass

    def set_wireframe_color(self):
        pass

    def translate(self, displacement):
        for shape in self.shapes.values():
            shape.translate(displacement)

    def rotate(self, quaternion):
        for shape in self.shapes.values():
            shape.rotate(quaternion)

    def scale(self, scale):
        for shape in self.shapes.values():
            shape.scale(scale)

    def set(self, property, values, mapper=lambda x: x,
            permissive=False):
        if not isinstance(values, collections.Sequence):
            values = [values] * len(self.shapes)
        for (shape, value) in zip(self.shapes.values(), values):
            try:
                setter = getattr(shape, "set_" + property)
                setter(mapper(value))
            except AttributeError, e:
                if permissive:
                    continue
                else:
                    content = """
                    Invalid method 'set_{2}' called for {0}({1}).
                    If the group contains different type of shapes, consider
                    calling Group.set_property with keyword argument
                    'permissive=True'. This will silently skip over shapes in
                    the group which do not support this property.
                    """
                    raise AttributeError(content)

    def get(self, property, mapper=lambda x: x, permissive=False):
        values = []
        for shape in self.shapes.values():
            try:
                getter = getattr(shape, "get_" + property)
                values.append(mapper(getter()))
            except AttributeError, e:
                if permissive:
                    values.append(None)
                else:
                    content = """
                    Invalid method 'get_{2}' called for {0}({1}).
                    If the group contains different type of shapes, consider
                    calling Group.get_property with keyword argument
                    'permissive=True'. This will return None for shapes in
                    the group which do not support this property.
                    """
                    raise AttributeError(content)

__all__ = ["Group"]
