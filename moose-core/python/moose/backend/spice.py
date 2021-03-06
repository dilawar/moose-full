#!/usr/bin/env python

"""backend_spice.py: 

    This backend write spice files.

Last modified: Wed May 14, 2014  04:48PM

"""
    
__author__           = "Dilawar Singh"
__copyright__        = "Copyright 2013, NCBS Bangalore"
__credits__          = ["NCBS Bangalore", "Bhalla Lab"]
__license__          = "GPL"
__version__          = "1.0.0"
__maintainer__       = "Dilawar Singh"
__email__            = "dilawars@ncbs.res.in"
__status__           = "Development"

from .backend  import Backend 
import print_utils as debug

import os 
import sys
import time
import datetime

thisDir = os.path.dirname( __file__ )
sys.path.append(os.path.join(thisDir, '..'))

import _moose
from methods_utils import idPathToObjPath

class Spice(Backend):
    """Converts moose to spice-netlist"""

    def __init__(self, *args):
        super(Spice, self).__init__()
        self.args = args
        self.spiceText = [ self.compartmentModel( ) ]
        self.outfile = None

    def moosePathToSpiceNode(self, path):
        """Make moose-path sane for spice """
        # 
        newPath = ''
        for p in path:
            if p in "[]/_+-'()":
                newPath += ''
            else:
                newPath += p
        return newPath

    def toSpiceNode(self, moosePath, type):
        """Return spice node name for a given moose-path """
        goodTypes = ["in1", "out1", "inject", 'x']
        if type not in goodTypes:
            debug.dump("ERROR"
                    , "Bad node type: Expecting {}".format(goodTypes)
                    )
            raise TypeError("Expecting {}, got {}".format(goodTypes, type))

        moosePath = idPathToObjPath( moosePath )
        name = self.moosePathToSpiceNode( moosePath )
        return 'n{}{}'.format(name, type)

    def compartmentModel(self):
        """ A default compartment model """
        model = []
        st = time.time()
        stamp = datetime.datetime.fromtimestamp(st).strftime('%Y-%m-%d-%H%M')
        model.append("* AUTOGENERATED MOOSE-MODEL IN SPICE.")
        model.append("* GENERATED ON: {}".format( stamp ))
        model.append(".SUBCKT {name} {ports} {params}".format(
                    ports = "in1 in2 inject out1 out2" 
                    , name = "moosecompartment"
                    , params = "Ra=10k Rm=10G Cm=10pF Em=-60m"
                    )
                )
        model.append("Ra1 in1 inject {Ra/2}")
        model.append("Ra2 inject out1 {Ra/2}")
        model.append("Cm inject in2 {Cm}")
        model.append("Rm inject rmcmnode {Rm}")
        model.append("VEm rmcmnode out2 dc {Em}")
        model.append(".ENDS \n\n")
        return "\n".join(model)


    def spiceLineForDevice(self, compartment):
        """Generates spice lines for a given compartment.
        axials: Axial compartments
        raxials: Raxial compartments
        """
        cPath = compartment.path
        spiceLine = ''
        compName = self.moosePathToSpiceNode( cPath )
        # Add a line for each compartments. To make connection, we add a 0V
        # voltage source. It does nothing to circuit but one can measure current
        # through it.
        spiceLine += 'X{name} {in1} GND {inject} {out1} GND'.format(
                name = compName
                , in1 = self.toSpiceNode(cPath, 'in1')
                , inject = self.toSpiceNode(cPath, 'inject')
                , out1 = self.toSpiceNode(cPath, 'out1')
                )
        spiceLine += ' moosecompartment '
        spiceLine += 'Ra={Ra} Rm={Rm} Cm={Cm} Em={Em}'.format(
                Ra = compartment.Ra
                , Rm = compartment.Rm
                , Cm = compartment.Cm
                , Em = compartment.Em
                )
        self.spiceText.append( spiceLine )

    def spiceLineForConnections(self, compartment):
        """Add a connection between two compartments
        """

        def spiceLine( src, tgt):
            # Write a line for connections.
            spiceLine = 'V{name} {src} {tgt} dc 0'.format(
                    name = self.moosePathToSpiceNode( src+tgt )
                    , src = self.toSpiceNode( src, 'out1' )
                    , tgt = self.toSpiceNode( tgt, 'in1' )
                    )
            return spiceLine

        compPath = compartment.path 
        raxials = compartment.neighbors['raxial']
        axials = compartment.neighbors['axial']

        raxialsPath = [ idPathToObjPath( p.path ) for p in raxials ]
        axialsPath = [ idPathToObjPath( p.path ) for p in axials ]

        for rpath in raxialsPath:
            if (compPath, rpath) not in self.connections:
                self.connections.add( (compPath, rpath) )
                spiceLine = spiceLine( compPath, rpath)
                self.spiceText.append( spiceLine )
            else:               # This connection is already added.
                pass

        for apath in axialsPath:
            if (apath, compPath) not in self.connections:
                self.connections.add( (apath, compPath) )
                spiceLine =  spiceLine( apath, compPath )
                self.spiceText.append( spiceLine )
            else:
                pass
            
    def spiceLineForPulseGen(self, pulseGen):
        """Write spice-line for pulse """
        pulsePath = idPathToObjPath( pulseGen.path )
        pulseName = self.moosePathToSpiceNode( pulsePath )

        td1 = pulseGen.delay[0]
        td2 = pulseGen.delay[1]
        width = pulseGen.width[0]
        level1 = 0.0
        level2 = pulseGen.level[0]
    
        targets = pulseGen.neighbors['output']
        if not targets:
            debug.dump("WARN"
                    , "It seems that pulse `%s` is not connected" % pulsePath
                    )
        for i, t in enumerate(targets):
            spiceLine = "* Fist node is where current enters. A voltage \n"
            spiceLine += "* source is added in series. Useful for reading current\n"
            vtarget = self.toSpiceNode('%s%s'%(pulseName, i), 'x')
            target = self.toSpiceNode( t.path, 'inject' )
            spiceLine += 'I{id}{name} GND {vtarget} dc 0 PULSE'.format(
                    id = i
                    , name = pulseName 
                    , vtarget = vtarget
                    )
            spiceLine += '({level1} {level2} {TD} {TR} {TF} {PW} {PER})'.format(
                    level1 = level1
                    , level2 = level2 
                    , TD = td1
                    , TR = 0.0
                    , TF = 0.0
                    , PW = width 
                    , PER = td1 + td2 + width
                    )
            self.spiceText.append(spiceLine)
            # A a voltage source in series
            self.spiceText.append(
                    "V{id}{name} {vtarget} {target} dc 0".format(
                        id = i
                        , name = pulseName
                        , vtarget = vtarget
                        , target = target
                        )
                    )

    def addControlLines(self):
        """Add .CONTROL lines to spice netlist """
        self.spiceText.append("\n\n** Simulation etc. ")
        self.spiceText.append(
            "\n.CONTROL"
            "\nset hcopydevtype=postscript"
            "\nset hcopypscolor=true"
            "\nset color0 = white      ;background"
            "\nset color1 = black      ;text and grid"
            "\nset color2 = rgb:f/0/0  ;vector0"
            "\nset color3 = rgb:0/f/0  ;vector1"
            "\nset color3 = rgb:0/0/f  ;vector2"
            "\nop"
            )

        # Here we get the time of simulation and total steps done by moose.
        assert self.clock, "Main clock is not foind"
        simTime = self.clock.runTime 
        dt = self.clock.dt
        self.spiceText.append("TRAN {0} {1}".format(dt, simTime))

        plotLine = "HARDCOPY {}.ps ".format(self.outputFile)
        for t in self.tables:
            targets = t.neighbors['requestOut']
            for tgt in targets:
                if tgt.className == 'PulseGen':
                    debug.dump("INFO"
                            , "Stimulus pulse are not plotted by defualt."
                            )
                    continue
                plotLine += "V({}) ".format(self.toSpiceNode(tgt.path,'out1'))
        self.spiceText.append(plotLine)
        self.spiceText.append(".ENDC")
        self.spiceText.append(".END")

    def buildModel(self):
        """Build data-stucture to write spice """
        self.populateStoreHouse()
        for c in self.compartments:
            self.spiceLineForDevice( c )
            self.spiceLineForConnections( c )

        self.spiceText.append("\n\n** Current pulses from moose ")
        for p in self.pulseGens:
            self.spiceLineForPulseGen(p)

        self.addControlLines( )

    def writeSpice(self, **kwargs):
        ''' Turn moose into  spice '''

        self.outputFile = kwargs.get('output', None)
        self.buildModel()
        spiceText = "\n".join( self.spiceText )

        if self.outputFile is not None:
            debug.dump("BACKEND"
                    , "Writing spice netlist to {}".format(self.outputFile)
                    )
            with open(self.outputFile, "w") as spiceFile:
                spiceFile.write( spiceText )
        else:
            return spiceText

def toSpiceNetlist(**kwargs):
    """Write an equivalent spice file"""
    sp = Spice( )
    sp.writeSpice( **kwargs )

def main():
    _moose.Neutral('/cable')
    c1 = _moose.Compartment('/cable/a')
    c2 = _moose.Compartment('/cable/b')
    c1.connect('raxial', c2, 'axial')
    p = _moose.PulseGen('/pulse1')
    p.delay[0] = 0.01
    p.level[0] = 1e-9
    p.width[0] = 0.10
    p.delay[1] = 0.08
    p.connect('output', c1, 'injectMsg')
    s = Spice()
    print(s.writeSpice( output='test_spice.spice' ))

if __name__ == '__main__':
    main()
