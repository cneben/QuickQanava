GTpo  (C++14 Topology library)
===========================

GTpo is a C++14 directed graphs modelling library available under BSD license. GTpo is highly configurable at compile time, with no runtime cost for not configured features.


+ Project homepage: https://github.com/cneben/QuickQanava/GTpo    

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-datamodel.png)

GTpo is **highly** alpha.

* QuickQanava2: Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: https://github.com/cneben/QuickQanava

Installation
------------------

## Installation:

GTpo is header only, and can be used with either _qmake_ or _CMake_.

The recommended way of using GTpo is to statically integrate the library as a GIT submodule in your own project. GTpo could then be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# project main .pro qmake configuration file
include(./GTpo/src/gtpo.pri)
~~~~~~~~~~~~~

## Dependencies

* **googletest / googlemock** (only for building tests): https://github.com/google/googletest/ 

Data model
------------------

## Adjacency lists

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-datamodel.png)

  Memory in GTpo is managed with `std::shared_ptr` and `std::weak_ptr`, using/typedef definitions in graph types are prefixed with either *shared_* or *weak_* according
to the underlying concrete container.

## Graph static configuration



Behaviours
-------------

  Behaviour in GTpo are the preferred way to observe changes in graph topology. Behaviours could be either *static* or *dynamic*, static behaviours are defined at compile time by modifying `gtpo::config` definition, wheras dynamic behaviours could be added or removed at runtime (with a virtual cost).

+ **Static behaviours**: Static behaviour 

+ **Dynamic behaviours**: FIXME.

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-behaviours-class.png)


 Behaviours could be disabled by calling `gtpo::behaviour<>::disable()` method, disabling all behaviours might be usefull before calling `gtpo::Graph<>::clear()` method or before serializing the graph in or out.
 
 
## Group topology

Edges and adjacent edges of a group could be searched with `gtpo::group<>::get_edges()` and `gtpo::group<>::get_adjacent_edges()`. Adjacent edge set won't be initialized until the `gtpo::graph_group_adjacent_edges_behaviour` and `gtpo::group_adjacent_edges_behaviour` static behaviour has been configured in `gtpo::config`.

For example: edges = **{** *e4*, *e5* **}**  and adjacent_edges = **{** *e2*, *e3*, *e4*, *e5* **}** :

![](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-topo-group_adjacent_edges.png)

Minimum static behaviour configuration to enable group adjacent edge support is the following (enabled in gtpo::default_config): 

~~~~~~~~~~~~~{.cpp}
struct my_config : public gtpo::config<my_config>
{
    using graph_behaviours = std::tuple< gtpo::graph_group_adjacent_edges_behaviour<my_config> >; 
    using group_behaviours = std::tuple< gtpo::group_adjacent_edges_behaviour<my_config> >; 
};
~~~~~~~~~~~~~

