GTpo  (C++14 Topology library)
===========================

GTpo (Generic Topology) is the C++11 graph topology library used in QuickQanava2 graph visualization library. GTpo is available under the 
MIT licence.

GTpo is not a generic library in the Boost Graph Library sense, but rather an highly configurable graph modelling library designed for
ease of use an maximum performances over "genericity". GTpo heavily rely on C++11/C++17 functionnalities to hide template complexity from the user. 

GTpo root namespace 'gtpo' contains generic code, user not interested in using specific containers could use one of the following namespaces for accessing a 
"concrete GTpo library" directly:
    
* QuickQanava2: Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: ![QuickQanava2](https://github.com/cneben/QuickQanava)

![GTpo data model schema](https://github.com/cneben/QuickQanava/GTpo/blob/devel/doc/gtpo-datamodel.png)

GTpo is **highly** alpha.

+ Project homepage: https://github.com/cneben/QuickQanava/GTpo

# Installation

## Installation:

GTpo is header only, and can be used with either _qmake_ or _CMake_.

The recommended way of using GTpo is to statically integrate the library as a GIT submodule in your own project. GTpo could then be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# in your project main .pro qmake configuration file
include(./GTpo/src/gtpo.pri)
~~~~~~~~~~~~~

## Dependencies

* Googletest: https://github.com/google/googletest/ 

Data model
------------------

![GTpo data model schema](https://github.com/cneben/QuickQanava/GTpo/blob/devel/doc/gtpo-datamodel.png)

  Memory in GTpo is managed with std::shared_ptr and std::weak_ptr, using definition in graph types are prefixed with either *shared_* or *weak_* according
to the underlying concrete container.

Behaviours
-------------

  Behaviour in GTpo are the preferred way to observe changes in graph topology. Behaviours could be binded 

+ **Observation interface**: all behaviour interface expose 'inserted', 'removed' and 'modified' pure virtual methods that could be used to
 observe topological changes in graph and groups.

   GTpo actually support both virtual behaviours that could be modified dynamically and static behaviours with full template code that could be configured
by modifying graph definition:

+ **Static behaviours**: FIXME.
+ **Virtual behaviours**: FIXME.

 Behaviours could be disabled by calling gtpo::behaviour::disable() method, disabling all behaviours might be usefull before calling gtpo::Graph<>::clear() method or before serializing the graph in or out.
 

### Group topology

When a gtpo::GroupEdgeSetBehaviour is registered in graph (ie added in gtpo::GraphConfig::GroupBehaviours std::tuple), edges and adjacent edges of a group could be searched 
with gtpo::GenGroup<>::getEdges() and gtpo::GenGroup<>::getAdjacentEdges():

![](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-topo-group_adjacent_edges.png)


