#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Neuron.hpp"

class Network
{
public:
    string id;
    osg::ref_ptr<osg::MatrixTransform> node;
    std::unordered_map<string, Neuron *> neuron_map;

    Network(const char * id);

    ~Network();

    void
    hide();

    void
    show();

    bool
    visible() const;

    size_t
    size() const;

    bool
    attach(Neuron * neuron);

    bool
    detach(Neuron * neuron);

    Neuron *
    get(const std::string & id) const;

    bool
    write(const std::string & filename) const;

    // void
    // catmull_rom_smoothen( uint axial_segments
    //                     , uint radial_segments
    //                     )
};

#endif /* _NETWORK_HPP_ */
