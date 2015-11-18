from PyQt4 import QtGui, QtCore
from PyQt4.QtGui import QSystemTrayIcon, QIcon, QAction, QMenu
from PyQt4.QtCore import SIGNAL
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import os
import subprocess
import sys

# class Test(QtGui.QDialog):
#     def __init__(self):
#         QtGui.QDialog.__init__(self, None, QtCore.Qt.WindowStaysOnTopHint)
#         self.setWindowTitle("Password")
#         self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)
#         self.line1 = QLabel("Please type your username and password", self)
#         self.line3 = QLabel("Passsord:", self)
#         self.line2 = QLabel("Username:", self)
#         self.textUsername = QtGui.QLineEdit(self)
#         self.textPassword = QtGui.QLineEdit(self)
#         self.textPassword.setEchoMode(QtGui.QLineEdit.Password)
#         self.loginbuton = QtGui.QPushButton('Login', self)
#         self.loginbuton.clicked.connect(self.Login)
#         layout = QtGui.QVBoxLayout(self)
#         layout.addWidget(self.line1)
#         layout.addWidget(self.line2)
#         layout.addWidget(self.textUsername)
#         layout.addWidget(self.line3)
#         layout.addWidget(self.textPassword)
#         layout.addWidget(self.loginbuton)
#         self._want_to_close = False

#     def exitEvent(self):
#         self.exitOnClose = True
#         self.close()
#     def closeEvent(self, evnt):
#         if self._want_to_close:
#             super(Test, self).closeEvent(evnt)
#         else:
#             evnt.ignore()
#             pass

#     def Login(self):
#         if (self.textUsername.text() == 'admin' and
#             self.textPassword.text() == 'password'):
#             self.accept()
#         else:
#             QtGui.QMessageBox.warning(
#                 self, 'Access Denied', 'Incorrect user or password')

#     def keyPressEvent(self, e):
#        if e.key() == QtCore.Qt.Key_Meta:
#          pass
#        if e.key() == QtCore.Qt.Key_Escape:
#           pass

class OverlayDialog(QtGui.QDialog):
    STYLESHEET = '''
    QDialog {
        color: white;
        background-color: red;
    }
    '''
    def __init__(self, parent=None):
        super(OverlayDialog, self).__init__(parent)
        # self.setAttribute( Qt.WA_TranslucentBackground, True )
        # self.setWindowOpacity(0.1)
        # self.setStyleSheet(self.STYLESHEET)
        pal = QPalette()
        pal.setBrush( QPalette.Window, QColor(255, 0, 0, 64) )
        self.setPalette(pal)
        self.setAutoFillBackground(True) #// maybe it's the default!


class FloatingButton(QtGui.QPushButton):

    STYLESHEET = '''
    QPushButton {
        background-color : black;
        color            : white;
        border: 0px solid;
        border-color: black;
        border-radius: 15px;
        padding: 0px;
        qproperty-icon: url('%(icon)s');
    }
    '''

    def __init__( self
                , text              = None
                , icon              = None
                , tooltip           = None
                , parent            = None
                , background_color  = "black"
                , color             = "white"
                , position          = None
                , size              = None
                ):
        super(FloatingButton, self).__init__(parent)
        print("color: {0};".format(color))
        print(background_color)
        print(icon)
        self.position = position
        self.size     = size
        # self.setStyleSheet(self.STYLESHEET.format( color, background_color, icon
        #                                          # , background_color = background_color
        #                                          # , color            = color
        #                                          )
        #                   )
        # self.x_percentage, self.y_percentage = position
        # self.margins = margins
        # if color is not None:
        #     self.setStyleSheet("color: {0};".format(color))
        # if background_color is not None:
        #     self.setStyleSheet("background-color: {0};".format(background_color))

        if icon is not None:
            self.setStyleSheet(self.STYLESHEET % { "icon" : icon} )
        (width, height) = size
        self.setMinimumSize(width, height)
        self.setMaximumSize(width, height)
        self.setFocusPolicy(QtCore.Qt.NoFocus)
        if tooltip is not None: self.setToolTip(tooltip)
        if text is not None   : self.setText(text)


    def resizeEvent(self, event):
        super(FloatingButton, self).resizeEvent(event)
        if self.position is None or self.size is None: return
        x_center, y_center = self.position
        width, height = self.size

        if isinstance(x_center, float):
            x_center = x_center * self.parent().width()
        if isinstance(y_center, float):
            y_center = y_center * self.parent().height()

        if x_center < 0: x_center = self.parent().width() + x_center

        if y_center < 0: y_center = self.parent().height() + y_center


        x_top  = int( x_center - width / 2.0 )
        y_left = int( y_center - height/ 2.0 )
        self.setGeometry(x_top, y_left, width, height)

        # self.setStyleSheet(self.STYLESHEET)


    # def setParent(self, parent):
    #     self.updatePosition()
    #     return super(FloatingButton, self).setParent(parent)

    # def updatePosition(self):

    #     if hasattr(self.parent(), 'viewport'):
    #         parentRect = self.parent().viewport().rect()
    #     else:
    #         parentRect = self.parent().rect()

    #     if not parentRect:
    #         return

    #     x = int(parentRect.width() * x_percentage) + self.margins["left"]
    #     y = int(parentRect.height() * (1 - y_percentage)) + self.paddingBottom - self.height() #/ 2 #- self.paddingBottom
    #     self.setGeometry(x, y, self.width(), self.height())

    # def resizeEvent(self, event):
    #     super(OverlayButton, self).resizeEvent(event)
    #     self.updatePosition()

    # def showEvent(self, event):
    #     self.updatePosition()
    #     return super(OverlayButton, self).showEvent(event)


class FloatingButtonGrid(QObject):
    def __init__(self, button_data, position = None, size = None, margin = None, parent = None):
        super(FloatingButtonGrid,self).__init__(parent)
        self.button_data = button_data
        self.position = position
        self.size     = size
        self.margin   = margin
        self.setup_buttons()

    def setup_buttons(self):
        self.row_count = len(self.button_data)
        self.col_count = len(self.button_data[0])
        self.buttons = [[0] * self.col_count for i in range(self.row_count)]
        for row_index in range(self.row_count):
            for col_index in range(self.col_count):
                data = self.button_data[row_index][col_index]
                if data is None:
                    button = None
                else:
                    button = FloatingButton( text = data["text"]
                                                              , icon = data["icon"]
                                                              , color = data["color"]
                                                              , background_color = data["background_color"]
                                                              , tooltip = data["tooltip"]
                                                              , parent  = self.parent()
                                                              , size = self.size
                                                              )
                self.buttons[row_index][col_index] = button

    def resizeEvent(self, event):
        x_position, y_position = self.position
        width, height = self.size
        x_margin, y_margin = self.margin

        if isinstance(x_position, float):
            x_position = x_position * self.parent().width()
        if isinstance(y_position, float):
            y_position = y_position * self.parent().height()

        if x_position < 0: x_position = self.parent().width() + x_position

        if y_position < 0: y_position = self.parent().height() + y_position


        x_start = int( x_position
                     - (self.col_count * width + (self.col_count - 1) * x_margin) / 2.0
                     )
        y_start = int( y_position
                     - (self.row_count * height + (self.row_count - 1) * y_margin) / 2.0
                     )
        for row_index in range(self.row_count):
            y = y_start + row_index * (height + y_margin)
            for col_index in range(self.col_count):
                x = x_start + col_index * (width + x_margin)
                button = self.buttons[row_index][col_index]
                if button is not None:
                    button.setGeometry(x, y, width, height)

class MoogliIve(QtGui.QWidget):
    def __init__(self, parent=None):
        super(MoogliIve, self).__init__(parent)
        self.setup_control_grid()
        self.setup_navigation_grid()
        self.setup_up_grid()
        self.setStyleSheet("background-color : white;")
        self.top_left_navigation_button = FloatingButton( icon      = "../data/icons/white/chevron-top-left.svg"
                                                        , background_color = "black"
                                                        , position  = (20, 20)
                                                        , size      = (30, 30)
                                                        , parent    = self
                                                        )

        # self.create_spherical_navigation_grid()
        # self.create_video_player_grid()

        # self.flat_left_button               =
        # self.flat_right_button              =
        # self.flat_up_button                 =
        # self.flat_down_button               =
        # self.spherical_left_button          =
        # self.spherical_right_button         =
        # self.spherical_up_button            =
        # self.spherical_down_button          =
        # self.zoom_in_button                 =
        # self.zoom_out_button                =
        # self.rotate_clockwise_button        =
        # self.rotate_anticlockwise_button    =
        # self.datafiles_button               =
        # self.snapshot_button                =
        # self.movie_button                   =
        # self.help_button                    =
        # self.settings_button                =
        # self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)
        # self.lockbutton = OverlayButton('Lock', self)
        # self.lockbutton.setParent(self)
        # self.lockbutton.setStyleSheet("""QPushButton { position    : relative bottom right;
        #                                              , margin      : 200px;
        #                                              }
        #                               """)
        # # self.setLayout(QHBoxLayout())
        # self.layout().addWidget(self.lockbutton)
        # self.dialog = OverlayDialog(self)
        # self.home_button.show()
        # self.home_button.clicked.connect(self.dialog.show)
        # self.setStyleSheet("background-color : white;")
        # self.setAttribute( Qt.WA_TranslucentBackground, True )

        # self.setWindowOpacity(1.0)

    def resizeEvent(self, event):
        super(MoogliIve, self).resizeEvent(event)
        self.control_grid.resizeEvent(event)
        self.move_grid.resizeEvent(event)
        self.up_grid.resizeEvent(event)
        self.top_left_navigation_button.resizeEvent(event)
        self.up_grid.setVisible(False)

    def generate_button_data(self, data, text, color, background_color):
        row_count  = len(data)
        col_count  = len(data[0])
        button_data = [[0] * col_count for i in range(row_count)]

        for row_index in range(row_count):
            for col_index in range(col_count):
                datum = data[row_index][col_index]
                if datum is None:
                    button_datum = None
                else:
                    button_datum = { "icon"                 :   datum[0]
                                   , "color"                :   color
                                   , "background_color"     :   background_color
                                   , "text"                 :   text
                                   , "tooltip"              :   datum[1]
                                   }
                button_data[row_index][col_index] = button_datum

        return button_data

    def setup_play_grid(self):
        data = ( (None, ("chevron-up.svg", "Move Up"), None)
               , (("chevron-left.svg", "Pan Left"), ("home.svg", "Go top home position"), ("chevron-right.svg", "Pan Right"))
               , (None, ("chevron-down.svg", "Pan Down"), None)
               )
        print(self)
        self.move_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (0.875, 0.875)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )

    def setup_control_grid(self):
        data = ( ( ("database.svg", "Move Up")
                 , ("paint-brush.svg", "Move Up")
                 , ("arrows-alt.svg", "Move Up")
                 )
               , (("camera.svg", "Pan Left"), ("video-camera.svg", "Go top home position"), ("arrows-h.svg", "Pan Right"))
               , (("cogs.svg", "Move Up"), ("question.svg", "Pan Down"),("eyedropper.svg", "Move Up"))
               )
        self.control_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (-60, -60)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )

    def setup_navigation_grid(self):
        data = ( (("search-plus.svg", "Move Up"), ("chevron-up.svg", "Move Up"), ("search-minus.svg", "Move Up"))
               , (("chevron-left.svg", "Pan Left"), ("home.svg", "Go top home position"), ("chevron-right.svg", "Pan Right"))
               , (("undo.svg", "Move Up"), ("chevron-down.svg", "Pan Down"),("repeat.svg", "Move Up"))
               )
        self.move_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (60, -60)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )


    def setup_pan_grid(self):
        data = ( (None, ("arrow-up.svg", "Pan Up"), None)
               , (("arrow-left.svg", "Pan Left"), ("home.svg", "Go top home position"), ("arrow-right.svg", "Pan Right"))
               , (None, ("arrow-down.svg", "Pan Down"), None)
               )
        print(self)
        self.pan_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (60, -60)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )

    def setup_up_grid(self):
        data = ( ( ("../data/icons/white/chevron-up.svg", "Pan Up"), ), )
        self.up_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (0.5, 20)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )

    def setup_top_left_grid(self):
        data = ( ( ("../data/icons/white/chevron-top-left.svg", "Pan Up"), ), )
        self.top_left_grid = FloatingButtonGrid( self.generate_button_data( data
                                                                , None
                                                                , "white"
                                                                , "black"
                                                                )
                                          , (0.5, 20)
                                          , (30, 30)
                                          , (2, 2)
                                          , self
                                          )

    # def resizeEvent(self, event):
    #     self.lockbutton.updatePosition()
        # super(ExampleOverlayedText, self).resizeEvent(event)

        # text_widget = text(self)
        # self.setCentralWidget(text_widget)
        # self.setStyleSheet("""
        #                    QMainWindow { background: 'black'
        #                                }
        #                   """);

# class text(QtGui.QWidget):
#     def __init__(self, parent=None):
#         super(text, self).__init__(parent)
#         self.label1 = QtGui.QLabel('Welcome')
#         layout = QtGui.QHBoxLayout()
#         layout.addWidget(self.label1)
#         self.setLayout(layout)

# class Window(QtGui.QMainWindow):
#     def __init__(self, parent = None):
#         QtGui.QMainWindow.__init__(self, parent)
#         self.resize(150, 50)
#         self.lockbutton = QtGui.QPushButton('Lock')
#         self.closebutton = QtGui.QPushButton('Close')
#         self.lockbutton.clicked.connect(self.Login2)
#         self.closebutton.clicked.connect(self.closenow)
#         widg = QtGui.QWidget()
#         splitter = QtGui.QSplitter(self)
#         self.exitOnClose = False
#         exit = QAction(QIcon(), "Exit", self)
#         self.connect(exit, SIGNAL("triggered()"), self.exitEvent)
#         self.trayIcon = QSystemTrayIcon(QIcon("lockicon.ico"), self)
#         menu = QMenu(self)
#         menu.addAction(exit)
#         self.trayIcon.setContextMenu(menu)
#         self.connect(self.trayIcon, \
#             SIGNAL("activated(QSystemTrayIcon::ActivationReason)"), \
#             self.trayIconActivated)
#         layout = QtGui.QHBoxLayout(widg)
#         layout.addWidget(self.lockbutton)
#         layout.addWidget(self.closebutton)

#         splitter.addWidget(widg)
#         self.setCentralWidget(splitter)

#         self.trayIcon.show()
#         self.trayIcon.showMessage("Lock Application", "Click To Open")
#     def exitEvent(self):
#         self.exitOnClose = True
#         self.close()
#     def trayIconActivated(self, reason):
#         if reason == QSystemTrayIcon.Context:
#             self.trayIcon.contextMenu().show()
#         elif reason == QSystemTrayIcon.Trigger:
#             self.show()
#             self.raise_()
#     def closeEvent(self, event):
#         if self.exitOnClose:
#             self.trayIcon.hide()
#             del self.trayIcon
#             event.accept()
#         else:
#             self.hide()
#             event.setAccepted(True)
#             event.ignore()
#     def Login2(self):
#        subprocess.Popen([sys.executable, "/python27/pyqt4lock1.py"])
#        sys.exit()
#     def closenow(self):
#         sys.exit()


# def Main():
app = QtGui.QApplication(sys.argv)
myapp = MoogliIve()
# myapp.setGeometry(app.desktop().screenGeometry())
myapp.show()
app.exec_()

    # if Test().exec_() == QtGui.QDialog.Accepted:
    #         myapp.hide()
    #         window = Window()
    #         window.show()
    #         sys.exit(app.exec_())

# if __name__ == '__main__':
#     Main()
