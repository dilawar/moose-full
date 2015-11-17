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

"""
https://en.wikipedia.org/wiki/Comma-separated_values
"""
import .table

LINE_SEPARATOR_REGEX        = r"[\n\r]*" # Windows or Unix style newline character
COLUMN_SEPARATOR_REGEX      = r","  # Any number of spaces followed by comma followed by any number of spaces
COMMENT_REGEX               = r"^[ \t]*#"  # Any number of space characters followed by a tab

def parse(filename):
    return table.parse( filename
                      , LINE_SEPARATOR_REGEX
                      , COLUMN_SEPARATOR_REGEX
                      , COMMENT_REGEX
                      , lambda line: ( table.to_string(line[0])
                                     , map(table.to_float, line[1:])
                                     )
                      )

# def create_network(csv_data, network_name):
#     print network_name + " has " + str(len(csv_data)) + " compartments."
#     network = moogli.Network(network_name)

#     for (nid, cid, cxp, cyp, czp, crp, cxd, cyd, czd, crd) in csv_data:
#         try:
#             neuron = network[str(nid)]
#         except KeyError:
#             neuron = moogli.Neuron(str(nid))
#             network.attach(neuron)
#         compartment = moogli.ElectricalCompartment(str(cid))
#         compartment.distal = (cxd, cyd, czd, crd)
#         compartment.proximal = (cxp, cyp, czp, crp)
#         neuron.attach(compartment)
#         compartment.add_representation()
#         compartment.show(1)
#         neuron.show(0)

#     return network
