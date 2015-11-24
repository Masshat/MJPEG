#!/usr/bin/env python

import dsx

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
