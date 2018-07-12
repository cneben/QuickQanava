GTpo  (C++14 Topology library)
===========================

:heavy_exclamation_mark: **This page is WIP, GTpo is used internally in QuickQanava and is not ready for general use** :heavy_exclamation_mark:

GTpo is a C++14 directed graphs modelling library available under BSD license. GTpo is highly configurable at compile time, with no runtime cost for unconfigured features.


+ Project homepage: https://github.com/cneben/QuickQanava/tree/master/GTpo

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-datamodel.png)

GTpo is **highly** alpha.

* QuickQanava2: Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: https://github.com/cneben/QuickQanava

Installation
------------------

GTpo is header only, and can be used with either _qmake_ or _CMake_.

The recommended way of using GTpo is to statically integrate the library as a GIT submodule in your own project. GTpo could then be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# project main .pro qmake configuration file
include(./GTpo/src/gtpo.pri)
~~~~~~~~~~~~~

### Dependencies

* **googletest / googlemock** (only for building tests): https://github.com/google/googletest/ 

Data model
------------------

### Adjacency lists

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-datamodel.png)

  Memory in GTpo is managed with `std::shared_ptr` and `std::weak_ptr`, using/typedef definitions in graph types are prefixed with either *shared_* or *weak_* according
to the underlying concrete container.

Static graph configuration
-------------

### Behaviours

  Behaviour in GTpo are the preferred way to observe changes in graph topology. Behaviours could be either *static* or *dynamic*, static behaviours are defined at compile time by modifying `gtpo::config` definition, wheras dynamic behaviours could be added or removed at runtime (with a virtual cost).

+ **Static behaviours**: Static behaviour 

+ **Dynamic behaviours**: FIXME.

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/develop/GTpo/doc/gtpo-behaviours-class.png)


 Behaviours could be disabled by calling `gtpo::behaviour<>::disable()` method, disabling all behaviours might be usefull before calling `gtpo::Graph<>::clear()` method or before serializing the graph in or out.
 
 
Topology
-------------

### Iterators (`gtpo/algorithm.h`)

Header: [<gtpo/algorithm.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/algorithm.h)

```cpp
#include <GTpo>

gtpo::graph<> g;
g.insert_node();
for (const auto n : g) { /* ... */ }
//or
for (auto n : g) { /* ... */ }
```

### Traversal algorithms (`gtpo/algorithm.h`)

**Note:** _all_ recursive functions are postfixed with *'_rec'*, there is no overflow protection for large deep graphs. Alternative non recursive stack based implementation are available for most functions (no postfix).

- `gtpo::depth_rec()`: Return graph depth.
- `gtpo::linearize_dfs_rec()`: Return a vector of graph nodes linearized in DFS order.
- `gtpo::levelize_dfs_rec()`: Return a vector of graph nodes ordered level by level in DFS order. Returned vector size equal graph depth, with a vector of ordered nodes for each level.

These traversal functions are specialized for trees:

- `gtpo::tree_depth_rec()`: Return tree depth.
- `gtpo::linearize_tree_dfs()` / `gtpo::linearize_tree_dfs_rec()`: Return a vector of tree nodes linearized in DFS order.
- `gtpo::levelize_tree_dfs_rec()`: Return a vector of tree nodes ordered level by level in DFS order. Returned vector size equal tree depth, with a vector of ordered nodes for each level.

Calling a *'_tree'* method is often faster, but using it against a non-tree graph might lead to overflow or infinite recursion. Following methods could be used to check specific graph topological properties:

- `gtp::is_dag_rec()` / `gtp::is_dag()`  (O(N)): Return true if graph is a directed acyclic graph.
- `gtp::is_tree_rec()` / `gtp::is_tree()` (O(N)): Return true if graph is a tree (or a forest).


### Generators

Header: [<gtpo/generator.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/generator.h)

Graph generation functions interface is similar to [NetworkX](https://networkx.github.io/) generators.

- `gtpo::complete_graph()`: Return a fully connected graph.

- Random GNP graphs:
  - `gtpo::gnp_random_graph()`:
  - `gtpo::erdos_renyi_graph()`: .
  
- `gtpo::complete_tree()`:    O(n<sup>2</sup>).


### Group topology

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

