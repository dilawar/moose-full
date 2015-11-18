#include "core/ElectricalCompartment.hpp"

ElectricalCompartment::ElectricalCompartment(const char * id) : Visualizable(id)
                                                              , chemical_compartment_group_node(new osg::Group())
                                                              , neuron(nullptr)
                                                              , parent(nullptr)
{
    // by default, neuron shows all its compartments, hence true
    node -> addChild(chemical_compartment_group_node.get(), true);
}

ElectricalCompartment::~ElectricalCompartment()
{
    for (auto & chemical_compartment_pair : chemical_compartment_map)
    {
        chemical_compartment_pair.second -> electrical_compartment = nullptr;
    }
}


void
ElectricalCompartment::set_parent(ElectricalCompartment * electrical_compartment)
{
    if(parent != nullptr)
    {
        auto iter = parent -> children.find(electrical_compartment -> id);
        parent -> children.erase(iter);
    }
    parent = electrical_compartment;
    if(parent != nullptr)
    {
        parent -> children[id] = this;
    }
}


ElectricalCompartment *
ElectricalCompartment::get_parent() const
{
    return parent;
}


size_t
ElectricalCompartment::size() const
{
    return chemical_compartment_map.size();
}


bool
ElectricalCompartment::attach(ChemicalCompartment * chemical_compartment)
{
    auto iter = chemical_compartment_map.find(chemical_compartment -> id);
    if(iter != chemical_compartment_map.end()) { return false; }
    chemical_compartment_map[chemical_compartment -> id] = chemical_compartment;
    chemical_compartment_group_node -> addChild(chemical_compartment -> node.get());
    chemical_compartment -> electrical_compartment = this;
    return true;
}

bool
ElectricalCompartment::detach(ChemicalCompartment * chemical_compartment)
{
    auto iter = chemical_compartment_map.find(chemical_compartment -> id);
    if(iter == chemical_compartment_map.end()) { return false; }
    chemical_compartment_map.erase(iter);
    chemical_compartment_group_node -> removeChild(chemical_compartment -> node.get());
    chemical_compartment -> electrical_compartment = nullptr;
    return true;
}

ChemicalCompartment *
ElectricalCompartment::get(const std::string & id) const
{
    auto iter = chemical_compartment_map.find(id);
    if(iter == chemical_compartment_map.end()) { return nullptr; }
    return iter -> second;
}


void
ElectricalCompartment::subdivide(PyObject * ids)
{
    if(proximal == distal) { subdivide_sphere(ids);   }
    else                   { subdivide_cylinder(ids); }
}

void
ElectricalCompartment::subdivide_cylinder(PyObject * ids)
{
    Vec4f electrical_proximal(proximal[0], proximal[1], proximal[2], proximal[3]);
    Vec4f electrical_distal(distal[0], distal[1], distal[2], distal[3]);

    Vec4f direction = electrical_distal - electrical_proximal;
    Vec4f chemical_proximal = electrical_proximal;
    Vec4f chemical_distal;
    uint size = static_cast<uint>(PySequence_Length(ids));
    for(uint i = 0; i < size; ++i)
    {
        chemical_proximal = electrical_proximal + direction * (i + 0.0f) / size;
        chemical_distal = electrical_proximal + direction * (i + 1.0) / size;
        const char * id = PyString_AsString(PySequence_GetItem(ids, i));
        ChemicalCompartment * chemical_compartment = new ChemicalCompartment(id);
        chemical_compartment -> proximal.set( chemical_proximal[0]
                                            , chemical_proximal[1]
                                            , chemical_proximal[2]
                                            , chemical_proximal[3]
                                            );
        chemical_compartment -> distal.set( chemical_distal[0]
                                          , chemical_distal[1]
                                          , chemical_distal[2]
                                          , chemical_distal[3]
                                          );
        chemical_compartment -> add_representation(false);
        attach(chemical_compartment);
    }
}

void
ElectricalCompartment::subdivide_sphere(PyObject * ids)
{

}

// interpolate between P1 and P2, only the right half, as the right half corresponds to compartment's left
// inline void
// catmull_rom_compartment_left( const osg::Vec3f &  P0
//                             , const osg::Vec3f &  P1 // point on parent's central ring
//                             , const osg::Vec3f &  P2 // point on current compartment's central ring
//                             , const osg::Vec3f &  P3
//                             , const uint          axial_segments
//                             , const uint          radial_segments
//                             , const uint          radial_segment_index
//                             , osg::Vec3Array   *  vertices
//                             )
// {

// /*
// A compartment is generated from 2 Catmull Rom Splines, L (left) and R (right).
// L interpolates between P1 and P2. Hence it requires P0, P1, P2, P3.
// R interpolates between P2 and P3. Hence it requires P1, P2, P3, P4.
// Based upon the number of axial segments, we can divide L and R into 2 parts.
// LL represents the left half of L and LR represents the right half of L.
// RL represents the left half of R and RR represents the right half of R.
// For the compartment in question, C2, we need to find out LR and RL.
// We have 2 cases -


// Even number of segments (6)
// ---------------------------

// |<----------C1--------->|<----------C2--------->|<---------C3---------->|
// 0   1   2   3   4   5   6                       |                       |
// |                       0   1   2   3   4   5   6                       |
// |                                               0   1   2   3   4   5   6
// |           |<----------L---------->|<----------R---------->|           |
// |           0   1   2   3   4   5   6                       |           |
// |           |                       0   1   2   3   4   5   6           |
// |           |           |<---LR---->|<----RL--->|           |           |
// |           |           0   1   2   3   4   5   6           |           |
// |           |           |  Segments to compute  |           |           |
// =========================================================================

// Odd number of segments (5)
// --------------------------

// |<--------C1------->|<--------C2------->|<-------C3-------->|
// 0   1   2   3   4   5                   |                   |
// |                   0   1   2   3   4   5                   |
// |                                       0   1   2   3   4   5
// |           |<--------L-------->|<--------R-------->|       |
// |           0   1   2   3   4   5                   |       |
// |           |                   0   1   2   3   4   5       |
// |           |       |<----LR--->|<-RL-->|           |       |
// |           |       0   1   2   3   4   5           |       |
// |           |       |Segments to compute|           |       |
// =============================================================


// tdelta = (t2 - t1) / axial_segments
// LR -> axial_segments / 2 to axial_segments - 1
// RL -> 1 to axial_segments / 2

// */

//     const uint half_axial_segments    = axial_segments / 2;
//     const uint axial_ring_index_begin = 0;
//     const uint axial_ring_index_end   = axial_segments / 2;

//     float tdelta;
//     uint axial_segment_index = 0;
//     uint i;

//     const osg::Vec3f P10(P1 - P0);
//     const osg::Vec3f P21(P2 - P1);
//     const osg::Vec3f P32(P3 - P2);

//     const float knot_factor_10 = sqrt(P10.length()); // alpha = 0.5
//     const float knot_factor_21 = sqrt(P21.length()); // alpha = 0.5
//     const float knot_factor_32 = sqrt(P32.length()); // alpha = 0.5

// /*******************************************************************************
// *                           Code to generate LR
// *******************************************************************************/
//     tdelta = knot_factor_21 / axial_segments;

//     for( i = half_axial_segments
//        ; i < axial_segments - 1
//        ; ++i, ++axial_segment_index
//        )
//     {
//         catmull_rom_point( P0
//                          , P1
//                          , P2
//                          , P3
//                          , P10
//                          , P21
//                          , P32
//                          , knot_factor_10
//                          , knot_factor_21
//                          , knot_factor_32
//                          , knot_factor_10 + i * tdelta
//                          , (*vertices)[ axial_segment_index * radial_segments
//                                       + radial_segment_index
//                                       ]
//                          );
//     }

// /*******************************************************************************
// *                           Code to generate RL
// *******************************************************************************/
//     tdelta = knot_factor_32 / (axial_segments - 1);

// /* The last value of axial segment index corresponds to the central ring of the
//    compartment. Central ring is pre-generated so segment index is +1ed to skip
//    it.
// */
//     axial_segment_index += 1;
//     for( i = 1
//        ; i <= half_axial_segments
//        ; ++i, ++axial_segment_index
//        )
//     {
//         catmull_rom_point( P1
//                          , P2
//                          , P3
//                          , P4
//                          , P21
//                          , P32
//                          , P43
//                          , knot_factor_21
//                          , knot_factor_32
//                          , knot_factor_43
//                          , knot_factor_21 + i * tdelta
//                          , (*vertices)[ axial_segment_index * radial_segments
//                                       + radial_segment_index
//                                       ]
//                          );
//     }

// }



// void
// pre_catmull_rom_interpolate()
// {
//     switch(_node_type)
//     {
//         case ROOT: sphere();
//         case LEAF_CONNECTED_TO_ROOT:
//         case LEAF_CONNECTED_TO_INTERNAL_NODE
//         case INTERNAL_NODE_CONNECTED_TO_ROOT
//         case INTERNAL_NODE_C

//     }

// }

// void
// ElectricalCompartment::create_left_half( const osg::Vec3f & P0
//                                        , osg::Vec3Array * V1
//                                        , osg::Vec3Array * V2
//                                        , osg::Vec3Array * V3
//                                        )
// {
        
// }

// void
// ElectricalCompartment::create_left_half( osg::Vec3Array * V0
//                                        , osg::Vec3Array * V1
//                                        , osg::Vec3Array * V2
//                                        , osg::Vec3Array * V3
//                                        )
// {
        
// }


// void
// ElectricalCompartment::catmull_rom_left_half()
// {
//     switch(type)
//     {

//         case ROOT_NODE:     //draw as sphere
//                             break;
//         case LEAF_NODE:     switch(parent -> type)
//                             {
//                                case ROOT_NODE:     /* interpolate between
//                                                       1) Axial Point on soma
//                                                       2) Radial ring of soma
//                                                       3) Central ring of this compartment
//                                                       4) Last ring of this compartment
//                                                    */
//                                  catmull_rom_interpolate_left_half( axial_point, radial_ring, 0, V, center, last_ring, 0, radial_segments, axial_segments);
//                                                     break;
//                                case INTERNAL_NODE: /*
//                                                       interpolate between
//                                                       1) Central ring of parent's parent
//                                                       2) Central ring of parent
//                                                       3) Central ring of this compartment
//                                                       4) Last ring of this compartment
//                                                    */
//                                  switch(parent -> parent -> type)
//                                  {
//                                      case ROOT_NODE:
//                                        break;
//                                      case INTERNAL_NODE:
//                                        break;
//                                  }
//                                  catmull_rom_interpolate_left_half(V_parent, center, )
//                                                    break;
//                             }
//                             break;
//         case INTERNAL_NODE: switch(parent -> type)
//                             {
//                                 case ROOT_NODE:     /* interpolate between
//                                                        1) Axial point on soma
//                                                        2) Radial ring of soma
//                                                        3) Central ring of this compartment
//                                                        4) Last ring of this compartment
//                                                     */
//                                                     break;
//                                 case INTERNAL_NODE: /* interpolate between
//                                                        1) Central ring of parent's parent
//                                                        2) Central ring of parent
//                                                        3) Central ring of this compartment
//                                                        4) Last ring of this compartment
//                                                     */
//                                                     break;
//                             }
//                             break;
//     }
// }

// void
// ElectricalCompartment::catmull_rom_right_half()
// {
//     switch(type)
//     {

//         case ROOT_NODE:     //draw remaining half
//                             break;
//         case LEAF_NODE:     break;
//         case INTERNAL_NODE: switch(parent -> type)
//                             {
//                                 case ROOT_NODE:     /* interpolate between
//                                                        1) Axial point on soma
//                                                        2) Radial ring of soma
//                                                        3) Central ring of this compartment
//                                                        4) Last ring of this compartment
//                                                     */
//                                                     break;
//                                 case INTERNAL_NODE: /* interpolate between
//                                                        1) Central ring of parent's parent
//                                                        2) Central ring of parent
//                                                        3) Central ring of this compartment
//                                                        4) Last ring of this compartment
//                                                     */
//                                                     break;
//                             }
//                             break;
//     }
// }

// void
// ElectricalCompartment::catmull_rom_smoothen( uint axial_segments
//                                            , uint radial_segments
//                                            )
// {

//     osg::Geometry * geometry = new Geometry();
//   /* There are five cases -
//      1) compartment has no parent - show as sphere
//      2) compartment has no child  - left half should be interpolated and right half should be a normal cylinder
//      3) compartment's child is a leaf but parent is not root - interpolate the left half normally, interpolate right half by taking the edge of child as the last interpolation ring
//      4) compartment's child is not a leaf but parent is root - interpolate the right half normally, interpolate the left half by taking a single point on the sphere along the compartment axis as the leftmost interpolation point
//      5) compartment's child is not a leaf and parent is not root - interpolate everything normally.

//   char type - 7 6 5 4  3      2                               1 0
//                        leaf / internal node   root / internal node    root / leaf / internal node
//   */

//   pre_catmull_rom_interpolate();
//   in_catmull_rom_interpolate();
//   post_catmull_rom_interpolate();
//   std::vector<ElectricalCompartment *> compartments(electrical_compartment_map.size());
//   size_t read_index = 0;
//   size_t write_index = 0;
//   compartments[write_index] = root();
//   ++write_index;
//   while(read_index < write_index)
//     {
//       ElectricalCompartment * parent = compartments[read_index];
//       ++read_index;
//       for(auto & electrical_compartment_pair : parent -> children)
//         {
//           compartments[write_index] = electrical_compartment_pair.second;
//           ++write_index;
//         }
//       parent -> catmull_rom_smoothen(axial_segments, radial_segments);
//     }
// }


