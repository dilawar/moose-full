#include "mesh/Ring.hpp"

Ring::Ring() : _rings(101)
{
}

osg::Vec3Array *
Ring::_unit( uint radial_segments
    )
{
    if(radial_segments > 100)
    {
        _rings.resize(radial_segments + 1);
    }

    if(_rings[radial_segments] == nullptr)
    {
        _rings[radial_segments] = new osg::Vec3Array(radial_segments);
        for(size_t i = 0; i < radial_segments; ++i)
        {
            double theta = (i * 2 * osg::PI) / static_cast<double>(radial_segments);
            (*(_rings[radial_segments]))[i].set( float(cos(theta))
                                                     , float(sin(theta))
                                                     , 0.0f
                                                     );
        }
    }
    return _rings[radial_segments];
}

void
Ring::unit( uint radial_segments
    , osg::Vec3Array * points
    , uint index
    )
{
    osg::Vec3Array * ring = _unit(radial_segments);
    for(size_t i = 0; i < ring -> size(); ++i)
    {
        (*points)[index + i] = (*ring)[i];
    }

}


// void
// Ring::operator( const osg::Vec3f & center
//               , const osg::Vec3f & u
//               , const osg::Vec3f & v
//               , const float        radius
//               , uint radial_segments
//               , osg::Vec3Array   * points
//               , uint               index
//               )
// {
//         for(size_t i = 0; i < ring -> size(); ++i)
//     {
//         (*points)[index + i] =  u * (*ring)[i][0] * radius
//                              +  v * (*ring)[i][1] * radius
//                              + center;
//         //rotate * (((*ring)[i] * radius)) + center;
//     }

// }

void
Ring::operator()( const osg::Vec3f & center
                , const osg::Vec3f & direction
                , const float        radius
                , uint               radial_segments
                , osg::Vec3Array   * points
                , uint               index
                )
{
    osg::Quat rotate, correction;
    osg::Vec3Array * ring = _unit(radial_segments);
    // direction.normalize();

    osg::Vec3f D = direction;
    // if(direction[2] < 0.0)
    // {
    //     D = direction * -1;
    //     std::cerr << "PROBLEM" << std::endl;
    // // }
    //     x = -x;
    //     y = -y;
    //     z = -z;
    // }
    float z = D[2];
    float y = D[1];
    float x = D[0];

    osg::Vec3f u;
    float r = sqrt(x * x + z * z);
    std::cerr << "Alpha => " << z / r << std::endl;
    if(r == 0.0f)
    {
        std::cerr << "r is zero" << std::endl;
        u = osg::Vec3f(1.0f, 0.0f, 0.0f);
    }
    else
    {
        u = osg::Vec3f( z / r
                      , 0.0f
                      , -x / r
                      );
    }

    // if(u[0] < 0.0)
    // {
    //     u = -u;
    // }
    // else if(u[0] == 0.0 && u[2] < 0.0)
    // {
    //     u = -u;
    // }
    osg::Vec3f v = D ^ u;

    // if(v[1] < 0.0)
    // {
    //     v = -v;
    // }


    // rotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);
    // float angle = atan2(direction[1], direction[0]);

    // std::cout << "Angle in Ring => " << (angle * 180.0 / osg::PI) << std::endl;
    // if(angle >= 0.0)
    // {
    //     correction.makeRotate(angle, osg::Vec3f(0.0f, 0.0f, -1.0f));
    // }
    // else
    // {
    //     correction.makeRotate(angle, osg::Vec3f(0.0f, 0.0f, -1.0f));
    // }

    // osg::Vec3f corrected( direction[0]
    //                     , direction[1]
    //                     , 0.0f
    //                     );
    // corrected.normalize();
    // correction.makeRotate(corrected, osg::Vec3f(1.0f, 0.0f, 0.0f));
    for(size_t i = 0; i < ring -> size(); ++i)
    {
        (*points)[index + i] =  u * (*ring)[i][0] * radius
                             +  v * (*ring)[i][1] * radius
                             + center;
        //rotate * (((*ring)[i] * radius)) + center;
    }

    std::cerr << "D         => " << D[0] << "," << D[1] << "," << D[2] << std::endl;

    std::cerr << "Red => " << (*points)[index + 0][0] << "," << (*points)[index + 0][1] << "," << (*points)[index + 0][2] << std::endl;
    std::cerr << "Green => " << (*points)[index + radial_segments / 4][0] << "," << (*points)[index + radial_segments / 4][1] << "," << (*points)[index + radial_segments / 4][2] << std::endl;
    std::cerr << "Blue => " << (*points)[index + radial_segments / 2][0] << "," << (*points)[index + radial_segments / 2][1] << "," << (*points)[index + radial_segments / 2][2] << std::endl;
    std::cerr << "Yellow => " << (*points)[index + 3 * radial_segments / 4][0] << "," << (*points)[index + 3 * radial_segments / 4][1] << "," << (*points)[index + 3 * radial_segments / 4][2] << std::endl;

}

void
Ring::operator()( const osg::Vec3Array * centers
                , const osg::Vec3Array * directions
                , const std::vector<float>  & radii
                , uint                   radial_segments
                , osg::Vec3Array       * points
                , uint                   index
                , uint                   delta
                )
{
    uint ring_count = centers -> size();
    // osg::Vec3Array ring = unit_ring(points);
    for(uint i = 0; i < ring_count; ++i)
    {
        operator()( (*centers)[i]
                  , (*directions)[i]
                  , radii[i]
                  , radial_segments
                  , points
                  , index + i * (radial_segments + delta)
                  );
    }
}
