GTpo  (C++14 Topology library)
===========================

:heavy_exclamation_mark: **This page is WIP, GTpo is used internally in QuickQanava and is not ready for general use** :heavy_exclamation_mark:

GTpo is a C++14 directed graphs modelling library available under BSD license. GTpo is highly configurable at compile time, with no runtime cost for unconfigured features.


+ Project homepage: https://github.com/cneben/QuickQanava/tree/master/GTpo

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/gtpo-datamodel.png)

GTpo is **highly** alpha.

- Example use: QuickQanava - Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: https://github.com/cneben/QuickQanava

**ROADMAP**:
  - **v0.1.0:**
    - [X] Push test coverage to 100%.
    - [ ] Clean the copy and move semantic requirements on topology primitives (and document it).
    - [ ] Clean operator= and operator== semantic requirements on topology primitives (and document it).
    - [X] Write complete documentation for static graph configuration.
    - [ ] Clean management of groups (with regular nodes and specific utility edges...).    
    - [ ] Add complete support for static properties.
    - [ ] Add generic utilities to ease JSON graph serialization.
    - [ ] Fork from QuickQanava, release in a dedicated GH repository.
  - **v0.2.x:**
    - [ ] Add a naive chinese whispers clustering algorithm (See [Wikipedia description](https://en.wikipedia.org/wiki/Chinese_Whispers_(clustering_method)) and [Dlib implementation](http://dlib.net/dlib/clustering/chinese_whispers_abstract.h.html)).
  - **v0.3.x:**
    - [ ] Port [cjgdev](https://github.com/cjgdev/aho_corasick) Aho Corasick implementation to GTpo.
  - **v0.4.x:**
    - [ ] Add basic naive (iterative) support for major social network metrics (HITS, PR, etc.)

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

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/gtpo-datamodel.png)

  Memory in GTpo is managed with `std::shared_ptr` and `std::weak_ptr`, using/typedef definitions in graph types are prefixed with either *shared_* or *weak_* according
to the underlying concrete container.

Static graph configuration
-------------

GTpo use "curiously recurring template pattern" [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) to allow complete configuration of graph topology at compile time using a single static configuration definition based on `gtpo::config<>` template.

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/gtpo-datamodel-static_configuration.png)

Available options:

- `gtpo::config::graph_base`: Base class for `gtpo::graph<>`.
- `gtpo::config::node_base`: Base class for `gtpo::node<>`.
- `gtpo::config::edge_base`: Base class for `gtpo::edge<>`.
- `gtpo::config::group_base`: Base class for `gtpo::group<>`.

- `gtpo::config::final_node_t`: Final concrete node type.
- `gtpo::config::final_edge_t`: Final concrete edge type.
- `gtpo::config::final_group_t`: Final concrete group type.

- `gtpo::config::container_adapter`: Generic adapter to insert and remove item in a container.
- `gtpo::config::node_container_t`: Main node container (default to `std::vector`).
- `gtpo::config::edge_container_t`: Main node container (default to `std::vector`).
- `gtpo::config::search_container_t`: Container with an `insert()` and `find()` interface used when fast lookup is necessary, mainly used for internal edge caches (default to `std::unordered_set`).

All options have working default values, for example it is usually not necessary to redefine `container_adapter`, since there is support for all standard and Qt containers.

### Behaviours

  Behaviour in GTpo are the preferred way to observe changes in graph topology. Behaviours could be either *static* or *dynamic*, static behaviours are defined at compile time by modifying `gtpo::config` definition, wheras dynamic behaviours could be added or removed at runtime (with a virtual cost).

+ **Static behaviours**: Static behaviour 

+ **Dynamic behaviours**: FIXME.

![GTpo data model schema](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/gtpo-behaviours-class.png)


 Behaviours could be disabled by calling `gtpo::behaviour<>::disable()` method, disabling all behaviours might be usefull before calling `gtpo::Graph<>::clear()` method or before serializing the graph in or out.
 
 
Topology
-------------

### Iterators (`gtpo/algorithm.h`)

Header: [<gtpo/algorithm.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/algorithm.h)

Graph nodes could be traversed with c++11 range for loops (usually in node insertion order, but it may vary following what containers have been defined in user static graph configuration):

```cpp
#include <GTpo>
// ...
gtpo::graph<> g;
g.insert_node();
for (const auto n : g) { /* ... */ }
// Or
for (auto n : g) { /* ... */ }
```

GTpo also offer a DFS iterator to allow inspection of a graph with DFS ordered nodes (with no temporaries):
```cpp
gtpo::graph<> g;
auto n = g.create_node();

// Iterating in DFS order in a one vertex graph:
auto it = gtpo::begin_dfs(g);
auto ni = *it;
  // With: ni.lock().get() == n.lock().get())
auto it_end = gtpo::end_dfs(g);

// Or with for loop:
for ( auto it = gtpo::begin_dfs(g); it != gtpo::end_dfs(g); it++ ) {
    // Do something with *it, which is a gtpo::graph_t::weak_node_t (ie a weak_ptr on a concreate shared_ptr node).
}
```

### Traversal algorithms (`gtpo/algorithm.h`)

Header: [<gtpo/algorithm.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/algorithm.h)

**Note:** _all_ recursive functions are postfixed with *'_rec'*, there is no overflow protection for large deep graphs. Alternative non recursive stack based implementation are available for most functions (no postfix).

- `gtpo::depth_rec()`: Return graph depth.
- `gtpo::linearize_dfs_rec()`: Return a vector of graph nodes linearized in DFS order.
- `gtpo::levelize_dfs_rec()`: Return a vector of graph nodes ordered level by level in DFS order. Returned vector size equal graph depth, with a vector of ordered nodes for each level.

These traversal functions are specialized for trees:

- `gtpo::tree_depth_rec()`: Return tree depth.
- `gtpo::linearize_tree_dfs()` / `gtpo::linearize_tree_dfs_rec()`: Return a vector of tree nodes linearized in DFS order.
- `gtpo::levelize_tree_dfs_rec()`: Return a vector of tree nodes ordered level by level in DFS order. Returned vector size equal tree depth, with a vector of ordered nodes for each level.

Calling a *'_tree'* method is often faster, but using it against a non-tree graph might lead to overflow or infinite recursion. Following methods could be used to check specific graph topological properties:

- `gtp::is_dag_rec()` / `gtp::is_dag()`  (O(N)): Return true if graph is a directed acyclic graph. **FIXME 20180815** There is actually a bug in is_dag_rec() implementation, see the failing test in 'gtpo_lgorithm_tests.cpp' and use with care...
- `gtp::is_tree_rec()` / `gtp::is_tree()` (O(N)): Return true if graph is a tree (or a forest).

### Functionnals (`gtpo/algorithm.h`)

Header: [<gtpo/functional.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/functional.h)

- `gtp::copy()` (O(N)): Copy a source graph to a destination graph.
  - Signature: `template <> auto copy(const src_graph_t& src, dst_graph_t& dst) -> bool`
  - Precondition (return false if not met): destination graph must be empty.
  - Note: source and destination may be of different types.
  - Limitations (20180815): Groups are not taken into account.
  - Throw: May throw std::bad_alloc

```cpp
#include <GTpo>
#include "GTpo/functional.h"
// ...
gtpo::graph<> src, dst;
auto n1 = src.create_node();
auto n2 = src.create_node();
src.create_edge(n1, n2);
const auto ok = gtpo::copy(src, dst);
// ok is true.
// dst.get_node_count() is 2, dst.get_edge_count() is 1, src and dst are isomorph.
```

- `gtp::filter()`: Apply a user defined unary functor to filter nodes from a source graph before copying the subset of source graph topology to a destination graph.
  - Signature: `template <> auto filter(const src_graph_t& src, dst_graph_t& dst, filter_node_func_t f) -> bool`
  - Precondition (return false if not met): destination graph must be empty.
  - Note: source and destination may be of different types.
  - Limitations (20180815): Groups are not taken into account.
  - Throw: May throw std::bad_alloc

```cpp
#include <GTpo>
#include "GTpo/functional.h"
// ...
gtpo::graph<> src, dst;
src.create_node();
const auto f = [](const auto& node) -> bool { return false; };
auto ok = gtpo::filter<gtpo::graph<>, gtpo::graph<>, decltype(f)>(src, dst, f);
// dst graph is now a filtered version of src graph

// Modern compilers could usually infer template arguments type, following syntax should be preferred:
gtpo::graph<> dst2;
ok = gtpo::filter<>(src, dst2, f);
```

User could filter a specific set of subnode, the subset of topology involving theses nodes will be preserved by filtering:

```cpp
gtpo::graph<> src, dst;
auto n1 = src.create_node().lock();
auto n2 = src.create_node().lock();
auto n3 = src.create_node();
  
src.create_edge(n1, n2);
src.create_edge(n1, n3);
src.create_edge(n3, n2);

// SOURCE GRAPH:  { [n1, n2, n3], [ n1 -> n2, n1 -> n3, n3 -> n2 ] },

const auto f = [n1, n2](auto& node) -> bool {
  return node == n1 || node == n2;        // Filtering all nodes but n1 and n2
};
gtpo::filter<>(src, dst, f);
  
// FILTERED DESTINATION GRAPH:  { [n1, n2], [ n1 -> n2 ] },
```


- `gtp::map()`: Map source graph nodes to a destination graph nodes using a factory/transformation functor.
  - Signature: `template <> auto map(const src_graph_t& src, dst_graph_t& dst, map_node_func_t f) -> bool`
  - Precondition (return false if not met): destination graph must be empty.
  - Note: source and destination may be of different types.
  - Limitations (20180815): Groups are not taken into account.
  - Throw: May throw std::bad_alloc

```cpp
gtpo::graph<> src;
const auto f = [](const auto& src_node) -> gtpo::graph<>::shared_node_t {
  static_cast<void>(src_node);
    return std::make_shared<typename gtpo::graph<>::node_t>();
};
gtpo::graph<> dst;
dst.create_node();
const auto ok = gtpo::map<gtpo::graph<>, gtpo::graph<>, decltype(f)>(src, dst, f);
```

  
*Help wanted*:
- Create an "inplace" version for `gtpo::filter`, where input graph is filtered inplace without a copy to a destination graph.
- Create a version `gtpo::filter` and `gtpo::copy` where destination (or result) graph is returned directly using RVO and not taken as a function argument.

### Generators (`gtpo/generator.h`)

Header: [<gtpo/generator.h>](https://github.com/cneben/QuickQanava/blob/master/GTpo/src/generator.h)

Graph generation functions interface is similar to [NetworkX](https://networkx.github.io/) generators.

- `gtpo::complete_graph()`: Return a fully connected directed graph. Complexity O(n<sup>2</sup>).

- Random GNP graphs:
  - `gtpo::gnp_random_graph()`: *TBD*. Complexity O(n<sup>2</sup>).
  - `gtpo::erdos_renyi_graph()`: *TBD*. Complexity O(n<sup>2</sup>).

- `gtpo::complete_tree_rec()`:  *TBD*.


### Group topology

Edges and adjacent edges of a group could be searched with `gtpo::group<>::get_edges()` and `gtpo::group<>::get_adjacent_edges()`. Adjacent edge set won't be initialized until the `gtpo::graph_group_adjacent_edges_behaviour` and `gtpo::group_adjacent_edges_behaviour` static behaviour are configured in `gtpo::config`.

For example: edges = **[** *e4*, *e5* **]**  and adjacent_edges = **[** *e2*, *e3*, *e4*, *e5* **]** :

![](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/gtpo-topo-group_adjacent_edges.png)

Minimum static behaviour configuration to enable group adjacent edge support is the following (enabled in gtpo::default_config): 

~~~~~~~~~~~~~{.cpp}
struct my_config : public gtpo::config<my_config>
{
    using graph_behaviours = std::tuple< gtpo::graph_group_adjacent_edges_behaviour<my_config> >; 
    using group_behaviours = std::tuple< gtpo::group_adjacent_edges_behaviour<my_config> >; 
};
~~~~~~~~~~~~~



Benchmarks
-------------

### Specialized tree functions vs general graph functions

![](https://github.com/cneben/QuickQanava/blob/master/GTpo/doc/linearize_dfs_tree.png)

