
- **Qt Item Model interface:** QuickProperties can expose containers (either QVector or QList) of QObject in a standard Qt Item Model interface, allowing visualization and edition of properties directly in existing QML components. qps::ContainerListModel also has dedicated code to manage standard container of smart pointer containing QObject pointer, for example you can expose a type such as QVector<std::shared_ptr<QObject>> directly as a QAbstractListModel useable in a QML ListView *model* property: http://www.destrat.io/quickproperties/doc/FIXME
 
License
=======
QuickProperties is distributed under the GNU Lesser General Public Licence v3 LGPLv3.

[LGPLv3](https://github.com/cneben/QuickProperties/blob/master/lgpl.txt)

Copyright (c) 2017 Delia Stratégie