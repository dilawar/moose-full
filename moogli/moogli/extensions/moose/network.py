import moogli
import moose

# class MooseSpineHead(moogli.core.Frustum):
#     pass

# class MooseSpineShaft(moogli.core.Frustum):
#     pass

# class MooseSoma(moogli.core.Sphere):
#     pass

# class MooseDendrite(moogli.core.Frustum):
#     def __init__(self, moose_element):
#         self.parent = pass

# class MooseChemicalCompartment(moogli.core.Frustum):
#     pass

# class MooseNetwork()


def read(path="", vertices=10, track_parent_radius=False):
    network = moogli.Group(path)
    neuron = moogli.Group("neuron")
    soma = moogli.Group("soma")
    axon = moogli.Group("axon")
    dendrite = moogli.Group("dendrite")
    basal = moogli.Group("basal")
    apical = moogli.Group("apical")
    spine = moogli.Group("spine")
    head = moogli.Group("head")
    shaft = moogli.Group("shaft")
    network.attach_group(soma)
    network.attach_group(axon)
    network.attach_group(dendrite)
    network.attach_group(spine)
    dendrite.attach_group(basal)
    dendrite.attach_group(apical)
    spine.attach_group(head)
    spine.attach_group(shaft)

    compartments = moose.wildcardFind(path + "/##[ISA=CompartmentBase]")
    for compartment in compartments:
        neuron_id = compartment.parent.path
        try:
            neuron = network.groups[neuron_id]
            soma = neuron.groups["soma"]
            axon = neuron.groups["axon"]
            dendrite = neuron.groups["dendrite"]
            spine = neuron.groups["spine"]
            head = spine.groups["head"]
            shaft = spine.groups["shaft"]
            basal = dendrite.groups["basal"]
            apical = dendrite.groups["apical"]
        except:
            neuron = moogli.Group(neuron_id)
            soma = moogli.Group("soma")
            axon = moogli.Group("axon")
            dendrite = moogli.Group("dendrite")
            basal = moogli.Group("basal")
            apical = moogli.Group("apical")
            spine = moogli.Group("spine")
            head = moogli.Group("head")
            shaft = moogli.Group("shaft")
            neuron.attach_group(soma)
            neuron.attach_group(axon)
            neuron.attach_group(dendrite)
            dendrite.attach_group(basal)
            dendrite.attach_group(apical)
            neuron.attach_group(spine)
            spine.attach_group(head)
            spine.attach_group(shaft)
            network.attach_group(neuron)

        distal = moogli.geometry.Vec3f(compartment.x * 1.0e6,
                                       compartment.y * 1.0e6,
                                       compartment.z * 1.0e6)
        proximal = moogli.geometry.Vec3f(compartment.x0 * 1.0e6,
                                         compartment.y0 * 1.0e6,
                                         compartment.z0 * 1.0e6)
        distal_radius = compartment.diameter * 5.0e5
        proximal_radius = compartment.diameter * 5.0e5
        if not track_parent_radius:
                proximal_radius = distal_radius
        if proximal == distal:
            shape = moogli.shapes.Sphere(compartment.path,
                                         proximal,
                                         distal_radius,
                                         vertices,
                                         moogli.colors.GREEN)
        else:
            shape = moogli.shapes.Frustum(compartment.path,
                                          proximal,
                                          distal,
                                          proximal_radius,
                                          distal_radius,
                                          vertices,
                                          moogli.colors.ORANGE,
                                          moogli.colors.ORANGE)
        if "axon" in compartment.name:
            axon.attach_shape(shape)
            network.groups["axon"].attach_shape(shape)
        elif "soma" in compartment.name:
            soma.attach_shape(shape)
            network.groups["soma"].attach_shape(shape)
        elif "head" in compartment.name:
            head.attach_shape(shape)
            network.groups["spine"].groups["head"].attach_shape(shape)
            spine.attach_shape(shape)
            network.groups["spine"].attach_shape(shape)
        elif "shaft" in compartment.name:
            shaft.attach_shape(shape)
            network.groups["spine"].groups["shaft"].attach_shape(shape)
            spine.attach_shape(shape)
            network.groups["spine"].attach_shape(shape)
        elif "basal" in compartment.name:
            basal.attach_shape(shape)
            network.groups["dendrite"].groups["basal"].attach_shape(shape)
            dendrite.attach_shape(shape)
            network.groups["dendrite"].attach_shape(shape)
        elif "apical" in compartment.name:
            apical.attach_shape(shape)
            network.groups["dendrite"].groups["apical"].attach_shape(shape)
            dendrite.attach_shape(shape)
            network.groups["dendrite"].attach_shape(shape)
        else:
            dendrite.attach_shape(shape)
            network.groups["dendrite"].attach_shape(shape)
        neuron.attach_shape(shape)
        network.attach_shape(shape)

    return network
