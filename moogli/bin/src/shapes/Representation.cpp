class Representation
{
    unsigned int points;
    int type;
    float height;
    osg::Vec3f center;
    float a;
    float b;
    float c;
    float phi_begin;
    float phi_end
    float phi_begin;
    float phi_end;
    float theta_begin;
    float theta_end;
    float radius
    float inner_radius;
    float outer_radius;
    float apex_radius;
    float base_radius;
    osg::Vec3f axis;
    osg::Vec3f base_direction;
    osg::Vec3f apex_direction;
    unsigned int longitudinal_segments;
    unsigned int horizontal_segments;


    set_color();
    update();


    static Geometry *
    ring(uint points);

    static Geometry *
    ring(uint points, Geometry * geometry, index)

};

class Tube
{
    osg::Vec3Array centers;
    osg::Vec3Array radii;
    unsigned int points;
    float apex_radius;
    float base_radius;



};

Pyramid - base_radius
          height;
          axis;
          points
          base_direction;

BiPyramid - center_radius;


Prism - base_radius
        apex_radius
        axis
        base_direction
        apex_direction
        points

AntiPrism - base_radius
            apex_radius
            axis
            base_direction
            apex_direction
            points



DoublePyramid


class Representation:
    def __init__(self, type =, ):
        pass

class Pyramid(Representation):
    def __init__(self, points = 3):
        Representation.__init__(type = PYRAMID)

class DoublePyramid(Representation):
    pass



class Prism(Representation):
    pass

class AntiPrism(Representation):
    pass

class Sphere

class Icosahedron

class Dodecahedron

class

