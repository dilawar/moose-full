class ElectricalCompartment : public Compartment
{
public:
    Neuron * neuron;
    ElectricalCompartment * parent;
    vector<ElectricalCompartment *> children;
    unordered_map<string, ChemicalCompartment *> chemical_compartment_map;
    vector<ChemicalCompartment *> chemical_compartment_seq;
    ref_ptr<Switch *> node;
    Vec3f distal;
    Vec3f proximal;
    float radius;

    ElectricalCompartment(const char * id) : ElectricalCompartment(string(id))
    {
    }

    ElectricalCompartment(const string id)
    {

    }

    Neuron *
    get_neuron()
    {
        return neuron
    }

    void
    set_neuron(Neuron * neuron)
    {
        this -> neuron = neuron;
    }

    ElectricalCompartment *
    get_parent()
    {
        return parent;
    }

    void
    set_parent(ElectricalCompartment * parent)
    {
        this -> parent = parent
    }

    ChemicalCompartment *
    add_chemical_compartment(ChemicalCompartment * chemical_compartment)
    {
        chemical_compartment_seq.push_back(chemical_compartment);
        chemical_compartment_group_node -> addDrawable(
            chemical_compartment -> node.get()
                                                      );
    }

    void
    remove_chemical_compartment(ChemicalCompartment * chemical_compartment)
    {
        size_t i;
        for(i = 0; i < chemical_compartment_seq.size(); ++i)
        {
            if(chemical_compartment_seq[i] == chemical_compartment)
            {
                chemical_compartment_group_node -> removeDrawable(
                        chemical_compartment -> node.get()
                                                                 );
                chemical_compartment_seq[i] = nullptr;
                for(; i < chemical_compartment_seq.size() - 1; ++i)
                {
                    chemical_compartment_seq[i] = chemical_compartment_seq[i + 1];
                }
                chemical_compartment_seq.resize(chemical_compartment_seq.size() - 1);
            }
        }
    }

    void
    hide()
    {
        node -> setNodeMask(HIDE_NODE_MASK);
    }

    void
    show()
    {
        node -> setNodeMask(SHOW_NODE_MASK);
    }

    void
    chemical_view()
    {

    }

    void
    electrical_view()
    {

    }

};



neuron -> create_chemical_compartments()
neuron -> create_electrical_compartments()

neuron_ids = moose.wildcardFind( "/##[ISA=Neuron]")
for neuron_id in neuron_ids:
    compartment_ids = moose.wildcardFind("##[ISA=CompartmentBase]")
    for compartment_id in compartment_ids:
        compartment_id =


handle_model(self, moose)
    network = self._handle_electrical_geometry(moose)
    self._handle_neuromesh(moose, network)
    self._handle_spinemesh(moose, network)
    self._handle_psdmesh(moose, network)
