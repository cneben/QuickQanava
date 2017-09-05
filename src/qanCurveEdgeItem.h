/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
// \file	qanCurvedEdgeItem.h
// \author	benoit@destrat.io
// \date	2017 09 05
//-----------------------------------------------------------------------------

#ifndef qanCurveEdgeItem_h
#define qanCurveEdgeItem_h

// Qt headers
#include <QLineF>

// QuickQanava headers
#include "./qanEdgeItem.h"

namespace qan { // ::qan

//! Weighted directed edge linking two nodes in a graph.
/*!
 *
 * \warning EdgeItem \c objectName property is set to "qan::EdgeItem" and should not be changed in subclasses.
 *
 * \nosubgrouping
 */
class CurveEdgeItem : public qan::EdgeItem
{
    /*! \name Edge Object Management *///--------------------------------------
    //@{
    Q_OBJECT
public:
    explicit CurveEdgeItem(QQuickItem* parent = nullptr);
    virtual ~CurveEdgeItem();
    CurveEdgeItem( const EdgeItem& ) = delete;
    CurveEdgeItem& operator=(const CurveEdgeItem&) = delete;
    CurveEdgeItem(CurveEdgeItem&&) = delete;
    CurveEdgeItem& operator=(CurveEdgeItem&&) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Curve Control Points Management *///-----------------------------
    //@{
public:
    /*! \brief Call qan::EdgeItem::updateItem(), then generate control points specific for curve edge.
     *
     * \copydoc qan::EdgeITem::updateItem()
     */
    virtual void        updateItem() noexcept override;

public:    
    //! Edge source point in item CS (with accurate source bounding shape intersection).
    Q_PROPERTY( QPointF c1 READ getC1() NOTIFY c1Changed FINAL )
    inline  auto    getC1() const -> const QPointF& { return _c1; }
    //! Edge destination point in item CS (with accurate destination bounding shape intersection).
    Q_PROPERTY( QPointF c2 READ getC2() NOTIFY c2Changed FINAL )
    inline  auto    getC2() const -> const QPointF& { return _c2; }
signals:
    void            c1Changed();
    void            c2Changed();
private:
    QPointF         _c1;
    QPointF         _c2;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::CurveEdgeItem )

#endif // qanCurveEdgeItem_h
