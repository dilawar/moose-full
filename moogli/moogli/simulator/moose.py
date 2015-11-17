# http://neuromorpho.org/neuroMorpho/SomaFormat.html
# http://neuromorpho.org/neuroMorpho/myfaq.jsp


from __future__ import absolute_import
import moose
import moogli
from moogli.utils import *
import operator
import PyQt4
import math
import time
# def _handle_spinemesh(network, spinemesh, moose = moose):
#     group = moogli.Group(spinemesh.path)

#     moose_neuron        = moose.element(spinemesh.cell)
#     moogli_neuron       = network.get_neuron(moose_neuron.path)
#     compartment_paths   = mesh.elecComptList
#     voxel_start_indices = neuromesh.startVoxelInCompt
#     voxel_end_indices   = neuromesh.endVoxelInCompt
#     voxel_counts        = map( operator.sub
#                              , voxel_end_indices
#                              , voxel_start_indices
#                              )
#     voxel_id_template   = "{electrical_compartment_id}/{voxel_index}"
#     for (compartment_path, voxel_start_index, voxel_end_index) in
#         zip(compartment_paths, voxel_start_indices, voxel_end_indices):
#         moogli_compartment = neuron.get_compartment(compartment_path)
#         moogli_neuron.create_chemical_compartments( moogli_compartment
#                                                   , voxel_count
#                                                   , map( lambda voxel_index: voxel_id_template.format( voxel_index = voxel_index
#                                                                                                       ,
#                                                                                                       )
#                                                         , range( voxel_start_index
#                                                                , voxel_end_index
#                                                                )
#                                                         )
#                                                   , repeat()
#                                                   )

def colorstyle(visualizable):
    return (1.0, 0.0, 1.0, 1.0)

def _compartment_parameters(moose_compartment):
    distal_radius = moose_compartment.diameter / 2.0
    distal = [ moose_compartment.x * 10000000
             , moose_compartment.y * 10000000
             , moose_compartment.z * 10000000
             , distal_radius       * 10000000
             ]
    try:
        parent_compartment = moose_compartment.neighbors["raxial"][0][0]
        proximal_radius  = parent_compartment.diameter / 2.0
        proximal = [ moose_compartment.x0 * 10000000
                   , moose_compartment.y0 * 10000000
                   , moose_compartment.z0 * 10000000
                   , proximal_radius      * 10000000
                   ]
        center = [ (proximal[0] + distal[0]) / 2.0
                 , (proximal[1] + distal[1]) / 2.0
                 , (proximal[2] + distal[2]) / 2.0
                 , distal[3]
                 ]
    except IndexError:
        center   = distal
        proximal = None

    return (proximal, distal, center)

def read_neuromesh(network, neuromesh, moose = moose, track_parent = False):
    # visualizables = moogli.Visualizables(neuromesh.path)
    print("Creating " + neuromesh.path)
    moose_neuron        = moose.element(neuromesh.cell)
    moogli_neuron       = network.get_neuron(moose_neuron.path)
    compartments        = neuromesh.elecComptList
    voxel_start_indices = neuromesh.startVoxelInCompt
    voxel_end_indices   = neuromesh.endVoxelInCompt
    voxel_counts        = map( operator.sub
                             , voxel_end_indices
                             , voxel_start_indices
                             )
    voxel_id_template   = "{electrical_compartment_id}/{voxel_index}"
    for (compartment, voxel_start_index, voxel_end_index) in zip(compartments, voxel_start_indices, voxel_end_indices):
        compartment = compartment[0]
        compartment_path = compartment.path
        moogli_compartment = moogli_neuron.get_electrical_compartment(compartment_path)
        ids = [ voxel_id_template.format( voxel_index = voxel_index
                                        , electrical_compartment_id = compartment_path
                                        )
                for voxel_index in range(voxel_start_index, voxel_end_index)
              ]
        moogli_neuron.create_chemical_compartments( moogli_compartment
                                                  , ids
                                                  )
        # for chemical_compartment_id in ids:
        # visualizables.add_visualizables(moogli_compartment.get_chemical_compartment(chemical_compartment_id))
    return None
    # return visualizables

def read_spinemesh(network, spinemesh, moose = moose, track_parent = False):
    # visualizables = moogli.Visualizables(spinemesh.path)
    print("Creating " + spinemesh.path)
    # moose_neuron        = moose.element(spinemesh.cell)
    moose_compartment = spinemesh.elecComptList[0][0]
    neuron      = network.get_neuron(moose_compartment.parent.path)

    moogli_neuron       = network.get_neuron(moose_compartment.parent.path)
    compartments        = spinemesh.elecComptList
    voxel_start_indices = spinemesh.startVoxelInCompt
    voxel_end_indices   = spinemesh.endVoxelInCompt
    # voxel_counts        = map( operator.sub
    #                          , voxel_end_indices
    #                          , voxel_start_indices
    #                          )
    voxel_id_template   = "{electrical_compartment_id}/{voxel_index}"
    for (compartment, voxel_start_index, voxel_end_index) in zip(compartments, voxel_start_indices, voxel_end_indices):
        compartment = compartment[0]
        compartment_path = compartment.path
        moogli_compartment = moogli_neuron.get_electrical_compartment(compartment_path)
        ids = [ voxel_id_template.format( voxel_index = voxel_index
                                        , electrical_compartment_id = compartment_path
                                        )
                for voxel_index in range(voxel_start_index, voxel_end_index)
              ]
        moogli_neuron.create_chemical_compartments( moogli_compartment
                                                  , ids
                                                  )
        # for chemical_compartment_id in ids:
        # visualizables.add_visualizables(moogli_compartment.get_chemical_compartment(chemical_compartment_id))
    return None
    # return visualizables

def is_soma(moose_ecompt):
    return len(moose_ecompt.neighbors["raxial"]) == 0

def parent_compartment(moose_ecompt):
    return moose_ecompt.neighbors["raxial"][0][0]

def is_soma_child(moose_ecompt):
    return not is_soma(moose_ecompt) and is_soma(parent_compartment(moose_ecompt))

def extract_electrical_compartment(moose_ecompt, moose = moose):
    # print("Extracting => " + moose_ecompt.path)
    compartment = moogli.ElectricalCompartment(moose_ecompt.path)
    distal_radius = moose_ecompt.diameter / 2.0
    distal = [ moose_ecompt.x * 10000000
             , moose_ecompt.y * 10000000
             , moose_ecompt.z * 10000000
             , distal_radius  * 20000000
             ]
    if moose_ecompt.name.find("head") != -1 or moose_ecompt.name.find("shaft") != -1:
        distal[3] = distal[3] / 2

    try:
        parent_compartment = moose_ecompt.neighbors["raxial"][0][0]
        proximal_radius  = parent_compartment.diameter / 2.0
        proximal = [ moose_ecompt.x0 * 10000000
                   , moose_ecompt.y0 * 10000000
                   , moose_ecompt.z0 * 10000000
                   , proximal_radius * 20000000
                   ]
        if moose_ecompt.name.find("head") != -1 or moose_ecompt.name.find("shaft") != -1:
            proximal[3] = proximal[3] / 2

    except IndexError:
        proximal = distal

    if moose_ecompt.path.rfind("head") != -1 : proximal[3] = distal[3]
    if moose_ecompt.path.rfind("shaft") != -1: proximal[3] = distal[3]
    if is_soma_child(moose_ecompt):            proximal[3] = distal[3]

    compartment.proximal = proximal
    compartment.distal   = distal
    return compartment


def get_parent(moose_ecompt):
    try:
        return moose_ecompt.neighbors["raxial"][0][0]
    except IndexError:
        return None

def extract_neuron(moose_neuron, moose = moose):
    print("Extracting => " + moose_neuron.path)
    neuron_id = moose_neuron.path
    neuron = moogli.Neuron(neuron_id)
    moose_ecompts = moose.wildcardFind(neuron_id + "/#[ISA=CompartmentBase]")
    map( neuron.attach
       , map(lambda x : extract_electrical_compartment(x, moose)
            , moose_ecompts
            )
       )
    for moose_ecompt in moose_ecompts:
        moose_ecompt_parent = get_parent(moose_ecompt)
        if moose_ecompt_parent is None:
            neuron[moose_ecompt.path].parent
        else:
            neuron[moose_ecompt.path].parent = neuron[moose_ecompt_parent.path]
        if len(moose_ecompt.neighbors["raxial"]) == 0:
            neuron.proximal     = neuron[moose_ecompt.path].proximal
            neuron.distal       = neuron[moose_ecompt.path].distal
    return neuron


def extract_network(moose_network, moose = moose):
    print("Extracting => " + moose_network.path)
    network_id = moose_network.path
    network = moogli.Network(network_id)
    moose_neurons = list(set(map( lambda x : x.parent
                                ,  moose.wildcardFind(network_id + "/##[ISA=CompartmentBase]")
                                )
                            )
                        )
    map( network.attach
       , map( lambda x : extract_neuron(x, moose)
            , moose_neurons
            )
       )
    for neuromesh in moose.wildcardFind(network_id + "/##[ISA=NeuroMesh]"):
        print(neuromesh)
        handle_neuromesh(network, neuromesh, moose)
    for spinemesh in moose.wildcardFind(network_id + "/##[ISA=SpineMesh]"):
        print(spinemesh)
        handle_spinemesh(network, spinemesh, moose)
    return network


def create_view(network, view = CHEMICAL_VIEW, colorstyle = colorstyle):
    t1 = time.time()
    for neuron in network.neurons():
        neuron.add_representation()

        neuron.set_color(colorstyle(neuron), 1)
        neuron.hide(-1) #Hide all representations
        for ecompt in neuron.electrical_compartments():
            ecompt.add_representation()
            ecompt.set_color(colorstyle(ecompt), 1)
            # if ecompt.id.find("head") != -1     : ecompt.set_color((1.0, 0.0, 0.0, 0.0), 1)
            # if ecompt.proximal == ecompt.distal : ecompt.set_color((1.0, 1.0, 0.0, 0.2), 1)
            # else                                : ecompt.set_color((0.0, 1.0, 0.0, 1.0), 1)
            ecompt.hide(-1)
            for ccompt in ecompt.chemical_compartments():
                ccompt.add_representation()
                ccompt.set_color(colorstyle(ecompt), 0)
                ccompt.hide(-1)
                ccompt.show(0)

            if view == ELECTRICAL_VIEW or len(ecompt) == 0:
                ecompt.show(1)
            elif view == CHEMICAL_VIEW:
                ecompt.show(0)

        if view == NEURON_VIEW or len(neuron) == 0:
            neuron.show(1)  #Show only the 1st representation
        else:
            neuron.show(0)
    print("Time => " + str(time.time() - t1))


def handle_neuromesh(network, neuromesh, moose = moose):
    print("Handling => " + neuromesh.path)
    ccompts = []
    for index, moose_ecompt in enumerate(neuromesh.elecComptList):
        moose_ecompt = moose_ecompt[0]
        ecompt = network[moose_ecompt.parent.path][moose_ecompt.path]
        ids = map( str
                 , range( neuromesh.startVoxelInCompt[index]
                        , neuromesh.endVoxelInCompt[index]
                        )
                 )
        ecompt.subdivide(ids)
        ccompts.extend(map(ecompt.__getitem__, ids))
    return ccompts

def handle_spinemesh(network, spinemesh, moose = moose):
    print("Handling => " + spinemesh.path)
    ccompts = []
    for index, moose_ecompt in enumerate(spinemesh.elecComptList):
        moose_ecompt = moose_ecompt[0]
        ecompt = network[moose_ecompt.parent.path][moose_ecompt.path]
        ids = map( str
                 , range( spinemesh.startVoxelInCompt[index]
                        , spinemesh.endVoxelInCompt[index]
                        )
                 )
        ecompt.subdivide(ids)
        ccompts.extend(map(ecompt.__getitem__, ids))
    return ccompts

def read_neurons(network, path = "", moose = moose, track_parent = True, details = 0):
    moose_neurons = moose.wildcardFind(path + "/##[ISA=Neuron]")
    for moose_neuron in moose_neurons:
        print("Creating " + moose_neuron.path)
        neuron = moogli.Neuron(moose_neuron.path)
        # neuron_map[neuron_id] = neuron
        soma_center = None
        moose_compartments = moose.wildcardFind(moose_neuron.path + "/#[ISA=CompartmentBase]")
        for moose_compartment in moose_compartments:
            parent_direction = None
            compartment = moogli.ElectricalCompartment(moose_compartment.path)
            # compartment_map[moose_compartment.path] = compartment
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            if proximal is None : soma_center = distal
            elif not track_parent: proximal[3] = distal[3]
            compartment.add_geometry(distal, proximal)
            neuron.add_electrical_compartment(compartment)
            compartment.show_geometry(details, True)
        neuron.add_geometry(soma_center)
        network.add_neuron(neuron)
        neuron.show_geometry(0, True)


def extract_network_old(name = "", path = "", moose = moose, track_parent = False, details = 0):
    network = moogli.Network(name)
    read_neurons( network
                , path = path
                , moose = moose
                , track_parent = track_parent
                , details = details
                )
    neuromesh_visualizables = [ read_neuromesh(network, neuromesh, moose = moose, track_parent = track_parent)
                                for neuromesh in moose.wildcardFind(path + "/##[ISA=NeuroMesh]")
                              ]

    spinemesh_visualizables = [ read_spinemesh(network, spinemesh, moose = moose, track_parent = track_parent)
                                for spinemesh in moose.wildcardFind(path + "/##[ISA=SpineMesh]")
                              ]

    return network

    # (neuron_visualizables, compartment_visualizable) = _handle_neurons(network, neuron, moose) for neur
    # for neuron_id in neurons:
    #     handle_compartments




    # print(path + "/##[ISA=NeuroMesh]")

    neuron_ids = set( map( lambda compartment : "" if compartment.parent.path == "/" else compartment.parent.path
                         , compartments
                         )
                    )
    for neuron_id in neuron_ids:
        neuron = moogli.Neuron(neuron_id)
        # neuron_map[neuron_id] = neuron
        soma_center = None
        moose_compartments = moose.wildcardFind(neuron_id + "/#[ISA=CompartmentBase]")
        for moose_compartment in moose_compartments:
            parent_direction = None
            compartment = moogli.Compartment(moose_compartment.path)
            # compartment_map[moose_compartment.path] = compartment
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            if proximal is None :
                soma_center = distal
            else:
                parent_compartment = moose_compartment.neighbors["raxial"][0][0]
                (parent_proximal, parent_distal, parent_center) = _compartment_parameters(parent_compartment)
                if parent_proximal is None:
                    parent_direction = None
                else:
                    parent_direction = map(operator.sub, parent_distal, parent_proximal)
                    parent_height = math.sqrt(sum(map(lambda x : x * x, parent_direction)))
                    parent_direction = [dim/parent_height for dim in parent_direction][0:3]
            print(parent_direction)
            compartment.add_geometry(distal, proximal, parent_direction)
            neuron.add_compartment(compartment)
        neuron.add_geometry(soma_center)
        network.add_neuron(neuron)
        #neuron.show_geometry(0, True)

    meshes = moose.wildcardFind(path + "/##[ISA=NeuroMesh]")
    # print(path + "/##[ISA=NeuroMesh]")
    for mesh in meshes:
        for i in range(len(mesh.elecComptList)):
            soma = False
            average_direction = [0.0, 0.0, 0.0, 0.0]
            moose_compartment = mesh.elecComptList[i][0]
            neuron      = network.get_neuron(moose_compartment.parent.path)
            compartment = neuron.get_compartment(moose_compartment.path)
            voxel_count = mesh.endVoxelInCompt[i] - mesh.startVoxelInCompt[i]
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            # print(proximal, distal, center)
            if proximal is None:
                soma = True
                for child in moose_compartment.neighbors["axial"]:
                    (child_proximal, child_distal, child_center) = _compartment_parameters(child[0])
                    average_direction = map( operator.add
                                           , average_direction
                                           , map( operator.sub
                                                , child_distal
                                                , child_proximal
                                                )
                                           )
                    print(average_direction)
                total_height = average_direction
                total_height[3] = 0.0 # radius will be recomputed in the for loop below
                magnitude = math.sqrt(sum([dim * dim for dim in total_height]))
                total_height = [ 2.0 * distal[3] * dim / magnitude for dim in total_height ]
                proximal = previous_distal = map(operator.sub, distal, [dim / 2.0 for dim in total_height])
                for j in range(0, voxel_count):
                    phi1 = 1 - (j + 0.0) / voxel_count
                    phi2 = 1 - (j + 1.0) / voxel_count
                    r1 = distal[3] * math.sin( math.pi * phi1)
                    r2 = distal[3] * math.sin( math.pi * phi2)
                    voxel_proximal = previous_distal
                    temp = [ (j + 1) * dim / voxel_count
                             for dim in total_height
                           ]
                    previous_distal = voxel_distal  = map(operator.add, proximal, temp)
                    voxel_distal[3] = r2
                    voxel_proximal[3] = r2
                    # print(voxel_proximal, voxel_distal)
                    voxel = moogli.Voxel(moose_compartment.path + "/" + str(j + mesh.startVoxelInCompt[i]))
                    voxel_map[mesh.startVoxelInCompt[i] + j] = voxel
                    voxel.set_geometry(voxel_distal, voxel_proximal)
                    compartment.add_voxel(voxel)
                compartment.show_geometry(0, True)
            else:
                total_height    = map(operator.sub, distal, proximal)
                previous_distal = proximal
                for j in range(0, voxel_count):
                    voxel_proximal = previous_distal
                    temp = [ (j + 1) * dim / voxel_count
                             for dim in total_height
                           ]
                    previous_distal = voxel_distal  = map(operator.add, proximal, temp)
                    # print(voxel_proximal, voxel_distal)
                    voxel = moogli.Voxel(moose_compartment.path + "/" + str(j + mesh.startVoxelInCompt[i]))
                    voxel.set_geometry(voxel_distal, voxel_proximal)
                    voxel_map[mesh.startVoxelInCompt[i] + j] = voxel
                    compartment.add_voxel(voxel)
            compartment.show_geometry(0, True)

    # SPINES

    meshes = moose.wildcardFind(path + "/##[ISA=SpineMesh]")
    spine_map = {}
    for mesh in meshes:
        for i in range(len(mesh.elecComptList)):
            soma = False
            average_direction = [0.0, 0.0, 0.0, 0.0]
            moose_compartment = mesh.elecComptList[i][0]
            neuron      = network.get_neuron(moose_compartment.parent.path)
            compartment = neuron.get_compartment(moose_compartment.path)
            voxel_count = mesh.endVoxelInCompt[i] - mesh.startVoxelInCompt[i]
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            # print(proximal, distal, center)
            if proximal is None:
                soma = True
                for child in moose_compartment.neighbors["axial"]:
                    (child_proximal, child_distal, child_center) = _compartment_parameters(child[0])
                    average_direction = map( operator.add
                                           , average_direction
                                           , map( operator.sub
                                                , child_distal
                                                , child_proximal
                                                )
                                           )
                    print(average_direction)
                total_height = average_direction
                total_height[3] = 0.0 # radius will be recomputed in the for loop below
                magnitude = math.sqrt(sum([dim * dim for dim in total_height]))
                total_height = [ 2.0 * distal[3] * dim / magnitude for dim in total_height ]
                proximal = previous_distal = map(operator.sub, distal, [dim / 2.0 for dim in total_height])
                for j in range(0, voxel_count):
                    phi1 = 1 - (j + 0.0) / voxel_count
                    phi2 = 1 - (j + 1.0) / voxel_count
                    r1 = distal[3] * math.sin( math.pi * phi1)
                    r2 = distal[3] * math.sin( math.pi * phi2)
                    voxel_proximal = previous_distal
                    temp = [ (j + 1) * dim / voxel_count
                             for dim in total_height
                           ]
                    previous_distal = voxel_distal  = map(operator.add, proximal, temp)
                    voxel_distal[3] = r2
                    voxel_proximal[3] = r2
                    # print(voxel_proximal, voxel_distal)
                    voxel = moogli.Voxel(moose_compartment.path + "/" + str(j + mesh.startVoxelInCompt[i]))
                    spine_map[mesh.startVoxelInCompt[i] + j] = voxel
                    voxel.set_geometry(voxel_distal, voxel_proximal)
                    compartment.add_voxel(voxel)
                compartment.show_geometry(0, True)
            else:
                total_height    = map(operator.sub, distal, proximal)
                previous_distal = proximal
                for j in range(0, voxel_count):
                    voxel_proximal = previous_distal
                    temp = [ (j + 1) * dim / voxel_count
                             for dim in total_height
                           ]
                    previous_distal = voxel_distal  = map(operator.add, proximal, temp)
                    # print(voxel_proximal, voxel_distal)
                    voxel = moogli.Voxel(moose_compartment.path + "/" + str(j + mesh.startVoxelInCompt[i]))
                    voxel.set_geometry(voxel_distal, voxel_proximal)
                    spine_map[mesh.startVoxelInCompt[i] + j] = voxel
                    compartment.add_voxel(voxel)
            compartment.show_geometry(0, True)



    return ( network
           , neuron_map.values()
           , [compartment_map[key.path] for key in moose.wildcardFind(path + "/##[ISA=CompartmentBase]")]
           , [voxel_map[key] for key in sorted(voxel_map.keys())]
           , [spine_map[key] for key in sorted(spine_map.keys())]
           )

def create_viewer( moose_network
                 , moose       = moose
                 , view        = CHEMICAL_VIEW
                 , colorstyle  = colorstyle
                 , prelude     = lambda viewer : viewer
                 , interlude   = lambda viewer : viewer
                 , postlude    = lambda viewer : viewer
                 ):
    network = extract_network(moose_network, moose = moose)
    create_view(network, view, colorstyle)
    viewer  = moogli.NetworkViewer( network
                                  , prelude
                                  , interlude
                                  , postlude
                                  )
    return viewer

def view(path="", callbacks = [(id, id, id)]):
    application = PyQt4.QtGui.QApplication([])
    viewers = []
    for (prelude, interlude, postlude) in callbacks:
        network = read_network(path = path, track_parent = False)
        viewer = moogli.NetworkViewer( network
                                     , prelude
                                     , interlude
                                     , postlude
                                     )
        viewer.neurons["neurons"] = []
        viewer.compartments["compartments"] = []
        viewer.voxels["voxels"] = []
        viewer.spines["spines"] = []
        viewer.show()
        viewers.append(viewer)
# def delete_gl_widget():
#     global vm_visualizer
#     global rm_visualizer
#     vm_visualizer.setParent(None)
#     del vm_visualizer
#     rm_visualizer.setParent(None)
#     del rm_visualizer

# QApplication.instance().aboutToQuit.connect( delete_gl_widget )

    # print viewers
    # map(lambda viewer : viewer.show(), viewers)
    return application.exec_()
