/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
