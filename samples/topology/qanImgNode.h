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
// \file	qanImgNode.h
// \author	benoit@qanava.org
// \date	2016 02 09
//-----------------------------------------------------------------------------

#ifndef qanImgNode_h
#define qanImgNode_h

// QuickQanava headers
#include <QuickQanava>

// Qt headers
#include <QQuickPaintedItem>

namespace qan { // ::qan

class ImgNode : public qan::Node
{
    Q_OBJECT
public:
    explicit ImgNode( QQuickItem* parent = nullptr ) :
        qan::Node( parent ) { }
    virtual ~ImgNode();
    virtual auto    getDynamicClassName() -> std::string { return "qan::ImgNode"; }
private:
    Q_DISABLE_COPY( ImgNode )
public:
    Q_PROPERTY( QImage image READ getImage WRITE setImage NOTIFY imageChanged )
    const QImage&       getImage( ) const { return _image; }
    void                setImage( QImage image ) { _image = image; emit imageChanged(); }
    Q_INVOKABLE void    loadImageFromFile( QString fileName );
private:
    QImage      _image;
signals:
    void        imageChanged();
};

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit ImageItem( QQuickItem* parent = nullptr ) :
        QQuickPaintedItem( parent ) { setAntialiasing(true); }
    virtual ~ImageItem();
private:
    Q_DISABLE_COPY( ImageItem )
public:
    virtual void    paint( QPainter* painter ) override;

public:
    Q_PROPERTY( QImage image READ getImage WRITE setImage NOTIFY imageChanged )
    const QImage&   getImage( ) const { return _image; }
    void            setImage( QImage image ) { _image = image; emit imageChanged(); }
private:
    QImage          _image;
signals:
    void        imageChanged();
};

} // ::qan

QML_DECLARE_TYPE( qan::ImgNode )
QML_DECLARE_TYPE( qan::ImageItem )

#endif // qanImgNode_h

