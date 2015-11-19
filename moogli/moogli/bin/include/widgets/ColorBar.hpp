#ifndef __COLOR_BAR_HPP__
#define __COLOR_BAR_HPP__

#include "utility/libraries.hpp"

class ColorBar
{
public:
    osg::ref_ptr<osgSim::ScalarBar> scalar_bar;
    osg::ref_ptr<osg::PositionAttitudeTransform> root;
    std::string id;

    struct LabelFormatter : public osgSim::ScalarBar::ScalarPrinter
    {
        unsigned int _precision;
        LabelFormatter(unsigned int precision);

        unsigned int
        get_precision() const;

        virtual std::string
        printScalar(float scalar);
    };


    ColorBar(const std::string & id);

    ColorBar( const std::string & id
            , const osg::Vec2f & scalar_range
            , const std::vector<osg::Vec4f> & colors
            , int label_count
            , const std::string & title
            , const osg::Vec2f & position
            , const osg::Vec2f & size
            , float orientation
            , const std::string & text_font
            , const osg::Vec2i text_font_resolution
            , float text_character_size
            , const osg::Vec4f & text_color
            );
    void
    resize();

    void
    resize(int width, int height);

    void
    resize(int x, int y, int width, int height);

    void
    set_parent_viewport(const osg::Vec4i & viewport);

    void
    set_scalar_range(const osg::Vec2f & scalar_range);

    osg::Vec2f
    get_scalar_range() const;

    void
    set_colors(const std::vector<osg::Vec4f> & colors);

    void
    set_label_count(int label_count);

    int
    get_label_count() const;

    void
    set_title(const std::string & title);

    const std::string &
    get_title () const;

    void
    set_position(const osg::Vec2f & position);

    const osg::Vec2f &
    get_position() const;

    void
    set_size(const osg::Vec2f & size);

    const osg::Vec2f &
    get_size() const;

    void
    set_orientation(float orientation);

    float
    get_orientation() const;

    void
    set_text_font(const std::string & font);

    const std::string &
    get_text_font() const;

    void
    set_text_font_resolution(const osg::Vec2i & font_resolution);

    osg::Vec2i
    get_text_font_resolution() const;

    void
    set_text_character_size(float character_size);

    float
    get_text_character_size() const;

    void
    set_text_color(const osg::Vec4f & color);

    const osg::Vec4f &
    get_text_color() const;

    void
    set_label_formatting_precision(unsigned int precision);

    unsigned int
    get_label_formatting_precision() const;

private:
    osg::Vec2f _position;
    osg::Vec2f _size;
    float _orientation;
    osg::Vec4i _parent_viewport;
};


#endif /* __COLOR_BAR_HPP__ */
