#!/usr/bin/env python

import dsx
from vgmn_noirq_mono import VgmnNoirqMono
from dsx import *

# Partie 1 : definition du TCG (Graphe des Taches et des Communications)

fifo0 = dsx.Mwmr('fifo0', 32, 4)

tcg = dsx.Tcg(
        dsx.Task('prod0', 'producer',
                 {'output':fifo0} ),
        dsx.Task('cons0', 'consumer',
                 {'input':fifo0} ),
        )

# Partie 2 : generation du code executable sur station de travail POSIX

tcg.generate(dsx.Posix())

#########################################################
# Section B : Hardware architecture
#
# The file containing the architecture definition
# must be included, and the path to the directory
# containing this file must be defined
#########################################################


archi = VgmnNoirqMono()

#########################################################
# Section C : Mapping
#
#########################################################

mapper = Mapper(archi, tcg)

# mapping the MWMR channel

mapper.map( "fifo0",
  buffer = "cram1",
  status = "cram1"
)

# mapping the "prod0" and "cons0" tasks

mapper.map("prod0",
   run   = "cpu0",
   stack = "cram0",
   desc  = "cram0"
)

mapper.map("cons0",
   run   = "cpu0",
   stack = "cram0",
   desc  = "cram0"
)

# mapping the software objects associated to a processor

mapper.map( 'cpu0',
  private = "cram0",
  shared  = "uram0")

# mapping the software objects used by the embedded OS

mapper.map(tcg,
  private = "cram1",
  shared  = "uram1",
  code    = "cram1",

  # These lines are for getting output messages:
  tty = "tty0",
  tty_no = 0)

######################################################
# Section D : Code generation
######################################################

# Embedded software linked with the Mutek/S OS

mapper.generate( MutekS() )

# The software application for a POSX workstation can still be generated

tcg.generate( Posix() )

