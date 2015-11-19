import moogli
from moogli.visualization import NetworkVisualizer
from moogli.extensions.swc import SwcNetwork
from matplotlib.cm import rainbow
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QSplitter
import sys

app = QApplication(sys.argv)
network = SwcNetwork(sys.argv[1])
visualizer = NetworkVisualizer(network)
visualizer.show()
app.exec_()
