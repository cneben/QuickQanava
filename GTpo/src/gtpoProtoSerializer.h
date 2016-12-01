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
// \author	benoit@destrat.io
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifndef gtpoProtoSerializer_h
#define gtpoProtoSerializer_h

// GTpo headers
#include "./gtpoSerializer.h"

// STD headers
#include <fstream>
#include <unordered_map>    // ObjectIdMap

#ifdef GTPO_HAS_PROTOBUF

// Protocol Buffer headers
#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#pragma warning(disable: 4267)  // Disable warning C4267: 'argument': conversion from 'size_t' to 'int', possible loss of data
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
                              std::string edgeDefaultName = "gtpo::Edge",
                              std::string groupDefaultName = "gtpo::Group" );
    virtual ~ProtoSerializer() = default;
    ProtoSerializer( const ProtoSerializer< GraphConfig >& ) = delete;

protected:
    const std::string&  getNodeDefaultName() { return _nodeDefaultName; }
    const std::string&  getEdgeDefaultName() { return _edgeDefaultName; }
    const std::string&  getGroupDefaultName() { return _groupDefaultName; }
private:
    //! Default class name for nodes (default to "gtpo::Node").
    std::string _nodeDefaultName = "gtpo::Node";
    //! Default class name for edges (default to "gtpo::Edge").
    std::string _edgeDefaultName = "gtpo::Edge";
    //! Default class name for groups (default to "gtpo::Group").
    std::string _groupDefaultName = "gtpo::Group";
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
    using WeakGroup     = std::weak_ptr< typename GraphConfig::Group >;
    using SharedGroup   = std::shared_ptr< typename GraphConfig::Group >;

public:
    using   ObjectIdMap = std::unordered_map< void*, int >;

    /*! Generate an int ID mapping between \c graph nodes/edges/groups.
     *
     * This method should be called before calling serializeOut() and between consecutive
     * calls to serializeOut().
     *
     * \note Existing mapping is cleared.
     */
    auto                    generateObjectIdMap( const Graph& graph ) -> ObjectIdMap&;
    /*! Generate a valid id for a new object, add an object to the actual object id map an return the id.
     *
     * \note if the object is already registered, it's actual id is returned.
     * \return -1 if an error occurs or an interger >= 0 if insertion suceed.
     */
    auto                    addObjectId( const void* object ) -> int;

    /*! Get the id of an already registered object, if it is not registered, return -1.
     *
     * \return -1 if an error occurs or an interger >= 0 if insertion suceed.
     */
    auto                    getObjectId( const void* object ) const noexcept( true ) -> int;

    ObjectIdMap&            getObjectIdMap() { return _objectIdMap; }
    const ObjectIdMap&      getObjectIdMap() const { return _objectIdMap; }
private:
    ObjectIdMap             _objectIdMap;
    int                     _maxId = 0;

public:
    using   IdObjectMap = std::unordered_map< int, void* >;
public:
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
    virtual void    serializeOut( const Graph& graph,
                                  std::ostream& os,
                                  gtpo::IProgressNotifier& progress ) override;

    //! Low level access to protocol buffer GTpoGraph message.
    auto    serializeOut( const Graph& graph,
                          gtpo::pb::Graph& pbGraph,
                          gtpo::IProgressNotifier& progress ) -> void;

    /*! Serialize GTpo node \c weakNode to Protocol Buffer gtpo.pb.Node \c pbNode.
     *
     * \throw noexcept
     */
    static void     serializeGTpoNodeOut( const WeakNode& weakNode, gtpo::pb::Node& pbNode, const ObjectIdMap& objectIdMap );

    static void     serializeGTpoEdgeOut( const WeakEdge& weakEdge, gtpo::pb::Edge& pbEdge, const ObjectIdMap& objectIdMap );

    /*! Serialize GTpo group \c weakGroup to Protocol Buffer gtpo.pb.Group \c pbGroup.
     *
     * \throw noexcept
     */
    static void     serializeGTpoGroupOut( const WeakGroup& weakGroup, gtpo::pb::Group& pbGroup, const ObjectIdMap& objectIdMap );

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

public:
    using   GroupOutFunctor = std::function<bool(google::protobuf::Any* anyGroups, const WeakGroup&, const ObjectIdMap& objectIdMap )>;
    auto    registerGroupOutFunctor( std::string groupClassName, GroupOutFunctor groupOutFunctor ) -> void;
protected:
    using   GroupOutFunctors = std::map< std::string, GroupOutFunctor >;
    GroupOutFunctors _groupOutFunctors;
    //@}
    //-------------------------------------------------------------------------

    /*! \name In Serialization *///--------------------------------------------
    //@{
public:
    /*! \brief Serialize a QuickQanava graph to Protocol Buffer v3.
     *  \throw std::exception if an error occurs.
     */
    virtual void            serializeIn( std::istream& is,
                                         Graph& graph,
                                         gtpo::IProgressNotifier& progress ) override;

    //! Low level direct access to protocol buffer GTpoGraph message.
    auto            serializeIn( const gtpo::pb::Graph& pbGraph,
                                 Graph& graph,
                                 gtpo::IProgressNotifier& progress ) -> void;

    /*! Serialize Protocol Buffer gtpo.pb.GTpoNode \c pbNode to GTpo node \c weakNode.
     *
     * \throw noexcept
     */
    static void     serializeGTpoNodeIn( const gtpo::pb::Node& pbNode, WeakNode& weakNode, IdObjectMap& idObjectMap );

    static void     serializeGTpoEdgeIn( const gtpo::pb::Edge& pbEdge, WeakEdge& weakEdge, IdObjectMap& idObjectMap );

    /*! Serialize Protocol Buffer gtpo.pb.GTpoGroup \c pbGroup to GTpo group \c weakGroup.
     *
     * \throw noexcept
     */
    static void     serializeGTpoGroupIn( const gtpo::pb::Group& pbGroup, WeakGroup& weakGroup, IdObjectMap& idObjectMap );

public:
    //! Serialize a Protocol Buffer "Any" node to a given graph, return true if serialization succeed.
    using   NodeInFunctor = std::function<WeakNode( const google::protobuf::Any&, Graph&, IdObjectMap& idObjectMap )>;
    auto    registerNodeInFunctor( NodeInFunctor nodeInFunctor ) -> void;
protected:
    using   NodeInFunctors = std::vector< NodeInFunctor >;
    NodeInFunctors _nodeInFunctors;

public:
    using   EdgeInFunctor = std::function<WeakEdge( const google::protobuf::Any&, Graph&, IdObjectMap& idObjectMap  )>;
    auto    registerEdgeInFunctor( EdgeInFunctor edgeInFunctor ) -> void;
protected:
    using   EdgeInFunctors = std::vector< EdgeInFunctor >;
    EdgeInFunctors _edgeInFunctors;

public:
    using   GroupInFunctor = std::function<WeakGroup( const google::protobuf::Any&, Graph&, IdObjectMap& idObjectMap  )>;
    auto    registerGroupInFunctor( GroupInFunctor groupInFunctor ) -> void;
protected:
    using   GroupInFunctors = std::vector< GroupInFunctor >;
    GroupInFunctors _groupInFunctors;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#include "./gtpoProtoSerializer.hpp"

#endif // GTPO_HAS_PROTOBUF

#endif // gtpoProtoSerializer_h

