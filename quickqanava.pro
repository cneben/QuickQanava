TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

<<<<<<< HEAD
include(./quickqanava-common.pri)

=======
>>>>>>> dev
quickqanava.file    = src/quickqanava-build-lib.pro

test-navigable.subdir   = samples/navigable
test-navigable.depends  = quickqanava

<<<<<<< HEAD
test-custom.subdir   = samples/custom
test-custom.depends  = quickqanava
=======
test-nodes.subdir   = samples/nodes
test-nodes.depends  = quickqanava

test-connector.subdir   = samples/connector
test-connector.depends  = quickqanava
>>>>>>> dev

test-groups.subdir   = samples/groups
test-groups.depends  = quickqanava

<<<<<<< HEAD
test-hedges.subdir   = samples/hedges
test-hedges.depends  = quickqanava

=======
>>>>>>> dev
test-style.subdir   = samples/style
test-style.depends  = quickqanava

test-topology.subdir   = samples/topology
test-topology.depends  = quickqanava

<<<<<<< HEAD
test-progress.subdir   = samples/progress
test-progress.depends  = quickqanava

test-40k.subdir     = samples/40k
test-40k.depends    = quickqanava

SUBDIRS +=  quickqanava
#SUBDIRS +=  test-40k
SUBDIRS +=  test-custom
SUBDIRS +=  test-navigable
SUBDIRS +=  test-groups
SUBDIRS +=  test-hedges
SUBDIRS +=  test-style
SUBDIRS +=  test-progress
SUBDIRS +=  test-topology
=======
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
>>>>>>> dev
