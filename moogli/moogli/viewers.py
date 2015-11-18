import PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import Qt
import math
import moogli.core._moogli

class View(moogli.core._moogli.MeshView):
    def __init__(self,
                 id,
                 position=moogli.geometry.Vec2f(0.0, 0.0),
                 size=moogli.geometry.Vec2f(1.0, 1.0),
                 background_color=moogli.colors.WHITE,
                 prelude=lambda x: x,
                 interlude=lambda x: x,
                 postlude=lambda x: x,
                 idletime=0):
        super(View, self).__init__()#id)
        super(View, self).set_position(position)
        super(View, self).set_size(size)
        super(View, self).set_background_color(background_color)
        self._id = id
        self._color_bars = set()
        self.prelude = prelude
        self.interlude = interlude
        self.postlude = postlude
        self.idletime = idletime
        self._done = False
        self._pause = False
        self._viewer = None

    def viewer(self):
        return self._viewer

    def id(self):
        return self._id

    def start(self):
        self._done = False
        self.viewer().update()
        self.home()
        self.prelude(self)
        self.run()

    def pause(self):
        self._pause = True

    def resume(self):
        self._pause = False
        self.run()

    def stop(self):
        self._done = True
        self.postlude(self)
        self.viewer().update()

    def run(self):
        self.interlude(self)
        self.viewer().update()
        if self._done: return
        if self._pause: return
        PyQt4.QtCore.QTimer.singleShot(self.idletime, self.run)

    def attach_color_bar(self, color_bar):
        if color_bar in self._color_bars: return
        color_bar.view = self
        super(View, self).attach_color_bar(color_bar)
        self._color_bars.add(color_bar)

    def resize(self, width, height):
        super(View, self).resize(width, height)
        for color_bar in self._color_bars:
            color_bar.resize(width, height)


class Viewer(moogli.core._moogli.Viewer):
    def __init__(self, id):
        self._id = id
        super(Viewer, self).__init__()
        self.setMouseTracking(True)
        self.groups     = set()
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

    def id(self):
        return self._id

    def start(self):
        [view.start() for view in self._views.values()]

    def pause(self):
        [view.pause() for view in self._views.values()]

    def resume(self):
        [view.resume() for view in self._views.values()]

    def stop(self):
        [view.stop() for view in self._views.values()]

    def run(self):
        [view.run() for view in self._views.values()]

    def resizeEvent(self, event):
        super(Viewer, self).resizeEvent(event)
        for view in self._views.values():
            view.resize(event.size().width(), event.size().height())

    def attach_view(self, view):
        if view in self._views.values(): return
        super(Viewer, self).attach_view(view)
        self._views[view.id()] = view
        view._viewer = self
        self.update()
        view.home()

    def detach_view(self, view):
        if view not in self._views.values(): return
        super(Viewer, self).detach_view(view)
        self._views.pop(view.id())
        view._viewer = None

    def attach_shape(self, shape):
        if shape in self._meshes: return
        super(Viewer, self).attach_shape(shape)
        self._meshes.add(shape)
        self.home()

    def detach_shape(self, shape):
        if shape not in self._meshes: return
        super(Viewer, self).detach_shape(shape)
        self._meshes.remove(shape)

    def attach_shapes(self, shapes):
        map(self.attach_shape, shapes)

    def detach_shapes(self, shapes):
        map(self.detach_shape, shapes)

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
        control_pressed = event.modifiers() & Qt.ControlModifier == Qt.ControlModifier
        shift_pressed = event.modifiers() & Qt.ShiftModifier == Qt.ShiftModifier

        if key == Qt.Key_Q and control_pressed:
            QtGui.QApplication.quit()
        elif key == Qt.Key_W and control_pressed:
            QtGui.QApplication.quit()
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

    # @QtCore.pyqtSlot(str)
    # def printme(self, name):
    #     # cursor = QtGui.QCursor().pos()
    #     menu = QtGui.QMenu()
    #     menu.addAction("a")
    #     menu.addAction("b")
    #     menu.addAction("c")
    #     menu.exec_(QtGui.QCursor().pos())
    #     print("Selected = > ", name)
