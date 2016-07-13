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
// This file is a part of the QuickQanava software library.
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
#include "./qanNavigable.h"

// QT headers
#include <QQuickItem>
#include <QSharedPointer>
#include <QAbstractListModel>

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

    Q_INVOKABLE void    addControlNode( qan::Node* node );
    Q_INVOKABLE void    removeControlNode( qan::Node* node );

public:
    /*! Similar to QQuickItem::childAt() method, except that it take edge bounding shape into account.
     *
     * Using childAt() method will most of the time return qan::Edge items since childAt() use bounding boxes
     * for item detection.
     *
     * \return nullptr if there is no child at requested position, or a QQuickItem that can be casted qan::Node, qan::Edge or qan::Group with qobject_cast<>.
     */
    Q_INVOKABLE QQuickItem* graphChildAt(qreal x, qreal y) const;

    /*! Similar to QQuickItem::childAt() method, except that it only take groups into account (and is hence faster, but still O(n)).
     *
     */
    Q_INVOKABLE qan::Group*  groupAt( const QPointF& p, const QSizeF& s ) const;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Selection Management *///----------------------------------------
    //@{
public:
    /*! Graph node selection policy (default to \c SelectOnClick);
     * \li \c NoSelection: Selection of nodes is disabled.
     * \li \c SelectOnClick (default): Node is selected when clicked, multiple selection is allowed with CTRL.
     * \li \c SelectOnCtrlClick: Node is selected only when CTRL is pressed, multiple selection is allowed with CTRL.
     */
    enum SelectionPolicy { NoSelection, SelectOnClick, SelectOnCtrlClick };
    Q_ENUM(SelectionPolicy)

public:
    /*! Current graph selection policy.
     *
     * \warning setting NoSeleciton will clear the actual \c selectedNodes model.
     */
    Q_PROPERTY( SelectionPolicy selectionPolicy READ getSelectionPolicy WRITE setSelectionPolicy NOTIFY selectionPolicyChanged )
    void            setSelectionPolicy( SelectionPolicy selectionPolicy );
    inline SelectionPolicy  getSelectionPolicy( ) const { return _selectionPolicy; }
private:
    SelectionPolicy _selectionPolicy{ SelectionPolicy::SelectOnClick };
signals:
    void            selectionPolicyChanged( );

public:
    //! Color for the node selection hilgither component (default to dark blue).
    Q_PROPERTY( QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged )
    void            setSelectionColor( QColor selectionColor );
    inline QColor   getSelectionColor( ) const { return _selectionColor; }
private:
    QColor          _selectionColor{ Qt::darkBlue };
signals:
    void            selectionColorChanged( );

public:
    //! Selection hilgither item stroke width (default to 3.0).
    Q_PROPERTY( qreal selectionWeight READ getSelectionWeight WRITE setSelectionWeight NOTIFY selectionWeightChanged )
    void            setSelectionWeight( qreal selectionWeight );
    inline qreal    getSelectionWeight( ) const { return _selectionWeight; }
private:
    qreal           _selectionWeight{ 3. };
signals:
    void            selectionWeightChanged( );

public:
    //! Margin between the selection hilgither item and a selected item (default to 3.0).
    Q_PROPERTY( qreal selectionMargin READ getSelectionMargin WRITE setSelectionMargin NOTIFY selectionMarginChanged )
    void            setSelectionMargin( qreal selectionMargin );
    inline qreal    getSelectionMargin( ) const { return _selectionMargin; }
private:
    qreal           _selectionMargin{ 3. };
signals:
    void            selectionMarginChanged( );

public:
    /*! Request insertion of a node in the current selection according to current policy and return true if the node was successfully added.
     *
     * \note If \c selectionPolicy is set to Qan.AbstractGraph.NoSelection or SelextionPolicy::NoSelection,
     * method will always return false.
     */
    bool    selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers );

    /*! Add a node in the current selection.
     */
    void    addToSelection( qan::Node& node );

    //! Remove a node from the selection.
    void    removeFromSelection( qan::Node& node );

    //! Clear the current selection.
    void    clearSelection();

    //! Return true if multiple node are selected.
    inline  bool    hasMultipleSelection() const { return _selectedNodes.size() > 0; }

public:
    using SelectedNodes = qps::ContainerListModel< QVector, qan::Node* > ;

    //! Read-only list model of currently selected nodes.
    Q_PROPERTY( QAbstractListModel* selectedNodes READ getSelectedNodesModel NOTIFY selectedNodesChanged )
    QAbstractListModel* getSelectedNodesModel( ) { return qobject_cast<QAbstractListModel*>( &_selectedNodes ); }

    auto    getSelectedNodes( ) -> SelectedNodes& { return _selectedNodes; }
    auto    getSelectedNodes( ) const -> const SelectedNodes& { return _selectedNodes; }
signals:
    void                selectedNodesChanged();
private:
    SelectedNodes       _selectedNodes;

protected:
    virtual void    mousePressEvent(QMouseEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Delegates Management *///----------------------------------------
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

    static void setCppOwnership( QQuickItem* item );

protected:
    //! Create a Qt Quick Canvas object (caller get ownership for the object flagged with CppOwnership).
    QQuickItem*     createCanvas( QQuickItem* parent );
private:
    std::unique_ptr< QQmlComponent >   _canvasComponent{nullptr};
protected:
    //! Create a Qt Quick Rectangle object (caller get ownership for the object flagged with CppOwnership).
    QQuickItem*     createRectangle( QQuickItem* parent );
private:
    std::unique_ptr< QQmlComponent >   _rectangleComponent{nullptr};

private:
    QMap< QString, QQmlComponent* > _nodeClassComponents;
    QMap< QString, QQmlComponent* > _edgeClassComponents;
    QMap< QString, QQmlComponent* > _groupClassComponents;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    using Node              = typename Config::Node;
    using WeakNode          = std::weak_ptr< typename Config::Node >;

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

    //! Shortcut to GTpoGraph::insertNode.
    WeakNode                insertNode( SharedNode node );

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

    //! Shortcut to gtpo::GenGraph<>::insertEdge().
    Q_INVOKABLE qan::Edge*  insertEdge( QString edgeClassName, qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent = nullptr );

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
    //! Defined for serialization support, do not use.
    virtual auto            createGroup( const std::string& className ) -> WeakGroup;

    //! Shortcut to gtpo::GenGraph<>::insertGroup().
    Q_INVOKABLE qan::Group* insertGroup( QString className = "qan::Group" );

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
    const qan::StyleManager*  getStyleManager( ) const { return _styleManager.data(); }
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

