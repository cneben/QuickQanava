QuickContainers
============================

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
[![Twitter URL](https://img.shields.io/twitter/url/https/twitter.com/fold_left.svg?style=social&label=Follow%20%40QuickQanava)](https://twitter.com/QuickQanava)

`QuickContainers` expose a container as with a unique "Qt style" interface.
 
`QuickContainers` support to following containers with a unique interface:
  - Qt containers: QVector, QList and QSet.
  - Std containers: std::vector

With any combinations of:
  - std::weak_ptr
  - std::unique_ptr
  - QPointer
  - Raw QObject pointers.
  - Any pod type.

*Note:* Using containers with non pointer QObject is not well supported (for example an std::vector<QObjectSub> with QObjectSub having a user defined 
copy constructor).


## Roadmap

  - [] Add better support for move semantic.
  - [] Add support for a simplified std interface (actually primary interface is "Qt style").
  - [] Add support for std::set and std::unordered_set.
  - [] Add direct "key"/"value" support for non-trivial associative containers.
  - [] Add support for non-pointer QObject with copy ctor.
  - [] Add support for Qt smart pointers (QSharedPointer/QWeakPointer)  
  
License
=======

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Copyright (c) 2017 Delia Stratégie