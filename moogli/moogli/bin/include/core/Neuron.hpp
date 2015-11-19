#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Visualizable.hpp"
#include "core/ElectricalCompartment.hpp"

class Network;

class Neuron  : public Visualizable
{
public:
    Network * network;
    osg::ref_ptr<osg::Group> electrical_compartment_group_node;
    std::unordered_map<string, ElectricalCompartment *> electrical_compartment_map;

    Neuron(const char * id);

    ~Neuron();

    ElectricalCompartment *
    get(const std::string & id) const ;

    bool
    attach(ElectricalCompartment * electrical_compartment);

    bool
    detach(ElectricalCompartment * electrical_compartment);

    size_t
    size() const;

    // void
    // catmull_rom_smoothen( uint axial_segments
    //                     , uint radial_segments
    //                     );

};

#endif /* _NEURON_HPP_ */
