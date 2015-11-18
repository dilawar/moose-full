import numpy
import os

from moogli.core import ElectricalCompartment, Neuron, Network


class SwcNetwork(Network):
    def __init__(self, filename):
        self.visualizables = {}
        network_name = neuron_name = os.path.splitext(
            os.path.basename(filename))[0]
        Network.__init__(self, network_name)
        dtype = numpy.dtype([('cid', numpy.int),
                             ('ctype', numpy.int),
                             ('cx', numpy.float64),
                             ('cy', numpy.float64),
                             ('cz', numpy.float64),
                             ('cr', numpy.float64),
                             ('cpid', numpy.int)])
        swc_data = numpy.genfromtxt(filename, dtype=dtype)
        neuron = Neuron(neuron_name)
        self.visualizables[neuron_name] = neuron
        print swc_data
        for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
            # if cid > 200 : break
            cid = str(cid)
            compartment = ElectricalCompartment(cid)
            compartment.distal = (cx, cy, cz, cr)
            neuron.attach(compartment)
            self.visualizables[cid] = compartment

        for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
            # if cid > 200 : break
            cid = str(cid)
            cpid = str(cpid)
            try:
                neuron[cid].parent = neuron[cpid]
                neuron[cid].proximal = neuron[cpid].distal
                neuron[cid].add_representation()
                neuron[cid].set_color((1.0, 1.0, 0.0, 1.0))

            except KeyError:
                neuron[cid].parent = None
                neuron[cid].proximal = neuron[cid].distal
                # neuron[cid].add_representation()
                # neuron[cid].set_color((0.0, 1.0, 0.0, 1.0))

            neuron[cid].show(1)

        neuron.show(0)
        self.attach(neuron)
