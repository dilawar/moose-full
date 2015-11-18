#include "utility/libraries.hpp"

namespace catmull_rom
{

enum node_t { UNDEFINED
            , LEAF
            , ROOT
            , INTERNAL
            };

inline void
point( const osg::Vec3f & P0
     , const osg::Vec3f & P1
     , const osg::Vec3f & P2
     , const osg::Vec3f & P3
     , const osg::Vec3f & P10
     , const osg::Vec3f & P21
     , const osg::Vec3f & P32
     , const float & knot_factor_10
     , const float & knot_factor_21
     , const float & knot_factor_32
     , const float & t
     , osg::Vec3f & C
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
    C.set( B21.x() * a + B1.x()
         , B21.y() * a + B1.y()
         , B21.z() * a + B1.z()
         );
}

inline void
spline( const osg::Vec3f &  P0
      , const osg::Vec3f &  P1
      , const osg::Vec3f &  P2
      , const osg::Vec3f &  P3
      , const osg::Vec3f &  P4
      , const uint          axial_divisions
      , const uint          radial_divisions
      , const uint          radial_division_index
      , osg::Vec3Array   *  vertices
      )
{

/*
A compartment is generated from 2 Catmull Rom Splines, L (left) and R (right).
L interpolates between P1 and P2. Hence it requires P0, P1, P2, P3.
R interpolates between P2 and P3. Hence it requires P1, P2, P3, P4.
Based upon the number of axial planes, we can divide L and R into 2 parts.
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


tdelta = (t2 - t1) / (axial_segments - 1)
LR -> (axial_segments - 1) / 2 to axial_segments - 2
RL -> 1 to (axial_segments - 1) / 2

*/

    const uint axial_planes = axial_segments + 1;
    const uint half_axial_segments = axial_segments / 2;
    float tdelta;
    uint axial_plane_index = 0;
    uint i;

    const osg::Vec3f P10(P1 - P0);
    const osg::Vec3f P21(P2 - P1);
    const osg::Vec3f P32(P3 - P2);
    const osg::Vec3f P43(P4 - P3);

    const float knot_factor_10 = catmull_rom_knot_factor(P10, alpha);
    const float knot_factor_21 = catmull_rom_knot_factor(P21, alpha);
    const float knot_factor_32 = catmull_rom_knot_factor(P32, alpha);
    const float knot_factor_43 = catmull_rom_knot_factor(P43, alpha);

/*******************************************************************************
*                           Code to generate LR
*******************************************************************************/
    tdelta = knot_factor_21 / (axial_segments - 1);

    for( i = half_axial_segments
       ; i < axial_segments - 1
       ; ++i, ++axial_segment_index
       )
    {
        point( P0
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
             , (*vertices)[ axial_segment_index * radial_segments
                          + radial_segment_index
                          ]
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
                         , (*vertices)[ axial_segment_index * radial_segments
                                      + radial_segment_index
                                      ]
                         );
    }

}

void
generate_left_spline(
           )
{
    spline( 
          )
}

void
generate_right_spline(
            );


void
generate_normals();


void
generate_surface()

inline node_t
node_type(ElectricalCompartment * electrical_compartment, compartment_type, parent_type, grandparent_type)
{
  if(electrical_compartment == nullptr)              { return UNDEFINED; }
  if(electrical_compartment -> parent == nullptr)    { return ROOT;      }
  if(electrical_compartment -> children.size() == 0) { return LEAF;      }
                                                       return INTERNAL_NODE;
}


void
polygon()
{}

void
triangulate( Neuron * neuron
           , ElectricalCompartment * electrical_compartment
           , node_t grandparent_type
           , node_t parent_type
           , node_t compartment_type
           )
{
    ElectricalCompartment * new_compartment = new ElectricalCompartment(electrical_compartment -> id);
    new_compartment -> proximal = compartment -> proximal;
    new_compartment -> distal   = compartment -> distal;
    neuron -> attach(new_compartment);

    osg::Vec3f proximal( compartment -> proximal.x()
                       , compartment -> proximal.y()
                       , compartment -> proximal.z()
                       );
    osg::Vec3f distal( compartment -> distal.x()
                     , compartment -> distal.y()
                     , compartment -> distal.z()
                     );
    osg::Vec3f direction = distal - proximal;
    float height = direction.normalize();
    osg::Vec3f center = (distal + proximal) / 2.0f;

    switch(compartment_type)
    {
        case ROOT :   new_compartment -> add_representation(true); 
                      break;
        case INTERNAL_NODE : switch(parent_type)
                             {
                                 case ROOT_NODE:
                                   /* There are 2 cases
                                      1) child compartment radius > parent radius (unlikely but possible)
                                      In this case, the child compartment will not intersect parent compartment
                                      2) child compartment radius < parent radius (most of the cases)
                                      In this case, the child compartment will intersect parent compartment.
                                      the second radial ring will lie between the ring of intersection of the cylinder with the sphere and the radial ring of the sphere.
                                   */
                                   osg::Quat rotation;
                                   rotation.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);
                                   osg::Vec3Array * radial_ring = fill_polygon( radial_divisions
                                                                              , 0.0f
                                                                              , parent -> distal.w()
                                                                              , rotation
                                                                              , osg::Vec3f( parent -> distal.x()
                                                                                          , parent -> distal.y()
                                                                                          , parent -> distal.z()
                                                                                          )
                                                                              );
                                   osg::Vec3Array * next_radial_ring = fill_polygon( radial_divisions
                                                                                   , 0.0f
                                                                                   , 
                                                                                   , rotation
                                                                                   , parent_center + compartment_center

                                               )
                                                    /* interpolate between
                                                       1) Radial ring of soma
                                                       2) immediate next ring of soma
                                                       3) Central ring of this compartment
                                                       4) Last ring of this compartment if number of children is more than one or the central ring of next compartment;
                                                    */
                                                    break;
                                 case INTERNAL_NODE: /* interpolate between
                                                       1) Central ring of parent's parent
                                                       2) Central ring of parent
                                                       3) Central ring of this compartment
                                                       4) Last ring of this compartment
                                                    */
                                                    break;
                            }
 
    }
    node_t compartment_t = node_type(electrical_compartment);
    node_t parent_t = ;
    node_t grandparent_t = ;



}

Neuron * neuron
triangulate(Neuron * neuron)
{
  ElectricalCompartment * root = neuron -> get_root();
  triangulate(new_neuron, root, UNDEFINED, UNDEFINED, ROOT);
  return new_neuron;
}

Network * network
triangulate(Network * network)
{
    
}

