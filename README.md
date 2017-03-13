
**2017/03/04:** There is already a lot of clones on 'dev', please: do not try 'dev' until your intention is to contribute (and drop me a line if you are using Qan on a regular basis: benoit@destrat.io).

**Roadmap:**

  - **GTpo** (Configurable topology library):
     - [X] Redesign and optimize GTpo generic containers accessors (remove properties management from GTpo, it has nothing to do with topology).
     - [X] Redesign GTPO to support group -> node and group -> group topology with full restricted hyperlink support.
     - [X] Increase GTpo test coverage to 100%.
	 - [X] Remove all virtual overhead with current getClassName() / getObjectName() code.
	 - [ ] Increase test coverage for subgroups.
  - **QuickContainers** (QT/QML observable adapter for STL or Qt containers):	 
    - [ ] Redesign QuickContainers: qcm::ContainerModel<> memory footprint is too high (inheritance from QAbstractItemModel comes with a strong virtual and signals/slots overhead)
	- [ ] Add support for standard library containers.
	- [ ] Increase test coverage.
  - [80%] Redesign qan::Graph interface for creating content.
  - [ ] Add simple _optional_ protobuf helpers for graph serialization.
  - [ ] Add a 100% C++ interface (currently QML delegate components definition still are necessary).
     - [ ] Publish the 4k sample (40k is probably too much for QML without dedicated culling and LOD code).
  - [50%] Add _efficient_ support for non visual nodes and edges, non visual edges should have a very small footprint (ie non QQuickItem...).
  - [ ] Update samples for full QQC2 controls support.
  - [X] Redesign visual connection of nodes (add support for custom visual component, invisible edge creation, etc.).
  - [ ] Add full support for groups inside group (ie subgraphs).
  - [ ] Add full support for Bezier curve edge.
     - [ ] Use Qt 5.9 PathItem for lines, curves and poly rendering instead of QuickGeoGL (QuickGeoGl is probably faster for simple AA lines with its vertex shader, but QuickGeoGL is too complex to maintain).
  - [ ] Fix current qan::PointGrid bugs and add "snap to grid" support.
  - [ ] Add support for multiple "docks" per node (ie. to connect in edges on multiple node side, not only at node center).
  - Qt 5.10 is targeted for the 1.0 release with full PathItem support.
