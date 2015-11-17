#include "core/ChemicalCompartment.hpp"

ChemicalCompartment::ChemicalCompartment( const char * id
                                        ) : Visualizable(id)
                                          , electrical_compartment(nullptr)
{ }


ChemicalCompartment::~ChemicalCompartment()
{ }
