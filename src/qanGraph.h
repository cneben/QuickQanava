/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

#ifndef qanGraph_h
#define qanGraph_h

// GTpo headers
#include <GTpo>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanStyleManager.h"
#include "./qanEdge.h"
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanNavigable.h"

// Qt headers
#include <QQuickItem>
#include <QQmlParserStatus>
#include <QSharedPointer>
#include <QAbstractListModel>

//! Main QuickQanava namespace
namespace qan { // ::qan

class Graph;
class Connector;

/*! \brief Main interface to manage graph topology.
 *
 * \nosubgrouping
 */
class Graph : public gtpo::GenGraph< qan::GraphConfig >
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    using GTpoGraph = gtpo::GenGraph< qan::GraphConfig >;

    /*! \name Graph Object Management *///-------------------------------------
    //@{
public:
    //! Graph default constructor.
    explicit Graph( QQuickItem* parent = nullptr ) noexcept;
    /*! \brief Graph default destructor.
     *
     * Graph is a factory for inserted nodes and edges, even if they have been created trought
     * QML delegates, they will be destroyed with the graph they have been created in.
     */
    virtual ~Graph();
    Graph( const Graph& ) = delete;
public:
    //! QQmlParserStatus Component.onCompleted() overload to initialize default graph delegate in a valid QQmlEngine.
    void    componentComplete() override;

public:
    /*! \brief Clear this graph topology and styles.
     *
     * \note Registered  node and edge delegates are not cleared, you must manually call clearDelegate()
     * to clear the delegates registered with registerNodeDelegate() and registerEdgeDelegate().
     */
    Q_INVOKABLE virtual void    qmlClearGraph() noexcept;
    void                        clear() noexcept;

public:
    /*! \brief Similar to QQuickItem::childAt() method, except that it take edge bounding shape into account.
     *
     * Using childAt() method will most of the time return qan::Edge items since childAt() use bounding boxes
     * for item detection.
     *
     * \return nullptr if there is no child at requested position, or a QQuickItem that can be casted qan::Node, qan::Edge or qan::Group with qobject_cast<>.
     */
    Q_INVOKABLE QQuickItem* graphChildAt(qreal x, qreal y) const;

    /*! \brief Similar to QQuickItem::childAt() method, except that it only take groups into account (and is hence faster, but still O(n)).
     *
     */
    Q_INVOKABLE qan::Group* groupAt( const QPointF& p, const QSizeF& s ) const;

public:
    /*! \brief Quick item used as a parent for all graphics item "factored" by this graph (default to this).
     *
     * \note Container item should be initialized at startup, any change will _not_ be refelected to existing
     * graphics items.
     */
    Q_PROPERTY( QQuickItem* containerItem READ getContainerItem NOTIFY containerItemChanged FINAL )
    //! \sa containerItem
    inline QQuickItem*          getContainerItem() noexcept { return _containerItem.data(); }
    inline const QQuickItem*    getContainerItem() const noexcept { return _containerItem.data(); }
    void                        setContainerItem( QQuickItem* containerItem );
signals:
    void                        containerItemChanged();
private:
    QPointer< QQuickItem >      _containerItem{nullptr};
    //@}
    //-------------------------------------------------------------------------

    /*! \name Visual connection Management *///--------------------------------
    //@{
public:
    /*! \brief Set the visual connector source node.
     *
     * \note If \c sourceNode is nullptr, visual connector is hidden.
     */
    Q_INVOKABLE void    setConnectorSource(qan::Node* sourceNode) noexcept;
signals:
    //! \copydoc hlg::Connector::requestEdgeCreation
    void                connectorRequestEdgeCreation(qan::Node* src, QObject* dst);
    //! \copydoc hlg::Connector::edgeInserted
    void                connectorEdgeInserted( qan::Edge* edge );

public:
    //! Alias to VisualConnector::edgeColor property (default to Black).
    Q_PROPERTY( QColor connectorEdgeColor READ getConnectorEdgeColor WRITE setConnectorEdgeColor NOTIFY connectorEdgeColorChanged FINAL )
    inline QColor   getConnectorEdgeColor() const noexcept { return _connectorEdgeColor; }
    void            setConnectorEdgeColor( QColor connectorEdgeColor ) noexcept;
signals:
    void            connectorEdgeColorChanged();
private:
    QColor          _connectorEdgeColor{Qt::black};

public:
    //! Alias to VisualConnector::connectorColor property (default to DarkBlue).
    Q_PROPERTY( QColor connectorColor READ getConnectorColor WRITE setConnectorColor NOTIFY connectorColorChanged FINAL )
    inline QColor   getConnectorColor() const noexcept { return _connectorColor; }
    void            setConnectorColor( QColor connectorColor ) noexcept;
signals:
    void            connectorColorChanged();
private:
    QColor          _connectorColor{Qt::darkBlue};   // #00008B = DarkBlue

public:
    //! Alias to VisualConnector::hEdgeEnabled (default to false).
    Q_PROPERTY( bool connectorHEdgeEnabled READ getConnectorHEdgeEnabled WRITE setConnectorHEdgeEnabled NOTIFY connectorHEdgeEnabledChanged FINAL )
    inline bool     getConnectorHEdgeEnabled() const noexcept { return _connectorHEdgeEnabled; }
    void            setConnectorHEdgeEnabled( bool connectorHEdgeEnabled ) noexcept;
signals:
    void            connectorHEdgeEnabledChanged();
private:
    bool            _connectorHEdgeEnabled{false};

public:
    //! Alias to VisualConnector::createDefaultEdge (default to true).
    Q_PROPERTY( bool connectorCreateDefaultEdge READ getConnectorCreateDefaultEdge WRITE setConnectorCreateDefaultEdge NOTIFY connectorCreateDefaultEdgeChanged FINAL )
    inline bool     getConnectorCreateDefaultEdge() const noexcept { return _connectorCreateDefaultEdge; }
    void            setConnectorCreateDefaultEdge( bool connectorCreateDefaultEdge ) noexcept;
signals:
    void            connectorCreateDefaultEdgeChanged();
private:
    bool            _connectorCreateDefaultEdge{true};

public:
    //! Alias to qan::Connector::connectorItem property (default to nullptr, ie default connector item).
    Q_PROPERTY( QQuickItem* connectorItem READ getConnectorItem WRITE setConnectorItem NOTIFY connectorItemChanged FINAL )
    inline QQuickItem*      getConnectorItem() const noexcept { return _connectorItem; }
    void                    setConnectorItem( QQuickItem* connectorItem ) noexcept;
signals:
    void                    connectorItemChanged();
private:
    QPointer<QQuickItem>    _connectorItem{nullptr};

public:
    //! Enable or disable visual connector of nodes in the graph (default to false).
    Q_PROPERTY( bool connectorEnabled READ getConnectorEnabled WRITE setConnectorEnabled NOTIFY connectorEnabledChanged FINAL )
    inline bool     getConnectorEnabled() const noexcept { return _connectorEnabled; }
    void            setConnectorEnabled( bool connectorEnabled ) noexcept;
signals:
    void            connectorEnabledChanged();
private:
    bool            _connectorEnabled{false};

public:
    //! Control node used as a connector when \c connectorEnabled is set to true (might be nullptr).
    Q_PROPERTY( qan::Connector* connector READ getConnector NOTIFY connectorChanged FINAL )
    qan::Connector*             getConnector() noexcept;
private:
    QScopedPointer<qan::Connector> _connector{};
signals:
    void                        connectorChanged();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Delegates Management *///----------------------------------------
    //@{
public:
    //! Default delegate for qan::Node and Qan.Node nodes.
    Q_PROPERTY( QQmlComponent* nodeDelegate READ getNodeDelegate WRITE setNodeDelegate NOTIFY nodeDelegateChanged FINAL )
    inline QQmlComponent*   getNodeDelegate() noexcept { return _nodeDelegate.get(); }
protected:
    void                    setNodeDelegate(QQmlComponent* nodeDelegate) noexcept;
    void                    setNodeDelegate(std::unique_ptr<QQmlComponent> nodeDelegate) noexcept;
signals:
    void                    nodeDelegateChanged();
private:
    std::unique_ptr<QQmlComponent> _nodeDelegate;

public:
    //! Default delegate for qan::Edge and Qan.Edge edges.
    Q_PROPERTY( QQmlComponent* edgeDelegate READ getEdgeDelegate WRITE setEdgeDelegate NOTIFY edgeDelegateChanged FINAL )
    inline QQmlComponent*   getEdgeDelegate() noexcept { return _edgeDelegate.get(); }
protected:
    void                    setEdgeDelegate(QQmlComponent* edgeDelegate) noexcept;
    void                    setEdgeDelegate(std::unique_ptr<QQmlComponent> edgeDelegate) noexcept;
signals:
    void                    edgeDelegateChanged();
private:
    std::unique_ptr<QQmlComponent> _edgeDelegate;

public:
    //! Default delegate for qan::Group and Qan.Group groups.
    Q_PROPERTY( QQmlComponent* groupDelegate READ getGroupDelegate WRITE setGroupDelegate NOTIFY groupDelegateChanged FINAL )
    inline QQmlComponent*   getGroupDelegate() noexcept { return _groupDelegate.get(); }
protected:
    void                    setGroupDelegate(QQmlComponent* groupDelegate) noexcept;
    void                    setGroupDelegate(std::unique_ptr<QQmlComponent> groupDelegate) noexcept;
signals:
    void                    groupDelegateChanged();
private:
    std::unique_ptr<QQmlComponent> _groupDelegate;

protected:
    //! Create a graph primitive using the given delegate \c component with either a source \c node or \c edge.
    QQuickItem*             createFromComponent( QQmlComponent* component,
                                                 qan::Style& style,
                                                 qan::Node* node = nullptr,
                                                 qan::Edge* edge = nullptr,
                                                 qan::Group* group = nullptr );

    //! Shortcut to createComponent(), mainly used in Qan.StyleList View to generate item for style pre visualization.
    Q_INVOKABLE QQuickItem* createFromComponent( QQmlComponent* component, qan::Style* style );

    //! Secure utility to set QQmlEngine::CppOwnership flag on a given Qt quick item.
    static void             setCppOwnership( QQuickItem* item );

public:
    /*! \brief Create a Qt Quick Rectangle object (caller get ownership for the object flagged with CppOwnership).
     *
     * \note Internally used to generate selection rectangles around node, but part of the public API.
     */
    QQuickItem*                     createRectangle( QQuickItem* parent );
private:
    std::unique_ptr<QQmlComponent>  _rectangleComponent{nullptr};
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    using Node              = typename GraphConfig::FinalNode;
    using WeakNode          = GTpoGraph::WeakNode;

    /*! \brief Insert a new node in this graph and return a pointer on it, or \c nullptr if creation fails.
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

    template < class Node_t >
    qan::Node*              insertNode( QQmlComponent* nodeComponent = nullptr );

    /*! \brief Remove node \c node from this graph. Shortcut to gtpo::GenGraph<>::removeNode().
     */
    Q_INVOKABLE void        removeNode( qan::Node* node );

    //! Shortcut to gtpo::GenGraph<>::getNodeCount().
    Q_INVOKABLE int         getNodeCount() { return GTpoGraph::getNodeCount(); }

public:
    //! Access the list of nodes with an abstract item model interface.
    Q_PROPERTY( QAbstractItemModel* nodes READ getNodesModel CONSTANT FINAL )
    QAbstractItemModel*     getNodesModel() const { return const_cast<QAbstractItemModel*>( static_cast<const QAbstractItemModel*>(&getNodes())); }

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
    //! QML shortcut to insertEdge() method.
    Q_INVOKABLE qan::Edge*  insertEdge( QObject* source, QObject* destination, QQmlComponent* edgeComponent = nullptr );

    //! Shortcut to gtpo::GenGraph<>::insertEdge().
    virtual qan::Edge*      insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent = nullptr );

    //! Shortcut to gtpo::GenGraph<>::insertEdge().
    virtual qan::Edge*      insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent = nullptr );

protected:
    template < class Edge_t >
    qan::Edge*              insertEdge( qan::Node& src, qan::Node* dstNode, qan::Edge* dstEdge = nullptr, QQmlComponent* edgeComponent = nullptr );

public:
    //! Shortcut to gtpo::GenGraph<>::removeEdge().
    Q_INVOKABLE void        removeEdge( qan::Node* source, qan::Node* destination );

    //! Shortcut to gtpo::GenGraph<>::removeEdge().
    Q_INVOKABLE void        removeEdge( qan::Edge* edge );

    //! Return true if there is at least one directed edge between \c source and \c destination (Shortcut to gtpo::GenGraph<>::hasEdge()).
    Q_INVOKABLE bool        hasEdge( qan::Node* source, qan::Node* destination ) const;

public:
    //! Access the list of edges with an abstract item model interface.
    Q_PROPERTY( QAbstractItemModel* edges READ getEdgesModel CONSTANT FINAL )
    QAbstractItemModel* getEdgesModel() const { return const_cast<QAbstractItemModel*>( static_cast<const QAbstractItemModel*>(&getEdges())); }

signals:
    /*! \brief Emitted whenever a node registered in this graph is clicked.
     *
     *  \sa nodeClicked()
     */
    void            edgeClicked( qan::Edge* edge, QPointF pos );
    /*! \brief Emitted whenever a node registered in this graph is right clicked.
     *
     *  \sa nodeRightClicked()
     */
    void            edgeRightClicked( qan::Edge* edge, QPointF pos );
    /*! \brief Emitted whenever a node registered in this graph is double clicked.
     *
     *  \sa nodeDoubleClicked()
     */
    void            edgeDoubleClicked( qan::Edge* edge, QPointF pos );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Group Management *///--------------------------------------
    //@{
public:
    //! Shortcut to gtpo::GenGraph<>::insertGroup().
    Q_INVOKABLE qan::Group* insertGroup();

    //! Shortcut to gtpo::GenGraph<>::removeGroup().
    Q_INVOKABLE void        removeGroup( qan::Group* group );

    //! Return true if \c group is registered in graph.
    bool                    hasGroup( qan::Group* group ) const;

    //! Shortcut to gtpo::GenGraph<>::getGroupCount().
    Q_INVOKABLE int         getGroupCount( ) const { return gtpo::GenGraph< qan::GraphConfig >::getGroupCount(); }

    //! \copydoc gtpo::GenGraph::groupNode()
    auto            groupNode( qan::Group* group, qan::Node* node ) noexcept(false) -> void;

    //! \copydoc gtpo::GenGraph::groupNode()
    auto            groupNode( qan::Group* group, qan::Group* node ) noexcept(false) -> void;

    //! \copydoc gtpo::GenGraph::ungroupNode()
    auto            ungroupNode( Group* group, qan::Node* node ) noexcept(false) -> void;

    //! \copydoc gtpo::GenGraph::ungroupNode()
    auto            ungroupNode( Group* group, qan::Group* node ) noexcept(false) -> void;

signals:
    /*! \brief Emitted when a group registered in this graph is clicked.
     */
    void            groupClicked( qan::Group* group, QPointF pos );
    /*! \brief Emitted when a group registered in this graph is right clicked.
     */
    void            groupRightClicked( qan::Group* group, QPointF pos );
    /*! \brief Emitted when a group registered in this graph is double clicked.
     */
    void            groupDoubleClicked( qan::Group* group, QPointF pos );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Selection Management *///----------------------------------------
    //@{
public:
    /*! \brief Graph node selection policy (default to \c SelectOnClick);
     * \li \c NoSelection: Selection of nodes is disabled.
     * \li \c SelectOnClick (default): Node is selected when clicked, multiple selection is allowed with CTRL.
     * \li \c SelectOnCtrlClick: Node is selected only when CTRL is pressed, multiple selection is allowed with CTRL.
     */
    enum SelectionPolicy { NoSelection, SelectOnClick, SelectOnCtrlClick };
    Q_ENUM(SelectionPolicy)

public:
    /*! \brief Current graph selection policy.
     *
     * \warning setting NoSeleciton will clear the actual \c selectedNodes model.
     */
    Q_PROPERTY( SelectionPolicy selectionPolicy READ getSelectionPolicy WRITE setSelectionPolicy NOTIFY selectionPolicyChanged FINAL )
    void                    setSelectionPolicy( SelectionPolicy selectionPolicy ) noexcept;
    inline SelectionPolicy  getSelectionPolicy() const noexcept { return _selectionPolicy; }
private:
    SelectionPolicy         _selectionPolicy{ SelectionPolicy::SelectOnClick };
signals:
    void                    selectionPolicyChanged();

public:
    //! Color for the node selection hilgither component (default to dark blue).
    Q_PROPERTY( QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged FINAL )
    void            setSelectionColor( QColor selectionColor ) noexcept;
    inline QColor   getSelectionColor() const noexcept { return _selectionColor; }
private:
    QColor          _selectionColor{ Qt::darkBlue };
signals:
    void            selectionColorChanged();

public:
    //! Selection hilgither item stroke width (default to 3.0).
    Q_PROPERTY( qreal selectionWeight READ getSelectionWeight WRITE setSelectionWeight NOTIFY selectionWeightChanged FINAL )
    void            setSelectionWeight( qreal selectionWeight ) noexcept;
    inline qreal    getSelectionWeight() const noexcept { return _selectionWeight; }
private:
    qreal           _selectionWeight{ 3. };
signals:
    void            selectionWeightChanged( );

public:
    //! Margin between the selection hilgither item and a selected item (default to 3.0).
    Q_PROPERTY( qreal selectionMargin READ getSelectionMargin WRITE setSelectionMargin NOTIFY selectionMarginChanged FINAL )
    void            setSelectionMargin( qreal selectionMargin ) noexcept;
    inline qreal    getSelectionMargin() const noexcept { return _selectionMargin; }
private:
    qreal           _selectionMargin{ 3. };
signals:
    void            selectionMarginChanged();

public:
    /*! \brief Request insertion of a node in the current selection according to current policy and return true if the node was successfully added.
     *
     * \note If \c selectionPolicy is set to Qan.AbstractGraph.NoSelection or SelextionPolicy::NoSelection,
     * method will always return false.
     */
    bool            selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers );

    //! Similar to selectNode() for qan::Group (internally group is a node).
    bool            selectGroup( qan::Group& group, Qt::KeyboardModifiers modifiers );

    /*! \brief Add a node in the current selection.
     */
    void            addToSelection( qan::Node& node );
    //! \copydoc addToSelection
    void            addToSelection( qan::Group& node );

    //! Remove a node from the selection.
    void            removeFromSelection( qan::Node& node );
    //! \copydoc removeFromSelection
    void            removeFromSelection( qan::Group& node );
    //! \copydoc removeFromSelection
    void            removeFromSelection( QQuickItem* item );

    //! Clear the current selection.
    void            clearSelection();

    //! Return true if multiple node are selected.
    inline  bool    hasMultipleSelection() const noexcept { return _selectedNodes.size() > 0 || _selectedGroups.size() > 0; }

public:
    using SelectedNodes = qcm::ContainerModel< QVector, qan::Node* > ;
    using SelectedGroups = qcm::ContainerModel< QVector, qan::Group* > ;

    //! Read-only list model of currently selected nodes.
    Q_PROPERTY( QAbstractListModel* selectedNodes READ getSelectedNodesModel NOTIFY selectedNodesChanged FINAL )
    QAbstractListModel* getSelectedNodesModel() { return qobject_cast<QAbstractListModel*>( &_selectedNodes ); }

    inline auto         getSelectedNodes() noexcept -> SelectedNodes& { return _selectedNodes; }
    inline auto         getSelectedNodes() const noexcept -> const SelectedNodes& { return _selectedNodes; }
signals:
    void                selectedNodesChanged();
public:
    inline auto         getSelectedGroups() noexcept -> SelectedGroups& { return _selectedGroups; }
    inline auto         getSelectedGroups() const noexcept -> const SelectedGroups& { return _selectedGroups; }
private:
    SelectedNodes       _selectedNodes;
    SelectedGroups      _selectedGroups;

protected:
    virtual void        mousePressEvent(QMouseEvent* event ) override;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    /*! \brief Graph style manager (ie list of style applicable to graph primitive).
     */
    Q_PROPERTY( qan::StyleManager* styleManager READ getStyleManager CONSTANT FINAL )
    inline qan::StyleManager*       getStyleManager() noexcept { return &_styleManager; }
    inline const qan::StyleManager* getStyleManager() const noexcept { return &_styleManager; }
private:
    qan::StyleManager   _styleManager;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

#include "./qanGraph.hpp"

QML_DECLARE_TYPE( qan::Graph::WeakNode )

#endif // qanGraph_h

