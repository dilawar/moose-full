#include "callbacks/CaptureView.hpp"

CaptureView::CaptureView(): _image(new osg::Image)
                          , _sequence_index(0)
                          , _frame_index(0)
                          , _capture_next(false)
{ }

void
CaptureView::start_capture( const std::string & filename_prefix
                          , const std::string & format
                          , const unsigned long frame_count
                          )
{
    _filename_prefix = filename_prefix;
    _format = format;
    _frame_count = frame_count;
    _frame_index = 0;
    continue_capture();
}

void
CaptureView::pause_capture()
{
    _capture_next = false;
}

void
CaptureView::continue_capture()
{
    _capture_next = true;
}

void
CaptureView::stop_capture() const
{
    _capture_next = false;
    ++_sequence_index;
}

void
CaptureView::operator()(osg::RenderInfo& renderInfo) const
{
    if(_frame_index == _frame_count) { stop_capture(); }

    if(!_capture_next) { return; }

    osg::Camera * camera = renderInfo.getCurrentCamera();
    osg::Viewport * viewport = camera ? camera -> getViewport() : 0;
    if(viewport && _image.valid())
    {
        _image->readPixels( int(viewport -> x())
                          , int(viewport -> y())
                          , int(viewport -> width())
                          , int(viewport -> height())
                          , GL_RGBA
                          , GL_UNSIGNED_BYTE
                          );
        osgDB::writeImageFile(*_image, get_filename(camera));
        ++_frame_index;
        //camera -> setPostDrawCallback(nullptr);
        return;
        // osg::notify(osg::NOTICE)<<"Taken screenshot, and written to '"<<_filename<<"'"<<std::endl;
    }
}

const std::string
CaptureView::get_filename(osg::Camera * camera) const
{
    char frame_index_buffer[11];
    char sequence_index_buffer[4];
    std::sprintf(sequence_index_buffer, "%.3lu", _sequence_index);
    std::sprintf(frame_index_buffer, "%.10lu", _frame_index);

    return ( _filename_prefix
           + "-"
           + camera -> getView() -> getName()
           + "-"
           + std::string(sequence_index_buffer)
           + "-"
           + std::string(frame_index_buffer)
           + "."
           + _format
           );
}
