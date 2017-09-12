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
    if ( getSelectionItem() != nullptr &&
         isSelectable() )
        getSelectionItem()->setVisible( selected );

    if ( _target &&
         _graph ) {
        if ( selected &&                // Eventually create selection item
             getSelectionItem() == nullptr )
            setSelectionItem( _graph->createRectangle( _target.data() ) );
        else if ( !selected )
            _graph->removeFromSelection(_target.data());
    }

    if ( _selected != selected ) {      // Binding loop protection
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
    _selectionItem.reset( selectionItem );
    if ( _target )
        _selectionItem->setParentItem( _target.data() );  // Configure Quick item
    if ( _graph )
        configureSelectionItem( _graph->getSelectionColor(), _graph->getSelectionWeight(), _graph->getSelectionMargin() );
    _selectionItem->setVisible( isSelectable() && getSelected() );
    emitSelectionItemChanged();
}

void    Selectable::configureSelectionItem( QColor selectionColor, qreal selectionWeight, qreal selectionMargin )
{
    if ( _selectionItem != nullptr ) {
        _selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
        QObject* rectangleBorder = _selectionItem->property( "border" ).value<QObject*>();
        if ( rectangleBorder != nullptr )
            rectangleBorder->setProperty( "color", QVariant::fromValue<QColor>(selectionColor) );
    }
    configureSelectionItem( selectionWeight, selectionMargin );
}

void    Selectable::configureSelectionItem( qreal selectionWeight, qreal selectionMargin )
{
    if ( _selectionItem != nullptr &&
         _target ) {
        _selectionItem->setX( -( selectionWeight / 2. + selectionMargin ) );
        _selectionItem->setY( -( selectionWeight / 2. + selectionMargin ) );

        _selectionItem->setWidth( _target->width() + selectionWeight + ( selectionMargin * 2 ));
        _selectionItem->setHeight( _target->height() + selectionWeight + ( selectionMargin * 2 ));
        _selectionItem->setOpacity( 0.80 );
        _selectionItem->setProperty( "radius", 4. );
        QObject* rectangleBorder = _selectionItem->property( "border" ).value<QObject*>();
        if ( rectangleBorder != nullptr )
            rectangleBorder->setProperty( "width", selectionWeight );
    }
}

void    Selectable::updateSelectionWidth()
{
    if ( getSelectionItem() != nullptr &&
         _target &&
         _graph ) {  // Update selection item
        qreal selectionWeight{ 3. }, selectionMargin{ 3. };
        selectionWeight = _graph->getSelectionWeight();
        selectionMargin = _graph->getSelectionMargin();
        getSelectionItem()->setWidth( _target->width() + selectionWeight + ( selectionMargin * 2 ));
    }
}

void    Selectable::updateSelectionHeight()
{
    if ( getSelectionItem() != nullptr &&
        _target &&
        _graph ) {  // Update selection item
        qreal selectionWeight{3.}, selectionMargin{3.};
        selectionWeight = _graph->getSelectionWeight();
        selectionMargin = _graph->getSelectionMargin();
        getSelectionItem()->setHeight( _target->height() + selectionWeight + ( selectionMargin * 2 ));
    }
}
//-----------------------------------------------------------------------------

} // ::qan
