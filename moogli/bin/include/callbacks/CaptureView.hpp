#ifndef __CAPTURE_VIEW_HPP__
#define __CAPTURE_VIEW_HPP__

#include "utility/libraries.hpp"

class CaptureView : public osg::Camera::DrawCallback
{
public:
    CaptureView();

    void
    start_capture( const std::string & filename_prefix
                 , const std::string & format
                 , const unsigned long frame_count
                 );
    void
    pause_capture();

    void
    continue_capture();

    void
    stop_capture() const;

    virtual void
    operator() (osg::RenderInfo& renderInfo) const;

private:

    const std::string
    get_filename(osg::Camera * camera) const;

    mutable bool _capture_next;
    std::string _filename_prefix;
    std::string _format;
    unsigned long _frame_count;
    mutable unsigned long _frame_index;
    mutable unsigned long _sequence_index;
    osg::ref_ptr<osg::Image> _image;
};

#endif /* __CAPTURE_VIEW_HPP__ */
