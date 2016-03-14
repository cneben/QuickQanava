# QuickProperties

QuickProperties is a C++/QML library designed to ease edition and management of QObject static and dynamic properties. While being developed with properties edition in mind, QuickProperties also contains simple UI components such as modal/modeless dialogs and a hideable panel that could be reused for fast QML applications prototyping.

![alt tag](http://www.qanava.org/doc/qps/qps-overview.png)

QuickProperties focus on providing a consistent data model based on and around QObject which is the simplest way of exposing data in advanced QML UIs. QuickProperties (QPS) main concepts are:
- **Properties:** properties are a set of standard Qt static and dynamic in a QObject.
- **Timed properties:** extend previous concept with date/time support; all property elements values could be changed and retrieved over time allowing the user to display a timegraph of values for specific properties.
- **DataSource:** either a synchronous or asynchronous source of data for timed properties (actually, an XML or CSV file, or a more advanced wrapper such as in to the Y!F  HTTP stock sample).
- **Qt Item Model interface:** QPS expose QObject properties in a standard Qt Item Model interface, allowing visualization and edition of properties directly in existing QML components.

More informations available on project homepage: http://www.qanava.org/

QuickProperties Reference documentation: http://www.qanava.org/doc/qps/index.html

QuickProperties screenshot:

![alt tag](http://www.qanava.org/doc/qps/timegraph.png)

![alt tag](http://www.qanava.org/wp-content/uploads/2015/10/20151020_sample-propsedit-1024x492.png)

![alt tag](http://www.qanava.org/wp-content/uploads/2015/10/20151029_quiproperties-sample-propslist.png)

QuickProperties is distributed under the GNU General Public Licences.

QuickProperties is still _early alpha_, contact benoit@qanava.org if you need specific features or non GPL licencing.
