TEMPLATE    =   subdirs
CONFIG      +=  ordered

test-resizer.subdir   = samples/resizer
test-navigable.subdir   = samples/navigable
test-nodes.subdir   = samples/nodes
test-connector.subdir   = samples/connector
test-groups.subdir   = samples/groups
test-style.subdir   = samples/style

#test-topology.subdir   = samples/topology
#test-topology.depends  = quickqanava
#test-40k.subdir     = samples/40k
#test-40k.depends    = quickqanava

#SUBDIRS +=  test-resizer
#SUBDIRS +=  test-navigable
#SUBDIRS +=  test-nodes
SUBDIRS +=  test-connector
#SUBDIRS +=  test-groups
#SUBDIRS +=  test-style

#SUBDIRS +=  test-topology
#SUBDIRS +=  test-40k
