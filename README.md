
QuickQanava 
============================

QuickQanava2 is an open-source C++11/QML library designed to display graphs in a Qt QML application. QuickQanava provide classes to generate various layouts of directed graphs and visualize their nodes with custom styles on a graphic canvas. 

QuickQanava2 is released under LGPL v3, and is shipped with *GTpo* library (distributed under MIT licence). 

QuickQanava2 source repository is hosted on GitHub: https://github.com/cneben/quickqanava

QuickQanava2 use code from the QuickProperties project on: https://github.com/cneben/quickproperties (code is actually shipped as a subtree in main QuickQanava source tree).

QuickQanava is primarily developed with Qt >= 5.6 and Qt Creator with MSVC 2015 but should compiles well with g++ for Linux and GCC-ARM for Android. 

+ Project homepage: http://www.qanava.org/
+ Reference documentation: http://www.delia-strategie.fr/doc/qan/index.html

For any questions, please contact: benoit@qanava.org

| Dependency                | Mandatory         |   Included in source tree       |   Licence       |
| ---                       | :---:             | :---:                           | :---:           |
| GTpo                      | **Yes**           |       **Yes**                   |      MIT        |
| Pugy XML                  | No                |       **Yes**                   |      MIT        |
| QuickProperties2          | **Yes**           |       **Yes**                   |      LPGL       |
| Protocol Buffer v3        | No                |       No                        |    Permissive   |
| Google Test/Mock          | No                |       No                        |    Permissive   |

*PugiXML* source beeing distributed under an MIT licence, it is compatible with *GTpo* an is included directly in the *GTpo* source tree. For more informations on PugyXML, see:
+ PugiXML homepage: http://pugixml.org/
+ PugiXML GitHub: https://github.com/zeux/pugixml

Google Protocol Buffer is used for binary serialization of graph, it is not a mandatory dependency, until you include the "qanSerializer.h" header. Protocol Buffer homepage and installation instructions could be found on:
+ Protocol Buffer homepage: https://developers.google.com/protocol-buffers/
+ Protocol Buffer v3 GitHub: https://github.com/google/protobuf

Google Test is a *GTpo* dependency, it is not mandatory for QuickQanava until you intent to use a graph with custom non-STL non-Qt containers:
+ Google Test GitHub: https://github.com/google/googletest/

## QuickQanava Screenshots:

![custom node sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/custom.png)

![navigable sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/navigable.png)

![style sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/style.png)

![topology sample](https://github.com/cneben/QuickQanava/blob/master/doc/samples/topology.png)

![sample](http://www.qanava.org/wp-content/uploads/2015/07/20150719_NP_Quick_Qanava_test-1024x787.png)


