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
// \author	benoit@qanava.org
// \date	2016 02 10
//-----------------------------------------------------------------------------

#ifndef qanProtoSerializer_h
#define qanProtoSerializer_h

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
    ProtoSerializer( QObject* parent = nullptr );
    virtual ~ProtoSerializer() { }
private:
    Q_DISABLE_COPY( ProtoSerializer )
    //@}
    //-------------------------------------------------------------------------

    /*! \name Composition of GTPO PB Serialization Interface *///--------------
    //@{
public:
    using   NodeOutFunctor = gtpo::ProtoSerializer< qan::Config >::NodeOutFunctor;
    auto    registerNodeOutFunctor( std::string nodeClassName, NodeOutFunctor nodeOutFunctor ) -> void { _gtpoSerializer.registerNodeOutFunctor( nodeClassName, nodeOutFunctor ); }
    using   EdgeOutFunctor = gtpo::ProtoSerializer< qan::Config >::EdgeOutFunctor;
    auto    registerEdgeOutFunctor( std::string edgeClassName, EdgeOutFunctor edgeOutFunctor ) -> void { _gtpoSerializer.registerEdgeOutFunctor( edgeClassName, edgeOutFunctor ); }

    using   NodeInFunctor = gtpo::ProtoSerializer< qan::Config >::NodeInFunctor;
    auto    registerNodeInFunctor( NodeInFunctor nodeInFunctor ) -> void { _gtpoSerializer.registerNodeInFunctor( nodeInFunctor ); }
    using   EdgeInFunctor = gtpo::ProtoSerializer< qan::Config >::EdgeInFunctor;
    auto    registerEdgeInFunctor( EdgeInFunctor edgeInFunctor ) -> void { _gtpoSerializer.registerEdgeInFunctor( edgeInFunctor ); }

public:
    using   ObjectIdMap = std::unordered_map< void*, int >;
    //! \copydoc gtpo::ProtoSerializer::generateObjectIdMap().
    auto                    generateObjectIdMap( const qan::Graph& graph ) -> ObjectIdMap& { return _gtpoSerializer.generateObjectIdMap( graph ); }
    ObjectIdMap&            getObjectIdMap() { return _gtpoSerializer.getObjectIdMap(); }
    const ObjectIdMap&      getObjectIdMap() const { return _gtpoSerializer.getObjectIdMap(); }
public:
    using   IdObjectMap = std::unordered_map< int, void* >;
public:
    IdObjectMap&            getIdObjectMap() { return _gtpoSerializer.getIdObjectMap(); }
    const IdObjectMap&      getIdObjectMap() const { return _gtpoSerializer.getIdObjectMap(); }
protected:
    gtpo::ProtoSerializer< qan::Config >    _gtpoSerializer;
    //@}
    //-------------------------------------------------------------------------

    /*! \name QML Serialization Interface *///---------------------------------
    //@{
public:
    using ObjectIdMap   = gtpo::ProtoSerializer< qan::Config >::ObjectIdMap;
    using IdObjectMap   = gtpo::ProtoSerializer< qan::Config >::IdObjectMap;

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

protected:
    /*! Save style manager \c styleManager in graph \c graph to Protocol Buffer message \c pbStyleManager.
     *
     * Arguments \c graph and \c objectIdMap are used serialize the mapping between graph nodes and their associed style.
     */
    static auto serializeStyleManagerOut( const qan::Graph& graph,
                                          const qan::StyleManager& styleManager,
                                          qan::pb::StyleManager& pbStyleManager,
                                          const ObjectIdMap& objectIdMap  ) -> void;
    static auto serializeStyleManagerIn( const qan::pb::StyleManager& pbStyleManager,
                                         qan::StyleManager& styleManager,
                                         const IdObjectMap& idObjectMap  ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::ProtoSerializer )

#endif // qanProtoSerializer_h

