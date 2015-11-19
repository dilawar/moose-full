#include "core/Network.hpp"

Network::Network(const char * id) : id(id)
                                  , node(new osg::MatrixTransform())
{
}


Network::~Network()
{
    for (auto & neuron_pair : neuron_map)
    {
        neuron_pair.second -> network = nullptr;
    }
}

void
Network::hide()
{
    node -> setNodeMask(NODE_HIDE_MASK);
}

void
Network::show()
{
    node -> setNodeMask(NODE_SHOW_MASK);
}

bool
Network::visible() const
{
    return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

size_t
Network::size() const
{
    return static_cast<unsigned int>(neuron_map.size());
}

bool
Network::attach(Neuron * neuron)
{
    auto iter = neuron_map.find(neuron -> id);
    if(iter != neuron_map.end()) { return false; }
    neuron_map[neuron -> id] = neuron;
    neuron -> network = this;
    node -> addChild(neuron -> node.get());
    return true;
}

bool
Network::detach(Neuron * neuron)
{
    auto iter = neuron_map.find(neuron -> id);
    if(iter == neuron_map.end()) { return false; }
    neuron_map.erase(iter);
    node -> removeChild(neuron -> node.get());
    neuron -> network = nullptr;
    return true;
}

Neuron *
Network::get(const std::string & id) const
{
    auto iter = neuron_map.find(id);
    if(iter == neuron_map.end()) { return nullptr; }
    return iter -> second;
}

bool
Network::write(const std::string & filename) const
{
    return osgDB::writeNodeFile( *node
                               , filename
                               );
}

// void
// Network::catmull_rom_smoothen( uint axial_segments
//                              , uint radial_segments
//                              )
// {
//     for(auto & neuron_pair : neuron_map)
//     {
//         neuron_pair.second -> catmull_rom_smoothen( axial_segments
//                                                   , radial_segments
//                                                   );
//     }
// }
