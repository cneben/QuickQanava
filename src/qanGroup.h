/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author or Destrat.io nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL AUTHOR BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library.
//
// \file    qanGroup.h
// \author  benoit@destrat.io
// \date    2016 03 22
//-----------------------------------------------------------------------------

#pragma once

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanStyle.h"
#include "./qanNode.h"

namespace qan { // ::qan

class Graph;
class GroupItem;

/*! \brief Model a graphics group of nodes.
 *
 * \nosubgrouping
 */
class Group : public qan::Node
{
    /*! \name Group Object Management *///-------------------------------------
    //@{
    Q_OBJECT
public:
    //! Group constructor.
    explicit Group( QObject* parent = nullptr );
    /*! \brief Remove any childs group who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~Group() override = default;
    Group( const Group& ) = delete;

    using gtpo_node_t = gtpo::node<qan::Config>;
public:
    Q_PROPERTY( qan::Graph* graph READ getGraph CONSTANT FINAL )
    //! Shortcut to gtpo::group<>::getGraph().
    qan::Graph*         getGraph() noexcept;
    //! \copydoc getGraph()
    const qan::Graph*   getGraph() const noexcept;

    /*! \brief Collect this group adjacent edges (ie adjacent edges of group and group nodes).
     *
     */
    std::unordered_set<qan::Edge*>  collectAdjacentEdges() const;

public:
    friend class qan::GroupItem;

    qan::GroupItem*         getGroupItem() noexcept;
    const qan::GroupItem*   getGroupItem() const noexcept;
    virtual void            setItem(qan::NodeItem* item) noexcept override;

public:
    //! Shortcut to getItem()->proposeNodeDrop(), defined only for g++ compatibility to avoid forward template declaration.
    void    itemProposeNodeDrop();
    //! Shortcut to getItem()->endProposeNodeDrop(), defined only for g++ compatibility to avoid forward template declaration.
    void    itemEndProposeNodeDrop();
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Static Factories *///---------------------------------------
    //@{
public:
    /*! \brief Return the default delegate QML component that should be used to generate group \c item.
     *
     *  \arg engine QML engine used for delegate QML component creation.
     *  \return Default delegate component or nullptr (when nullptr is returned, QuickQanava default to Qan.Group component).
     */
    static  QQmlComponent*      delegate(QQmlEngine& engine) noexcept;

    /*! \brief Return the default style that should be used with qan::Group.
     *
     *  \return Default style or nullptr (when nullptr is returned, qan::StyleManager default group style will be used).
     */
    static  qan::Style*         style() noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group Nodes Management *///--------------------------------------
    //@{
public:
    //! Return true if node \c node is registered in this group, shortcut to gtpo::group<qan::Config>::hasNode().
    Q_INVOKABLE bool    hasNode( qan::Node* node ) const;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Group DnD Management *///----------------------------------------
    //@{
public:
    /*! \brief Define if the group could actually be dragged by mouse.
     *
     * Set to true to allow this group to be moved by mouse drag (if false, the node position is
     * fixed and should be changed programmatically).
     *
     * Default to true (ie group is draggable by mouse).
     */
    Q_PROPERTY( bool draggable READ getDraggable WRITE setDraggable NOTIFY draggableChanged FINAL )
    void            setDraggable(bool draggable) noexcept;
    bool            getDraggable() const noexcept;
private:
    bool            _draggable = true;
signals:
    void            draggableChanged();
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Group )
