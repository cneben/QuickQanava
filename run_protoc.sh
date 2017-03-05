#!/bin/sh
set -x # Echo on
protoc  -I./GTpo/src --cpp_out=./GTpo/src/ ./GTpo/src/gtpo.proto
protoc  -I./GTpo/src -I./src --cpp_out=./src ./src/quickqanava.proto
protoc  -I./GTpo/src -I./src -I./src -I./samples/topology --cpp_out=./samples/topology/ ./samples/topology/topology.proto
