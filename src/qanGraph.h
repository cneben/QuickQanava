/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include <gtpo/GTpo>

// QuickQanava headers
#include "./qanUtils.h"
#include "./qanGraphConfig.h"
#include "./qanStyleManager.h"
#include "./qanEdge.h"
#include "./qanNode.h"
#include "./qanGroup.h"
#include "./qanNavigable.h"
#include "./qanSelectable.h"
#include "./qanConnector.h"

// Qt headers
#include <QQuickItem>
#include <QQmlParserStatus>
#include <QSharedPointer>
#include <QAbstractListModel>

//! Main QuickQanava namespace
namespace qan { // ::qan

class Graph;
class PortItem;

/*! \brief Main interface to manage graph topology.
 *
 * \nosubgrouping
 */
class Graph : public gtpo::graph<qan::Config>
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    using gtpo_graph_t = gtpo::graph<qan::Config>;

    friend class qan::Selectable;

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
    virtual ~Graph() override;
    Graph( const Graph& ) = delete;
    Graph& operator=( const Graph& ) = delete;
    Graph( Graph&& ) = delete;
    Graph& operator=( Graph&& ) = delete;
public:
    //! QQmlParserStatus Component.onCompleted() overload to initialize default graph delegate in a valid QQmlEngine.
    virtual void    componentComplete() override;

    virtual void    classBegin() override;

public:
    /*! \brief Clear this graph topology and styles.
     *
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
    //! Alias to VisualConnector::connectorColor property (default to DodgerBlue).
    Q_PROPERTY( QColor connectorColor READ getConnectorColor WRITE setConnectorColor NOTIFY connectorColorChanged FINAL )
    inline QColor   getConnectorColor() const noexcept { return _connectorColor; }
    void            setConnectorColor( QColor connectorColor ) noexcept;
signals:
    void            connectorColorChanged();
private:
    QColor          _connectorColor{30, 144, 255};  // dodgerblue=rgb(30, 144, 255)

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
    //! Create a _styleable_ graph primitive using the given delegate \c component with either a source \c node or \c edge.
    QQuickItem*             createFromComponent( QQmlComponent* component,
                                                 qan::Style& style,
                                                 qan::Node* node = nullptr,
                                                 qan::Edge* edge = nullptr,
                                                 qan::Group* group = nullptr ) noexcept;

    //! Shortcut to createComponent(), mainly used in Qan.StyleList View to generate item for style pre visualization.
    Q_INVOKABLE QQuickItem* createFromComponent( QQmlComponent* component, qan::Style* style );

public:
    /*! \brief QML component used to create qan::NodeItem or qan::GroupItem \c selectionItem, could be dynamically changed from either c++ or QML.
     *
     *  \note Using setSelectionDelegate(nullptr) from c++ or Qan.Graph.selectionDelegate=null from QML is valid, QuickQanava will
     *  default to a basic selection item delegate.
     */
    Q_PROPERTY( QQmlComponent* selectionDelegate READ getSelectionDelegate WRITE setSelectionDelegate NOTIFY selectionDelegateChanged FINAL )
    //! \copydoc selectionDelegate
    inline QQmlComponent*   getSelectionDelegate() noexcept { return _selectionDelegate.get(); }
protected:
    //! \copydoc selectionDelegate
    void                    setSelectionDelegate(QQmlComponent* selectionDelegate) noexcept;
    //! \copydoc selectionDelegate
    void                    setSelectionDelegate(std::unique_ptr<QQmlComponent> selectionDelegate) noexcept;
signals:
    //! \copydoc selectionDelegate
    void                    selectionDelegateChanged();
public: // should be considered private
    /*! \brief Create a concrete QQuickItem using the current \c selectionDelegate (private API).
     *
     * \arg parent Returned selection item is automatically reparented to \c parent (could be nullptr).
     * \return A selection item or nullptr if graph \c selectionDelegate is invalid, ownershipd goes to the caller with QmlEngine::CppOwnership, might be nullptr.
     */
    QPointer<QQuickItem>            createSelectionItem(QQuickItem* parent) noexcept;
protected:

    struct QObjectDeleteLater {
        void operator()(QObject *o) {
            o->deleteLater();
        }
    };
    template<typename T>
    using unique_qptr = std::unique_ptr<T, QObjectDeleteLater>;

    std::unique_ptr<QQmlComponent>  _selectionDelegate{nullptr};
private:
    //! Secure factory for QML components, errors are reported on stderr.
    std::unique_ptr<QQmlComponent>  createComponent(const QString& url) noexcept;
    //! Secure utility to create a QQuickItem from a given QML component \c component (might issue warning if component is nullptr or not successfully loaded).
    QPointer<QQuickItem>            createItemFromComponent(QQmlComponent* component) noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    using Node              = typename Config::final_node_t;
    using WeakNode          = std::weak_ptr<typename Config::final_node_t>;
    using SharedNode        = std::shared_ptr<typename Config::final_node_t>;

    //! Proxy for GTpo graph insertNode().
    auto    insertNode( SharedNode node ) noexcept( false ) -> WeakNode { return gtpo_graph_t::insert_node(node); }

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
    Q_INVOKABLE qan::Node*  insertNode( QQmlComponent* nodeComponent = nullptr, qan::NodeStyle* nodeStyle = nullptr );

    //! Insert a node using Node_t::delegate() and Node_t::style(), if no delegate is defined, default on graph \c nodeDelegate.
    template < class Node_t >
    qan::Node*              insertNode( QQmlComponent* nodeComponent = nullptr, qan::NodeStyle* nodeStyle = nullptr);

    //! Same semantic than insertNode<>() but for non visual nodes.
    template < class Node_t >
    qan::Node*              insertNonVisualNode();

    /*! \brief Remove node \c node from this graph. Shortcut to gtpo::GenGraph<>::removeNode().
     */
    Q_INVOKABLE void        removeNode( qan::Node* node );

    //! Shortcut to gtpo::GenGraph<>::getNodeCount().
    Q_INVOKABLE int         getNodeCount() { return gtpo_graph_t::get_node_count(); }

public:
    //! Access the list of nodes with an abstract item model interface.
    Q_PROPERTY( QAbstractItemModel* nodes READ getNodesModel CONSTANT FINAL )
    QAbstractItemModel*     getNodesModel() const { return get_nodes().model(); }

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

    //! Bind an existing edge source to a visual out port from QML.
    Q_INVOKABLE void        bindEdgeSource( qan::Edge* edge, qan::PortItem* outPort ) noexcept;

    //! Bind an existing edge destination to a visual in port from QML.
    Q_INVOKABLE void        bindEdgeDestination( qan::Edge* edge, qan::PortItem* inPort ) noexcept;

    //! Shortcut to bindEdgeSource() and bindEdgeDestination() for edge on \c outPort and \c inPort.
    Q_INVOKABLE void        bindEdge(qan::Edge* edge, qan::PortItem* outPort, qan::PortItem* inPort ) noexcept;

    /*! \brief Test if an edge source is actually bindable to a given port.
     *
     * This method could be used to check if an edge is bindable to a given port
     * _before_ actually creating the edge and calling bindEdgeSource(). Port \c multiplicity
     * and \c connectable properties are taken into account to return a result.
     *
     * Example: for an out port with \c Single \c multiplicity where an out edge already
     * exists, this method returns false.
     */
    virtual bool            isEdgeSourceBindable( const qan::PortItem& outPort) const noexcept;

    /*! \brief Test if an edge destination is bindable to a visual in port.
     *
     * Same behaviour than isEdgeSourceBindable() for edge destination port.
     */
    virtual bool            isEdgeDestinationBindable( const qan::PortItem& inPort ) const noexcept;

    //! Bind an existing edge source to a visual out port.
    virtual void            bindEdgeSource( qan::Edge& edge, qan::PortItem& outPort) noexcept;

    //! Bind an existing edge destination to a visual in port.
    virtual void            bindEdgeDestination( qan::Edge& edge, qan::PortItem& inPort ) noexcept;

public:
    template < class Edge_t >
    qan::Edge*              insertEdge( qan::Node& src, qan::Node* dstNode, QQmlComponent* edgeComponent = nullptr );
private:
    /*! \brief Internal utility used to insert an existing edge \c edge to either a destination \c dstNode node OR edge \c dstEdge.
     *
     * \note insertEdgeImpl() will automatically create \c edge graphical delegate using \c edgeComponent and \c style.
     */
    bool                    configureEdge( qan::Edge& source, QQmlComponent& edgeComponent, qan::EdgeStyle& style,
                                           qan::Node& src, qan::Node* dstNode );
public:
    template < class Edge_t >
    qan::Edge*              insertNonVisualEdge( qan::Node& src, qan::Node* dstNode );

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
    QAbstractItemModel* getEdgesModel() const { return get_edges().model(); }

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

    //! Insert a group using its static delegate() and style() factories.
    template < class Group_t >
    qan::Group*             insertGroup();

    //! Shortcut to gtpo::GenGraph<>::removeGroup().
    Q_INVOKABLE void        removeGroup( qan::Group* group );

    //! Return true if \c group is registered in graph.
    bool                    hasGroup( qan::Group* group ) const;

    //! Shortcut to gtpo::GenGraph<>::getGroupCount().
    Q_INVOKABLE int         getGroupCount( ) const { return gtpo::graph<qan::Config>::get_group_count(); }

    //! \copydoc gtpo::GenGraph::groupNode()
    Q_INVOKABLE void        groupNode( qan::Group* group, qan::Node* node, bool transformPosition = true ) noexcept(false);

    //! Empty, defined to provide a compatible interface for qan::DraggableCtrl<>.
    void                    groupNode( qan::Group*, qan::Group* ) noexcept(false) { }

    //! Ungroup node \c node from group \c group (using nullptr for \c group ungroup node from it's current group without further topology checks).
    Q_INVOKABLE void        ungroupNode( qan::Node* node, qan::Group* group = nullptr) noexcept(false);

    //! Empty, defined to provide a compatible interface for qan::DraggableCtrl<>.
    void                    ungroupNode( qan::Group*, qan::Group* ) noexcept(false) { }

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
    //! Color for the node selection hilgither component (default to dodgerblue).
    Q_PROPERTY( QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged FINAL )
    void            setSelectionColor( QColor selectionColor ) noexcept;
    inline QColor   getSelectionColor() const noexcept { return _selectionColor; }
private:
    QColor          _selectionColor{30, 144, 255};  // dodgerblue=rgb(30, 144, 255)
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

protected:
    /*! \brief Force a call to qan::Selectable::configureSelectionItem() call on all currently selected primitives (either nodes or group).
     */
    void            configureSelectionItems() noexcept;

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
    using SelectedNodes = qcm::Container< QVector, qan::Node* > ;
    using SelectedGroups = qcm::Container< QVector, qan::Group* > ;

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

signals:
    void                rightClicked(QPointF pos);
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

    /*! \name Port/Dock Management *///----------------------------------------
    //@{
public:

    /*! QML interface for adding an in port to node \c node using default port delegate.
     *
     * \note might return nullptr if std::bad_alloc is thrown internally or \c node is invalid or nullptr.
     * \note After creation, you should configure node multiplicity (hability to have multiple connections
     * per port), connecivity default to qan::PortItem::Multiple.
     *
     * \param node  port host node.
     * \param dock  port dock, default to left for in port (either Dock::Top, Dock::Bottom, Dock::Right, Dock::Left).
     * \param type  either an in, out or in/out port (default to in/out).
     * \param label port visible label.
     * \param id    Optional unique ID for the port (caller must ensure uniqueness), could be used to retrieve ports using qan::NodeItem::getPort().
     */
    Q_INVOKABLE qan::PortItem*  insertPort(qan::Node* node,
                                           qan::NodeItem::Dock dock,
                                           qan::PortItem::Type portType = qan::PortItem::Type::InOut,
                                           QString label = "",
                                           QString id = "" ) noexcept;

    /*! Remove a port from a node.
     *
     * \param node  port host node.
     * \param port  node's port to remove.
     */
    Q_INVOKABLE void        removePort(qan::Node* node, qan::PortItem* port) noexcept;

public:
    //! Default delegate for node in/out port.
    Q_PROPERTY( QQmlComponent* portDelegate READ getPortDelegate WRITE qmlSetPortDelegate NOTIFY portDelegateChanged FINAL )
    //! \copydoc portDelegate
    inline QQmlComponent*   getPortDelegate() noexcept { return _portDelegate.get(); }
protected:
    //! \copydoc portDelegate
    void                    qmlSetPortDelegate(QQmlComponent* portDelegate) noexcept;
    //! \copydoc portDelegate
    void                    setPortDelegate(std::unique_ptr<QQmlComponent> portDelegate) noexcept;
signals:
    //! \copydoc portDelegate
    void                    portDelegateChanged();
private:
    //! \copydoc portDelegate
    std::unique_ptr<QQmlComponent> _portDelegate;

signals:
    /*! \brief Emitted whenever a port node registered in this graph is clicked.
     */
    void            portClicked( qan::PortItem* port, QPointF pos );
    /*! \brief Emitted whenever a port node registered in this graph is right clicked.
     */
    void            portRightClicked( qan::PortItem* port, QPointF pos );

public:
    //! Default delegate for horizontal (either NodeItem::Dock::Top or NodeItem::Dock::Bottom) docks.
    Q_PROPERTY( QQmlComponent* horizontalDockDelegate READ getHorizontalDockDelegate WRITE setHorizontalDockDelegate NOTIFY horizontalDockDelegateChanged FINAL )
    //! \copydoc horizontalDockDelegate
    inline QQmlComponent*   getHorizontalDockDelegate() noexcept { return _horizontalDockDelegate.get(); }
protected:
    //! \copydoc horizontalDockDelegate
    void                    setHorizontalDockDelegate(QQmlComponent* horizontalDockDelegate) noexcept;
    //! \copydoc horizontalDockDelegate
    void                    setHorizontalDockDelegate(std::unique_ptr<QQmlComponent> horizontalDockDelegate) noexcept;
signals:
    //! \copydoc horizontalDockDelegate
    void                    horizontalDockDelegateChanged();
private:
    //! \copydoc horizontalDockDelegate
    std::unique_ptr<QQmlComponent> _horizontalDockDelegate;

public:
    //! Default delegate for vertical (either NodeItem::Dock::Left or NodeItem::Dock::Right) docks.
    Q_PROPERTY( QQmlComponent* verticalDockDelegate READ getVerticalDockDelegate WRITE setVerticalDockDelegate NOTIFY verticalDockDelegateChanged FINAL )
    //! \copydoc horizontalDockDelegate
    inline QQmlComponent*   getVerticalDockDelegate() noexcept { return _verticalDockDelegate.get(); }
protected:
    //! \copydoc horizontalDockDelegate
    void                    setVerticalDockDelegate(QQmlComponent* verticalDockDelegate) noexcept;
    //! \copydoc horizontalDockDelegate
    void                    setVerticalDockDelegate(std::unique_ptr<QQmlComponent> verticalDockDelegate) noexcept;
signals:
    //! \copydoc horizontalDockDelegate
    void                    verticalDockDelegateChanged();
private:
    //! \copydoc verticalDockDelegate
    std::unique_ptr<QQmlComponent> _verticalDockDelegate;

protected:
    //! Create a dock item from an existing dock item delegate.
    QPointer<QQuickItem>     createDockFromDelegate(qan::NodeItem::Dock dock, qan::Node& node) noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node auto-positioning *///----------------------------------------
    //@{
public:
    //! Auto-position nodes using Graphviz's dot layout.
    Q_INVOKABLE void autoPositionNodes() noexcept;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

#include "./qanGraph.hpp"

QML_DECLARE_TYPE( qan::Graph::WeakNode )

#endif // qanGraph_h

