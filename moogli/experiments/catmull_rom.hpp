osg::Vec3f FACE_NORMAL(osg::Vec3f x, osg::Vec3f y, osg::Vec3f z)
{
  return (y - x) ^ (z - y);
}


void
compute_triangles( osg::Geometry * G
                 , size_t axial_segments
                 , size_t radial_segments
                 )
{
    // RECORD_ERROR("BEGIN");
    osg::DrawElementsUShort * I = static_cast<osg::DrawElementsUShort *>( G -> getPrimitiveSet(0) );

    size_t triangle_index = 0;
    size_t row, col;
    for(row = 0; row < axial_segments - 1; ++row)
    {
        for(col = 0 ; col < radial_segments; ++col)
        {
            /*************************************
            d---------------c
            |               |
            |               |
            |               |
            |               |
            |               |
            |               |
            a---------------b

            ***************************************/

            // RECORD_ERROR("BEGIN");
            ushort a = row * radial_segments + col;
            ushort b = a + 1;
            ushort d = a + radial_segments;
            ushort c = d + 1;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*I)[triangle_index    ] = a;
            (*I)[triangle_index + 1] = b;
            (*I)[triangle_index + 2] = d;

            (*I)[triangle_index + 3] = d;
            (*I)[triangle_index + 4] = b;
            (*I)[triangle_index + 5] = c;

            // (*indices)[triangle_index    ] = a;
            // (*indices)[triangle_index + 1] = b;
            // // (*indices)[triangle_index + 2] = b; //
            // // (*indices)[triangle_index + 3] = b + 1; //
            // (*indices)[triangle_index + 2] = d;
            // (*indices)[triangle_index + 3] = c;
            // (*indices)[triangle_index + 4] = b;
            // (*indices)[triangle_index + 5] = c;

            triangle_index += 6;
        }

        (*I)[triangle_index - 1]     = (row + 1) * radial_segments;
        (*I)[triangle_index - 2]     = row       * radial_segments;
        (*I)[triangle_index - 5]     = row       * radial_segments;
        // (*indices)[triangle_index - 1]     = row * radial_segments + radial_segments ;
        // (*indices)[triangle_index - 3]     = row * radial_segments;
    }
    // RECORD_ERROR("ENDED");
}


void
compute_normals( osg::Geometry * G
               , size_t axial_segments
               , size_t radial_segments
               )
{
    osg::Vec3Array * V = static_cast<osg::Vec3Array *>( G -> getVertexArray() );
    osg::Vec3Array * N = static_cast<osg::Vec3Array *>( G -> getNormalArray() );
    size_t row, col;
//     for(size_t row = 0; row < axial_segments; ++row)
//     {
//         for(size_t col = 0; col < radial_segments / 2; ++col)
//         {
//             size_t current_index    = row * radial_segments + col;
//             size_t opposite_index   = current_index + radial_segments / 2;
//             (*normals)[current_index] = (*vertices)[current_index]
//                                       - (*vertices)[opposite_index];
//             (*normals)[current_index].normalize();
//             (*normals)[opposite_index] = (*normals)[current_index] * -1.0f;
//         }
//     }
//
    size_t current_index;
    ushort a;
    ushort b;
    ushort e;
    ushort d;
    ushort c;
    ushort f;
    ushort g;
    ushort h;
    ushort i;


    /*****************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e

      ***************************************/
      row = 0;
      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
              + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
              + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 
      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            d = a + radial_segments;
            c = b + radial_segments;
            f = e + radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
                    + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
                    + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
            (*N)[b].normalize();
        }

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
              + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
              + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 

    for(row = 1; row < axial_segments - 1; ++row)
    {

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/

      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
              + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
              + FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
              + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 
      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

            d---------------c---------------f
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            a---------------b---------------e
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            g---------------h---------------i

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            d = a + radial_segments;
            c = b + radial_segments;
            f = e + radial_segments;
            g = a - radial_segments;
            h = b - radial_segments;
            i = e - radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
                                + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
                                + FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
                                + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
                                + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b])
                                + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
            (*N)[b].normalize();
        }

      /*************************************
        d---------------c---------------f
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      d = a + radial_segments;
      c = b + radial_segments;
      f = e + radial_segments;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[a], (*V)[b], (*V)[d])
              + FACE_NORMAL((*V)[d], (*V)[b], (*V)[c])
              + FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
              + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[e], (*V)[c]);
      (*N)[b].normalize();
 
    }

      /*************************************
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/

      col = 0;

      b = row * radial_segments;
      e = b + 1;
      a = b + radial_segments - 1;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
              + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b]);
      (*N)[b].normalize();
 
      for(col = 1; col < radial_segments - 1; ++col)
      {
            /*************************************

            a---------------b---------------e
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            |               |               |
            g---------------h---------------i

            ***************************************/
            b = row * radial_segments + col;
            a = b - 1;
            e = b + 1;
            g = a - radial_segments;
            h = b - radial_segments;
            i = e - radial_segments;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*N)[b] = FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
                    + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
              + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b]);
            (*N)[b].normalize();
        }

      /*************************************
        a---------------b---------------e
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        |               |               |
        g---------------h---------------i

      ***************************************/


      b = row * radial_segments + col;
      e = row * radial_segments;
      a = b - 1;
      g = a - radial_segments;
      h = b - radial_segments;
      i = e - radial_segments;
      (*N)[b] = FACE_NORMAL((*V)[h], (*V)[i], (*V)[b])
              + FACE_NORMAL((*V)[b], (*V)[i], (*V)[e])
        + FACE_NORMAL((*V)[a], (*V)[h], (*V)[b]);
      (*N)[b].normalize();


    return;

    /*

      ⊗--------⊙
      ⍿         ⍿
      ⍿         ⍿
      ⍿         ⍿
      ⍿         ⍿
      ⊙--------⊙
      ⊙
      ⚝



      index = axial_segments * i + j

    (i    , j - 1) - - - - - (i    , j    ) - - - - - (i    , j + 1)
          |                        |                        |
          |                        |                        |
          |                        |                        |
          |                        |                        |
          |                        |                        |
          |                        |                        |
    (i + 1, j - 1) - - - - - (i + 1, j     ) - - - - - (i + 1, j + 1)

    */

    // RECORD_ERROR("BEGIN");
    osg::Vec3Array * vertices = static_cast<osg::Vec3Array *>( G -> getVertexArray() );
    osg::Vec3Array * normals = static_cast<osg::Vec3Array *>( G -> getNormalArray() );
    for(size_t row = 0; row < axial_segments; ++row)
    {
        for(size_t col = 0; col < radial_segments / 2; ++col)
        {
            size_t current_index    = row * radial_segments + col;
            size_t opposite_index   = current_index + radial_segments / 2;
            (*normals)[current_index] = (*vertices)[current_index]
                                      - (*vertices)[opposite_index];
            (*normals)[current_index].normalize();
            (*normals)[opposite_index] = (*normals)[current_index] * -1.0f;
        }
    }
    // RECORD_ERROR("ENDED");
}


inline void
catmull_rom_point( const osg::Vec3f & P0
                 , const osg::Vec3f & P1
                 , const osg::Vec3f & P2
                 , const osg::Vec3f & P3
                 , const osg::Vec3f & P10
                 , const osg::Vec3f & P21
                 , const osg::Vec3f & P32
                 , const float knot_factor_10
                 , const float knot_factor_21
                 , const float knot_factor_32
                 , const float t
                 , osg::Vec3Array * vertices
                 , uint index
                 )
{
    osg::Vec3f A1 = P0
                  + P10 * ( (t                                  )
                          / knot_factor_10
                          );
    osg::Vec3f A2 = P1
                  + P21 * ( (t - knot_factor_10                 )
                          / knot_factor_21
                          );
    osg::Vec3f A3 = P2
                  + P32 * ( (t - knot_factor_21 - knot_factor_10)
                          / knot_factor_32
                          );
    osg::Vec3f B1 = A1
                  + (A2 - A1) * ( t
                                / (knot_factor_21 + knot_factor_10)
                                );
    osg::Vec3f B2 = A2
                  + (A3 - A2) * ( (t - knot_factor_10)
                                / (knot_factor_32 + knot_factor_21)
                                );

    float a = (t - knot_factor_10) / (knot_factor_21);
    osg::Vec3f B21 = B2 - B1;
    (*vertices)[index].set( B21.x() * a + B1.x()
                          , B21.y() * a + B1.y()
                          , B21.z() * a + B1.z()
                          );

    // std::cerr << (*vertices)[index].x() << std::endl;
    // std::cerr << (*vertices)[index].y() << std::endl;
    // std::cerr << (*vertices)[index].z() << std::endl;
}

inline float
catmull_rom_knot_factor(const osg::Vec3f & P, float alpha)
{
    return pow(P.length2(), alpha);
}

inline void
catmull_rom_spline( const osg::Vec3f &  P0
                  , const osg::Vec3f &  P1
                  , const osg::Vec3f &  P2
                  , const osg::Vec3f &  P3
                  , const osg::Vec3f &  P4
                  , const float         alpha
                  , const uint          axial_segments
                  , const uint          radial_segments
                  , const uint          radial_segment_index
                  , osg::Vec3Array   *  vertices
                  )
{

/*
A compartment is generated from 2 Catmull Rom Splines, L (left) and R (right).
L interpolates between P1 and P2. Hence it requires P0, P1, P2, P3.
R interpolates between P2 and P3. Hence it requires P1, P2, P3, P4.
Based upon the number of axial segments, we can divide L and R into 2 parts.
LL represents the left half of L and LR represents the right half of L.
RL represents the left half of R and RR represents the right half of R.
For the compartment in question, C2, we need to find out LR and RL.
We have 2 cases -


Odd number of segments (7)
--------------------------

|<----------C1--------->|<----------C2--------->|<---------C3---------->|
0   1   2   3   4   5   6                       |                       |
|                       0   1   2   3   4   5   6                       |
|                                               0   1   2   3   4   5   6
|           |<----------L---------->|<----------R---------->|           |
|           0   1   2   3   4   5   6                       |           |
|           |                       0   1   2   3   4   5   6           |
|           |           |<---LR---->|<----RL--->|           |           |
|           |           0   1   2   3   4   5   6           |           |
|           |           |  Segments to compute  |           |           |
=========================================================================

Even number of segments (6)
---------------------------

|<--------C1------->|<--------C2------->|<-------C3-------->|
0   1   2   3   4   5                   |                   |
|                   0   1   2   3   4   5                   |
|                                       0   1   2   3   4   5
|           |<--------L-------->|<--------R-------->|       |
|           0   1   2   3   4   5                   |       |
|           |                   0   1   2   3   4   5       |
|           |       |<----LR--->|<-RL-->|           |       |
|           |       0   1   2   3   4   5           |       |
|           |       |Segments to compute|           |       |
=============================================================


t_delta = (t2 - t1) / (axial_segments - 1)
LR -> (axial_segments - 1) / 2 to axial_segments - 2
RL -> 1 to (axial_segments - 1) / 2
*/


    const uint half_axial_segments = (axial_segments - 1) / 2;
    float tdelta;
    uint axial_segment_index = 0;
    uint i;

    const osg::Vec3f P10(P1 - P0);
    const osg::Vec3f P21(P2 - P1);
    const osg::Vec3f P32(P3 - P2);
    const osg::Vec3f P43(P4 - P3);

    const float knot_factor_10 = catmull_rom_knot_factor(P10, alpha);
    const float knot_factor_21 = catmull_rom_knot_factor(P21, alpha);
    const float knot_factor_32 = catmull_rom_knot_factor(P32, alpha);
    const float knot_factor_43 = catmull_rom_knot_factor(P43, alpha);

    // RECORD_ERROR(std::to_string(knot_factor_10));
    // RECORD_ERROR(std::to_string(knot_factor_21));
    // RECORD_ERROR(std::to_string(knot_factor_32));
    // RECORD_ERROR(std::to_string(knot_factor_43));

/*******************************************************************************
*                           Code to generate LR
*******************************************************************************/
    tdelta = knot_factor_21 / (axial_segments - 1);

    for( i = half_axial_segments
       ; i < axial_segments - 1
       ; ++i, ++axial_segment_index
       )
    {
        catmull_rom_point( P0
                         , P1
                         , P2
                         , P3
                         , P10
                         , P21
                         , P32
                         , knot_factor_10
                         , knot_factor_21
                         , knot_factor_32
                         , knot_factor_10 + i * tdelta
                         , vertices
                         , axial_segment_index * radial_segments
                         + radial_segment_index
                         );
    }

/*******************************************************************************
*                           Code to generate RL
*******************************************************************************/
    tdelta = knot_factor_32 / (axial_segments - 1);

/* The last value of axial segment index corresponds to the central ring of the
   compartment. Central ring is pre-generated so segment index is +1ed to skip
   it.
*/
    axial_segment_index += 1;
    for( i = 1
       ; i <= half_axial_segments
       ; ++i, ++axial_segment_index
       )
    {
        catmull_rom_point( P1
                         , P2
                         , P3
                         , P4
                         , P21
                         , P32
                         , P43
                         , knot_factor_21
                         , knot_factor_32
                         , knot_factor_43
                         , knot_factor_21 + i * tdelta
                         , vertices
                         , axial_segment_index * radial_segments
                         + radial_segment_index
                         );
    }
}

/*
Catmull-Rom interpolation for compartment C2
*/
void
catmull_rom_bounded_compartment( osg::Geometry  *   G0
                               , osg::Geometry  *   G1
                               , osg::Geometry  *   G2
                               , osg::Geometry  *   G3
                               , osg::Geometry  *   G4
                               , const float              alpha
                               , const uint               axial_segments
                               , const uint               radial_segments
                               )
{
    osg::Vec3Array * V0 =
            static_cast<osg::Vec3Array *>( G0 -> getVertexArray() );
    osg::Vec3Array * V1 =
            static_cast<osg::Vec3Array *>( G1 -> getVertexArray() );
    osg::Vec3Array * V2 =
            static_cast<osg::Vec3Array *>( G2 -> getVertexArray() );
    osg::Vec3Array * V3 =
            static_cast<osg::Vec3Array *>( G3 -> getVertexArray() );
    osg::Vec3Array * V4 =
            static_cast<osg::Vec3Array *>( G4 -> getVertexArray() );

    uint central_segment_index = (radial_segments * axial_segments) / 2;
    uint radial_segment_index;
    uint index;
    for( radial_segment_index = 0
       ; radial_segment_index < radial_segments
       ; ++radial_segment_index
       )
    {
        index = central_segment_index + radial_segment_index;
        // RECORD_ERROR("STARTED " + std::to_string(radial_segment_index));
        catmull_rom_spline( (*V0)[index]
                          , (*V1)[index]
                          , (*V2)[index]
                          , (*V3)[index]
                          , (*V4)[index]
                          , alpha
                          , axial_segments
                          , radial_segments
                          , radial_segment_index
                          , V2
                          );
        // radial_segment_index += radial_segments / 4;
        // RECORD_ERROR("ENDED");
    }

    compute_triangles(G2, axial_segments, radial_segments);
    compute_normals(G2, axial_segments, radial_segments);
}



void
compute_direction( const osg::Vec3f & P0
                 , const osg::Vec3f & P1
                 , osg::Vec3f & D
                 )
{
    D.set( P1.x() - P0.x()
         , P1.y() - P0.y()
         , P1.z() - P0.z()
         );
    D.normalize();
}

void
compute_center( const osg::Vec3f & P0
              , const osg::Vec3f & P1
              , osg::Vec3f & C
              )
{
    C.set( (P1.x() + P0.x()) / 2.0f
         , (P1.y() + P0.y()) / 2.0f
         , (P1.z() + P0.z()) / 2.0f
         );
}

void
twist( osg::Geometry * G0
     , osg::Geometry * G1
     , osg::Vec3f parent_proximal
     , osg::Vec3f parent_distal
     , osg::Vec3f child_proximal
     , osg::Vec3f child_distal
     , const uint axial_segments
     , const uint radial_segments
     )
{
    uint central_segment_index =(radial_segments * axial_segments) / 2;

    const osg::Vec3Array & V0 = *(static_cast<osg::Vec3Array *>(G0 -> getVertexArray()));
    osg::Vec3Array & V1 = *(static_cast<osg::Vec3Array *>(G1 -> getVertexArray()));

    osg::Vec3f C0   = (parent_distal + parent_proximal) / 2.0f;
    osg::Vec3f C1   = (child_distal + child_proximal) / 2.0f;
    osg::Vec3f D0   = parent_distal - parent_proximal;    D0.normalize();
    osg::Vec3f D1   = child_distal - child_proximal;      D1.normalize();
    osg::Vec3f C10  = C1 - C0;                            C10.normalize();
    float factor    = (V1[central_segment_index] - C1).length() / (V0[central_segment_index] - C0).length();
    float d1 = (V0[central_segment_index] - C0 - V1[central_segment_index] + C1).length();
    float d2 = (V0[central_segment_index] - C0 - V1[central_segment_index + radial_segments / 2.0] + C1).length();
    osg::Vec3f temp;
    float temp2 = C10 * D1;

    // osg::Vec3f VD00 = V0[central_segment_index] - C0; VD00.normalize();
    // osg::Vec3f VD10 = V1[central_segment_index] - C1; VD10.normalize();

    osg::Quat roton;
    roton.makeRotate(D0, D1);
    std::cerr << "Angle between direction vectors -> " << acos(D0 * D1) * 180.0 / osg::PI << std::endl;
    for(uint i = 0; i < radial_segments; ++i)
    {
        V1[central_segment_index + i] = roton
                                      * ((V0[central_segment_index + i] - C0) * factor)
                                      + C1;
        // std::cerr << "Final "   << V1[central_segment_index + i][0]
        //           << ", "       << V1[central_segment_index + ][1]
        //           << ", "       << V1[central_segment_index + 10][2];
    }
    return;
}

void
twist( osg::Geometry * G0
     , osg::Geometry * G1
     , osg::Vec3f parent_proximal
     , osg::Vec3f parent_distal
     , osg::Vec3f child_distal
     , const uint axial_segments
     , const uint radial_segments
     )
{
    uint central_segment_index =(radial_segments * axial_segments) / 2;

    const osg::Vec3Array & V0 = *(static_cast<osg::Vec3Array *>(G0 -> getVertexArray()));
    osg::Vec3Array & V1 = *(static_cast<osg::Vec3Array *>(G1 -> getVertexArray()));

    osg::Vec3f C0   = (parent_distal + parent_proximal) / 2.0f;
    osg::Vec3f C1   = (child_distal + parent_distal) / 2.0f;
    osg::Vec3f D0   = parent_distal - parent_proximal;    D0.normalize();
    osg::Vec3f D1   = child_distal - parent_distal;       D1.normalize();
    osg::Vec3f C10  = C1 - C0;                            C10.normalize();
    float factor    = (V1[central_segment_index] - C1).length() / (V0[central_segment_index] - C0).length();
    float d1 = (V0[central_segment_index] - C0 - V1[central_segment_index] + C1).length();
    float d2 = (V0[central_segment_index] - C0 - V1[central_segment_index + radial_segments / 2.0] + C1).length();
    osg::Vec3f temp;
    float temp2 = C10 * D1;

    // osg::Vec3f VD00 = V0[central_segment_index] - C0; VD00.normalize();
    // osg::Vec3f VD10 = V1[central_segment_index] - C1; VD10.normalize();

    osg::Quat roton;
    roton.makeRotate(D0, D1);
    std::cerr << "Angle between direction vectors -> " << acos(D0 * D1) * 180.0 / osg::PI << std::endl;
    float a = acos(D0 * D1);
    //if (a >= osg::PI / 2.0) { a = a - osg::PI / 2.0; }
    if(a < osg::PI / 2.0f)
    {
        for(uint i = 0; i < radial_segments; ++i)
        {
            V1[central_segment_index + i] = roton
                                          * ((V0[central_segment_index + i] - C0) * factor)
                                          + C1;
        // std::cerr << "Final "   << V1[central_segment_index + i][0]
        //           << ", "       << V1[central_segment_index + ][1]
        //           << ", "       << V1[central_segment_index + 10][2];
        }
    }
    else
    {
        osg::Quat roton2(osg::PI / 2.0f, D1);
        for(uint i = 0, j = radial_segments; i < radial_segments; ++i, j = j - 1)
        {
            V1[central_segment_index + (j % radial_segments)] = roton
                                                              * ((V0[central_segment_index + i] - C0) * factor)
                                                              + C1;
        }
    }
    return;
    // if(d1 > d2)
    // {
    //     uint i,j;
    //     RECORD_ERROR("Locha!!!");
    //     for(i = 0, j = radial_segments - 1; i < radial_segments / 2; ++i, --j)
    //     {
    //         temp = V1[central_segment_index + i];
    //         V1[central_segment_index + i] = V1[central_segment_index + j];
    //         V1[central_segment_index + j] = temp;
    //     }
    // }

    // return;
    // osg::Vec3f C0 = (parent_distal + parent_proximal) / 2.0f;
    // osg::Vec3f C1 = (child_distal + parent_distal) / 2.0f;
    std::cerr << "Angle between direction vectors => " << acos(D0 * D1) * 180.0 / osg::PI << std::endl;

    float l;
    if(temp2 == 0.0f)
    {
        std::cerr << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{";
        return;
    }
    else if(temp2 > 0.0f)
    {
        l = ((C1 - V0[central_segment_index]) * D1) / temp2;
    }
    else
    {
        std::cerr << "(((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))";
        l = ((C1 - V0[central_segment_index]) * D1) / (-temp2);
    }
    osg::Vec3f ND = V0[central_segment_index] + C10 * l - C1;
    ND.normalize();
    std::cerr << "Sanity Check " << acos(ND * D1) * 180.0 / 3.14 << std::endl;
    osg::Vec3f OD = V1[central_segment_index] - C1;
    OD.normalize();
    osg::Quat rotate;
    rotate.makeRotate(OD, ND);

    float angle = acos(OD * ND);
    if(acos(D0 * D1) > osg::PI / 2.0)
    {
        std::cerr << "************************************************";
        // rotate.makeRotate(ND, OD);
    }
    else
    {
        // rotate.makeRotate(OD, ND);
    }
    std::cerr << "angle => " << angle * 180.0 / 3.14 << std::endl;
    std::cerr << "Initial " << V1[central_segment_index + 10][0]
              << ", "       << V1[central_segment_index + 10][1]
              << ", "       << V1[central_segment_index + 10][2]
              << std::endl;
    for(uint i = 0; i < radial_segments; ++i)
    {
        V1[central_segment_index + i] = rotate
                                      * (V1[central_segment_index + i] - C1)
                                      + C1;
        // std::cerr << "Final "   << V1[central_segment_index + i][0]
        //           << ", "       << V1[central_segment_index + ][1]
        //           << ", "       << V1[central_segment_index + 10][2];
    }
    osg::Vec3f direction = (V1[central_segment_index + 10] - C1) ^ (V1[central_segment_index] - C1);
    direction.normalize();
    std::cerr << "direction" << acos(direction * D1) * 180.0 / 3.14 << std::endl;

    std::cerr << "Final "   << (V1[central_segment_index + 10] - C1)[0]
              << ", "       << (V1[central_segment_index + 10] - C1)[1]
              << ", "       << (V1[central_segment_index + 10] - C1)[2]
              << std::endl;
    std::cerr << "Radius "   << (V1[central_segment_index] - C1).length()
              << std::endl;

    // osg::Vec3f D0 = parent_distal - parent_proximal;
    // D0.normalize();
    std::cerr << "Dirdiff => " << acos(D0 * D1) * 180.0 / osg::PI << std::endl;

}
// void
// catmull_rom_bounded_compartment( const osg::Geometry  *   G0
//                                , const osg::Geometry  *   G1
//                                , const osg::Geometry  *   G2
//                                , const osg::Geometry  *   G3
//                                , const osg::Geometry  *   G4
//                                , const float              alpha
//                                , const uint               axial_segments
//                                , const uint               radial_segments
//                                )
// {
//     G2
// }


// std::tuple<osg::Geometry *, osg::Geometry *, osg::Geometry *>
// catmull_rom_bounded_compartment_chain( const osg::Geometry  * G0
//                                      , const osg::Geometry  * G1
//                                      , const osg::Vec3f     & C2
//                                      , const osg::Vec3f     & C3
//                                      , const osg::Vec3f     & C4
//                                      , const float            r2
//                                      , const float            r3
//                                      , const float            r4
//                                      )
// {

//     osg::Geometry * G2 = new osg::Geometry();
//     osg::Geometry * G3 = new osg::Geometry();
//     osg::Geometry * G4 = new osg::Geometry();

//     make_tuple<osg::Geometry *, osg::Geometry *, osg::Geometry *> ( G2
//                                                                   , G3
//                                                                   , G4
//                                                                   );
// }

// void
// catmull_rom_segment( osg::Vec3Array           * joints
//                    , std::vector<float>       & radii
//                    , const osg::Geometry      * GA
//                    , const osg::Geometry      * GB
//                    , const osg::Geometry      * GY
//                    , const osg::Geometry      * GZ
//                    , const osg::Vec3f         & CA
//                    , const osg::Vec3f         & CB
//                    , const osg::Vec3f         & CY
//                    , const osg::Vec3f         & CZ
//                    , const osg::Vec3f         & DA
//                    , const osg::Vec3f         & DB
//                    , const osg::Vec3f         & DY
//                    , const osg::Vec3f         & DZ
//                    , uint                       radial_segments
//                    , uint                       axial_segments
//                    )
// {
//     osg::Vec3f * C0 = &CA;
//     osg::Vec3f * C1 = &CB;
//     osg::Vec3f * C2 = new osg::Vec3f();

//     osg::Vec3f * C3 = new osg::Vec3f();
//     compute_center((*joints)[3], (*joints)[4], *C3);

//     osg::Vec3f * C4 = new osg::Vec3f();
//     compute_center((*joints)[4], (*joints)[5], *C4);

//     osg::Vec3f * D0 = new osg::Vec3f();
//     compute_direction((*joints)[0], (*joints)[1], *D0);

//     osg::Vec3f * D1 = new osg::Vec3f();
//     compute_direction((*joints)[1], (*joints)[2], *D1);

//     osg::Vec3f * D2 = new osg::Vec3f();
//     compute_direction((*joints)[2], (*joints)[3], *D2);

//     osg::Vec3f * D3 = new osg::Vec3f();
//     compute_direction((*joints)[3], (*joints)[4], *D3);

//     osg::Vec3f * D4 = new osg::Vec3f();
//     compute_direction((*joints)[4], (*joints)[5], *D4);

//     osg::Vec3f * temp;


//     osg::Geometry * G0 = GA;
//     osg::Geometry * G1 = GB;
//     osg::Geometry * G2 = nullptr;

//     for(i = 0; i < joints -> size - 1; ++i)
//     {
//         compute_center((*joints)[2], (*joints)[3], *C2);
//         temp_geometry = new osg::Geometry();
//         segment -> addDrawable(temp_geometry);
//         catmull_rom_compartment( G0
//                                , G1
//                                , G2
//                                , G3
//                                , G4
//                                , axial_segments
//                                , radial_segments
//                                );
//         temp    = C0;
//         C0      = C1;
//         C1      = C2;
//         C2      = C3;
//         C3      = C4;
//         C4      = temp;
//         compute_center()
//         C4 -> set(
//                  )
//     }

// }


// void
// catmull_rom_interpolate( osg::Vec3Array & points
//                        , osg::Vec3Array & radii
//                        , uint radial_segments
//                        , uint axial_segments
//                        , float alpha = 0.5
//                        )
// {
//     uint factor = (1 + axial_segments) * radial_segments;
//     uint i0 = index;
//     uint i1 = i0 + factor;
//     uint i2 = i1 + factor;
//     uint i3 = i2 + factor;

//     for(uint spline_index = 0; spline_index < radial_segments; ++spline_index, ++i0, ++i1, ++i2, ++i3)
//     {
//         float t0 = 0.0f;
//         float t1 = pow( ((*points)[i1] - (*points)[i0]).length2()
//                       , alpha / 2.0
//                       )
//                  + t0;
//         float t2 = pow( ((*points)[i2] - (*points)[i1]).length2()
//                       , alpha / 2.0
//                       )
//                  + t1;
//         float t3 = pow( ((*points)[i3] - (*points)[i2]).length2()
//                       , alpha / 2.0
//                       )
//                  + t2;
//         for( uint parameter_index = 1
//            ; parameter_index < axial_segments
//            ; ++parameter_index
//            )
//         {
//             float t = static_cast<float>(parameter_index)
//                     / static_cast<float>(axial_segments + 1);
//             osg::Vec3f a1
//             osg::Vec3f a2
//             osg::Vec3f a3
//             osg::Vec3f b1
//             osg::Vec3f b2
//             osg::Vec3f c

//         }

//     }

// }

// void
// catmull_rom_chain_interpolate( osg::Vec3Array & points
//                        , osg::Vec3Array & radii
//                        , uint radial_segments
//                        , uint axial_segments
//                        , float alpha = 0.5
//                        )
// {


//     for( uint i = index
//        ; i < points -> size()
//        ; i += (1 + axial_segments) * radial_segments
//        )
//     {
//         for(uint )
//     }
//     def tj(ti, Pi, Pj):
//     xi, yi = Pi
//     xj, yj = Pj
//     return ( ( (xj-xi)**2 + (yj-yi)**2 )**0.5 )**alpha + ti
// }


void
transform( osg::Vec3Array *    ring
              , float               scale
              , osg::Vec3f          direction
              , osg::Vec3f          center
              , osg::Vec3Array *    vertices
              , size_t              index
              )
{
    osg::Quat rotate;
    rotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);
    for(size_t i = 0; i < ring -> size(); ++i)
    {
        (*vertices)[index + i] = rotate * ((*ring)[i] * scale) + center;
    }
}


// void
// generate_rings( const osg::Vec3Array        * centers
//               , const std::vector<float>    & radii
//               , const osg::Vec3Array        * direction
//               , uint                          radial_segments
//               , osg::Vec3Array              * points
//               )
// {
//     uint ring_count = centers -> size();
//     osg::Vec3Array ring = unit_ring(points);
//     for(i = 0; i < ring_count; ++i)
//     {
//         ring( radii[i]
//             , (*direction)[i]
//             , (*centers)[i]
//             , points
//             , i * radial_segments
//             );

//     }
// }



void
compute_centers( const osg::Vec3Array * joints
               , osg::Vec3Array       * centers
               , uint index
               )
{
    for(uint i = 0; i < joints -> size() - 1; ++i)
    {
        (*centers)[index + i].set( ((*joints)[i].x() + (*joints)[i + 1].x()) / 2.0f
                                 , ((*joints)[i].y() + (*joints)[i + 1].y()) / 2.0f
                                 , ((*joints)[i].z() + (*joints)[i + 1].z()) / 2.0f
                                 );
    }
}

void
compute_directions( const osg::Vec3Array * joints
                  , osg::Vec3Array       * directions
                  , uint index
                  )
{
    for(uint i = 0; i < joints -> size() - 1; ++i)
    {
        (*directions)[index + i].set( (*joints)[i + 1].x() - (*joints)[i].x()
                                    , (*joints)[i + 1].y() - (*joints)[i].y()
                                    , (*joints)[i + 1].z() - (*joints)[i].z()
                                    );
        (*directions)[index + i].normalize();
    }
}


// void
// compute_normals( const osg::Vec3Array * joints
//                , osg::Vec3Array       * directions
//                , uint index
//                )
// {

// }


// void
// compute_triangles(
//                  )
