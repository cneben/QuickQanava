# GTpo  (C++14 Topology library)
===========================

GTpo (Generic Topology) is the C++11 graph topology library used in QuickQanava2 graph visualization library. GTpo is available under the 
MIT licence.

GTpo is not a generic library in the Boost Graph Library sense, but rather an highly configurable graph modelling library designed for
ease of use an maximum performances over "genericity". GTpo heavily rely on C++11/C++17 functionnalities to hide template complexity from the user. 

GTpo root namespace 'gtpo' contains generic code, user not interested in using specific containers could use one of the following namespaces for accessing a 
"concrete GTpo library" directly:
    
* QuickQanava2: Concrete implementation of all GTpo features for QT/QML with complete graph visualization, see: ![QuickQanava2](https://github.com/cneben/QuickQanava)

![GTpo data model schema](https://github.com/cneben/QuickQanava/GTpo/blob/devel/doc/graph-datamodel.png)

GTpo is **highly** alpha.

+ Project homepage: http://www.qanava.org/
+ Reference documentation: http://www.delia-strategie.fr/doc/gtpo/index.html
+ Developper manual: https://github.com/cneben/GTpo/blob/master/doc/manual/manual.md

# Installation

## Dependencies
* Googletest: https://github.com/google/googletest/ 

## Roadmap

- [X] Basic directed graph topology support with custom containers and property support.
- [X] Topology serialization using Protocol Buffer v3.
- [X] Basic GraphML support (OUT).
- [X] Virtual behaviours/layouts/observers support.
- [ ] Gephi GEXF file format IN/OUT support.
- [ ] Complete asynchronous graph access with a read/write graph view MUTEX protection.
- [ ] Advanced asynchronous graph access with Intel TBD thread safe containers.
- [X] Static behaviours/layouts/observers support (C++14 only).
- [ ] Graph search algorithms (Djikstra/A-Star).

## Installation:
GTpo use _qmake_ as its main build configuration system, dependencies are configured in the root directory _common.pri_ file:

~~~~~~~~~~~~~{.cpp}master
# Comment if you do not intent to serialize graph to Protocol Buffer
DEFINES     += "GTPO_HAVE_PROTOCOL_BUFFER"

INCLUDEPATH += c:/path/to/protobuf3/src
PROTOCOL_BUFFER_LIBDIR_RELEASE = C:/path/to/protobuf3/cmake/build/Release
PROTOCOL_BUFFER_LIBDIR_DEBUG = C:/path/to/protobuf3/cmake/build/Debug
~~~~~~~~~~~~~

The simplest way of using GTpo is to statically integrate the library as a GIT subtree in your own project:

~~~~~~~~~~~~~{.cpp}
// Install GTpo as a remote repository:
git remote add GTpo https://github.com/cneben/GTpo

// Then in your project root, use the following command:
git subtree add --prefix=./GTpo --squash GTpo master 
~~~~~~~~~~~~~

Once GIT has download the whole subtree, GTpo could be included directly in your main
qmake .pro file:

~~~~~~~~~~~~~{.cpp}
# in your project main .pro qmake configuration file
include(./GTpo/src/gtpo.pri)
~~~~~~~~~~~~~



Data model
------------------

![GTpo data model schema](https://github.com/cneben/GTpo/blob/master/doc/graph-datamodel.png)


Behaviours
-------------

  Behaviour in GTpo are the preferred way to observe and validate changes in graph and groups topology. Behaviours could be attached to either
groups or graph with addBehaviour() methods.  Behaviours have two distinct interfaces:
+ **Observation interface**: all behaviour interface expose 'inserted', 'removed' and 'modified' pure virtual methods that could be used to
 observe topological changes in graph and groups.
+ **Validation interface**: FIXME.

   GTpo actually support both virtual behaviours that could be modified dynamically and static behaviours with full template code that could be configured
by modifying graph definition:
+ **Virtual behaviours**: FIXME.
+ **Static behaviours**: FIXME.

 Behaviours could be disabled by calling gtpo::Behaviour::disable() method, it is usually preferrable to disable all behaviours before calling gtpo::GenGraph::clear() or
 gtpo::GenGroup::clear() methods or when you are serializing a graph in or out.

 Method gtpo::Behaviorable::addBehaviour() is the only "sink" method in GTpo that is not protected using smart pointer, consider using the following code when registering 
~~~~~~~~~~~~~{.cpp}
new behaviours with gtpo::GenGraph::addBehaviour() or gtpo::GenGroup::addBehaviour():
stpo::Graph sg;
auto myBehaviour = std::make_unique< MyBehaviour >( );
sg.addBehaviour( myBehaviour.release() );
~~~~~~~~~~~~~
 
 See the 'behaviour' sample for a simple example behaviour that echo every topology changes in graph to std::cout.

Memory management
------------------

  Memory in GTpo is managed exclusively with std::shared_ptr and std::weak_ptr, using definition in graph types are prefixed with either *Shared* or *Weak* according
to the concrete container.

  The user is reponsible for checking smart pointers returned by gtpo::GenGraph<> "factory methods", for example:
~~~~~~~~~~~~~{.cpp}
  stpo::Graph g;
  auto n = g.createNode();	// typeid(n) = stpo::Graph::WeakNode, ie std::weak_ptr<stpo::Graph::Configuration::Node>, ie std::weak_ptr<stpo::Node>
  
  g.removeNode( n );  // Node is removed, it will be automatically destroyed

  // Be carefull, you can still try to lock n, no exception will be thrown automatically:
  auto nn = n.lock();	            // Don't throw, nn must be checked for nullptr
  bool valid = n.lock() != nullptr	// Don't throw, but return false (ie invalid)
  
  // All std::weak_ptr (ie all "Weak*" types in GTpo) should be checked with their 'expired()' method before use:
  valid = n.expired();			    // Don't thow, but return false (ie node invalid)
  
  // It is recommended to use the gtpo::assert_thow to check smart pointer:
  gtpo::assert_throw( !n.expired(), "Error: trying to use an expired weak reference on node" );			// Throw a gtpo::topology_error with an error message
  gtpo::assert_throw( n.lock() != nullptr, "Error: trying to use an expired weak reference on node" );  // Throw a gtpo::topology_error with an error message
~~~~~~~~~~~~~

### Group topology

When a gtpo::GroupEdgeSetBehaviour is registered in graph (ie added in gtpo::GraphConfig::GroupBehaviours std::tuple), edges and adjacent edges of a group could be searched 
with gtpo::GenGroup<>::getEdges() and gtpo::GenGroup<>::getAdjacentEdges():

![](https://github.com/cneben/QuickQanava/GTpo/blob/devel/doc/gtpo_topo_group_adjacent_edges.png)


