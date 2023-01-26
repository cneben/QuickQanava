/*
 Copyright (c) 2008-2022, Benoit AUTHEMAN All rights reserved.

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
// \file    qanTableGroup.h
// \author  benoit@destrat.io
// \date    2023 01 25
//-----------------------------------------------------------------------------

#pragma once

// Qt headers
#include <QQuickItem>
#include <QPointF>
#include <QPolygonF>

// QuickQanava headers
#include "./qanGroup.h"

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
Q_MOC_INCLUDE("./qanGraph.h")
Q_MOC_INCLUDE("./qanTableGroupItem.h")
#endif

namespace qan { // ::qan

/*! \brief FIXME
 *
 * \nosubgrouping
 */
class TableGroup : public qan::Group
{
    /*! \name TableGroup Object Management *///--------------------------------
    //@{
    Q_OBJECT
public:
    //! TableGroup constructor.
    explicit TableGroup(QObject* parent = nullptr);
    /*! \brief Remove any childs group who have no QQmlEngine::CppOwnership.
     *
     */
    virtual ~TableGroup() override = default;
    TableGroup(const TableGroup&) = delete;

public:
    //friend class qan::TableGroupItem;

public:
    //! Shortcut to getItem()->proposeNodeDrop(), defined only for g++ compatibility to avoid forward template declaration.
    void    itemProposeNodeDrop();
    //! Shortcut to getItem()->endProposeNodeDrop(), defined only for g++ compatibility to avoid forward template declaration.
    void    itemEndProposeNodeDrop();
    //@}
    //-------------------------------------------------------------------------

    /*! \name TableGroup Static Factories *///---------------------------------
    //@{
public:
    /*! \brief Return the default delegate QML component that should be used to generate group \c item.
     *
     *  \arg engine QML engine used for delegate QML component creation.
     *  \return Default delegate component or nullptr (when nullptr is returned, QuickQanava default to Qan.TableGroup component).
     */
    static  QQmlComponent*      delegate(QQmlEngine& engine, QObject* parent = nullptr) noexcept;

    /*! \brief Return the default style that should be used with qan::TableGroup.
     *
     *  \return Default style or nullptr (when nullptr is returned, qan::StyleManager default group style will be used).
     */
    static  qan::NodeStyle*     style(QObject* parent = nullptr) noexcept;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Table Properties *///--------------------------------------------
    //@{
public:
    //! \brief FIXME #190.
    Q_PROPERTY(qreal cellSpacing READ getCellSpacing WRITE setCellSpacing NOTIFY cellSpacingChanged FINAL)
    void        setCellSpacing(qreal cellSpacing);
    qreal       getCellSpacing() const { return _cellSpacing; }
signals:
    void        cellSpacingChanged();
private:
    qreal       _cellSpacing = 5.0;

public:
    //! \brief FIXME #190.
    Q_PROPERTY(QSizeF cellMinimumSize READ getCellMinimumSize WRITE setCellMinimumSize NOTIFY cellMinimumSizeChanged FINAL)
    void        setCellMinimumSize(QSizeF cellMinimumSize);
    QSizeF      getCellMinimumSize() const { return _cellMinimumSize; }
signals:
    void        cellMinimumSizeChanged();
private:
    QSizeF      _cellMinimumSize = QSizeF{10., 10.};

public:
    //! \brief FIXME #190.
    Q_PROPERTY(qreal tablePadding READ getTablePadding WRITE setTablePadding NOTIFY tablePaddingChanged FINAL)
    void        setTablePadding(qreal tablePadding);
    qreal       getTablePadding() const { return _tablePadding; }
signals:
    void        tablePaddingChanged();
private:
    qreal       _tablePadding = 2.0;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE(qan::TableGroup)
