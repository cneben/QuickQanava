QuickQanava Samples  {#samples}
============================

"custom" - Custom Nodes
------------------

Demonstrate how to define node with custom graphic content using QuickQanava node QML templates.

![custom sample](http://www.delia-strategie.fr/doc/qan/custom.png)

"gml" - Graph Markup Language Export: 
------------------

"navigable" - Navigable Area: 
------------------

Demonstrate use of qan::Navigable.

![navigable sample](http://www.delia-strategie.fr/doc/qan/navigable.png)

"style" - Style Management: 
------------------

![style sample](http://www.delia-strategie.fr/doc/qan/style.png)

"topology" - Topology Sample: 
------------------

Compilation of Protocol Buffer message for topology sample:

~~~~~~~~~~~~~{.cpp}
protoc topology.proto --cpp_out=. -I. -I../../GTpo/src
~~~~~~~~~~~~~

![topology sample](http://www.delia-strategie.fr/doc/qan/topology.png)




