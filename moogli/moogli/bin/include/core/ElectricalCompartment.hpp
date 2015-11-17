#ifndef __ELECTRICAL_COMPARTMENT_HPP__
#define __ELECTRICAL_COMPARTMENT_HPP__

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Visualizable.hpp"
#include "core/ChemicalCompartment.hpp"

class Neuron; // forward declaration to avoid cyclic dependency issues

class ElectricalCompartment : public Visualizable
{
public:
    // string id;
    Neuron * neuron;
    osg::ref_ptr<osg::Group> chemical_compartment_group_node;
    std::unordered_map<string, ChemicalCompartment *> chemical_compartment_map;
    std::unordered_map<string, ElectricalCompartment *> children;
    ElectricalCompartment * parent;

    ElectricalCompartment(const char * id);

    ~ElectricalCompartment();

    void
    set_parent(ElectricalCompartment * electrical_compartment);

    ElectricalCompartment *
    get_parent() const;

    size_t
    size() const;

    ChemicalCompartment *
    get(const std::string & id) const;

    bool
    attach(ChemicalCompartment * chemical_compartment);

    bool
    detach(ChemicalCompartment * chemical_compartment);

    void
    subdivide(PyObject * ids);

    void
    subdivide_cylinder(PyObject * ids);

    void
    subdivide_sphere(PyObject * ids);

};

#endif /* __ELECTRICAL_COMPARTMENT_HPP__ */
