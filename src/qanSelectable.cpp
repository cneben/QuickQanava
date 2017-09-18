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
// \file	qanSelectable.cpp
// \author	benoit@destrat.io
// \date	2016 03 15
//-----------------------------------------------------------------------------

// QuickQanava headers
#include "./qanSelectable.h"
#include "./qanGraph.h"

namespace qan { // ::qan

/* Node Object Management *///-------------------------------------------------
Selectable::Selectable() { /* Nil */ }

Selectable::~Selectable() { /* Nil */ }

void    Selectable::configure(QQuickItem* target, qan::Graph* graph)
{
    _target = target;
    _graph = graph;
    if ( _selectionItem )
        _selectionItem->setParentItem( _target );
}
//-----------------------------------------------------------------------------

/* Selection Management *///---------------------------------------------------
void    Selectable::setSelectable( bool selectable ) noexcept
{
    if ( _selectable == selectable )
        return;
    if ( getSelectionItem() != nullptr &&
         getSelectionItem()->isVisible() )
        getSelectionItem()->setVisible( false );
    _selectable = selectable;
    if ( _selected &&
         !_selectable )
        setSelected(false);
    emitSelectableChanged();
}

void    Selectable::setSelected( bool selected ) noexcept
{
    if ( getSelectionItem() != nullptr && isSelectable() )
        getSelectionItem()->setVisible( selected );

    if ( _target && _graph ) {  // Eventually create selection item
        if ( selected && getSelectionItem() == nullptr )
            setSelectionItem( _graph->createSelectionItemFromDelegate( _target.data() ).data() );
        else if ( !selected )
            _graph->removeFromSelection(_target.data());
    }

    if ( _selected != selected ) {  // Binding loop protection
        _selected = selected;
        emitSelectedChanged();
    }
}

void    Selectable::setSelectionItem( QQuickItem* selectionItem )
{
    if ( selectionItem == nullptr ) {
        qWarning() << "qan::Selectable::setSelectionItem(): Error: Can't set a nullptr selection hilight item.";
        return;
    }
    _selectionItem = QPointer<QQuickItem>(selectionItem);
    if ( _target )
        _selectionItem->setParentItem( _target.data() );  // Configure Quick item
    if ( _graph )
        configureSelectionItem();
    _selectionItem->setVisible( isSelectable() && getSelected() );
    emitSelectionItemChanged();
}

void    Selectable::configureSelectionItem()
{
    if ( _target && _selectionItem ) {
        QObject* anchors = _selectionItem->property( "anchors" ).value<QObject*>();
        QObject* border = _selectionItem->property( "border" ).value<QObject*>();

        qreal selectionMargin = anchors ? anchors->property("margins").toReal() : 3;
        qreal selectionWeight = border ? border->property("width").toReal() : 3;
        qreal x = -( selectionWeight / 2. + selectionMargin );
        qreal y = -( selectionWeight / 2. + selectionMargin );
        qreal width = _target->width() + selectionWeight + ( selectionMargin * 2 );
        qreal height = _target->height() + selectionWeight + ( selectionMargin * 2 );

        _selectionItem->setX( x );
        _selectionItem->setY( y );
        _selectionItem->setWidth( width );
        _selectionItem->setHeight( height );
    }
}

//-----------------------------------------------------------------------------

} // ::qan
