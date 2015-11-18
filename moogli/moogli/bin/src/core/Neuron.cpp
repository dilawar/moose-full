#include "core/Neuron.hpp"

Neuron::Neuron(const char * id) : Visualizable(id)
                                , electrical_compartment_group_node(new osg::Group())
                                , network(nullptr)
{
    // by default, neuron shows all its compartments, hence true
    node -> addChild(electrical_compartment_group_node.get(), true);
}

Neuron::~Neuron()
{
    for (auto & electrical_compartment_pair : electrical_compartment_map)
    {
        electrical_compartment_pair.second -> neuron = nullptr;
    }
}

ElectricalCompartment *
Neuron::get(const std::string & id) const
{
    auto iter = electrical_compartment_map.find(id);
    if(iter == electrical_compartment_map.end()) { return nullptr; }
    return iter -> second;
}

size_t
Neuron::size() const
{
    return electrical_compartment_map.size();
}

bool
Neuron::attach(ElectricalCompartment * electrical_compartment)
{
    auto iter = electrical_compartment_map.find(electrical_compartment -> id);
    if(iter != electrical_compartment_map.end()) { return false; }
    electrical_compartment_map[electrical_compartment -> id] = electrical_compartment;
    electrical_compartment_group_node -> addChild(electrical_compartment -> node.get());
    electrical_compartment -> neuron = this;
    return true;
}

bool
Neuron::detach(ElectricalCompartment * electrical_compartment)
{
    auto iter = electrical_compartment_map.find(electrical_compartment -> id);
    if(iter == electrical_compartment_map.end()) { return false; }
    electrical_compartment_map.erase(iter);
    electrical_compartment_group_node -> removeChild(electrical_compartment -> node.get());
    electrical_compartment -> neuron = nullptr;
    return true;
}


// ElectricalCompartment *
// Neuron::root()
// {
//     for(auto & electrical_compartment_pair : electrical_compartment_map)
//     {
//         if(electrical_compartment_pair.second -> parent = nullptr)
//         {
//             return electrical_compartment.second;
//         }
//     }
//     RECORD_ERROR("Root compartment not found for neuron '" + id + "'");
// }

/*
Identify root compartment. This is the one with no parent. Draw this as a nice sphere.

*/
// void
// Neuron::catmull_rom_smoothen( uint axial_segments
//                             , uint radial_segments
//                             )
// {
//     // ElectricalCompartment * root_compartment = root();
//     std::vector<ElectricalCompartment *> compartments(electrical_compartment_map.size());
//     size_t read_index = 0;
//     size_t write_index = 0;
//     compartments[write_index] = root();
//     ++write_index;
//     while(read_index < write_index)
//     {
//         ElectricalCompartment * parent = compartments[read_index];
//         ++read_index;
//         for(auto & electrical_compartment_pair : parent -> children)
//         {
//           compartments[write_index] = electrical_compartment_pair.second;
//           ++write_index;
//         }
//         parent -> catmull_rom_smoothen(axial_segments, radial_segments);
//     }
// }

// void
// catmull_rom_smoothen_spine()
// {

// }

// void
// catmull_rom_smoothen_soma()
// {

// }

// void
// catmull_rom_smoothen_terminal()
// {

// }

// void
// catmull_rom_smoothen_ends()
// {

// }


// void
// catmull_rom_smoothen_segment( ElectricalCompartment * EC2
//                             , uint axial_segments
//                             , uint radial_segments
//                             )
// {
//     ElectricalCompartment * EC1 = EC2 -> parent;
//     ElectricalCompartment * EC0 = EC1 -> parent;
//     for(auto & child_pair : electrical_compartment_map)
//     {
//         EC1 = child_pair.second;
//         for(auto & )
//         electrical_compartment_pair.second -> neuron = nullptr;
//     }


//     ElectricalCompartment * EC1 = EC0

//     G4 = create_geometry_ring( osg::Vec3f( (*points)[i - 1][0]
//                                          , (*points)[i - 1][1]
//                                          , (*points)[i - 1][2]
//                                          )
//                              , osg::Vec3f( (*points)[i][0]
//                                          , (*points)[i][1]
//                                          , (*points)[i][2]
//                                          )
//                              , (*points)[i][3]
//                              , axial_segments
//                              , radial_segments
//                              );

//     catmull_rom_bounded_compartment( G0
//                                    , G1
//                                    , G2
//                                    , G3
//                                    , G4
//                                    , 0.25f
//                                    , axial_segments
//                                    , radial_segments
//                                    );
//     geode -> addDrawable(G2);
//     G0 = G1;
//     G1 = G2;
//     G2 = G3;
//     G3 = G4;
//     }
//     group -> addChild(geode);
//     return group;
// }
