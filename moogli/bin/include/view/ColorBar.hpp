#include "utility/libraries.hpp"

class OSG_EXPORT ColorBar : public MatrixTransform
{
public :

     ColorBar();

        /** Copy constructor using CopyOp to manage deep vs shallow copy.*/
     ColorBar(const MatrixTransform&,const CopyOp& copyop=CopyOp::SHALLOW_COPY);

     ColorBar(const Matrix& matix);

     META_Node(osg, ColorBar);

     virtual ColorBar* asColorBar() { return this; }
     virtual const ColorBar* asColorBar() const { return this; }

    protected :

        virtual ~MatrixTransform();

        Matrix                              _matrix;
        mutable Matrix                      _inverse;
        mutable bool                        _inverseDirty;


};

#endif
