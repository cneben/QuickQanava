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


Topology
------------------

Groups
------------------


Behaviours
-------------

  Behaviour in GTpo are the preferred way to observe and validate topology change in graph and groups. Behaviour could be attached to either
groups or graph with addBehaviour() methods.

+ Observation interface: all behaviour interface expose 'inserted', 'removed' and 'modified' pure virtual methods that could be used to
 observe topological changes in graph and groups.
+ Validation interface: FIXME.

![GTpo behaviour class diagram](https://github.com/cneben/GTpo/blob/master/doc/manual/behaviour-class.png)

 See the 'behaviour' sample for a simple example behaviour that echo every topology changes in graph to std::cout.

Persistence
-------------


