import moogli
import moogli.simulator
import moose
from moose import neuroml
from PyQt4 import Qt, QtCore, QtGui
import sys
import os

# app = QtGui.QApplication(sys.argv)
filename = sys.argv[1]
moose.neuroml.loadNeuroML_L123(filename)

moogli.simulator.moose.view( path = ""
                           , callbacks = [ (id, id, id)
                                         , (id, id, id)
                                         ]
                           )

# network_distal = moogli.simulator.moose.read_network( name = ""
#                                                     , path = ""
#                                                     , track_parent = False
#                                                     )
# viewer_distal = moogli.NetworkViewer(network_distal)
# viewer_distal.setWindowTitle("Distal Radius")

# network_averaged = moogli.simulator.moose.read_network( name = ""
#                                                       , path = "/cells[0]"
#                                                       , radius = moogli.simulator.moose.AVERAGED
#                                                       )
# viewer_averaged = moogli.NetworkViewer(network_averaged)
# viewer_averaged.setWindowTitle("Averaged Radius")

# network_proximal_distal = moogli.simulator.moose.read_network( name = ""
#                                                              , path = "/cells[0]"
#                                                              , radius = moogli.simulator.moose.PROXIMAL_DISTAL
#                                                              )
# viewer_proximal_distal = moogli.NetworkViewer(network_proximal_distal)
# viewer_proximal_distal.setWindowTitle("Continuous Variation in Radius")

# viewer_distal.show()
# viewer_averaged.show()
# viewer_proximal_distal.show()
# app.exec_()
