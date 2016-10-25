TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

quickqanava2.file    = src/quickqanava2.pro
quickqanava2.depends = 

test-navigable.subdir   = samples/navigable
test-navigable.depends  = quickqanava2

test-custom.subdir   = samples/custom
test-custom.depends  = quickqanava2

test-groups.subdir   = samples/groups
test-groups.depends  = quickqanava2

test-hedges.subdir   = samples/hedges
test-hedges.depends  = quickqanava2

test-style.subdir   = samples/style
test-style.depends  = quickqanava2

test-topology.subdir   = samples/topology
test-topology.depends  = quickqanava2

test-progress.subdir   = samples/progress
test-progress.depends  = quickqanava2

SUBDIRS +=  quickqanava2 test-navigable test-custom test-groups test-hedges test-style test-topology test-progress
#SUBDIRS +=  test-hedges
