/*
    This file is part of QuickProperties2 library.

    Copyright (C) 2016  Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickProperties2 library.
//
// \file    qpsSerializer.h
// \author  benoit@qanava.org
// \date    2016 01 20
//-----------------------------------------------------------------------------

#ifndef qpsSerializer_h
#define qpsSerializer_h

// QuickProperties headers
#include "./qpsTimeValueMap.h"
#include "./qpsProperties.h"
#include "./quickproperties.pb.h"

namespace qps { // ::qps

/*! \brief
 *
 * \nosubgrouping
 */
class Serializer : public QObject
{
    /*! \name Primitive Serialization Management *///--------------------------
    //@{
    Q_OBJECT
public:
    Serializer( QObject* parent = nullptr ) : QObject( parent ) { }
    virtual ~Serializer() { }
private:
    Q_DISABLE_COPY( Serializer )
    //Serializer( const Serializer& ) = delete;
public:
    Q_INVOKABLE virtual void    open( QString source ) { Q_UNUSED( source ); }
    Q_INVOKABLE virtual void    serialize( qps::TimeValueMap* tvm, int id ) { Q_UNUSED(tvm); Q_UNUSED(id); }
    Q_INVOKABLE virtual void    close() { }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Static Helpers *///----------------------------------------------
    //@{
    //@}
    //-------------------------------------------------------------------------
};

class VoidSerializer : public Serializer
{
    Q_OBJECT
public:
    VoidSerializer( QObject* parent = nullptr ) : Serializer( parent ) { }
    virtual ~VoidSerializer() { }
public:
    Q_INVOKABLE virtual void    open( QString dest ) override { Q_UNUSED(dest); }
    Q_INVOKABLE virtual void    serialize( qps::TimeValueMap* tvm, int id ) override { Q_UNUSED(tvm); Q_UNUSED(id); }
    Q_INVOKABLE virtual void    close() override { }
};

class ProtoSerializer
{
public:
    /*! \name Static Helpers *///----------------------------------------------
    //@{
    static auto serializeOut( const qps::Properties& properties, qps::pb::Properties& pbProperties ) -> void;
    static auto serializeIn( const qps::pb::Properties& pbProperties, qps::Properties& properties ) -> void;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qps

QML_DECLARE_TYPE( qps::Serializer )

#endif // qpsSerializer_h

