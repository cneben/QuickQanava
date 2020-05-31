TEMPLATE    =   subdirs
CONFIG      +=  ordered

gtpo.file    = src/gtpo.pro

sample-basic.subdir   = samples/basic
sample-basic.depends  = gtpo

sample-behaviour.subdir   = samples/behaviour
sample-behaviour.depends  = gtpo

sample-serializer.subdir   = samples/serializer
sample-serializer.depends  = gtpo

tests.subdir     = tests
tests.depends   = gtpo

benchmarks.subdir   = benchmarks
benchmarks.depends  = gtpo

#SUBDIRS +=  gtpo
SUBDIRS +=  tests
#SUBDIRS +=  sample-basic
#SUBDIRS +=  sample-behaviour
#SUBDIRS +=  sample-serializer
#SUBDIRS +=  benchmarks
