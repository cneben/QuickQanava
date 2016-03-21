/*
    The MIT License (MIT)

    Copyright (c) 2016 Benoit AUTHEMAN

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software library.
//
// \file	gtpoGraph.h
// \author	benoit@qanava.org
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef gtpoGraph_h
#define gtpoGraph_h

// STD headers
#include <list>
#include <unordered_set>
#include <memory>           // std::shared_ptr std::weak_ptr and std::make_shared
#include <functional>       // std::hash
#include <cassert>
#include <iterator>         // std::back_inserter

// Qan.Topo headers
#include "./gtpoUtils.h"
#include "./gtpoBehaviour.h"

/*! \brief Main GTpo namespace (\#include \<GTpo\>).
 */
namespace gtpo { // ::gtpo

template <class Config>
class GenGraph;

template <class Config>
class GenNode;

template <class Config>
class GenEdge;

template < typename T >
using Owned = std::shared_ptr< T >;

/*! Configuration interface for accessing standard library (STL) containers.
 *
 */
struct StdContainerConfig : public ContainerConfig {
    template <typename T>
    struct insert { };

    template <typename T>
    struct insert< std::list<T> > {
        static void  into( std::list<T>& c, T t ) { c.push_back( t ); }
    };
    template <typename T>
    struct insert< std::vector<T> > {
        static void  into( std::vector<T>& c, T t ) { c.emplace_back( t ); }
    };
    template <typename T>
    struct insert< std::unordered_set<T> > {
        static void  into( std::unordered_set<T>& c, T t ) { c.insert( t ); }
    };

    template <typename T>
    struct remove { };

    template <typename T>
    struct remove< std::list<T> > {
        static void  from( std::list<T>& c, const T t ) { c.erase( std::remove(c.begin(), c.end(), t), c.end()); }
    };

    template <typename T>
    struct remove< std::unordered_set<T> > {
        static void  from( std::unordered_set<T>& c, const T t ) { c.erase( t ); }
    };
    template <typename T>
    struct remove< std::vector<std::weak_ptr<T>> > {
        static void  from( std::vector<std::weak_ptr<T>>& c, const std::weak_ptr<T> t ) {  // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
            c.erase( std::remove_if(c.begin(), c.end(), [=](const std::weak_ptr<T>& wp){
                    return gtpo::compare_weak_ptr( wp, t );
                }), c.end());
            }
    };
    template <typename T>
    struct remove< std::vector<std::shared_ptr<T>> > {
        static void  from( std::vector<std::shared_ptr<T>>& c, const std::shared_ptr<T> t ) {  // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
            c.erase( std::remove(c.begin(), c.end(), t), c.end()); }
    };
};

/*! Default empty "no-action" accessors for GTpo primitives base classes (GenNode and GenEdge).
 *
 */
template < typename Node, typename Edge >
struct PropertiesConfig {
    using SharedNode = std::shared_ptr< Node >;
    using SharedEdge = std::shared_ptr< Edge >;

    static inline std::string   getNodeLabel( const SharedNode& /*n*/ ) { return std::string( "" ); }
    static inline void          setNodeLabel( SharedNode& /*n*/, const std::string& ) { }

    static inline double        getNodeX( const SharedNode& /*n*/ ) { return 0.; }
    static inline void          setNodeX( SharedNode& /*n*/, double /*x*/ ) { }

    static inline double        getNodeY( const SharedNode& /*n*/ ) { return 0.; }
    static inline void          setNodeY( SharedNode& /*n*/, double /*y*/ ) { }

    static inline double        getNodeWidth( const SharedNode& /*n*/ ) { return 0.; }
    static inline void          setNodeWidth( SharedNode& /*n*/, double /*w*/ ) { }

    static inline double        getNodeHeight( const SharedNode& /*n*/ ) { return 0.; }
    static inline void          setNodeHeight( SharedNode& /*n*/, double /*h*/ ) { }

    static inline double        getEdgeWeight( const SharedEdge& /*e*/ ) { return 0.; }
    static inline void          setEdgeWeight( SharedEdge& /*e*/, double /*w*/ ) { }
};

/*! Default configuration for GTpo primitives base classes.
 *
 */
struct BaseConfig {
    using GraphBase = Empty;
    using NodeBase = Empty;
    using EdgeBase = Empty;
};

struct DefaultConfig :  public BaseConfig,
                        public StdContainerConfig,
                        public PropertiesConfig<GenNode<DefaultConfig>,
                                                GenEdge<DefaultConfig>>
{
    using Node = GenNode<DefaultConfig>;
    using Edge = GenEdge<DefaultConfig>;

    template <class...Ts>
    using NodeContainer = std::vector<Ts...>;

    template <class...Ts>
    using EdgeContainer = std::vector<Ts...>;

    template <class T>
    using SearchContainer = std::unordered_set<T>;
};

//! Directed edge linking two nodes in a graph.
/*!
    \nosubgrouping
 */
template <class Config = DefaultConfig>
class GenEdge : public Config::EdgeBase,
                public std::enable_shared_from_this<typename Config::Edge>
{
    friend GenGraph<Config>;   // GenGraph need access to setGraph()
public:
    using Graph         = GenGraph<Config>;
    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using SharedNode    = std::shared_ptr< typename Config::Node >;

    GenEdge() : Config::EdgeBase( ) { }
    explicit GenEdge( typename Config::EdgeBase* parent ) : Config::EdgeBase( parent ) { }
    explicit GenEdge( WeakNode& src, WeakNode& dst ) :
        _src{ src }, _dst{ dst } { }
    virtual ~GenEdge() {
        //std::cout << "GenEdge::~GenEdge()" << this << std::endl;
        if ( _graph != nullptr ) {
            std::cerr << "GenEdge::~GenEdge(): Error, an edge has been deleted before beeing " <<
                         "removed from the graph." << std::endl;
            //_graph->removeEdge( static_cast< typename Config::Edge& >( *this ) );
        }
        _graph = nullptr;
    }

    //! Return node class name (default to "gtpo::Edge").
    std::string getClassName() const { return "gtpo::Edge"; }

    void    setSrc( WeakNode src ) { _src = src; }
    void    setDst( WeakNode dst ) { _dst = dst; }
    auto    getSrc( ) -> WeakNode const { return _src; }
    auto    getDst( ) -> WeakNode const { return _dst; }

private:
    WeakNode  _src;
    WeakNode  _dst;

protected:
    Graph*      getGraph() { return _graph; }
private:
    void        setGraph( Graph* graph ) { _graph = graph; }
    Graph*      _graph = nullptr;
};

/*! \brief Base class for modelling nodes with an in/out edges list in a qan::Graph graph.
 *
 * \nosubgrouping
*/
template <class Config = DefaultConfig>
class GenNode : public Config::NodeBase,
                public std::enable_shared_from_this<typename Config::Node>
{
    friend GenGraph<Config>;   // GenGraph need access to setGraph()

    /*! \name Node Management *///---------------------------------------------
    //@{
public:
    using Graph         = GenGraph<Config>;
    using WeakNode      = std::weak_ptr< typename Config::Node >;
    using SharedNode    = std::shared_ptr< typename Config::Node >;
    using WeakNodes     = typename Config::template NodeContainer< WeakNode >;

    GenNode() : Config::NodeBase( ) { }
    explicit GenNode( typename Config::NodeBase* parent ) : Config::NodeBase( parent ) { }
    virtual ~GenNode() {
        _inEdges.clear(); _outEdges.clear();
        _inNodes.clear(); _outNodes.clear();
        //std::cout << "GenNode::~GenNode()" << this << std::endl;
        if ( _graph != nullptr ) {
            //_graph->removeNode( static_cast< typename Config::Node& >( *this ) );
            //std::shared_ptr<GenNode<Config>>
            /*typename Config::Node* self = static_cast< typename Config::Node* >( this );
            SharedNode ownedSelf = self->shared_from_this();
            std::weak_ptr<GenNode<Config>> sharedSelf = self;
            _graph->removeNode( sharedSelf );*/
            std::cerr << "~GenNode(): A node has been destroyed before beeing removed " <<
                         "from the graph, something bas happen, GTpo will try to recover " <<
                         "graph topology." << std::endl;
        }
        _graph = nullptr;
    }
    GenNode(const GenNode& node ) = delete;
    GenNode& operator=( GenNode const& ) = delete;

    //! Return node class name (default to "gtpo::Node").
    std::string getClassName() const { return "gtpo::Node"; }

protected:
    Graph*      getGraph() { return _graph; }
private:
    void        setGraph( Graph* graph ) { _graph = graph; }
    Graph*      _graph = nullptr;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Meta Properties *///----------------------------------------
    //@{
public:
    //! Get the node current serializable property (false=not serializable, for example a control node).
    inline  auto    getSerializable( ) const -> bool { return _serializable; }
    //! Shortcut to getSerializable().
    inline  auto    isSerializable( ) const -> bool { return getSerializable(); }
    //! Change the node serializable property (it will not trigger a node changed call in graph behaviour).
    inline  auto    setSerializable( bool serializable ) -> void { _serializable = serializable; }
private:
    //! Node serializable property (default to true ie serializable).
    bool            _serializable = true;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Node Edges Management *///---------------------------------------
    //@{
public:
    using WeakEdge      = std::weak_ptr< typename Config::Edge >;
    using WeakEdges     = typename Config::template EdgeContainer< WeakEdge >;
    using SharedEdge    = std::shared_ptr< typename Config::Edge >;

    /*! \brief Insert edge \c outEdge as an out edge for this node.
     *
     * \note if \c outEdge source node is different from this node, it is set to this node.
     */
    auto    addOutEdge( WeakEdge sharedOutEdge ) noexcept( false ) -> void;
    /*! \brief Insert edge \c inEdge as an in edge for \c node.
     *
     * \note if \c inEdge destination node is different from \c node, it is automatically set to \c node.
     */
    auto    addInEdge( WeakEdge sharedInEdge ) noexcept( false ) -> void;
    /*! \brief Remove edge \c outEdge from this node out edges.
     *
     * \throw gtpo::bad_topology_error
     */
    auto    removeOutEdge( const WeakEdge outEdge ) noexcept( false ) -> void;
    /*! \brief Remove edge \c inEdge from this node in edges.
     *
     * \throw gtpo::bad_topology_error
     */
    auto    removeInEdge( const WeakEdge inEdge ) noexcept( false ) -> void;

    auto    getInEdges() const -> const WeakEdges& { return _inEdges; }
    auto    getOutEdges() const -> const WeakEdges& { return _outEdges; }

    auto    getInNodes() const -> const WeakNodes& { return _inNodes; }
    auto    getOutNodes() const -> const WeakNodes& { return _outNodes; }

    auto    getInDegree() -> typename WeakEdges::size_type const { return _inEdges.size(); }
    auto    getOutDegree() -> typename WeakEdges::size_type const { return _outEdges.size(); }
private:
    WeakEdges   _inEdges;
    WeakEdges   _outEdges;
    WeakNodes   _inNodes;
    WeakNodes   _outNodes;
    //@}
    //-------------------------------------------------------------------------
};

/*! \brief Weighted directed graph using a node-list, edge-list representation.
 *
 * \image html graph-datamodel.png
 *
 * \param BaseClass Optional base class for this graph class
 *
 * \note See http://en.cppreference.com/w/cpp/language/dependent_name for
 *       typename X::template T c++11 syntax and using Nodes = typename Config::template NodeContainer< Node* >;
 *
 */
template < class Config = DefaultConfig >
class GenGraph : public Config::GraphBase
{
    /*! \name Graph Management *///--------------------------------------------
    //@{
public:
    using Configuration = Config;
    using Node = typename Config::Node;
    using SharedNode = std::shared_ptr< typename Config::Node >;
    using SharedEdge = std::shared_ptr< typename Config::Edge >;
    using WeakNode  = std::weak_ptr< typename Config::Node >;
    using WeakEdge  = std::weak_ptr< typename Config::Edge >;
    using WeakEdges = typename Config::template EdgeContainer< WeakEdge >;
    using Graph     = GenGraph<Config>;

    using SharedNodes       = typename Config::template NodeContainer< SharedNode >;
    using WeakNodes         = typename Config::template NodeContainer< WeakNode >;
    using WeakNodesSearch   = typename Config::template SearchContainer< WeakNode >;
    using SharedEdges       = typename Config::template EdgeContainer< SharedEdge >;

public:
    using Size  = typename SharedNodes::size_type;

    GenGraph() : Config::GraphBase() { }
    explicit GenGraph( typename Config::GraphBase* parent ) : Config::GraphBase( parent ) { }
    virtual ~GenGraph();

    GenGraph( GenGraph const& ) = delete;
    GenGraph& operator=( GenGraph const& ) = delete;

    /*! Clear the graph from all its content (nodes, edges, groups, behaviours, layouts).
     *
     * \note Graph behaviours are cleared after the topology, if you do not want to take into account topology
     * changes when clearing the graph, disable all behaviours before calling clear().
     */
    void    clear();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Node Management *///---------------------------------------
    //@{
public:
    /*! \brief Create node and insert it in the graph an return a reference to it.
     *
     * \code
     * try{
     *   gtpo::GenGraph<> g;
     *   std::weak_ptr<gtpo::GenNode<>> n1 = g.createNode();
     *   // or preferably
     *   auto n2 = g.createNode();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     *
     * With STpo:
     * \code
     * using namespace gtpo;
     * try{
     *   stpo::Graph g;
     *   std::weak_ptr<stpo::Node> n1 = g.createNode();
     *   // or preferably
     *   auto n2 = g.createNode();
     * } catch ( const std::runtime_error& e ) { }
     * \endcode
     * \return a reference to the created node (graph has ownership for the node).
     * \throw gtpo::bad_topology_error with an error description if creation fails.
     */
    auto    createNode( ) noexcept( false ) -> WeakNode;

    /*! Create a node with the given \c className and insert it into the graph.
     *
     * Default implementation only create node and edges corresponding to gtpo::GenNode::getClassName().
     * This method is mainly usefull when a hierarchy of virtual node should be serialized.
     * Always prefere createNode() and insertNode( SharedNode ) to add content in the graph until you
     * are writing custom in/out serialization functors for a serializer dealing with specialized version
     * of GenNode (see gtpo::ProtoSerializer documentation for more informations).
     *
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    virtual auto    createNode( const std::string& nodeClassName ) noexcept( false ) -> WeakNode;

    /*! Insert a node created outside of GTpo into the graph.
     *
     * If your nodes must be created outside of GTpo (ie not with the createNode() method),
     * the only way of giving node ownership to GTpo is trought the insertNode method.
     * Example of a node created from a QML component:
     * \code
     *  qan::Node* node = static_cast< qan::Node* >( createFromQmlComponent( nodeComponent ) );
     *  if ( node != nullptr )
     *    GenGraph<QGraphcConfig>::insertNode( std::shared_ptr<qan::Node>{node} );
     * \endcode
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto    insertNode( SharedNode node ) noexcept( false ) -> WeakNode;

    /*! Remove node \c node from graph.
     *
     * Complexity depend on Config::NodeContainer.
     * \throw gtpo::bad_topology_error if node can't be removed (or node is not valid).
     */
    auto    removeNode( WeakNode weakNode ) noexcept( false ) -> void;

    //! Return the number of nodes actually registered in graph.
    auto    getNodeCount( ) const -> Size { return _nodes.size(); }
    //! Return the number of root nodes (actually registered in graph)ie nodes with a zero in degree).
    auto    getRootNodeCount( ) const -> Size { return _rootNodes.size(); }

    /*! \brief Install a given \c node in the root node cache.
     *
     * This method should not be directly used by an end user until you have deeply
     * modified graph topology with non gtpo::GenGraph<> methods.
     *
     * \throw gtpo::bad_topology_error if \c node in degree is different from 0.
     */
    auto    installRootNode( WeakNode node ) noexcept( false ) -> void;
    /*! Test if a given \c node is a root node.
     *
     * This method is safer than testing node->getInDegree()==0, since it check
     * \c node in degree and its presence in the internal root node cache.
     *
     * \return true if \c node is a root node, false otherwise.
     * \throw gtpo::bad_topology_error if there is a graph cohenrency problem (ie node is in the root node cache but has a zero in degree).
     */
    auto    isRootNode( WeakNode node ) const noexcept( false ) -> bool;

    //! Use fast search container to find if a given \c node is part of this graph.
    auto    containsNode( WeakNode node ) const noexcept -> bool;

    //! Graph main nodes container.
    auto    getNodes() const -> const SharedNodes& { return _nodes; }
    //! Return a const begin iterator over graph SharedNode nodes.
    auto    begin( ) const -> typename SharedNodes::const_iterator { return _nodes.begin( ); }
    //! Return a const begin iterator over graph SharedNode nodes.
    auto    end( ) const -> typename SharedNodes::const_iterator { return _nodes.end( ); }

    //! Return a const begin iterator over graph SharedNode nodes.
    auto    cbegin() const -> typename SharedNodes::const_iterator { return _nodes.cbegin(); }
    //! Return a const end iterator over graph SharedNode nodes.
    auto    cend() const -> typename SharedNodes::const_iterator { return _nodes.cend(); }

    void    notifyXChanged( WeakNode& node ) {
        (void)node;
        std::cout << "Node " << "x" << " position changed.." << std::endl;
    }

private:
    SharedNodes         _nodes;
    WeakNodes           _rootNodes;
    WeakNodesSearch     _nodesSearch;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Graph Edge Management *///---------------------------------------
    //@{
public:
    /*! \brief Create a directed edge between \c source and \destination node.
     *
     * Complexity is O(1).
     * \return the inserted edge (if an error occurs edge == false and gtpo::bad_topology_error is thrown).
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination does not exists).
     */
    auto        createEdge( WeakNode source, WeakNode destination ) noexcept( false ) -> WeakEdge;

    /*! \brief Create a directed edge of a given \c className between \c source and \destination node.
     *
     * Do not use this method until you are writing a serializer input functor with complex edge virtual hierarchy.
     *
     * Complexity is O(1).
     * \return the inserted edge (if an error occurs edge == false and gtpo::bad_topology_error is thrown).
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination does not exists).
     */
    virtual auto    createEdge( const std::string& className, WeakNode source, WeakNode destination ) noexcept( false ) -> WeakEdge;

    /*! Insert a directed edge created outside of GTpo into the graph.
     *
     * \param edge must have a valid source and destination set otherwise a bad topology exception will be thrown.
     * \sa insertNode()
     * \throw gtpo::bad_topology_error with an error description if insertion fails.
     */
    auto        insertEdge( SharedEdge edge ) noexcept( false ) -> WeakEdge;

    /*! \brief Remove first directed edge found between \c source and \destination node.
     *
     * If the current GenGraph<> Config::EdgeContainer and Config::NodeContainer allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Complexity is O(edge count) at worst.
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        removeEdge( WeakNode source, WeakNode destination ) noexcept( false ) -> void;

    /*! \brief Remove all directed edge between \c source and \destination node.
     *
     * If the current GenGraph<> Config::EdgeContainer and Config::NodeContainer allow parrallel edges support, the first
     * edge found between \c source and \c destination will be removed.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (either \c source or \c destination or edge does not exists).
     */
    auto        removeAllEdges( WeakNode source, WeakNode destination ) noexcept( false ) -> void;

    /*! \brief Remove directed edge \c edge.
     *
     * Worst case complexity is O(edge count).
     * \throw a gtpo::bad_topology_error if suppression fails (\c edge does not exists).
     */
    auto        removeEdge( WeakEdge edge ) noexcept( false ) -> void;

    /*! \brief Look for the first directed edge between \c source and \c destination and return it.
     *
     * Worst case complexity is O(edge count).
     * \return A shared reference on edge, en empty shared reference otherwise (result == false).
     * \throw no GTpo exception (might throw a std::bad_weak_ptr).
     */
    auto        findEdge( WeakNode source, WeakNode destination ) const noexcept( false ) -> WeakEdge;
    /*! \brief Test if a directed edge exists between nodes \c source and \c destination.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw no GTpo exception (might throw a std::bad_weak_ptr).
     */
    auto        hasEdge( WeakNode source, WeakNode destination ) const noexcept( false ) -> bool;
    //! Return the number of edges currently existing in graph.
    auto        getEdgeCount( ) const noexcept -> unsigned int { return (int)_edges.size(); }
    /*! \brief Return the number of (parallel) directed edges between nodes \c source and \c destination.
     *
     * Graph EdgeContainer should support multiple insertions (std::vector, std::list) to enable
     * parrallel edge support, otherwise, getEdgeCount() will always return 1 or 0.
     *
     * This method only test a 1 degree relationship (ie a direct edge between \c source
     * and \c destination). Worst case complexity is O(edge count).
     * \throw no GTpo exception (might throw a std::bad_weak_ptr).
     */
    auto        getEdgeCount( WeakNode source, WeakNode destination ) const noexcept( false ) -> unsigned int;

    //! Graph main edges container.
    auto    getEdges() const -> const SharedEdges& { return _edges; }
    //! Return a const begin iterator over graph SharedEdge edges.
    auto        cbeginEdges() const -> typename SharedEdges::const_iterator { return _edges.cbegin(); }
    //! Return a const end iterator over graph SharedEdge edges.
    auto        cendEdges() const -> typename SharedEdges::const_iterator { return _edges.cend(); }
private:
    SharedEdges  _edges;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Behaviours Management *///---------------------------------------
    //@{
public:
    using Behaviour = GraphBehaviour<WeakNode,WeakEdge>;
    /*! \brief Add a behaviour to this graph (graph get ownership for behaviour).
     *
     * \param behaviour Graph will get behaviour ownership.
     */
    auto    addBehaviour( Behaviour* behaviour ) -> void {
        _behaviours.push_back( std::unique_ptr< Behaviour >( behaviour ) );
    }

    auto    notifyNodeModified( WeakNode node ) -> void;
    auto    notifyEdgeModified( WeakEdge edge ) -> void;
private:
    std::vector< std::unique_ptr< Behaviour > >  _behaviours;
private:
    auto    notifyBehaviours( void (Behaviour::*method)(WeakNode), WeakNode arg ) -> void;
    auto    notifyBehaviours( void (Behaviour::*method)(WeakEdge), WeakEdge arg ) -> void;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Serialization Management *///------------------------------------
    //@{
public:
    auto        serializeToGml( std::string fileName ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoEdge.hpp"
#include "./gtpoNode.hpp"
#include "./gtpoGraph.hpp"

#endif // gtpoGraph_h

