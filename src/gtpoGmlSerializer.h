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
// \file	gtpoGmlSerializer.h
// \author	benoit@qanava.org
// \date	2016 01 31
//-----------------------------------------------------------------------------

#ifndef gtpoGmlSerializer_h
#define gtpoGmlSerializer_h

// GTpo headers
#include "./gtpoSerializer.h"

// PugiXml headers
#include "./pugixml/src/pugixml.hpp"

namespace gtpo { // ::gtpo

/*! \brief Serialize a graph in GraphML XML format using PugiXML.
 *
 * OutGmlPugiSerializer support a subset of the GraphML format:
 * \li http://graphml.graphdrawing.org
 * \li http://graphml.graphdrawing.org/primer/graphml-primer.html
 *
 * GTpo try to comply with the GraphML standard with the following exceptions:
 * \li While it is valid in GraphML to have \<edge\> and \<node\> XML nodes merged, a graph
 * exported from GTpo will _always_ export nodes first, then edges.
 * \li GrapgML parse info attributes are not supported (see section 2.5 of the GraphML
 * Primer http://graphml.graphdrawing.org/primer/graphml-primer.html#ParseInfo).
 *
 */
/*template < class GraphConfig = DefaultConfig,
           class Notifier = EmptyProgressNotifier >
class OutGmlSerializer : public OutSerializer< GraphConfig, Notifier >
{
public:
    OutGmlSerializer( std::string xmlFileName);
*/
    /*! \brief Serialize \c graph out to an in-memory PugyXML DOM tree.
     *
     * XML DOM tree is not written to disk until finish() is called.
     *
     * \throw std::exception if an error occurs.
     */
/*    virtual void    serializeOut( Graph& graph, Notifier* progress = nullptr ) override;
    //! Write the GML XML DOM to the \c filename set in ctor.
    virtual void    finishOut( );

protected:
    void            serializeNode( const WeakNode& node ) noexcept( false );
    void            serializeNodeAttribute( pugi::xml_node& nodeNode, const std::string& id, const std::string& value );

    void            serializeEdge( const WeakEdge& edge ) noexcept( false );
*/
    /*! \brief Generate a GraphML \c key node.
     *
     * Params \c name, \c target, \c type and \c def will be expended in the following way:
     * (with \c keyNode beeing created as a child node of \c graphNode)
     * \code
     * <graphml>
     *   <key id="name" for="target" attr.name="name" attr.type="type">
     *     <default>def</default>
     *   </key>
     * </graphml>
     * \endcode
     *
     * \param graphNode main graph node.
     * \throw std::runtime_error with an error message if graphNode is invalid or node creation fails.
     */
/*    void    serializeAttribute( pugi::xml_node& graphNode, const std::string& name, const std::string& target,
                                const std::string& type, const std::string& def );

private:
    std::string         _xmlFileName;
    pugi::xml_document  _xmlDoc;
    pugi::xml_node      _xmlCurrentParent;
};*/

/*! \brief Serialize a graph in GraphML XML format.
 *
 * InGmlPugiSerializer support a subset of the GraphML format:
 * \li http://graphml.graphdrawing.org
 * \li http://graphml.graphdrawing.org/primer/graphml-primer.html
 *
 * GTpo try to comply with the GraphML standard with the following exceptions:
 * \li GTpo will not successfully loads GraphML files until a node refered in an edge
 * has been previously defined.
 * \li GTpo will ignore GrapgML parse info attributes (see section 2.5 of the GraphML
 * Primer http://graphml.graphdrawing.org/primer/graphml-primer.html#ParseInfo).
 *
 */
/*template < class GraphConfig = DefaultConfig,
           class Notifier = EmptyProgressNotifier >
class InGmlSerializer : public InSerializer< GraphConfig, Notifier >
{
public:
    InGmlSerializer( std::string fileName);

    virtual void    serializeIn( Graph& graph, Notifier* progress = nullptr ) override;
    //! Do nothing in the actual implementation, should be called for out serialization symetry.
    virtual void    finishIn( );

protected:
    void            serializeNode( Node& node ) noexcept( false );
    void            serializeEdge( Node& node ) noexcept( false );

private:
    pugi::xml_document  _xmlDoc;
    pugi::xml_node      _xmlCurrentParent;
};*/

} // ::gtpo

#include "./gtpoGmlSerializer.hpp"

#endif // gtpoGmlSerializer_h

