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
// \file	qtpoGraph.h
// \author	benoit@qanava.org
// \date	2016 01 22
//-----------------------------------------------------------------------------

#ifndef qtpoIDInterface_h
#define qtpoIDInterface_h

// QT headers
#include <QMap>
#include <QHash>
#include <QList>

namespace gtpo { // ::gtpo

/*!
 * \brief Interface used to provide qan::Graph a way to generate unique id for QuickQanava objects serialization (mainly graph, nodes, edges and styles).
 *
 * This interface could be usefull when dealing with network application were nodes are
 * shared across multiple users, and where an unique server generate id for all graphs. An
 * appropriate object implementing this interface could be provided to Graph with the setIdInterface()
 * method.
 * It is usually not necessary to provide a custom object if only basic serialization is required.
 */
class IDInterface
{
public:
    IDInterface( ) { }
    virtual ~IDInterface( ) { }
private:
    Q_DISABLE_COPY( IDInterface )

public:

    /*! Register an object and generate an unique id if id = 0 (or use a given id that must be unique).
     *
     * \return an unique id >= 0, or -1 if registering fails (eg provided id already exists).   */
    virtual int     registerObject( void* object, int id = -1 ) = 0;

    /*! Get the id of a registered object, if it is not registered register it automatically.
     *
     * \return object unique id or -1 if the object could not been registered using rgisterObject().
     * \sa registerObject()     */
    virtual int     getId( void* object ) = 0;

    /*! Get the object registered with the given id.
     *
     * \return a pointer on the object registered with the given id, or nullptr if no object has been registered with the given id.
     * \sa registerObject()
     */
    virtual void*   getObject( int id ) = 0;

    /*! Cleat all id generated from this interface.           */
    virtual void    clear( ) = 0;
};

/*!
 * \brief Basic working implementation for the qan::IDInterface class.
 *
 * Object destruction is not taken into account, ie an object registered and destrocyed by the user will stay in the object
 * database, and its id could still be queried while it has been destroyed.
 */
class SimpleIDInterface : public IDInterface
{
    /*! \name SimpleIDInterface object/id Mapping *///-------------------------
    //@{
public:
    SimpleIDInterface( ) : IDInterface( ), _maxId( 0 ) { }
    virtual ~SimpleIDInterface( ) { }
private:
    Q_DISABLE_COPY( SimpleIDInterface )

public:
    //! \copydoc IDInterface::registerObject()
    virtual int     registerObject( void* object, int id = -1 );
    //! \copydoc IDInterface::getId()
    virtual int     getId( void* object );
    //! \copydoc IDInterface::getObject()
    virtual void*   getObject( int id );
    //! \copydoc IDInterface::clear()
    virtual void    clear( );
protected:
    QHash< void*, int > _objectIdMap;
    QHash< int, void* > _idObjectMap;
    int                 _maxId;
    //@}
    //-------------------------------------------------------------------------
};

} // ::gtpo

#endif // qtpoIDInterface_h

