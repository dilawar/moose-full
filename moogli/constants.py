import os
import pkg_resources

DEFAULT = "DEFAULT"
_ON_FOCUS_COLOR      = "white"
_OFF_FOCUS_COLOR     = "black"

_RELATIVE_ICON_PATH      = os.path.join("data", "icons")
_ON_FOCUS_ICON_PATH      = os.path.join(_RELATIVE_ICON_PATH, _ON_FOCUS_COLOR)
_OFF_FOCUS_ICON_PATH     = os.path.join(_RELATIVE_ICON_PATH, _OFF_FOCUS_COLOR)


import moogli
def _on_focus_icon_path(icon):
    return pkg_resources.resource_filename(moogli.__name__
                                          , os.path.join( _ON_FOCUS_ICON_PATH
                                                        , icon
                                                        )
                                          )

def _off_focus_icon_path(icon):
    return pkg_resources.resource_filename( moogli.__name__
                                          , os.path.join( _OFF_FOCUS_ICON_PATH
                                                        , icon
                                                        )
                                          )

NAVIGATE_UP_ON_FOCUS_ICON_PATH                  = _on_focus_icon_path("chevron-up.svg")
NAVIGATE_UP_OFF_FOCUS_ICON_PATH                 = _off_focus_icon_path("chevron-up.svg")

NAVIGATE_DOWN_ON_FOCUS_ICON_PATH                = _on_focus_icon_path("chevron-down.svg")
NAVIGATE_DOWN_OFF_FOCUS_ICON_PATH               = _off_focus_icon_path("chevron-down.svg")

NAVIGATE_RIGHT_ON_FOCUS_ICON_PATH               = _on_focus_icon_path("chevron-right.svg")
NAVIGATE_RIGHT_OFF_FOCUS_ICON_PATH              = _off_focus_icon_path("chevron-right.svg")

NAVIGATE_LEFT_ON_FOCUS_ICON_PATH                = _on_focus_icon_path("chevron-left.svg")
NAVIGATE_LEFT_OFF_FOCUS_ICON_PATH               = _off_focus_icon_path("chevron-left.svg")

NAVIGATE_HOME_ON_FOCUS_ICON_PATH                = _on_focus_icon_path("home.svg")
NAVIGATE_HOME_OFF_FOCUS_ICON_PATH               = _off_focus_icon_path("home.svg")

ROTATE_CLOCKWISE_ON_FOCUS_ICON_PATH             = _on_focus_icon_path("repeat.svg")
ROTATE_CLOCKWISE_OFF_FOCUS_ICON_PATH            = _off_focus_icon_path("repeat.svg")

ROTATE_ANTICLOCKWISE_ON_FOCUS_ICON_PATH         = _on_focus_icon_path("undo.svg")
ROTATE_ANTICLOCKWISE_OFF_FOCUS_ICON_PATH        = _off_focus_icon_path("undo.svg")

ZOOM_IN_ON_FOCUS_ICON_PATH                      = _on_focus_icon_path("search-plus.svg")
ZOOM_IN_OFF_FOCUS_ICON_PATH                     = _off_focus_icon_path("search-plus.svg")

ZOOM_OUT_ON_FOCUS_ICON_PATH                     = _on_focus_icon_path("search-minus.svg")
ZOOM_OUT_OFF_FOCUS_ICON_PATH                    = _off_focus_icon_path("search-minus.svg")

IMPORT_DATA_ON_FOCUS_ICON_PATH                  = _on_focus_icon_path("database.svg")
IMPORT_DATA_OFF_FOCUS_ICON_PATH                 = _off_focus_icon_path("database.svg")

SET_COLORSCHEME_ON_FOCUS_ICON_PATH              = _on_focus_icon_path("eyedropper.svg")
SET_COLORSCHEME_OFF_FOCUS_ICON_PATH             = _off_focus_icon_path("eyedropper.svg")

TAKE_SNAPSHOT_ON_FOCUS_ICON_PATH                = _on_focus_icon_path("camera.svg")
TAKE_SNAPSHOT_OFF_FOCUS_ICON_PATH               = _off_focus_icon_path("camera.svg")

RECORD_MOVIE_ON_FOCUS_ICON_PATH                 = _on_focus_icon_path("movie-camera.svg")
RECORD_MOVIE_OFF_FOCUS_ICON_PATH                = _off_focus_icon_path("movie-camera.svg")

SETTINGS_PANEL_ON_FOCUS_ICON_PATH               = _on_focus_icon_path("cogs.svg")
SETTINGS_PANEL_OFF_FOCUS_ICON_PATH              = _off_focus_icon_path("cogs.svg")

HELP_PANEL_ON_FOCUS_ICON_PATH                   = _on_focus_icon_path("question.svg")
HELP_PANEL_OFF_FOCUS_ICON_PATH                  = _off_focus_icon_path("question.svg")

SHOW_CONTOLS_ON_FOCUS_ICON_PATH                 = _on_focus_icon_path("toggle-on.svg")
SHOW_CONTOLS_OFF_FOCUS_ICON_PATH                = _off_focus_icon_path("toggle-on.svg")

HIDE_CONTROLS_ON_FOCUS_ICON_PATH                = _on_focus_icon_path("toggle-off.svg")
HIDE_CONTROLS_OFF_FOCUS_ICON_PATH               = _off_focus_icon_path("toggle-off.svg")

PLAY_SIMULATION_ON_FOCUS_ICON_PATH              = _on_focus_icon_path("play.svg")
PLAY_SIMULATION_OFF_FOCUS_ICON_PATH             = _off_focus_icon_path("play.svg")

PAUSE_SIMULATION_ON_FOCUS_ICON_PATH             = _on_focus_icon_path("pause.svg")
PAUSE_SIMULATION_OFF_FOCUS_ICON_PATH            = _off_focus_icon_path("pause.svg")

STOP_SIMULATION_ON_FOCUS_ICON_PATH              = _on_focus_icon_path("stop.svg")
STOP_SIMULATION_OFF_FOCUS_ICON_PATH             = _off_focus_icon_path("stop.svg")

FAST_FORWARD_SIMULATION_ON_FOCUS_ICON_PATH      = _on_focus_icon_path("fast-forward.svg")
FAST_FORWARD_SIMULATION_OFF_FOCUS_ICON_PATH     = _off_focus_icon_path("fast-forward.svg")

FAST_BACKWARD_SIMULATION_ON_FOCUS_ICON_PATH     = _on_focus_icon_path("fast-backward.svg")
FAST_BACKWARD_SIMULATION_OFF_FOCUS_ICON_PATH    = _off_focus_icon_path("fast-backward.svg")

STEP_FORWARD_SIMULATION_ON_FOCUS_ICON_PATH      = _on_focus_icon_path("step-forward.svg")
STEP_FORWARD_SIMULATION_OFF_FOCUS_ICON_PATH     = _off_focus_icon_path("step-forward.svg")

STEP_BACKWARD_SIMULATION_ON_FOCUS_ICON_PATH     = _on_focus_icon_path("step-backward.svg")
STEP_BACKWARD_SIMULATION_OFF_FOCUS_ICON_PATH    = _off_focus_icon_path("step-backward.svg")

MOOSE_SPINEMESH_WILDCARD    = "/##[ISA=SpineMesh]"
MOOSE_NEUROMESH_WILDCARD    = "/##[ISA=NeuroMesh]"
MOOSE_NEURON_WILDCARD       = "/##[ISA=Neuron]"
MOOSE_COMPARTMENT_WILDCARD  = "/#[ISA=CompartmentBase]"


ELECTRICAL_VIEW = "ELECTRICAL_VIEW"
CHEMICAL_VIEW   = "CHEMICAL_VIEW"
NEURON_VIEW     = "NEURON_VIEW"
