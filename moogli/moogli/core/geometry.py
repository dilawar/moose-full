import _moogli
import moogli.core
# sequence()


class Frustum(_moogli.FrustumMesh):

    def subdivide(self, n, ids=None):
        if ids is None:
            ids = [self.get_id() + "[" + str(i) + "]" for i in range(n)]
        frustums = moogli.core.Group(self.get_id())
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
            frustums.attach(Frustum(ids[i],
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

class Sphere(_moogli.SphereMesh):
    pass


# @staticmethod

#     def sequence(sequence_center, sequence_axis, sphere_separation, sphere_count, identifier):
#         span = (sphere_count - 1)* sphere_separation
#         leftmost_center =  sequence_center - sequence_axis * span / 2
#         rightmost_center = sequence_center + sequence_axis * span / 2
        
#         return [ Sphere( sphere_identifier
#                        , sphere_center
#                        , sphere_radius
#                        , sphere_vertices
#                        , sphere_color
#                        ) for sphere_center in
            
#                ]

#     def grid(rows, cols, grid_center, radius, separation):
#         pass
