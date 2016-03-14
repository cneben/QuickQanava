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
// \file    qpsPropertiesList.h
// \author	benoit@qanava.org
// \date	2015 06 20
//-----------------------------------------------------------------------------

#ifndef qpsPropertiesList_h
#define qpsPropertiesList_h

// Qt headers
// Nil

// QuickProperties headers
#include "./qpsProperties.h"
#include "./qpsContainerListModel.h"

namespace qps { // ::qps

/*! \brief Model a serializable list of qps::Properties objects with an abstract item model interface.
 *
 * \sa qps::Properties
 */
class PropertiesList : public qps::ContainerListModel< QVector, qps::Properties* >
{
    /*! \name PropertiesList Object Management *///----------------------------
    //@{
    Q_OBJECT
public:
    //! Parent must be the qps::Graph managing nodes exposed in this model.
    PropertiesList( QObject* parent = nullptr );
    virtual ~PropertiesList( ) { }
    //! Defined for QList<> foreach iteration, do not use.
    PropertiesList( const PropertiesList& propertiesList ) :
        qps::ContainerListModel< QVector, qps::Properties* >( propertiesList ) { }
    //@}
    //-------------------------------------------------------------------------

    /*! \name QList Interface *///---------------------------------------------
    //@{
signals:
    void        propertiesInserted( qps::Properties* properties );
    void        propertiesRemoved( qps::Properties* properties );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qps

QML_DECLARE_TYPE( qps::PropertiesList )

#endif // qpsPropertiesList_h

