/*
    This file is part of Quick Qanava library.

    Copyright (C) 2008-2015 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the Qanava software.
//
// \file	qanGraph.h
// \author	benoit@qanava.org
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanGraph_h
#define qanGraph_h

// GTpo headers
#include <GTpo>

// Qanava headers
#include "./qanConfig.h"
#include "./qanStyleManager.h"
#include "./qanEdge.h"
#include "./qanNode.h"
#include "./qanGroup.h"

// QT headers
#include <QQuickItem>
#include <QSharedPointer>

namespace qan { // ::qan

/*! \brief FIXME.
 *
 * \nosubgrouping
*/
class Graph : public gtpo::GenGraph< qan::Config >
{
    Q_OBJECT

    using GTpoGraph = gtpo::GenGraph< qan::Config >;

    /*! \name Graph Object Management *///-------------------------------------
    //@{
public:
    //! Graph default constructor.
    Graph( QQuickItem* parent = 0 );
    /*! Graph default destructor.
     *
     * Graph is a factory for inserted nodes and edges, even if they have been created trought
     * QML delegates, they will be destroyed with the graph they have been created in.
     */
    virtual ~Graph( ) { clearDelegates(); }
private:
    Q_DISABLE_COPY( Graph )
public:
    /*! Clear this graph topology and styles.
     *
     * \note Registered  node and edge delegates are not cleared, you must manually call clearDelegate()
     * to clear the delegates registered with registerNodeDelegate() and registerEdgeDelegate().
     */
    Q_INVOKABLE void    clear( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Delegates Management *///-----------------------------------------
    //@{
public:
    //! Clear the currently regsitered delegates.
    void                    clearDelegates( ) { _nodeClassComponents.clear(); _edgeClassComponents.clear(); _groupClassComponents.clear(); }

    /*! \brief Register a QML component that should be used as a delegate for a class nodes referred as \c nodeClass.
     *
     * Graph keep ownership for \c nodeComponent.
     */
    Q_INVOKABLE void        registerNodeDelegate( QString nodeClass, QQmlComponent* nodeComponent );

    /*! \brief Register a QML component that should be used as a delegate for a class of edges referred as \c edgeClass.
     *
     * Graph keep ownership for \c nodeComponent.
     */
    Q_INVOKABLE void        registerEdgeDelegate( QString edgeClass, QQmlComponent* edgeComponent );

    /*! \brief Register a QML component that should be used as a delegate for a class of groups referred as \c groupClass.
     *
     * Graph keep ownership for \c groupComponent.
     */
    Q_INVOKABLE void        registerGroupDelegate( QString groupClass, QQmlComponent* groupComponent );

    /*! \brief Create a node item using the delegate component currently registered under \c nodeClass.
     *
     * Graph keep ownership for returned item.
     * \return an item created with the delegate registered for \c edgeClass, or an empty QQuickItem (\c nullptr is never returned).
     * \sa registerNodeDelegate()
     */
    Q_INVOKABLE QQuickItem* createNodeItem( QString nodeClass );
    /*! \brief Create an edge item using the delegate component currently registered under \c edgeClass.
     *
     * Graph keep ownership for returned item.
     * \return an item created with the delegate registered for \c nodeClass, or an empty QQuickItem (\c nullptr is never returned).
     * \sa registerNodeDelegate()
     */
    Q_INVOKABLE QQuickItem* createEdgeItem( QString edgeClass );

protected:
    //! Create a graph primitive using the given delegate \c component.
    Q_INVOKABLE QQuickItem* createFromDelegate( QQmlComponent* component );
private:
    QMap< QString, QQmlComponent* > _nodeClassComponents;
    QMap< QString, QQmlComponent* > _edgeClassComponents;
    QMap< QString, QQmlComponent* > _groupClassComponents;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    /*! Insert a new node in this graph and return a pointer on it, or \c nullptr if creation fails.
     *
     * A gtpo::bad_topology_error could be thrown if insertion in base graph fails.
     *
     * A default node delegate must have been registered with registerNodeDelegate() if
     * \c nodeComponent is unspecified (ie \c nullptr); it is done automatically if
     * Qan.Graph is used, with a rectangular node delegate for default node.
     *
     * \note graph keep ownership of the returned node.
     */
    Q_INVOKABLE qan::Node*  insertNode( QQmlComponent* nodeComponent = nullptr );

    //! Insert a new node with a given class name \c nodeClassName and return a pointer on it if creation succeed (\c nullptr otherwise).
    Q_INVOKABLE qan::Node*  insertNode( QString nodeClassName );

    /*! Call either insertNode(QQmlComponent) or insertNode(QString) according to the QVariant concrete type.
     *
     * End user should usually not care about this method, it is declared only because QML often implicitely
     * cast QML "Component" object to "string", creating confusion when insertNode() is called.
     */
    Q_INVOKABLE qan::Node*  insertNode( QVariant nodeArguments );

    //! Defined for serialization support, do not use.
    virtual auto            createNode( const std::string& className ) -> WeakNode;

    /*! Remove node \c node from this graph. Shortcut to gtpo::GenGraph<>::removeNode().
     */
    Q_INVOKABLE void        removeNode( qan::Node* node );

    //! Test if a given \c item is a node registered in the graph.
    Q_INVOKABLE bool        isNode( QQuickItem* item ) const;

    //! Shortcut to gtpo::GenGraph<>::getNodeCount().
    Q_INVOKABLE int         getNodeCount( ) { return GTpoGraph::getNodeCount(); }

public:
    //! Access the list of nodes with an abstract item model interface.
    Q_PROPERTY( QAbstractItemModel* nodes READ getNodesModel NOTIFY nodesModelChanged )
    QAbstractItemModel* getNodesModel( ) const { return ( QAbstractItemModel* )( &getNodes() ); }
signals:
    /*! \note Never used, defined for QML compatibility. */
    void                nodesModelChanged();

signals:
    /*! \brief Emitted whenever a node registered in this graph is clicked.
     */
    void            nodeClicked( qan::Node* node, QPointF pos );
    /*! \brief Emitted whenever a node registered in this graph is right clicked.
     */
    void            nodeRightClicked( qan::Node* node, QPointF pos );
    /*! \brief Emitted whenever a node registered in this graph is double clicked.
     */
    void            nodeDoubleClicked( qan::Node* node, QPointF pos );
    //-------------------------------------------------------------------------

    /*! \name Graph Edge Management *///---------------------------------------
    //@{
public:
    //! Shortcut to gtpo::GenGraph<>::insertEdge().
    Q_INVOKABLE qan::Edge*  insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent = nullptr );

    //! Defined for serialization support, do not use.
    virtual auto            createEdge( const std::string& className, WeakNode source, WeakNode destination  ) -> WeakEdge override;

    //! Shortcut to gtpo::GenGraph<>::removeEdge().
    Q_INVOKABLE void        removeEdge( qan::Node* source, qan::Node* destination );

    //! Shortcut to gtpo::GenGraph<>::removeEdge().
    Q_INVOKABLE void        removeEdge( qan::Edge* edge );

    //! Return true if there is at least one directed edge between \c source and \c destination (Shortcut to gtpo::GenGraph<>::hasEdge()).
    Q_INVOKABLE bool        hasEdge( qan::Node* source, qan::Node* destination ) const;

public:
    //! Access the list of edges with an abstract item model interface.
    Q_PROPERTY( QAbstractItemModel* edges READ getEdgesModel NOTIFY edgesModelChanged )
    QAbstractItemModel* getEdgesModel( ) const { return ( QAbstractItemModel* )( &getEdges() ); }
signals:
    /*! \note Never used, defined for QML compatibility. */
    void                edgesModelChanged();

signals:
    /*! Emitted whenever a node registered in this graph is clicked.
     *
     *  \sa nodeClicked()
     */
    void            edgeClicked( QVariant edge, QVariant pos );
    /*! Emitted whenever a node registered in this graph is right clicked.
     *
     *  \sa nodeRightClicked()
     */
    void            edgeRightClicked( QVariant edge, QVariant pos );
    /*! Emitted whenever a node registered in this graph is double clicked.
     *
     *  \sa nodeDoubleClicked()
     */
    void            edgeDoubleClicked( QVariant edge, QVariant pos );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Group Management *///--------------------------------------
    //@{
public:
    //! Shortcut to gtpo::GenGraph<>::insertGroup().
    Q_INVOKABLE qan::Group* insertGroup( QQmlComponent* groupComponent = nullptr );

    //! Shortcut to gtpo::GenGraph<>::removeGroup().
    Q_INVOKABLE void        removeGroup( qan::Group* group );

    //! Return true if \c group is registered in graph.
    bool                    hasGroup( qan::Group* group ) const;

    //! Shortcut to gtpo::GenGraph<>::getGroupCount().
    Q_INVOKABLE int         getGroupCount( ) const { return gtpo::GenGraph< qan::Config >::getGroupCount(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    /*! Graph style manager (ie list of style applicable to graph primitive).
     */
    Q_PROPERTY( qan::StyleManager* styleManager READ getStyleManager NOTIFY styleManagerChanged )
    qan::StyleManager*  getStyleManager( ) { return _styleManager.data(); }
    qan::StyleManager&  styleManager( ) { Q_ASSERT( _styleManager != nullptr ); return *_styleManager.data(); }
private:
    using SharedStyleManager = QSharedPointer< qan::StyleManager >;
    SharedStyleManager  _styleManager;
signals:
    void                styleManagerChanged( );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Serialization Management *///------------------------------
    //@{
public:
    Q_INVOKABLE void    saveToGml( QString fileName ) { Q_UNUSED( fileName); /*serializeToGml( fileName.toStdString( ) );*/ }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Initialization Management *///-----------------------------
    //@{
public:
    Q_INVOKABLE void    initializeRandom( int   nodeCount,
                                          int   minOutNodes, int maxOutNodes,
                                          qreal minWidth, qreal maxWidth,
                                          qreal minHeight, qreal maxHeight,
                                          QRectF br );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Graph::WeakNode )

#endif // qanGraph_h

