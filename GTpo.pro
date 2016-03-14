TEMPLATE    =   subdirs
CONFIG      +=  ordered      # Well I know, but no choice...

gtpo.file    = src/gtpo.pro

sample-basic.subdir   = samples/basic
sample-basic.depends  = gtpo

sample-behaviour.subdir   = samples/behaviour
sample-behaviour.depends  = gtpo

tests.subdir     = tests
tests.depends   = gtpo

SUBDIRS     +=  gtpo sample-basic sample-behaviour tests
