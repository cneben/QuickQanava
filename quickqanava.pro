TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

quickqanava.file    = src/quickqanava-build-lib.pro

test-navigable.subdir   = samples/navigable
test-navigable.depends  = quickqanava

test-nodes.subdir   = samples/nodes
test-nodes.depends  = quickqanava

test-connector.subdir   = samples/connector
test-connector.depends  = quickqanava

test-groups.subdir   = samples/groups
test-groups.depends  = quickqanava

test-style.subdir   = samples/style
test-style.depends  = quickqanava

test-topology.subdir   = samples/topology
test-topology.depends  = quickqanava

test-40k.subdir     = samples/40k
test-40k.depends    = quickqanava

#SUBDIRS +=  quickqanava
#SUBDIRS +=  test-navigable
SUBDIRS +=  test-nodes
SUBDIRS +=  test-connector
SUBDIRS +=  test-groups
SUBDIRS +=  test-style
#SUBDIRS +=  test-topology
#SUBDIRS +=  test-40k
