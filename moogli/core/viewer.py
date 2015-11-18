import PyQt4
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt
import math
import _moogli

class View(_moogli.MeshView):
    def __init__(self, id):
        self._id = id
        _moogli.MeshView.__init__(self)
        self._color_bars = set()

    def get_id(self):
        return self._id

    def attach_color_bar(self, color_bar):
        if color_bar in self._color_bars: return
        color_bar.view = self
        super(View, self).attach_color_bar(color_bar)
        self._color_bars.add(color_bar)

    def resize(self, width, height):
        super(View, self).resize(width, height)
        for color_bar in self._color_bars:
            color_bar.resize(width, height)



class Viewer(_moogli.Viewer):
    def __init__( self
                # , parent
                # , shareWidget
                # , flags
                , prelude   = lambda x : x
                , interlude = lambda x : x
                , postlude  = lambda x : x
                , idletime  = 0
                ):
        PyQt4.QtGui.QApplication.instance().aboutToQuit.connect(self.stop)
        _moogli.Viewer.__init__(self)
        self.groups     = set()
        # self.network    = network
        self.prelude    = prelude
        self.interlude  = interlude
        self.postlude   = postlude
        self.idletime   = idletime
        self._done      = False
        self._pause     = False
        self._views = dict()
        self._meshes = set()
        self.roll_angle = math.pi / 36.0
        self.pitch_angle = math.pi / 36.0
        self.yaw_angle = math.pi / 36.0
        self.zoom_factor = 0.10
        self.forward_distance = 10.0
        self.backward_distance = 10.0
        self.left_distance = 10.0
        self.right_distance = 10.0
        self.up_distance = 10.0
        self.down_distance = 10.0

    def start(self):
        self._done = False
        self.prelude(self)
        self.update()
        self.run()

    def pause(self):
        self._pause = True

    def resume(self):
        self._pause = False
        self.run()

    def stop(self):
        self._done = True
        self.postlude(self)

    def run(self):
        self.interlude(self)
        self.update()
        if self._done: return
        if self._pause: return
        PyQt4.QtCore.QTimer.singleShot(self.idletime, self.run)

    def resizeEvent(self, event):
        super(Viewer, self).resizeEvent(event)
        for view in self._views.values():
            view.resize(event.size().width(), event.size().height())

    def attach_view(self, view):
        if view in self._views.values(): return
        super(Viewer, self).attach_view(view)
        self._views[view.get_id()] = view

    def detach_view(self, view):
        if view not in self._views.values(): return
        super(Viewer, self).detach_view(view)
        self._views.pop(view.get_id())

    def attach_geometry(self, mesh):
        if mesh in self._meshes: return
        super(Viewer, self).attach_geometry(mesh)
        self._meshes.add(mesh)

    def home(self):
        [view.home() for view in self._views.values()]

    def forward(self, distance_delta):
        [view.forward(distance_delta) for view in self._views.values()]

    def backward(self, distance_delta):
        [view.backward(distance_delta) for view in self._views.values()]

    def left(self, distance_delta):
        [view.left(distance_delta) for view in self._views.values()]

    def right(self, distance_delta):
        [view.right(distance_delta) for view in self._views.values()]

    def up(self, distance_delta):
        [view.up(distance_delta) for view in self._views.values()]

    def down(self, distance_delta):
        [view.down(distance_delta) for view in self._views.values()]

    def zoom(self, factor):
        [view.zoom(factor) for view in self._views.values()]

    def roll(self, angle):
        [view.roll(angle) for view in self._views.values()]

    def pitch(self, angle):
        [view.pitch(angle) for view in self._views.values()]

    def yaw(self, angle):
        [view.yaw(angle) for view in self._views.values()]


    def keyPressEvent(self, event):
        #super(Viewer, self).event(event)
        key = event.key()
        # modifiers = QtGui.QApplication.keyboardModifiers()
        control_pressed = event.modifiers() & Qt.ControlModifier == 1
        shift_pressed = event.modifiers() & Qt.ShiftModifier == 1
        if key == Qt.Key_Q and control_pressed:
            QApplication.quit()
        elif key == Qt.Key_W and control_pressed:
            QApplication.quit()
        elif key == Qt.Key_Space:
            self.home()
        elif key == Qt.Key_Up:
            self.up(self.up_distance)
        elif key == Qt.Key_Down:
            self.down(self.down_distance)
        elif key == Qt.Key_Left:
            self.left(self.left_distance)
        elif key == Qt.Key_Right:
            self.right(self.right_distance)
        elif key == Qt.Key_F:
            self.forward(self.forward_distance)
        elif key == Qt.Key_B:
            self.backward(self.backward_distance)
        elif key == Qt.Key_Plus:
            self.zoom(self.zoom_factor)
        elif key == Qt.Key_Equal:
            self.zoom(self.zoom_factor)
        elif key == Qt.Key_Period:
            self.zoom(self.zoom_factor)
        elif key == Qt.Key_Greater:
            self.zoom(self.zoom_factor)
        elif key == Qt.Key_Minus:
            self.zoom(-self.zoom_factor)
        elif key == Qt.Key_Underscore:
            self.zoom(-self.zoom_factor)
        elif key == Qt.Key_Comma:
            self.zoom(-self.zoom_factor)
        elif key == Qt.Key_Less:
            self.zoom(-self.zoom_factor)
        elif key == Qt.Key_R:
            if shift_pressed:
                self.roll(-self.roll_angle)
            else:
                self.roll(self.roll_angle)
        elif key == Qt.Key_P:
            if shift_pressed:
                self.pitch(-self.pitch_angle)
            else:
                self.pitch(self.pitch_angle)
        elif key == Qt.Key_Y:
            if shift_pressed:
                self.yaw(-self.yaw_angle)
            else:
                self.yaw(self.yaw_angle)

    def get_view_with_focus(self):
        view_id = super(Viewer, self).get_view_with_focus()
        return self._views[view_id]

    @QtCore.pyqtSlot(str)
    def printme(self, name):
        # cursor = QtGui.QCursor().pos()
        menu = QtGui.QMenu()
        menu.addAction("a")
        menu.addAction("b")
        menu.addAction("c")
        menu.exec_(QtGui.QCursor().pos())
        print("Selected = > ", name)
