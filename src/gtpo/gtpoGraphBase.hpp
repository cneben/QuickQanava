/*
 Copyright (c) 2008-2021, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the GTpo software library.
//
// \file	graph.hpp
// \author	benoit@destrat.io
// \date	2016 01 22
//-----------------------------------------------------------------------------

namespace gtpo { // ::gtpo

/* Graph Node Management *///--------------------------------------------------
template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
graph<graph_base_t, node_t,
      group_t, edge_t>::~graph()
{
    clear();
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
void    graph<graph_base_t, node_t,
              group_t, edge_t>::clear() noexcept
{
    // Note 20160104: First edges, then nodes (it helps maintaining topology if
    // womething went wrong during destruction
    for (auto& node: _nodes)   // Do not maintain topology during node deletion
        node->set_graph(nullptr);
    _root_nodes.clear();         // Remove weak_ptr containers first
    _nodes_search.clear();
    _nodes.clear();

    for (auto& edge: _edges)   // Do not maintain topology during edge deletion
        edge->set_graph(nullptr);
    _edges_search.clear();
    _edges.clear();

    // Clearing groups and behaviours (Not: group->_graph is resetted with nodes)
    _groups.clear();

    // FIXME v2
    //behaviourable_base::clear();
}
//-----------------------------------------------------------------------------

/* Graph Node Management *///--------------------------------------------------
template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto graph<graph_base_t, node_t,
           group_t, edge_t>::create_node( ) -> node_t*
{
    node_t* node = nullptr;
    try {
        // FIXME v2 unique_ptr
        node = insert_node(new node_t{});
    } catch (...) { gtpo::assert_throw( false, "graph<>::create_node(): Error: can't insert node in graph." ); }
    return node;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::insert_node(node_t* node) -> bool
{
    //assert_throw(node != nullptr, "gtpo::graph<>::insert_node(): Error: Trying to insert a nullptr node in graph.");
    if (node == nullptr)
        return false;
    try {
        node->set_graph(this);
        // FIXME v2
        container_adapter<nodes_t>::insert(node, _nodes);
        container_adapter<nodes_search_t>::insert(node, _nodes_search);
        container_adapter<nodes_t>::insert(node, _root_nodes);

        // FIXME v2
        //behaviourable_base::notify_node_inserted(weak_node);
        return true;
    } catch (...) { gtpo::assert_throw(false, "gtpo::graph<>::insert_node(): Error: can't insert node in graph."); }
    return false;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::remove_node(node_t* node) -> bool
{
    if (node == nullptr)
        return false;

    // Eventually, ungroup node
    auto group = node->get_group();
    if (group != nullptr)
        ungroup_node(node, group);

    // FIXME v2
    //behaviourable_base::notify_node_removed( weak_node );

    // Removing all orphant edges pointing to node.
    edges_t nodeInEdges;
    std::copy(node->get_in_edges().cbegin(), node->get_in_edges().cend(),
              std::back_inserter(nodeInEdges));
    for (auto inEdge: nodeInEdges)
        node->remove_in_edge(inEdge);
    for (auto inEdge: nodeInEdges)
        remove_edge(inEdge);

    edges_t nodeOutEdges;
    std::copy(node->get_out_edges().cbegin(), node->get_out_edges().cend(),
              std::back_inserter(nodeOutEdges));
    for (auto outEdge: nodeOutEdges)
        node->remove_out_edge(outEdge);
    for (auto outEdge: nodeOutEdges)
        remove_edge(outEdge);

    // Remove node from main graph containers (it will generate node destruction)
    container_adapter<nodes_search_t>::remove(node, _nodes_search);
    container_adapter<nodes_t>::remove(node, _root_nodes);
    node->set_graph(nullptr);
    container_adapter<nodes_t>::remove(node, _nodes);
    return true;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::install_root_node(node_t* node) -> void
{
    if (node == nullptr)
        return;
    if (node->get_in_degree() != 0) {
        // FIXME v2 log output...
        //"gtpo::graph<>::setRootNode(): Error: trying to set a node with non 0 in degree as a root node." );
        return;
    }
    container_adapter<nodes_t>::insert(node, _root_nodes);
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::is_root_node(node_t* node) const -> bool
{
    if (node == nullptr)
        return false;
    //assert_throw( !node.expired(), "gtpo::graph<>::is_root_node(): Error: node is expired." );
    if (node->get_in_degree() != 0)   // Fast exit when node in degree != 0, it can't be a root node
        return false;
    // FIXME v2  REPLACED THAT...
    //return gtpo::find_weak_ptr( _root_nodes, node );
    return false;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::contains(const node_t* node) const -> bool
{
    if (node == nullptr)
        return false;
    return _nodes_search.contains(const_cast<node_t*>(node));
    // FIXME v2
    //auto nodeIter = std::find_if( _nodes_search.cbegin(), _nodes_search.cend(),
    //                                        [=](const weak_node_t& n ) { return ( compare_weak_ptr<>( node, n ) ); } );
    //return nodeIter != _nodes_search.cend();
}
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::create_edge(node_t* source, node_t* destination) -> edge_t*
{
    if (source == nullptr ||
        destination == nullptr)
        return nullptr;
    // FIXME v2
        //throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, either source or destination nodes are expired." );

    // FIXME v2 unique_ptr
    auto edge = new edge_t{};
    edge->set_graph(this);
    container_adapter<edges_t>::insert(edge, _edges);
    container_adapter<edges_search_t>::insert(edge, _edges_search);
    edge->set_src(source);
    edge->set_dst(destination);
    try {
        source->add_out_edge(edge);
        destination->add_in_edge(edge);
        if (source != destination ) // If edge define is a trivial circuit, do not remove destination from root nodes
            container_adapter<nodes_t>::remove(destination, _root_nodes);    // Otherwise destination is no longer a root node

        // FIXME v2 notify
        //behaviourable_base::notify_edge_inserted(edge);
    } catch ( ... ) {
        // FIXME v2 remove that...
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(Node,Node): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return edge;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::insert_edge(edge_t* edge) -> bool
{
    if (edge == nullptr)
        return false;
    auto source = edge->get_src();
    if (source == nullptr ||
        edge->get_dst() == nullptr) {
        // FIXME v2
        //throw gtpo::bad_topology_error( "gtpo::graph<>::insert_edge(): Error: Either source and/or destination nodes are expired." );
        return false;
    }
    edge->set_graph(this);
    container_adapter<edges_t>::insert(edge, _edges);
    container_adapter<edges_search_t>::insert(edge, _edges_search);
    try {
        source->add_out_edge(edge);
        auto destination = edge->get_dst();
        if (destination != nullptr) {
            destination->add_in_edge(edge);
            if (source != destination) // If edge define is a trivial circuit, do not remove destination from root nodes
                container_adapter<nodes_t>::remove(destination, _root_nodes);    // Otherwise destination is no longer a root node
        }
        // FIXME v2 notify
        //behaviourable_base::notify_edge_inserted( weak_edge );
        return true;
    } catch ( ... ) {
        throw gtpo::bad_topology_error( "gtpo::graph<>::create_edge(): Insertion of edge failed, source or destination nodes topology can't be modified." );
    }
    return false;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::remove_edge(node_t* source, node_t* destination) -> bool
{
    // FIXME v2
    //if ( source.expired() ||
    //     destination.expired() )
    //    throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Topology error." );
    if (source == nullptr ||
        destination == nullptr)
        return false;

    // Find the edge associed with source / destination
    if (_edges.size() == 0)
        return false; // Fast exit
    // FIXME v2 test
    /*auto edgeIter = std::find_if( _edges.begin(), _edges.end(),
                                 [=](const shared_edge_t& e ){
                                    return ( compare_weak_ptr<>( e->get_src(), source ) &&
                                             compare_weak_ptr<>( e->get_dst(), destination ) );
                                    }
                                 ); // std::find_if*/
    auto edgeIter = std::find_if(_edges.begin(), _edges.end(),
                                 [=](const edge_t* e ) {
                                    return (e->get_src() == source &&
                                            e->get_dst() == destination);
                                    }
                                 ); // std::find_if
    if (edgeIter != _edges.end())
        return remove_edge(*edgeIter);
    return false;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::remove_all_edges(node_t* source, node_t* destination) -> bool
{
    // FIXME v2
    /*if ( source.expired() ||
         destination.expired() )
        throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Topology error." );*/
    if (source == nullptr ||
        destination == nullptr)
        return false;

    while (get_edge_count(source, destination) > 0)
        remove_edge(source, destination);
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::remove_edge(edge_t* edge) -> bool
{
    // FIXME v2 test
    if (edge == nullptr)
        return false;
        //throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge to be removed is already expired." );
    auto source = edge->get_src();
    auto destination = edge->get_dst();
    if (source == nullptr      ||           // Expecting a non null source and either a destination or an hyper destination
        destination == nullptr)
        return false;
        // FIXME v2
       //throw gtpo::bad_topology_error( "gtpo::graph<>::remove_edge(): Error: Edge source or destination are expired." );

    // FIXME v2 notify
    //behaviourable_base::notify_edge_removed( weak_edge );
    source->remove_out_edge(edge);
    destination->remove_in_edge(edge);

    edge->set_graph(nullptr);
    container_adapter<edges_t>::remove(edge, _edges);
    container_adapter<edges_search_t>::remove(edge, _edges_search);
    return true;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::find_edge(const node_t* source, const node_t* destination) const -> edge_t*
{
    // Find the edge associed with source / destination
    auto edgeIter = std::find_if(_edges.begin(), _edges.end(),
                                 [=](const edge_t* e ) noexcept {
                                    return ((e->get_src() == source) &&
                                            (e->get_dst() == destination));
                                } );
    return (edgeIter != _edges.end() ? *edgeIter : nullptr);
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::has_edge(const node_t* source, const node_t* destination) const -> bool
{
    return (find_edge(source, destination) != nullptr);
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::get_edge_count(node_t* source, node_t* destination ) const -> unsigned int
{
    unsigned int edgeCount = 0;
    std::for_each(_edges.begin(), _edges.end(), [&](const edge_t* e ) {
                                                       if ((e->get_src() == source) &&
                                                           (e->get_dst() == destination))
                                                           ++edgeCount;
                                                   } );
    return edgeCount;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::contains(edge_t* edge ) const -> bool
{
    if (edge == nullptr)   // Fast exit.
        return false;
    auto edgeIter = std::find_if(_edges_search.cbegin(), _edges_search.cend(),
                                 [=](const edge_t* other_edge) { return (other_edge == edge); } );
    return edgeIter != _edges_search.cend();
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::insert_group(group_t* group) -> bool
{
    if (group == nullptr)
        return false;
    //assert_throw( group != nullptr, "gtpo::graph<>::insert_group(): Error: trying to insert a nullptr shared_node_t" );
    // FIXME v2 serious bug in v1 ;)
    if (insert_node(group)) {
        group->set_graph(this);
        container_adapter<groups_t>::insert(group, _groups);

        // FIXME GROUPS  v2 notify
        //this->notify_group_inserted( weakGroup );
        return true;
    }
    return false;
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::remove_group(group_t* group) -> void
{
    // FIXME v2
/*    shared_node_t group = group_ptr.lock();
    if ( !group )
        gtpo::assert_throw( false, "graph<>::remove_group(): Error: trying to remove and expired group." );

    // Remove group (it will be automatically deallocated)
    // FIXME GROUPS
    //this->notify_group_removed( group_ptr );
    group->set_graph( nullptr );
    config_t::template container_adapter<weak_groups_t>::remove( group_ptr, _groups );

    remove_node(group_ptr);
    */
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::has_group(const group_t* group) const -> bool
{
    if (group == nullptr)
        return false;
    // FIXME v2 test
    //auto groupIter = std::find_if(_groups.begin(), _groups.end(),
    //                                    [=](const weak_group_t& graphGroup  ){ return ( compare_weak_ptr<>( group, graphGroup ) ); } );
    return _groups.contains(const_cast<group_t*>(group));
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::group_node(node_t* node, group_t* group) -> void
{
    if (node == nullptr || group == nullptr)
        return;
    // FIXME v2
    //auto group_ptr = group.lock();
    //gtpo::assert_throw( group_ptr != nullptr, "gtpo::group<>::group_node(): Error: trying to insert a node into an expired group." );

    //auto node_ptr = node.lock();
    //gtpo::assert_throw( node_ptr != nullptr, "gtpo::group<>::group_node(): Error: trying to insert an expired node in group." );

    node->set_group(group);
    container_adapter<nodes_t>::insert(node, group->get_nodes());

    // FIXME GROUPS
    //group_ptr->notify_node_inserted( node );
}

template <class graph_base_t,
          class node_t,
          class group_t,
          class edge_t>
auto    graph<graph_base_t, node_t,
              group_t, edge_t>::ungroup_node(node_t* node, node_t* group) -> void
{
    if (node == nullptr || group == nullptr)
        return;

    //auto group = weakGroup.lock();
    //gtpo::assert_throw( group != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup from an expired group." );
    //auto node = weakNode.lock();
    //gtpo::assert_throw( node != nullptr, "gtpo::group<>::ungroup_node(): Error: trying to ungroup an expired node from a group." );

    //gtpo::assert_throw( node->get_group().lock() == group, "gtpo::group<>::ungroup_node(): Error: trying to ungroup a node that is not part of group." );
    if (node->get_group() != group) {
        // FIXME v2 error
        return;
    }

    container_adapter<nodes_t>::remove(node, group->get_nodes());
    // FIXME GROUPS
    //group->notify_node_removed( weakNode );
    node->set_group(nullptr);  // Warning: group must remain valid while notify_node_removed() is called
}
//-----------------------------------------------------------------------------

} // ::gtpo

