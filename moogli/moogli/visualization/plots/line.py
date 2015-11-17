from moogli import constants
from moogli.visualization.pipeline import SimulationDataConsumer
from PyQt4.QtGui import QWidget
from PyQt4.QtGui import QMenu
from PyQt4.QtGui import QAction
from PyQt4.QtGui import QGridLayout
from PyQt4.QtCore import pyqtSlot
from PyQt4 import QtCore
from matplotlib.lines import Line2D
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
from matplotlib.figure import Figure
import numpy
import csv
import time
from PyQt4.QtCore import SLOT, SIGNAL


class LinePlotter(SimulationDataConsumer, QWidget):
    def __init__(self, visualizables, colors=None, parent=None,
                 figure_width=5.0, figure_height=4.0, dpi=100,
                 facecolor="", legend_alpha=0.5):
        QWidget.__init__(self, parent)
        SimulationDataConsumer.__init__(self)
        self.figure_width = figure_width
        self.figure_height = figure_height
        self.dpi = dpi
        self.grid_visible = False
        self.legend_visible = True
        self.facecolor = facecolor
        self.legend_alpha = legend_alpha
        self.visualizables = visualizables
        self._setup_prelude()
        self._setup_plot_canvas()
        self._setup_navigation_toolbar()
        self._setup_actions()
        self._setup_context_menu()
        self._setup_signal_slot_connections()
        self._setup_postlude()

    """
    def lineplot(self, visids, *args, **kwargs):
        axes = self.figure.add_subplot(args, kwargs)
        for visid in visids:
            line = axes.plot(numpy.array([]),
                             numpy.array([]),
                             label=visid,
                             gid=visid)[0]
       self._create_legend()


        return axes

    def eventplot():
        pass

    def eventplot(self, nrows, ncols, plot_number, ids, **kwargs, ):
        pass

    def add_subplot(self, nrows, ncols, plot_number, plot_type,
                    ids, **kwargs):

        axes = self.add_subplot(rows, cols, plot_number, kwargs)
        if plot_type == moogli.constants.EVENT_PLOT:
            axes.eventplot
        elif:

        return axes


    def get_subplot(plot_number):
        pass
    """

    def set_title(self, title):
        self.axes.set_title(title)

    def get_field(self):
        return self.field

    def _create_line(self,
                     label,
                     color=constants.DEFAULT,
                     zorder=0):
        line = self.axes.plot(numpy.array([]),
                              numpy.array([]),
                              label=label,
                              gid=label)[0]
        if color is not constants.DEFAULT:
            line.set_color(color)
        self._create_legend()

    def _setup_prelude(self):
        self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.setLayout(QGridLayout())

    def _setup_postlude(self):
        map(self._create_line, self.visualizables)

    def _setup_plot_canvas(self):
        self.figure = Figure((self.figure_width, self.figure_height),
                             dpi=self.dpi, facecolor='#F3EFEE')
        self.canvas = FigureCanvas(self.figure)
        self.layout().addWidget(self.canvas, 0, 0)
        self.axes = self.figure.add_subplot(1, 1, 1)
        self.legend = None

    def _create_legend(self):
        self.legend = self.axes.legend(loc='upper right',
                                       prop={'size': 10},
                                       # bbox_to_anchor=(1.0, 0.5),
                                       fancybox=True,
                                       shadow=False,
                                       ncol=1)
        self.legend.draggable()
        self.legend.get_frame().set_alpha(self.legend_alpha)
        if self.legend_visible:
            self.show_legend_slot()
        else:
            self.hide_legend_slot()

    def _setup_navigation_toolbar(self):
        self.navigation_toolbar = NavigationToolbar(self.canvas, self)
        self.layout().addWidget(self.navigation_toolbar, 1, 0)

    def _setup_actions(self):

        self.toggle_grid_action = QAction(self)
        self.toggle_grid_action.setText("Show Grid")

        self.toggle_autoscale_action = QAction(self)
        self.toggle_autoscale_action.setText("Enable Autoscaling")

        self.toggle_axis_hold_action = QAction(self)
        self.toggle_axis_hold_action.setText("Hold Axes")

        self.toggle_legend_action = QAction(self)
        self.toggle_legend_action.setText("Hide Legend")

        self.export_to_csv_action = QAction(self)
        self.export_to_csv_action.setText("CSV")

    def _setup_signal_slot_connections(self):
        self.canvas.mpl_connect('pick_event', self.pick_event_slot)
        self.toggle_grid_action.triggered.connect(self.toggle_grid_slot)
        self.toggle_autoscale_action.triggered.connect(
            self.toggle_autoscale_slot)
        self.toggle_axis_hold_action.triggered.connect(
            self.toggle_axis_hold_slot)
        self.toggle_legend_action.triggered.connect(self.toggle_legend_slot)
        self.export_to_csv_action.triggered.connect(self.export_to_csv_slot)
        self.connect(self, SIGNAL("customContextMenuRequested(QPoint)"),
                     self, SLOT("show_context_menu_slot(QPoint)"))

    @QtCore.pyqtSlot()
    def draw_slot(self):
        self.canvas.draw()

    def pick_event_slot(self, event):
        pass

    def show_grid_slot(self):
        self.axes.grid(True)
        self.toggle_grid_action.setText("Hide Grid")
        self.draw_slot()

    def hide_grid_slot(self):
        self.axes.grid(False)
        self.toggle_grid_action.setText("Show Grid")
        self.draw_slot()

    def toggle_grid_slot(self):
        self.hide_grid_slot() if self.grid_visible else self.show_grid_slot()
        self.grid_visible = not self.grid_visible

    def toggle_autoscale_slot(self):
        pass

    def toggle_axis_hold_slot(self):
        pass

    def show_legend_slot(self):
        if self.legend is None:
            return
        self.legend.set_visible(True)
        self.toggle_legend_action.setText("Hide Legend")
        self.draw_slot()

    def hide_legend_slot(self):
        if self.legend is None:
            return
        self.legend.set_visible(False)
        self.toggle_legend_action.setText("Show Legend")
        self.draw_slot()

    def toggle_legend_slot(self):
        if self.legend_visible:
            self.hide_legend_slot()
        else:
            self.show_legend_slot()
        self.legend_visible = not self.legend_visible

    def set_xlabel(self, xlabel):
        self.axes.set_xlabel(xlabel)

    def set_ylabel(self, ylable):
        self.axes.set_ylabel(ylable)

    @pyqtSlot(QtCore.QPoint)
    def show_context_menu_slot(self, point):
        self.context_menu.exec_(self.mapToGlobal(point))

    def _setup_context_menu(self):

        self.context_menu = QMenu()

        self.context_menu.addAction(self.toggle_grid_action)

        self.context_menu.addAction(self.toggle_autoscale_action)

        self.context_menu.addAction(self.toggle_axis_hold_action)

        self.context_menu.addAction(self.toggle_legend_action)

        export_menu = self.context_menu.addMenu("Export")
        export_menu.addAction(self.export_to_csv_action)

    def toggle_line_visibility_slot(self, line):
        line.set_visible(not line.get_visible())

    @QtCore.pyqtSlot(str)
    def export_to_csv_slot(self, filepath):
        with open(filepath,"wb") as csv_file:
            writer = csv.writer( csv_file
                               , delimiter  = ' '
                               , quotechar  = '"'
                               , fieldnames = ["Time"] + self._tables.keys()
                               , quoting    = csv.QUOTE_NONNUMERIC
                               )
            writer.writerow(["#MODEL             : {model}".format(model = self.instance["model"])])
            writer.writerow(["#TIMESTAMP         : {timestamp}".format(timestamp = int(time.time()))])
            writer.writerow(["#MOOSE VERSION     : {moose_version}".format(moose_version = "123")])
            writer.writerow(["#GOOSE VERSION     : {goose_version}".format(goose_version = "456")])
            writer.writeheader()
            units = ["s"] + [self._unit] * len(self._tables)
            writer.writerow(units)
            data = [ value.get_ydata() for value in self._tables.values() ]
            data = data + data[0].get_xdata()
            data = transpose(data)
            writer.writerows(data)

    def consume(self):
        self.axes.set_xlim([self.clock.begin, self.clock.end])
        for line in self.axes.lines:
            for mediator in self.mediators:
                try:
                    print "gid", line.get_gid()
                    yvalue = mediator.output[line.get_gid()]
                    ydata = numpy.append(line.get_ydata(),
                                 yvalue)
                    print("y_data", ydata)
                    line.set_ydata(ydata)
                    line.set_xdata(numpy.linspace(self.clock.begin, self.clock.now(), len(ydata)))
                    break
                except:
                    continue
        self.draw_slot()
