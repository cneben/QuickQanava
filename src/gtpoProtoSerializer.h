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
// \file	gtpoProtoSerializer.h
// \author	benoit@qanava.org
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifndef gtpoProtoSerializer_h
#define gtpoProtoSerializer_h

// GTpo headers
#include "./gtpoSerializer.h"

// STD headers
#include <fstream>
#include <unordered_map>    // ObjectIdMap

#ifdef GTPO_HAVE_PROTOCOL_BUFFER

// Protocol Buffer headers
#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#endif
#include "./gtpo.pb.h"

namespace gtpo { // ::gtpo

/*! \brief Serialize a graph in Protocol Buffer format.
 *
 * When using this serializer with a custom graph, initialize ProtoSerializer() with
 * a \c nodeDefaultName and \c edgeDefaultName with the values reported by your base
 * node and edge getClassName(), otherwise nodes and edges will not be serialized.
 *
 * \nosubgrouping
 */
template < class GraphConfig = DefaultConfig >
class ProtoSerializer : public gtpo::Serializer< GraphConfig >
{
    /*! \name Protocol Buffer Serializer Object Management *///----------------
    //@{
public:
    static auto initProtocolBuffer() -> void;
    static auto shutDownProtocolBuffer() -> void;

    //! Input/ouput functor are registered in ctor.
    explicit ProtoSerializer( std::string nodeDefaultName = "gtpo::Node",
                              std::string edgeDefaultName = "gtpo::Edge" );
    virtual ~ProtoSerializer() = default;
    ProtoSerializer( const ProtoSerializer< GraphConfig >& ) = delete;

protected:
    const std::string&  getNodeDefaultName() { return _nodeDefaultName; }
    const std::string&  getEdgeDefaultName() { return _edgeDefaultName; }
private:
    //! Default node class name for edges (default to "gtpo::Node").
    std::string _nodeDefaultName = std::string( "gtpo::Node" );
    //! Default node class name for edges (default to "gtpo::Edge").
    std::string _edgeDefaultName = std::string( "gtpo::Edge" );
    //@}
    //-------------------------------------------------------------------------

    /*! \name Out Serialization *///-------------------------------------------
    //@{
public:
    using Graph         = GenGraph< GraphConfig >;
    using SharedNode    = std::shared_ptr< typename GraphConfig::Node >;
    using Node          = typename GraphConfig::Node;
    using WeakNode      = std::weak_ptr< typename GraphConfig::Node >;
    using WeakEdge      = std::weak_ptr< typename GraphConfig::Edge >;
    using SharedEdge    = std::shared_ptr< typename GraphConfig::Edge >;

    using   ObjectIdMap = std::unordered_map< void*, int >;

    /*! Generate an int ID mapping between \c graph nodes/edges/groups.
     *
     * This method should be called before calling serializeOut() and between consecutive
     * calls to serializeOut().
     *
     * \note Existing mapping is cleared.
     */
    auto                    generateObjectIdMap( const Graph& graph ) -> ObjectIdMap&;
protected:
    ObjectIdMap&            getObjectIdMap() { return _objectIdMap; }
    const ObjectIdMap&      getObjectIdMap() const { return _objectIdMap; }

private:
    ObjectIdMap             _objectIdMap;

public:
    using   IdObjectMap = std::unordered_map< int, void* >;
protected:
    IdObjectMap&            getIdObjectMap() { return _idObjectMap; }
    const IdObjectMap&      getIdObjectMap() const { return _idObjectMap; }
private:
    IdObjectMap             _idObjectMap;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Out Serialization *///-------------------------------------------
    //@{
public:
    /*! \brief Serialize a QuickQanava graph to Protocol Buffer v3.
     *
     *  The object/Id map accessible trought getObjectIdMap() will be used to generate nodes
     *  edges and groups id (if null, it will be generated at startup).
     *
     *  User can specify specific data associed to graph (for exemple styles...) with
     *  \c user1 and \c user2 arguments by passing generic Protocol Buffer messages:
     *  \code
     *    gtpo::ProtoSerializer s;
     *    pb::MyMessage pbMessage;      // Specific Protocol Message buffer.
     *    feedMyMessage( pbMessage );   // Feed message with data
     *    // Then serialize the graph using your PB message as user1 argument
     *    s.serializeOut( *graph, os, nullptr, &pbMessage );
     *  \endcode
     *
     *  \throw std::exception if an error occurs.
     */
    auto            serializeOut( const Graph& graph,
                                  std::ostream& os,
                                  const ::google::protobuf::Message* user1 = nullptr,
                                  const ::google::protobuf::Message* user2 = nullptr  ) -> void;
    //! \copydoc serializeOut()
    virtual auto    serializeOut( const Graph& graph,
                                  std::ostream& os ) -> void override;

    /*! Serialize GTpo node \c weakNode to Protocol Buffer gtpo.pb.GTpoNode \c pbNode.
     *
     * \throw noexcept
     */
    static void     serializeGTpoNodeOut( const WeakNode& weakNode, gtpo::pb::GTpoNode& pbNode, const ObjectIdMap& objectIdMap );

public:
    using   NodeOutFunctor = std::function<void(google::protobuf::Any* anyNodes, const WeakNode&, const ObjectIdMap& objectIdMap )>;
    auto    registerNodeOutFunctor( std::string nodeClassName, NodeOutFunctor nodeOutFunctor ) -> void;
protected:
    using   NodeOutFunctors = std::map< std::string, NodeOutFunctor >;
    NodeOutFunctors _nodeOutFunctors;

public:
    using   EdgeOutFunctor = std::function<bool(google::protobuf::Any* anyEdges, const WeakEdge&, const ObjectIdMap& objectIdMap )>;
    auto    registerEdgeOutFunctor( std::string edgeClassName, EdgeOutFunctor edgeOutFunctor ) -> void;
protected:
    using   EdgeOutFunctors = std::map< std::string, EdgeOutFunctor >;
    EdgeOutFunctors _edgeOutFunctors;
    //@}
    //-------------------------------------------------------------------------

    /*! \name In Serialization *///--------------------------------------------
    //@{
public:
    /*! \brief Serialize a QuickQanava graph to Protocol Buffer v3.
     *  \throw std::exception if an error occurs.
     */
    template < class User1 = gtpo::pb::GTpoVoid >
    auto            serializeIn( std::istream& is,
                                 Graph& graph,
                                 User1* user1 = nullptr ) -> void;

    virtual auto    serializeIn( std::istream& is,
                                 Graph& graph ) -> void override;

    /*! Serialize Protocol Buffer gtpo.pb.GTpoNode \c pbNode to GTpo node \c weakNode.
     *
     * \throw noexcept
     */
    static void     serializeGTpoNodeIn( const gtpo::pb::GTpoNode& pbNode, WeakNode& weakNode, IdObjectMap& idObjectMap );

public:
    //! Serialize a Protocol Buffer "Any" node to a given graph, return true if serialization succeed.
    using   NodeInFunctor = std::function<WeakNode( const google::protobuf::Any&, Graph&, IdObjectMap& idObjectMap )>;
    auto    registerNodeInFunctor( NodeInFunctor nodeInFunctor ) -> void;
protected:
    using   NodeInFunctors = std::vector< NodeInFunctor >;
    NodeInFunctors _nodeInFunctors;

public:
    using   EdgeInFunctor = std::function<void*( const google::protobuf::Any&, Graph&, IdObjectMap& idObjectMap  )>;
    auto    registerEdgeInFunctor( EdgeInFunctor edgeInFunctor ) -> void;
protected:
    using   EdgeInFunctors = std::vector< EdgeInFunctor >;
    EdgeInFunctors _edgeInFunctors;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoProtoSerializer.hpp"

#endif

#endif // gtpoProtoSerializer_h

