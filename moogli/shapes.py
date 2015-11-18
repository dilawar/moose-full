from core import _moogli
import moogli
import collections

class Frustum(_moogli.Frustum):
    def subdivide(self, n, ids=None):
        if ids is None:
            ids = [self.id() + "[" + str(i) + "]" for i in range(n)]
        frustums = moogli.Group(self.id())
        base = self.get_base()
        axis = self.get_axis()
        length = self.get_length()
        base_radius = self.get_base_radius()
        apex_radius = self.get_apex_radius()
        base_color = self.get_base_color()
        apex_color = self.get_apex_color()
        vertices = self.get_vertices()
        delta_length = length / n
        delta_radius = (apex_radius - base_radius) / n
        new_base = base
        new_base_radius = base_radius
        for i in range(n):
            apex = base + axis * delta_length * (i + 1)
            apex_radius = base_radius + delta_radius * (i + 1)
            frustums.attach_shape(Frustum(ids[i],
                                          new_base,
                                          apex,
                                          new_base_radius,
                                          apex_radius,
                                          vertices,
                                          base_color,
                                          apex_color))
            new_base = apex
            new_base_radius = apex_radius

        return frustums

    @classmethod
    def sequence(cls):
        pass

    @classmethod
    def grid(cls):
        pass

    @classmethod
    def lattice(cls):
        pass


class Sphere(_moogli.Sphere):

    @classmethod
    def sequence(cls):
        pass

    @classmethod
    def grid(cls,
             id,
             center,
             row_axis,
             row_count,
             row_separation,
             col_axis,
             col_count,
             col_separation,
             radii,
             colors,
             vertices):
        group = moogli.Group(id)
        if not isinstance(colors, collections.Sequence):
            colors = [colors]
        if not isinstance(radii, collections.Sequence):
            radii = [radii]
        pseudo_center = row_axis * row_separation * (col_count - 1) / 2.0 + col_axis * col_separation * (row_count - 1) / 2.0
        translation = center - pseudo_center
        for row_index in range(row_count):
            for col_index in range(col_count):
                index = row_index * col_count + col_index
                shape_center = translation + row_axis * col_index * col_separation + col_axis * row_index * row_separation
                # new_center = moogli.core.Vec3f(i * soma_radius * 5.0,
                #                                0.0,
                #                                j * soma_radius * 5.0) + soma_center - moogli.core.Vec3f( 49 * soma_radius * 2.5, 0.0, 49 * soma_radius * 2.5)
                shape_color = colors[index % len(colors)]
                shape_radius = radii[index % len(radii)]
                group.attach_shape(cls("{0}[{1}][{2}]".format(id,
                                                              row_index,
                                                              col_index),
                                       shape_center,
                                       shape_radius,
                                       vertices,
                                       shape_color))
        return group

    @classmethod
    def lattice(cls):
        pass

    def subdivide():
        pass

__all__ = ["Frustum",
           "Sphere"]
