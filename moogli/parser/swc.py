# an integer number as compartment identifier
# type of neuronal compartment
#    0 - undefined
#    1 - soma
#    2 - axon
#    3 - basal dendrite
#    4 - apical dendrite
# x coordinate of the compartment
# y coordinate of the compartment
# z coordinate of the compartment
# radius of the compartment
# parent compartment

from itertools import *
import moogli

COMMENT_MARKER      =   "#"
COLUMN_SEPARATOR    =   " "
# UNDEFINED_TYPE_ID
# SOMA_TYPE_ID
# AXON_TYPE_ID
# BASAL_DENDRITE_TYPE_ID
# APICAL_DENDRITE_TYPE_ID


def split(predicate, iterable):
    left = []
    right = []
    flag = True
    for x in iterable:
        if not flag: right.append(x)
        else:
            flag = predicate(x)
            if flag: left.append(x)
            else   : right.append(x)
    return (left, right)



def parse(filename):
    swc_data = open(filename).readlines()
    swc_data = dropwhile( lambda line : line.startswith(COMMENT_MARKER), swc_data)
    swc_data = map( lambda line : line.strip().split(), swc_data)
    swc_data = map( lambda line : ( int(line[0])
                                  , int(line[1])
                                  , float(line[2])
                                  , float(line[3])
                                  , float(line[4])
                                  , float(line[5])
                                  , int(line[6])
                                  )
                  , swc_data
                  )
    return create_network( swc_data
                         , filename
                         , filename
                         )


def create_network(swc_data, network_name, neuron_name):
    print( network_name + " has " + str(len(swc_data)) + " compartments.")
    network = moogli.Network(network_name)
    neuron  = moogli.Neuron(neuron_name)
    for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
        compartment = moogli.ElectricalCompartment(str(cid))
        compartment.distal = (cx, cy, cz, cr)
        neuron.attach(compartment)

    for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
        try:
            neuron[str(cid)].parent   = neuron[str(cpid)]
            neuron[str(cid)].proximal = neuron[str(cpid)].distal
            neuron[str(cid)].add_representation()
            neuron[str(cid)].set_color((1.0, 0.0, 0.0, 1.0))

        except KeyError:
            neuron[str(cid)].parent   = None
            neuron[str(cid)].proximal = neuron[str(cid)].distal
            neuron[str(cid)].add_representation()
            neuron[str(cid)].set_color((0.0, 1.0, 0.0, 1.0))

        neuron[str(cid)].show(1)

    neuron.show(0)
    network.attach(neuron)
    return network



# def _network(swc_data):
#     network = Network(filename)
#     (somas, non_somas) = split( lambda line: line[COMPARTMENT_TYPE_INDEX] == SOMA_TYPE
#                               , swc_data
#                               )

#     neuron = Neuron(filename)
#     if len(somas) == 1:
#         soma = Compartment( [somas[0]]
#                           )
#         neuron.add_compartment(soma)


#     def compartmentalize(child, parent = None):
#         compartment = Compartment(child["id"])
#         proximal =
#         distal   = [child["x"], child["y"], child["z"], child["r"]]
#         compartment.add_geometry(
#                                 )
