TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

quickqanava2.file    = src/quickqanava2.pro
quickqanava2.depends = 

test-navigable.subdir   = samples/navigable
test-navigable.depends  = quickqanava2

test-custom.subdir   = samples/custom
test-custom.depends  = quickqanava2

test-gml.subdir   = samples/gml
test-gml.depends  = quickqanava2

test-style.subdir   = samples/style
test-style.depends  = quickqanava2

test-topology.subdir   = samples/topology
test-topology.depends  = quickqanava2

SUBDIRS     +=  quickqanava2 test-navigable test-custom test-gml test-style test-topology
