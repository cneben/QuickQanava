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
// \file	qanLayout.h
// \author	benoit@destrat.io
// \date	2015 09 07
//-----------------------------------------------------------------------------

#ifndef qanLayout_h
#define qanLayout_h

// Qt headers
#include <QObject>
#include <QSizeF>
#include <QQuickItem>

// QuickQanava headers
#include "./qanConfig.h"
#include "./qanBehaviour.h"
#include "./qanNode.h"

namespace qan { // ::qan

class Group;

/*! Base class for all layouts algorithms in Qanava.
 *
 *
 * \nosubgrouping
 */
class Layout : public QObject,
               public gtpo::GroupBehaviour< qan::Config >
{
    /*! \name Layout Object Management *///------------------------------------
    //@{
    Q_OBJECT
public:
    //! Layout constructor.
    Layout( QObject* parent = 0 ) : QObject( parent ), _orientation( Qt::Horizontal ) { }
    virtual ~Layout( ) { }
private:
    Q_DISABLE_COPY( Layout )
    //@}
    //-------------------------------------------------------------------------

    /*! \name Layout Interface *///--------------------------------------------
    //@{
public:
    /*! List of nodes managed by this layout.
     *
     * This property could be set either from QML or C++, usually, it is a qan::Group or qan::Graph node list get
     * with qan::Group::getNodes() or qan::Graph::getNodes().
     *
     * List content can be changed dynamically (and list can also be changed dynamically).
     *
     * Default to nullptr.
     */
//    Q_PROPERTY( qan::NodeList* nodes READ getNodes WRITE setNodes NOTIFY nodesChanged )
    /*! \sa nodes   */
//    virtual void    setNodes( qan::NodeList* nodes ) { _nodes = nodes; layout( ); emit nodesChanged( ); }
    /*! \sa nodes   */
//    qan::NodeList*  getNodes( ) { return _nodes; }
protected:
//    qan::NodeList*  _nodes;
signals:
    /*! \sa nodes   */
//    void            nodesChanged( );

public:
    /*! Layout orientation (might not be supported by all actal layouts implementation).
     *
     * Default to Qt::Horizontal.
     *
     * Supported by qan::HierarchyTree.
     */
    Q_PROPERTY( Qt::Orientation orientation READ getOrientation WRITE setOrientation NOTIFY orientationChanged FINAL )
    void            setOrientation( Qt::Orientation orientation ) { _orientation = orientation; emit orientationChanged( ); }
    Qt::Orientation getOrientation( ) { return _orientation; }
protected:
    Qt::Orientation _orientation;
signals:
    void            orientationChanged( );

public slots:
    //! Pure virtual method used to actually layout nodes in the 'nodes' property.
    virtual void    layout( ) { }

public:
    /*! Propose a node drop before it is actually dropped into the layout (usually called by a qan::Group while a node is hovered over a group before insertion).
     *
     *  This method doesn't return anything, it must modify 'shadowNode' position or topology in the '_nodes' node list. Theses modification will be reflected on
     *  candidate node 'node' if the node is finally dropped in this layout trought qan::Group. An example implementation of this method in qan::Linear show how to
     *  dynamically modify node insertion order in '_nodes', and previsualize its insertion position with a shadow node.
     *
     * \note    default implementation just call layout(), calling base method is not necessary in overriden methods.
     *
     * \param   layoutItem  quick item that is the parent for the layout quickitems nodes (usually it is qan::Group::container property).
     * \param   node        node candidate for insertion in this layout.
     * \param   shadowNode  node (usually qan::Group::_shadowDropNode) used to previsualize the insertion position of candidate node in this layout (it is already registered in layout nodes when this method is called).
     */
    virtual void    proposeNodeDrop( QQuickItem* layoutItem, qan::Node* node, qan::Node* shadowNode ) { Q_UNUSED( layoutItem ); Q_UNUSED( node ); Q_UNUSED( shadowNode ); layout( ); }
    //@}
    //-------------------------------------------------------------------------


    /*! \name Layout Topology Utilities *///-----------------------------------
    //@{
public:
    /*! Return all root nodes in a given restricted set of nodes (ie every nodes with a 0 in degree, or no super nodes in the given nodes set).
     *
     * \param node must be inside nodes
     */
    //static void            collectNodeGroupRootNodes( qan::NodeList& nodes, qan::Node::List& rootNodes );
    /*! Return a node in degree in a given restricted set of nodes (ie a node that has multiple super nodes, but none of them member of the group will have an in degree of 0).
     *
     * \param node must be inside nodes
     */
    //static int             getNodeGroupInDegree( qan::NodeList& nodes, qan::Node& node );
    /*! Collect every node super node that is actually a member of the given node set.
     *
     * \param node must be inside nodes
     */
    //static void            collectNodeGroupInNodes( qan::NodeList& nodes, qan::Node& node, qan::Node::List& groupInNodes );
    /*! Collect every node sub node that is actually a member of the given node set.
     *
     * \param node must be inside nodes
     */
    //static void            collectNodeGroupOutNodes( qan::NodeList& nodes, qan::Node& node, qan::Node::List& groupOutNodes );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Layout* )

#endif // qanLayout_h
