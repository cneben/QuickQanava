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
// \file	qanProtoSerializer.h
// \author	benoit@destrat.io
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifndef qanProtoSerializer_h
#define qanProtoSerializer_h

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#pragma warning(disable: 4267)  // Disable warning C4267: 'argument': conversion from 'size_t' to 'int', possible loss of data
#endif

// GTpo headers
#include "gtpoProtoSerializer.h"

// QuickQanava headers
#include "./qanConfig.h"
#include "./qanGraph.h"
#include "./qanProgressNotifier.h"

#ifdef _MSC_VER
#pragma warning(disable: 4100)  // Disable unreferenced formal parameter warning for Protocol Buffer generated code
#pragma warning(disable: 4267)  // Disable converstion warning from size_t to int
#endif
#include "./quickqanava.pb.h"

namespace qan { // ::qan

/*! \brief Serialize a QuickQanava graph to Protocol Buffer v3 repository.
 *
 * \nosubgrouping
 */
class ProtoSerializer : public QObject,
                        public gtpo::Serializer< qan::Config >
{
    /*! \name Qan ProtoSerializer Object Management *///-----------------------
    //@{
    Q_OBJECT
public:
    explicit ProtoSerializer( QObject* parent = nullptr );
    virtual ~ProtoSerializer() { }
    ProtoSerializer( const ProtoSerializer& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Composition of GTPO PB Serialization Interface *///--------------
    //@{
public:
    using   NodeOutFunctor = gtpo::ProtoSerializer< qan::Config >::NodeOutFunctor;
    auto    registerNodeOutFunctor( std::string nodeClassName, NodeOutFunctor nodeOutFunctor ) -> void { _gtpoSerializer.registerNodeOutFunctor( nodeClassName, nodeOutFunctor ); }
    using   EdgeOutFunctor = gtpo::ProtoSerializer< qan::Config >::EdgeOutFunctor;
    auto    registerEdgeOutFunctor( std::string edgeClassName, EdgeOutFunctor edgeOutFunctor ) -> void { _gtpoSerializer.registerEdgeOutFunctor( edgeClassName, edgeOutFunctor ); }
    using   GroupOutFunctor = gtpo::ProtoSerializer< qan::Config >::GroupOutFunctor;
    auto    registerGroupOutFunctor( std::string groupClassName, GroupOutFunctor groupOutFunctor ) -> void { _gtpoSerializer.registerGroupOutFunctor( groupClassName, groupOutFunctor ); }

    using   NodeInFunctor = gtpo::ProtoSerializer< qan::Config >::NodeInFunctor;
    auto    registerNodeInFunctor( NodeInFunctor nodeInFunctor ) -> void { _gtpoSerializer.registerNodeInFunctor( nodeInFunctor ); }
    using   EdgeInFunctor = gtpo::ProtoSerializer< qan::Config >::EdgeInFunctor;
    auto    registerEdgeInFunctor( EdgeInFunctor edgeInFunctor ) -> void { _gtpoSerializer.registerEdgeInFunctor( edgeInFunctor ); }
    using   GroupInFunctor = gtpo::ProtoSerializer< qan::Config >::GroupInFunctor;
    auto    registerGroupInFunctor( GroupInFunctor groupInFunctor ) -> void { _gtpoSerializer.registerGroupInFunctor( groupInFunctor ); }

public:
    using ObjectIdMap   = gtpo::ProtoSerializer< qan::Config >::ObjectIdMap;

    //! \copydoc gtpo::ProtoSerializer::generateObjectIdMap().
    auto                    generateObjectIdMap( const qan::Graph& graph ) -> ObjectIdMap& { return _gtpoSerializer.generateObjectIdMap( graph ); }
    ObjectIdMap&            getObjectIdMap() { return _gtpoSerializer.getObjectIdMap(); }
    const ObjectIdMap&      getObjectIdMap() const { return _gtpoSerializer.getObjectIdMap(); }
    auto                    addObjectId( const void* object ) -> int { return _gtpoSerializer.addObjectId( object ); }
    auto                    getObjectId( const void* object ) const noexcept( true ) -> int { return _gtpoSerializer.getObjectId( object ); }

public:
    using IdObjectMap   = gtpo::ProtoSerializer< qan::Config >::IdObjectMap;

    IdObjectMap&            getIdObjectMap() { return _gtpoSerializer.getIdObjectMap(); }
    const IdObjectMap&      getIdObjectMap() const { return _gtpoSerializer.getIdObjectMap(); }
protected:
    gtpo::ProtoSerializer< qan::Config >    _gtpoSerializer;

public:
    //! Shortcut to gtpo::ProtoSerializer< qan::Config >::serializeGTpoEdgeOut().
    static  auto    serializeGTpoEdgeOut( const WeakEdge& weakEdge, gtpo::pb::Edge& pbEdge, const ObjectIdMap& objectIdMap ) -> void { gtpo::ProtoSerializer< qan::Config >::serializeGTpoEdgeOut( weakEdge, pbEdge, objectIdMap ); }
    //! Shortcut to gtpo::ProtoSerializer< qan::Config >::serializeGTpoEdgeIn().
    static  auto    serializeGTpoEdgeIn(  const gtpo::pb::Edge& pbEdge, WeakEdge& weakEdge, IdObjectMap& idObjectMap ) -> void { gtpo::ProtoSerializer< qan::Config >::serializeGTpoEdgeIn( pbEdge, weakEdge, idObjectMap ); }

public:
    //! Shortcut to gtpo::ProtoSerializer< qan::Config >::serializeGTpoGroupOut().
    static  auto    serializeGTpoGroupOut( const WeakGroup& weakGroup, gtpo::pb::Group& pbGroup, const ObjectIdMap& objectIdMap ) -> void { gtpo::ProtoSerializer< qan::Config >::serializeGTpoGroupOut( weakGroup, pbGroup, objectIdMap ); }
    //! Shortcut to gtpo::ProtoSerializer< qan::Config >::serializeGTpoGroupIn().
    static  auto    serializeGTpoGroupIn(  const gtpo::pb::Group& pbGroup, WeakGroup& weakGroup, IdObjectMap& idObjectMap ) -> void { gtpo::ProtoSerializer< qan::Config >::serializeGTpoGroupIn( pbGroup, weakGroup, idObjectMap ); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name QML Serialization Interface *///---------------------------------
    //@{
public:
    //! Serialize graph \c graph to file \c fileName (fileName could a be a file:// URL).
    Q_INVOKABLE void    saveGraphTo( qan::Graph* graph, QString fileName, qan::ProgressNotifier* progress = nullptr );
    //! Serialize graph \c graph from file \c fileName (fileName could a be a file:// URL).
    Q_INVOKABLE void    loadGraphFrom( QString fileName, qan::Graph* graph, qan::ProgressNotifier* progress = nullptr );

    //! Low level access to protocol buffer GTpoGraph message.
    auto    serializeOut( const qan::Graph& graph,
                                qan::pb::Graph& pbGraph,
                                gtpo::IProgressNotifier& progress ) -> void;

    //! Low level direct access to protocol buffer GTpoGraph message.
    auto    serializeIn( const qan::pb::Graph& pbGraph,
                               qan::Graph& graph,
                               gtpo::IProgressNotifier& progress ) -> void;

    static  auto    serializeQanNodeOut( const WeakNode& weakNode, qan::pb::Node& pbQanNode, const ObjectIdMap& objectIdMap ) -> void;
    static  auto    serializeQanNodeIn(  const qan::pb::Node& pbQanNode, WeakNode& weakNode, IdObjectMap& idObjectMap ) -> void;

protected:
    /*! Save style manager \c styleManager in graph \c graph to Protocol Buffer message \c pbStyleManager.
     *
     * Arguments \c graph and \c objectIdMap are used to serialize the mapping between graph nodes and their associed style.
     */
    auto serializeStyleManagerOut( const qan::Graph& graph,
                                   const qan::StyleManager& styleManager,
                                   qan::pb::StyleManager& pbStyleManager ) -> void;
    auto serializeStyleManagerIn( const qan::pb::StyleManager& pbStyleManager,
                                  qan::StyleManager& styleManager  ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::ProtoSerializer )

#endif // qanProtoSerializer_h

