TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

include(./quickqanava-common.pri)

quickqanava.file    = src/quickqanava-build-lib.pro
quickqanava.depends =

test-navigable.subdir   = samples/navigable
test-navigable.depends  = quickqanava

test-custom.subdir   = samples/custom
test-custom.depends  = quickqanava

test-groups.subdir   = samples/groups
test-groups.depends  = quickqanava

test-hedges.subdir   = samples/hedges
test-hedges.depends  = quickqanava

test-style.subdir   = samples/style
test-style.depends  = quickqanava

test-topology.subdir   = samples/topology
test-topology.depends  = quickqanava

test-progress.subdir   = samples/progress
test-progress.depends  = quickqanava

SUBDIRS +=  test-custom
SUBDIRS +=  quickqanava
SUBDIRS +=  test-navigable
SUBDIRS +=  test-groups
SUBDIRS +=  test-style
SUBDIRS +=  test-progress
SUBDIRS +=  test-hedges
SUBDIRS +=  test-topology
