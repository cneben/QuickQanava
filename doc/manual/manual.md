GTpo Developper Manual
============================

+ Reference documentation: http://www.delia-strategie.fr/doc/gtpo/index.html

For any questions, please contact: benoit@qanava.org

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| Pugy XML                  | No                |       **Yes**                   |      MIT        |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

*PugiXML* source beeing distributed under an MIT licence, it is compatible with *GTpo* an is included directly in the *GTpo* source tree. For more informations on *PugyXML*, see:
+ PugiXML homepage: http://pugixml.org/
+ PugiXML GitHub: https://github.com/zeux/pugixml

Google Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer homepage and installation instructions could be found on:
+ Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
+ Protocol Buffer v3 GitHub: https://github.com/google/protobuf

Google Test is a *GTpo* dependency, it is not mandatory for QuickQanava until you intent to use a graph with custom non-STL non-Qt containers:
+ Google Test GitHub: https://github.com/google/googletest/

[TOC]

Installation
------------------

Samples
------------------


Data model
------------------

![GTpo data model schema](https://github.com/cneben/GTpo/blob/master/doc/graph-datamodel.png)

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

Topology
------------------

Groups
------------------

Properties
------------------

There is two way to access graph primitives properties in GTpo:
+ Direct access to properties trough the gtpo::PropertiesConfig interface.
+ Access via gtpo::GenGraph accessors: it will enforce correct properties changes notifications to graph/node/edge and groups behaviours.
+ Define efficient accessors in your own code and access it directly when no notification reporting is necessary or to access user defined properties uninteresting for the framework.

| Primitive       | Name         |   Type         |   Accessors                                            |   Description       |
| ---             | :---:        | :---:          | :---:                                                  | :---:               |
| gtpo::GenNode   | label        | std::string    | GenGraph::setNodeLabel(), GenGraph::getNodeLabel()     |                     |
| gtpo::GenNode   | label        | std::string    | GenGraph::setNodeLabel(), GenGraph::getNodeLabel()     |                     |
| gtpo::GenNode   | x            | double         | GenGraph::setNodeX(), GenGraph::getNodeX()             |                     |
| gtpo::GenNode   | y            | double         | GenGraph::setNodeY(), GenGraph::getNodeY()             |                     |
| gtpo::GenNode   | width        | double         | GenGraph::setNodeWidth(), GenGraph::getNodeWidth()     |                     |
| gtpo::GenNode   | height       | double         | GenGraph::setNodeHeight(), GenGraph::getNodeHeight()   |                     |
| ---             | :---:        | :---:          | :---:                                                  | :---:               |
| gtpo::GenEdge   | label        | std::string    | GenGraph::setEdgeLabel(), GenGraph::getEdgeLabel()     |                     |
| gtpo::GenEdge   | weight       | double         | GenGraph::setEdgeWeight(), GenGraph::getEdgeWeight()   |                     |
| ---             | :---:        | :---:          | :---:                                                  | :---:               |
| gtpo::GenGroup  | label        | std::string    | GenGraph::setGroupLabel(), GenGraph::getGroupLabel()   |                     |
| gtpo::GenGroup  | x            | double         | GenGraph::setGroupX(), GenGraph::getGroupX()           |                     |
| gtpo::GenGroup  | y            | double         | GenGraph::setGroupY(), GenGraph::getGroupY()           |                     |
| gtpo::GenGroup  | width        | double         | GenGraph::setGroupWidth(), GenGraph::getGroupWidth()   |                     |
| gtpo::GenGroup  | height       | double         | GenGraph::setGroupHeight(), GenGraph::getGroupHeight() |                     |

Note: It is often better to use accessors with a gtpo::Shared* type versus a gtpo::Weak* one, it avoid costly locking of weak pointer before accessing properties.

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

See the gtpo::Behaviour class documentation for reference documentation: gtpo::Behaviour.

![GTpo behaviour class diagram](https://github.com/cneben/GTpo/blob/master/doc/manual/behaviour-class.png)

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

Persistence
-------------

Custom Graph Configuration
-------------



