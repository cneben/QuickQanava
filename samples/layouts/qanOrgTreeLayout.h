/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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
// \file	qanOrgTreeLayout.h
// \author	benoit@destrat.io
// \date	2024 08 13
//-----------------------------------------------------------------------------

#pragma once

// Qt headers
#include <QString>
#include <QQuickItem>
#include <QQmlParserStatus>
#include <QSharedPointer>
#include <QAbstractListModel>

// QuickQanava headers
#include "../../src/qanGraph.h"


namespace qan { // ::qan

/*! \brief
 * \nosubgrouping
 */
class NaiveTreeLayout : public QObject
{
    Q_OBJECT
    /*! \name NaiveTreeLayout Object Management *///---------------------------
    //@{
public:
    explicit NaiveTreeLayout(QObject* parent = nullptr) noexcept;
    virtual ~NaiveTreeLayout() override;
    NaiveTreeLayout(const NaiveTreeLayout&) = delete;
    NaiveTreeLayout& operator=(const NaiveTreeLayout&) = delete;
    NaiveTreeLayout(NaiveTreeLayout&&) = delete;
    NaiveTreeLayout& operator=(NaiveTreeLayout&&) = delete;

public:
    // FIXME
    void                layout(qan::Node& root) noexcept;

    //! QML invokable version of layout().
    Q_INVOKABLE void    layout(qan::Node* root) noexcept;
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief
 * \nosubgrouping
 */
class OrgTreeLayout : public QObject
{
    Q_OBJECT
    /*! \name OrgTreeLayout Object Management *///-----------------------------
    //@{
public:
    explicit OrgTreeLayout(QObject* parent = nullptr) noexcept;
    virtual ~OrgTreeLayout() override;
    OrgTreeLayout(const OrgTreeLayout&) = delete;
    OrgTreeLayout& operator=(const OrgTreeLayout&) = delete;
    OrgTreeLayout(OrgTreeLayout&&) = delete;
    OrgTreeLayout& operator=(OrgTreeLayout&&) = delete;

public:
    /*! \brief Apply an "organisational chart tree layout algorithm" to subgraph \c root.
     *
     * OrgChart layout _will preserve_ node orders.
     *
     * While the current implementation is not "space optimal" it run in O(n), n beeing the
     * number of nodes in root "tree subgraph".
     *
     * \note \c root must be a tree subgraph, this method will not enforce this condition,
     * running this algorithm on a non tree subgraph might lead to inifinite recursions or
     * invalid layouts.
     */
    void                layout(qan::Node& root) noexcept;

    //! QML invokable version of layout().
    Q_INVOKABLE void    layout(qan::Node* root) noexcept;
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE(qan::OrgTreeLayout)

