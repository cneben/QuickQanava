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
// This file is a part of the Qanava software.
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
#endif
#include "./quickqanava.pb.h"

namespace qan { // ::qan

/*! \brief Serialize a QuickQanava graph to Protocol Buffer v3 repository.
 *
 * \nosubgrouping
 */
class ProtoSerializer : public QObject,
                        public gtpo::ProtoSerializer< qan::Config >
{
    Q_OBJECT
public:
    ProtoSerializer( QObject* parent = nullptr )  :
        QObject( parent ),
        gtpo::ProtoSerializer< qan::Config >( "qan::Node", "qan::Edge", "qan::Group" ) { }

    virtual ~ProtoSerializer() { }
private:
    Q_DISABLE_COPY( ProtoSerializer )

    /*! \name QML Serialization Interface *///---------------------------------
    //@{
public:
    //! Serialize graph \c graph to file \c fileName (fileName could a be a file:// URL).
    Q_INVOKABLE void    saveGraphTo( qan::Graph* graph, QString fileName, qan::ProgressNotifier* progress = nullptr );
    //! Serialize graph \c graph from file \c fileName (fileName could a be a file:// URL).
    Q_INVOKABLE void    loadGraphFrom( QString fileName, qan::Graph* graph, qan::ProgressNotifier* progress = nullptr );

protected:
    /*! Save style manager \c styleManager in graph \c graph to Protocol Buffer message \c pbStyleManager.
     *
     * Arguments \c graph and \c objectIdMap are used serialize the mapping between graph nodes and their associed style.
     */
    static auto saveStyleManager( const qan::Graph& graph,
                                  const qan::StyleManager& styleManager,
                                  qan::pb::StyleManager& pbStyleManager,
                                  const ObjectIdMap& objectIdMap  ) -> void;
    static auto loadStyleManager( const qan::pb::StyleManager& pbStyleManager, qan::StyleManager& styleManager, const IdObjectMap& idObjectMap  ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::ProtoSerializer )

#endif // qanProtoSerializer_h

