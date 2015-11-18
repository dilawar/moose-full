%Module _moogli

%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip

class Vec2i
{
%TypeHeaderCode
#include <osg/Vec2i>
using osg::Vec2i;
%End
public:
    Vec2i();
 	  Vec2i(int x, int y);
    bool 	operator== (const Vec2i &v) const;
    bool 	operator!= (const Vec2i &v) const;
    bool 	operator< (const Vec2i &v) const;
    void 	set(int x, int y);
    void 	set(const Vec2i &rhs);
    int x() const;
    int y() const;
    int r() const;
    int g() const;
    Vec2i	operator* (int rhs) const;
    Vec2i	operator/ (int rhs) const;
    Vec2i	operator+ (int rhs) const;
    Vec2i	operator- (int rhs) const;
    Vec2i	operator+ (const Vec2i &rhs) const;
    Vec2i	operator- (const Vec2i &rhs) const;
    Vec2i	operator* (const Vec2i &rhs) const;
};

class Vec2f
{
%TypeHeaderCode
#include <osg/Vec2f>
using osg::Vec2f;
%End
public:
 	  Vec2f();
 	  Vec2f(float x, float y);
    bool operator== (const Vec2f &v) const;
    bool operator!= (const Vec2f &v) const;
    bool operator< (const Vec2f &v) const;
    void set(float x, float y);
    float x() const;
    float y() const;
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec2f &rhs) const;
    const Vec2f	operator* (float rhs) const;
    const Vec2f	operator/ (float rhs) const;
    const Vec2f	operator+ (const Vec2f &rhs) const;
    const Vec2f	operator- (const Vec2f &rhs) const;
    const Vec2f	operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Vec3i
{
%TypeHeaderCode
#include <osg/Vec3i>
using osg::Vec3i;
%End
public:
    Vec3i();
 	  Vec3i(int r, int g, int b);
    bool operator== (const Vec3i &v) const;
    bool operator!= (const Vec3i &v) const;
    bool operator< (const Vec3i &v) const;
    void set(int r, int g, int b);
    void set(const Vec3i &rhs);
    int	x() const;
    int	y() const;
    int	z() const;
    int	r() const;
    int	g() const;
    int	b() const;
    Vec3i	operator* (int rhs) const;
    Vec3i	operator/ (int rhs) const;
    Vec3i	operator+ (int rhs) const;
    Vec3i	operator- (int rhs) const;
    Vec3i	operator+ (const Vec3i &rhs) const;
    Vec3i	operator- (const Vec3i &rhs) const;
    Vec3i	operator* (const Vec3i &rhs) const;
};

class Vec3f
{
%TypeHeaderCode
#include <osg/Vec3f>
using  osg::Vec3f;
%End
public:
    Vec3f();
    Vec3f(const Vec2f &v2, float z);
 	  Vec3f(float x, float y, float z);
    bool operator== (const Vec3f &v) const;
    bool operator!= (const Vec3f &v) const;
    bool operator< (const Vec3f &v) const;
    void set(float x, float y, float z);
    void set(const Vec3f &rhs);
    float x() const;
    float y() const;
    float z() const;
    bool valid() const;
    bool isNaN () const;
    float operator* (const Vec3f &rhs) const;
    const Vec3f operator^ (const Vec3f &rhs) const;
    const Vec3f	operator* (float rhs) const;
    const Vec3f operator/ (float rhs) const;
    const Vec3f operator+ (const Vec3f &rhs) const;
    const Vec3f operator- (const Vec3f &rhs) const;
    const Vec3f operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Vec4i
{
%TypeHeaderCode
#include <osg/Vec4i>
using osg::Vec4i;
%End
public:
    Vec4i();
 	  Vec4i(int x, int y, int z, int w);
    bool operator== (const Vec4i &v) const;
    bool operator!= (const Vec4i &v) const;
    bool operator< (const Vec4i &v) const;
    void set(int x, int y, int z, int w);
    int x() const;
    int y() const;
    int	z() const;
    int	w() const;
    int r() const;
    int	g() const;
    int	b() const;
    int	a() const;
    Vec4i	operator* (int rhs) const;
    Vec4i	operator/ (int rhs) const;
    Vec4i	operator+ (int rhs) const;
    Vec4i	operator- (int rhs) const;
    Vec4i	operator+ (const Vec4i &rhs) const;
    Vec4i operator- (const Vec4i &rhs) const;
    Vec4i	operator* (const Vec4i &rhs) const;
};


class Vec4f
{
%TypeHeaderCode
#include <osg/Vec4f>
using osg::Vec4f;
%End
public:
 	  Vec4f();
 	  Vec4f(float x, float y, float z, float w);
 	  Vec4f(const Vec3f &v3, float w);
    bool operator== (const Vec4f &v) const;
    bool operator!= (const Vec4f &v) const;
    bool operator< (const Vec4f &v) const;
    void set(float x, float y, float z, float w);
    float x() const;
    float y() const;
    float z() const;
    float w() const;
    float r() const;
    float g() const;
    float b() const;
    float a() const;
    unsigned int asABGR() const;
    unsigned int asRGBA() const;
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec4f &rhs) const;
    Vec4f operator* (float rhs) const;
    Vec4f operator/ (float rhs) const;
    Vec4f operator+ (const Vec4f &rhs) const;
    Vec4f operator- (const Vec4f &rhs) const;
    const Vec4f operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Quat
{
%TypeHeaderCode
#include <osg/Quat>
using namespace osg;
%End
public:
    Quat();
 	  Quat(float x, float y, float z, float w);
 	  Quat(const Vec4f &v);
 	  Quat(float angle, const Vec3f &axis);
 	  Quat(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    bool operator== (const Quat &v) const;
    bool operator!= (const Quat &v) const;
    bool operator< (const Quat &v) const;
    Vec4f asVec4() const;
    Vec3f asVec3() const;
    void set(float x, float y, float z, float w);
    void set(const Vec4f &v);
    float x() const;
    float y() const;
    float z() const;
    float w() const;
    bool zeroRotation() const;
    const Quat operator* (float rhs) const;
    const Quat operator* (const Quat &rhs) const;
    Quat operator/ (float rhs) const;
    const Quat operator/ (const Quat &denom) const;
    const Quat operator+ (const Quat &rhs) const;
    const Quat operator- (const Quat &rhs) const;
    const Quat operator- () const;
    float length() const;
    float length2() const;
    Quat conj() const;
    const Quat inverse() const;
    void makeRotate(float angle, float x, float y, float z);
    void makeRotate(float angle, const Vec3f &vec);
    void makeRotate(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    void makeRotate(const Vec3f &vec1, const Vec3f &vec2);
    Vec3f operator* (const Vec3f &v) const;
};


class MoogliShape /Abstract/ 
{
%TypeHeaderCode
#include "shapes/Shape.hpp"
%End

public:
    MoogliShape(const char * id);

    PyObject *
    id() const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> id().c_str());
    %End

    // Mesh *
    // get_parent() const;

    // void
    // set_parent(Mesh * parent /Transfer/);

    // SIP_PYTUPLE
    // subdivide(unsigned int n) const;
    // %MethodCode
    //     sipRes = PyTuple_New(n);
    //     SIP_SSIZE_T index = 0;
    //     for(const Mesh * mesh : sipCpp -> subdivide(a0))
    //     {
    //         PyTuple_SET_ITEM( sipRes
    //                         , index
    //                         , sipConvertFromType( mesh
    //                                             , sipFindType("Mesh")
    //                                             , NULL
    //                                             )
    //                         );
    //         index += 1;
    //     }
    // %End
    void
    hide();

    void
    show();

    bool
    visible();

    bool
    export_geometry(const char * filename, bool with_effects = false) const;

    void
    enable_effects();

    void
    disable_effects();

    void
    toggle_effects();

    void
    enable_outline_effect();

    void
    disable_outline_effect();

    void
    toggle_outline_effect();

    bool
    outline_effect_enabled() const;

    void
    set_outline_effect_line_width(float width);

    float
    get_outline_effect_line_width() const;

    void
    set_outline_effect_line_color(const Vec4f & color);

    const Vec4f &
    get_outline_effect_line_color() const;

    void
    enable_cartoon_effect();

    void
    disable_cartoon_effect();

    void
    toggle_cartoon_effect();

    bool
    cartoon_effect_enabled() const;

    void
    set_cartoon_effect_outline_width(float width);

    float
    get_cartoon_effect_outline_width() const;

    void
    set_cartoon_effect_outline_color(const Vec4f & color);

    const Vec4f &
    get_cartoon_effect_outline_color() const;

    void
    enable_wireframe_effect();

    void
    disable_wireframe_effect();

    void
    toggle_wireframe_effect();

    bool
    wireframe_effect_enabled() const;

    void
    set_wireframe_effect_line_width(float width);

    float
    get_wireframe_effect_line_width() const;

    void
    set_wireframe_effect_line_color(const Vec4f & color);

    const Vec4f &
    get_wireframe_effect_line_color() const;

    void
    enable_specular_highlights_effect();

    void
    disable_specular_highlights_effect();

    void
    toggle_specular_highlights_effect();

    bool
    specular_highlights_effect_enabled() const;

    void
    set_specular_highlights_effect_exponent(float exponent);

    float
    get_specular_highlights_effect_exponent() const;

    void
    get_specular_highlights_effect_color(const Vec4f & color);

    const Vec4f &
    get_specular_highlights_effect_color() const;
};


class Frustum : public MoogliShape 
{
%TypeHeaderCode
#include "shapes/Frustum.hpp"
%End

public:

    Frustum(const char * id);

    Frustum( const char * id
           , const Vec3f & center
           , const Vec3f & axis
           , float length
           , float base_radius
           , float apex_radius
           , unsigned int vertices
           , const Vec4f & base_color
           , const Vec4f & apex_color
           );

    Frustum( const char * id
           , const Vec3f & base
           , const Vec3f & apex
           , float base_radius
           , float apex_radius
           , unsigned int vertices
           , const Vec4f & base_color
           , const Vec4f & apex_color
           );
    void
    set_geometry( const Vec3f & center
                , const Vec3f & axis
                , float length
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set_geometry( const Vec3f & base
                , const Vec3f & apex
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , const Vec3f & axis
       , float length
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set( const Vec3f & base
       , const Vec3f & apex
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_axis(const Vec3f & axis);

    Vec3f
    get_axis() const;

    void
    set_length(const float & length);

    float
    get_length() const;

    void
    set_center(const Vec3f & center);

    Vec3f
    get_center() const;

    void
    set_apex(const Vec3f & apex);

    Vec3f
    get_apex() const;

    void
    set_base(const Vec3f & base);

    Vec3f
    get_base() const;

    void
    set_base_radius(const float base_radius);

    float
    get_base_radius() const;

    void
    set_apex_radius(const float apex_radius);

    float
    get_apex_radius() const;

    void
    set_radii(float base_radius, float apex_radius);

    void
    set_radius(float radius);

    void
    set_base_color(const Vec4f & base_color);

    void
    set_apex_color(const Vec4f & apex_color);

    const Vec4f &
    get_base_color() const;

    const Vec4f &
    get_apex_color() const;

    void
    set_color(const Vec4f & color);

    void
    set_colors(const Vec4f & base_color, const Vec4f & apex_color);

    void
    move_apex_by(float dl);

    void
    move_base_by(float dl);

    void
    move_apex_along(const Vec3f & direction, float dl);

    void
    move_base_along(const Vec3f & direction, float dl);

    void
    move_center_by(const Vec3f & displacement);

    void
    rotate_axis_along(const Vec3f & rotation_axis, float angle);

};

class MoogliSphere : public MoogliShape 
{
%TypeHeaderCode
#include "shapes/Sphere.hpp"
%End

public:

    MoogliSphere(const char * id);

    MoogliSphere( const char * id
          , const Vec3f & center
          , const float radius
          , unsigned int vertices
          , const Vec4f & color
          );

    void
    set_geometry( const Vec3f & center
                , float radius
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , float radius
       , unsigned int vertices
       , const Vec4f & color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_center(const Vec3f & center);

    Vec3f
    get_center() const;

    void
    set_radius(float radius);

    float
    get_radius() const;

    const Vec4f &
    get_color() const;

    void
    set_color(const Vec4f & color);

    void
    move_center_by(const Vec3f & displacement);
};


class ColorBar
{
%TypeHeaderCode
#include "widgets/ColorBar.hpp"
%End

public:
    ColorBar(const char * id);

    // ColorBar( const char * id
    //         , Vec2f & scalar_range
    //         , const std::vector<osg::Vec4f> & colors
    //         , int label_count
    //         , const char * title
    //         , const Vec2f & position
    //         , const Vec2f & size
    //         , float orientation
    //         , const char * text_font
    //         , const osg::Vec2i text_font_resolution
    //         , float text_character_size
    //         , const osg::Vec4f & text_color
    //         );

    void
    resize();

    void
    resize(int width, int height);

    void
    resize(int x, int y, int width, int height);

    void
    set_parent_viewport(const Vec4i & viewport);

    void
    set_scalar_range(const Vec2f & scalar_range);

    Vec2f
    get_scalar_range() const;

    void
    set_colors(PyObject * colors);
    %MethodCode
        unsigned int length = PySequence_Length(a0);
        unsigned int i;
        int iserr = 0;
        int state;

        std::vector<osg::Vec4f> colors(length);
        for(i = 0; i < length; ++i)
        {
            osg::Vec4f * color = static_cast<osg::Vec4f *>(sipConvertToType( PySequence_GetItem(a0, i)
                                                                           , sipFindType("Vec4f")
                                                                           , NULL
                                                                           , SIP_NO_CONVERTORS
                                                                           , NULL
                                                                           , &iserr
                                                                           )
                                                          );
            colors[i] = *color;
        }
        sipCpp -> set_colors(colors);
    %End

    void
    set_label_count(int label_count);

    int
    get_label_count() const;

    void
    set_title(const char * title);

    PyObject *
    get_title () const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> get_title().c_str());
    %End

    void
    set_position(const Vec2f & position);

    const Vec2f &
    get_position() const;

    void
    set_size(const Vec2f & size);

    const Vec2f &
    get_size() const;

    void
    set_orientation(float orientation);

    float
    get_orientation() const;

    void
    set_text_font(const char * font);

    PyObject *
    get_text_font() const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> get_text_font().c_str());
    %End

    void
    set_text_font_resolution(const Vec2i & font_resolution);

    Vec2i
    get_text_font_resolution() const;

    void
    set_text_character_size(float character_size);

    float
    get_text_character_size() const;

    void
    set_text_color(const Vec4f & color);

    const Vec4f &
    get_text_color() const;

    void
    set_label_formatting_precision(unsigned int precision);

    unsigned int
    get_label_formatting_precision() const;
};

class MeshView 
{
%TypeHeaderCode
#include "view/View.hpp"
%End

public:
    MeshView();

    void
    set_position(const Vec2f & position);

    const Vec2f &
    get_position() const;

    void
    set_size(const Vec2f & size);

    const Vec2f &
    get_size() const;

    void
    set_background_color(const Vec4f & background_color);

    const Vec4f &
    get_background_color();

    void
    resize(int width, int height);

    void
    attach_color_bar(ColorBar * color_bar);

    void
    detach_color_bar(ColorBar * color_bar);

    void
    home();

    void
    forward(double distance_delta);

    void
    backward(double distance_delta);

    void
    left(double distance_delta);

    void
    right(double distance_delta);

    void
    up(double distance_delta);

    void
    down(double distance_delta);

    void
    zoom(double factor);

    void
    roll(double angle);

    void
    pitch(double angle);

    void
    yaw(double angle);

};



class Viewer : public QGLWidget
{
%TypeHeaderCode
#include "view/Viewer.hpp"
%End

public:

    Viewer( QWidget * parent = nullptr
          , const QGLWidget * share_widget = nullptr
          , Qt::WindowFlags f = 0
          );

    PyObject *
    get_view_with_focus();
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> get_view_with_focus() -> getName().c_str());
    %End

    void
    detach_view(MeshView * mesh_view);
 
    void
    attach_view(MeshView * mesh_view);

    void
    attach_shape(MoogliShape * shape);

    void
    detach_shape(MoogliShape * shape);

};
