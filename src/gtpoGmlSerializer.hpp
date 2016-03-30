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
// \file	gtpoGmlSerializer.hpp
// \author	benoit@qanava.org
// \date	2016 01 31
//-----------------------------------------------------------------------------

#include <cassert>
#include <stdexcept>

namespace gtpo { // ::gtpo

template < class GraphConfig >
OutGmlSerializer< GraphConfig >::OutGmlSerializer( std::string xmlFileName ) :
    _xmlFileName( xmlFileName )
{
    assert( xmlFileName.length() > 0 );
}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::serializeOut( Graph& graph, gtpo::IProgressNotifier* progress )
{
    gtpo::IProgressNotifier voidNotifier;
    if ( progress == nullptr )
        progress = &voidNotifier;
    try {
        auto graphmlNode = _xmlDoc.append_child( "graphml" );
        if ( !graphmlNode )
            throw std::exception();
        auto graphNode = graphmlNode.append_child( "graph" );
        if ( !graphNode )
            throw std::exception();
        graphNode.append_attribute( "id" );
        graphNode.append_attribute( "edgedefault" );
        _xmlCurrentParent = graphNode;

        // Serialize GTpo default attributes
        progress->beginProgress();
        serializeAttribute( graphNode, "x", "node", "double", "0." );
        serializeAttribute( graphNode, "y", "node", "double", "0." );
        serializeAttribute( graphNode, "width", "node", "double", "0." );
        serializeAttribute( graphNode, "height", "node", "double", "0." );
        serializeAttribute( graphNode, "serializable", "node", "bool", "true" );
        serializeAttribute( graphNode, "weight", "edge", "double", "0." );

        // FIXME 20160208 virer getNodes()
        progress->setPhaseCount( 2 );
        progress->beginPhase( "Saving nodes" );
        int     primitive = 0;
        double  primitiveCount = static_cast< double >( graph.getNodes().size() );
        for ( auto node : graph.getNodes() ) {
            serializeNode( node );
            progress->setPhaseProgress( ++primitive / primitiveCount );
        }
        progress->beginPhase( "Saving edges" );
        primitive = 0;
        primitiveCount = static_cast< double >( graph.getEdges().size() );
        for ( auto edge : graph.getEdges() ) {
            serializeEdge( edge );
            progress->setPhaseProgress( ++primitive / primitiveCount );
        }
    } catch ( std::exception const& ) {
        progress->endProgress();
    }
    progress->endProgress();
}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::serializeNode( const WeakNode& node )
{
    SharedNode sharedNode = node.lock();
    if ( !sharedNode )
        throw std::runtime_error( "gtpo::OutGmlPugiSerializer<>::serializeNode(): can't lock node." );
    if ( !_xmlCurrentParent )
        throw std::exception();
    auto nodeNode = _xmlCurrentParent.append_child( "node" );
    if ( !nodeNode )
        throw std::exception();
    auto idAttr = nodeNode.append_attribute( "id" );
    idAttr.set_value( std::to_string( reinterpret_cast<uint64_t>( sharedNode.get() ) ).c_str() );
    serializeNodeAttribute( nodeNode, "x", std::to_string( GraphConfig::getNodeX( sharedNode.get() ) ) );
    serializeNodeAttribute( nodeNode, "y", std::to_string( GraphConfig::getNodeY( sharedNode.get() ) ) );
    serializeNodeAttribute( nodeNode, "width", std::to_string( GraphConfig::getNodeWidth( sharedNode.get() ) ) );
    serializeNodeAttribute( nodeNode, "height", std::to_string( GraphConfig::getNodeHeight( sharedNode.get() ) ) );
}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::serializeNodeAttribute( pugi::xml_node& nodeNode, const std::string& id, const std::string& value )
{
    auto dataNode = nodeNode.append_child( "data" );
    if ( !dataNode )
        throw std::runtime_error( "gtpo::OutGmlPugiSerializer<>::serializeNodeAttribute(): error while creating node data sub node." );
    dataNode.set_name( "data" );
    auto idAttr = dataNode.append_attribute( "id" );
    idAttr.set_value( id.c_str() );
    dataNode.append_child( pugi::node_pcdata ).set_value( value.c_str() );
}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::serializeEdge( const WeakEdge& edge ) noexcept( false )
{
    SharedEdge sharedEdge = edge.lock();
    gtpo::assert_throw( sharedEdge != nullptr, "gtpo::OutGmlPugiSerializer<>::serializeEdge(): Error: Can't lock edge." );
    gtpo::assert_throw( _xmlCurrentParent, "gtpo::OutGmlPugiSerializer<>::serializeEdge(): Error: Invalid XML parent node." );
    auto edgeNode = _xmlCurrentParent.append_child( "edge" );
    gtpo::assert_throw( edgeNode, "gtpo::OutGmlPugiSerializer<>::serializeEdge(): Error: Can't create edge XML node." );

    auto idAttr = edgeNode.append_attribute( "id" );
    idAttr.set_value( std::to_string( reinterpret_cast<uint64_t>( sharedEdge.get() ) ).c_str() );

    SharedNode ownedSource = sharedEdge->getSrc().lock();
    SharedNode ownedDestination = sharedEdge->getDst().lock();
    gtpo::assert_throw( ownedSource && ownedDestination, "gtpo::OutGmlPugiSerializer<>::serializeEdge(): Error: Can't lock edge source or destination." );
    auto sourceAttr = edgeNode.append_attribute( "source" );
    sourceAttr.set_value( std::to_string( reinterpret_cast<uint64_t>( ownedSource.get() ) ).c_str() );
    auto targetAttr = edgeNode.append_attribute( "target" );
    targetAttr.set_value( std::to_string( reinterpret_cast<uint64_t>( ownedDestination.get() ) ).c_str() );

}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::serializeAttribute( pugi::xml_node& graphNode, const std::string& name, const std::string& target,
                                                             const std::string& type, const std::string& def )
{
    auto keyNode = graphNode.append_child( "key" );
    if ( !keyNode )
        throw std::runtime_error( "gtpo::OutGmlPugiSerializer<>::serializeAttribute(): invalid key node." );
    auto idAttr = keyNode.append_attribute( "id" );
    idAttr.set_value( name.c_str() );
    auto forAttr = keyNode.append_attribute( "for" );
    forAttr.set_value( target.c_str() );
    auto nameAttr = keyNode.append_attribute( "attr.name" );
    nameAttr.set_value( name.c_str() );
    auto typeAttr = keyNode.append_attribute( "attr.type" );
    typeAttr.set_value( type.c_str() );
    auto defaultNode = keyNode.append_child( "default" );
    if ( !defaultNode )
        throw std::runtime_error( "gtpo::OutGmlPugiSerializer<>::serializeAttribute(): error creating attribute key node default sub node." );
    defaultNode.set_name( "default" );
    defaultNode.append_child( pugi::node_pcdata ).set_value( def.c_str() );
}

template < class GraphConfig >
void    OutGmlSerializer< GraphConfig >::finishOut( )
{
    assert( _xmlFileName.length() > 0 );
    _xmlDoc.save_file( _xmlFileName.c_str() );
}

template < class GraphConfig >
InGmlSerializer< GraphConfig >::InGmlSerializer( std::string fileName ) :
    _xmlFileName( fileName )
{
    gtpo::assert_throw( fileName.length() > 0, "gtpo::InGmlSerializer<>::InGmlSerializer(): Error: Trying to serialize an empty file name" );
    pugi::xml_parse_result result = _xmlDoc.load_file( fileName.c_str() );
    if ( !result ) {
        std::cerr << "qtpo::InGmlPugiSerializer<>::InGmlPugiSerializer(): XML " << fileName.c_str() << " parsed with errors" << std::endl;
        std::cerr << "\tDescription: " << result.description() << std::endl;
        std::cerr << "\tOffset: " << result.offset << std::endl;
        gtpo::assert_throw( false, "gtpo::InGmlSerializer<>::InGmlSerializer(): Error: PugyXML parse error." );
    }
}

template < class GraphConfig >
void    InGmlSerializer< GraphConfig >::serializeIn( Graph& graph, gtpo::IProgressNotifier* progress )
{
    (void)graph;
    (void)progress;
}

template < class GraphConfig >
void    InGmlSerializer< GraphConfig >::serializeNode( Node& node )
{
}

template < class GraphConfig >
void    InGmlSerializer< GraphConfig >::finishIn( )
{
    // No cleanups necessary
}

} // ::gtpo

