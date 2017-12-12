TEMPLATE    =   subdirs
CONFIG      += ordered      # Well I know, but no choice...

include(./quickcontainers-common.pri)

quickcontainers.file            = src/quickcontainers-build-lib.pro

sample-containermodel.subdir    = samples/containermodel
sample-containermodel.depends   = quickcontainers

qcm-tests.subdir   = tests
qcm-tests.depends  = quickcontainers

#SUBDIRS     +=  quickcontainers
SUBDIRS     +=  sample-containermodel
SUBDIRS     +=  qcm-tests



