#ifndef __CHEMICAL_COMPARTMENT_HPP__
#define __CHEMICAL_COMPARTMENT_HPP__
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Visualizable.hpp"

class ElectricalCompartment; // forward declaration to avoid cyclic dependency issues

class ChemicalCompartment : public Visualizable
{
public:
    ElectricalCompartment * electrical_compartment;

    ChemicalCompartment(const char * id);

    ~ChemicalCompartment();

};

#endif /* __CHEMICAL_COMPARTMENT_HPP__ */
