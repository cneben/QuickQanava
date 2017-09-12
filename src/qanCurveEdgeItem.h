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
    //! \copydoc c1
    inline  auto    getC1() const noexcept -> const QPointF& { return _c1; }
    //! Edge destination point in item CS (with accurate destination bounding shape intersection).
    Q_PROPERTY( QPointF c2 READ getC2() NOTIFY c2Changed FINAL )
    //! \copydoc c2
    inline  auto    getC2() const noexcept -> const QPointF& { return _c2; }
signals:
    //! \copydoc c1
    void            c1Changed();
    //! \copydoc c2
    void            c2Changed();
private:
    //! \copydoc c1
    QPointF         _c1;
    //! \copydoc c2
    QPointF         _c2;

protected:
    /*! Return cubic curve angle at position \c pos between [0.; 1.] on curve defined by \c start, \c end and controls points \c c1 and \c c2.
     *
     * \param  pos  linear position on curve, between [0.; 1.0].
     * \return angle in degree or a value < 0.0 if an error occurs.
     */
    qreal           cubicCurveAngleAt(qreal pos, const QPointF& start, const QPointF& end, const QPointF& c1, const QPointF& c2) const noexcept;

    /*! Return line angle on line \c line.
     *
     * \return angle in degree or a value < 0.0 if an error occurs.
     */
    qreal           lineAngle(const QLineF& line) const noexcept;

public:
    //! Destination edge arrow angle.
    Q_PROPERTY( qreal dstAngle READ getDstAngle() NOTIFY dstAngleChanged FINAL )
    //! \copydoc dstAngle
    inline  auto    getDstAngle() const noexcept -> qreal { return _dstAngle; }
private:
    //! \copydoc dstAngle
    qreal           _dstAngle{0.};
signals:
    //! \copydoc dstAngle
    void            dstAngleChanged();

public:
    //! Source edge arrow angle.
    Q_PROPERTY( qreal srcAngle READ getSrcAngle() NOTIFY srcAngleChanged FINAL )
    //! \copydoc srcAngle
    inline  auto    getSrcAngle() const noexcept -> qreal { return _srcAngle; }
private:
    //! \copydoc srcAngle
    qreal           _srcAngle{0.};
signals:
    //! \copydoc srcAngle
    void            srcAngleChanged();

public:
    //! Edge destination arrow control points (\c dstA1 is top corner, \c dstA2 is tip, \c dstA3 is bottom corner).
    Q_PROPERTY( QPointF dstA1 READ getDstA1() NOTIFY dstA1Changed FINAL )
    //! \copydoc dstA1
    inline  auto    getDstA1() const noexcept -> const QPointF& { return _dstA1; }
    //! \copydoc dstA1
    Q_PROPERTY( QPointF dstA2 READ getDstA2() NOTIFY dstA2Changed FINAL )
    //! \copydoc dstA1
    inline  auto    getDstA2() const noexcept -> const QPointF& { return _dstA2; }
    //! \copydoc dstA1
    Q_PROPERTY( QPointF dstA3 READ getDstA3() NOTIFY dstA3Changed FINAL )
    //! \copydoc dstA1
    inline  auto    getDstA3() const noexcept -> const QPointF& { return _dstA3; }
private:
    //! \copydoc dstA1
    QPointF         _dstA1, _dstA2, _dstA3;
signals:
    void            dstA1Changed();
    void            dstA2Changed();
    void            dstA3Changed();
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::CurveEdgeItem )

#endif // qanCurveEdgeItem_h
