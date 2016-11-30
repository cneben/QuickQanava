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
// \file    qanStyle.h
// \author  benoit@destrat.io
// \date    2015 06 05
//-----------------------------------------------------------------------------

#ifndef qanStyle_h
#define qanStyle_h

// QuickProperties headers
#include "../QuickProperties/src/qpsProperties.h"

// QT headers
#include <QColor>
#include <QFont>

namespace qan { // ::qan

/*! Models a set of properties affecting a graph primitive visual appearance.
 *
 *  Style instances should be created from StyleManager createStyle() or createStyleFrom() methods,
 *  and associed to nodes trought their setStyle() method. Changing a style property will
 *  automatically be reflected in styled nodes or edges appearance. While Style use standard QObject
 *  properties to store style settings, the qps::Properties interface should be use to sets properties
 *  or style edition for Properties exposed trought Interview item model will fail.
 *
 *  Main qan::Style properties are:
 * \li \b name: Style name as it will appears in style edition dialogs.
 * \li \b target: The concrete style target, for example a style dedicated to a qan::CustomNode will have "qan::CustomNode" as its target.
 * \li \b metaTarget: "qan::Node" for a node style, "qan::Edge" for an edge style and so on. This property is used for
 *      accessing list models of styles for different categories of graph primitive, see method qan::Style::Manager::getStylesModelForTarget()
 *      for more documentation.
 * \sa qan::Node::setStyle()
 * \sa qan::Edge::setStyle()
 */
class Style : public qps::Properties
{
    Q_OBJECT
    /*! \name Style Object Management *///-------------------------------------
    //@{
public:
    /*! Style constructor with style name initialisation.
     * \param target  class name for this style default target (ex: with target
     * set to qan::Node, style will be applied by default to every qan::Node
     * created in the graph).
     */
    explicit Style( QString name = "", QString target = "", QString metaTarget = "", QObject* parent = 0 );
    virtual ~Style( ) { }
    Style( const Style& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Style Management *///--------------------------------------------
    //@{
public:
    //! Style target (class name of the target primitive for example "qan::Node" or "qan::Edge").
    Q_PROPERTY( QString target READ getTarget WRITE setTarget NOTIFY targetChanged FINAL )
    //! Get this style default target class name (eg qan::Node for a style that apply to node).
    QString     getTarget( ) { return _target; }
    //! Defined only for serialization purposes, do not change style target dynamically.
    void        setTarget( QString target ) { _target = target; emit targetChanged(); }
signals:
    //! \sa target
    void        targetChanged( );
private:
    //! \sa target
    QString     _target = QString{ "" };

public:
    //! Style meta target (root class name of the target primitive for example "qan::Node" for a custom qan::CustomNode node).
    Q_PROPERTY( QString metaTarget READ getMetaTarget WRITE setMetaTarget NOTIFY metaTargetChanged FINAL )
    //! Get this style default meta target class name, it should return either "qan::Node", "qan::Edge" or "qan::HEdge".
    QString     getMetaTarget( ) { return _metaTarget; }
    //! Defined only for serialization purposes, do not change style meta target dynamically.
    void        setMetaTarget( QString metaTarget ) { _metaTarget = metaTarget; emit metaTargetChanged(); }
signals:
    //! \sa metaTarget
    void        metaTargetChanged( );
private:
    //! \sa metaTarget
    QString     _metaTarget = QString{ "" };
protected:
    //! Style target class name is usually set in Style ctor.
    void        setTargets( QString target, QString metaTarget ) { _target = target; _metaTarget = metaTarget; }

public:
    Q_PROPERTY( QString name READ getName WRITE setName NOTIFY nameChanged FINAL )
    void        setName( QString name ) { _name = name; emit nameChanged( ); }
    QString     getName( ) { return _name; }
signals:
    void        nameChanged( );
private:
    QString     _name = QString{ "" };

public:
    /*! Generate a deep copy of this Style object, ownsership return to the caller.
     *
     * Static properties of this source object may be converted in dynamic properties in
     * destination object as their is no way to dynamically generate static properties with
     * QObject outside QML.
     *
     * Existing properties (either static or dynamic) in duplicated object will be set to
     * this style value (for exemple objectName, target, etc.).
     *
     * \note 20150630: deep copy of time values is currently not supported.
     * \param   dstParent optionnal parent for the duplicated style object.
     */
    qan::Style*     duplicate( QString duplicatedStyleName, QObject* dstParent = nullptr );
    //@}
    //-------------------------------------------------------------------------
};

class NodeStyle : public qan::Style
{
    Q_OBJECT

    /*! \name NodeStyle Object Management *///---------------------------------
    //@{
public:
    /*! Style constructor with style \c name and \c target initialisation.
     *
     * Style \c metaTarget is "qan::Node". NodeStyle objects are usually created
     * with qan:StyleManager::createNodeStyle() factory method.
     */
    explicit NodeStyle( QString name = "", QString target = "", QObject* parent = 0 );
    virtual ~NodeStyle( ) { }
    NodeStyle( const NodeStyle& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Properties Management *///---------------------------------------
    //@{
public:
    Q_PROPERTY( QColor backColor READ getBackColor WRITE setBackColor NOTIFY backColorChanged FINAL )
    void            setBackColor( const QColor& backColor ) { _backColor = backColor; emit backColorChanged( ); }
    const QColor&   getBackColor( ) const { return _backColor; }
protected:
    QColor          _backColor = QColor( Qt::white );
signals:
    void            backColorChanged( );

public:
    Q_PROPERTY( QColor borderColor READ getBorderColor WRITE setBorderColor NOTIFY borderColorChanged FINAL )
    void            setBorderColor( const QColor& borderColor ) { _borderColor = borderColor; emit borderColorChanged( ); }
    const QColor&   getBorderColor( ) const { return _borderColor; }
protected:
    QColor          _borderColor = QColor( Qt::black );
signals:
    void            borderColorChanged( );

public:
    Q_PROPERTY( qreal borderWidth READ getBorderWidth WRITE setBorderWidth NOTIFY borderWidthChanged FINAL )
    void            setBorderWidth( qreal borderWidth ) { _borderWidth = borderWidth; emit borderWidthChanged( ); }
    qreal           getBorderWidth( ) const { return _borderWidth; }
protected:
    qreal           _borderWidth = 1.0;
signals:
    void            borderWidthChanged( );

public:
    Q_PROPERTY( QFont labelFont READ getLabelFont WRITE setLabelFont NOTIFY labelFontChanged FINAL )
    void            setLabelFont( QFont labelFont ) { _labelFont = labelFont; emit labelFontChanged( ); }
    QFont           getLabelFont( ) const { return _labelFont; }
protected:
    QFont           _labelFont;
signals:
    void            labelFontChanged( );

public:
    Q_PROPERTY( bool hasShadow READ getHasShadow WRITE setHasShadow NOTIFY hasShadowChanged FINAL )
    void            setHasShadow( bool hasShadow ) { _hasShadow = hasShadow; emit hasShadowChanged( ); }
    bool            getHasShadow( ) const { return _hasShadow; }
protected:
    bool            _hasShadow = true;
signals:
    void            hasShadowChanged( );

public:
    Q_PROPERTY( QColor shadowColor READ getShadowColor WRITE setShadowColor NOTIFY shadowColorChanged FINAL )
    void            setShadowColor( QColor shadowColor ) { _shadowColor = shadowColor; emit shadowColorChanged( ); }
    QColor          getShadowColor( ) const { return _shadowColor; }
protected:
    QColor          _shadowColor = QColor{ 0, 0, 0, 127 };
signals:
    void            shadowColorChanged( );

public:
    Q_PROPERTY( QSizeF shadowOffset READ getShadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged FINAL )
    void            setShadowOffset( QSizeF shadowOffset ) { _shadowOffset = shadowOffset; emit shadowOffsetChanged( ); }
    QSizeF          getShadowOffset( ) const { return _shadowOffset; }
protected:
    QSizeF          _shadowOffset = QSizeF{ 3., 3. };
signals:
    void            shadowOffsetChanged( );
    //@}
    //-------------------------------------------------------------------------
};

class EdgeStyle : public qan::Style
{
    Q_OBJECT

    /*! \name EdgeStyle Object Management *///---------------------------------
    //@{
public:
    /*! Edge style constructor with style \c name and \c target initialisation.
     *
     * Style \c metaTarget is "qan::Edge". EdgeStyle objects are usually created
     * with qan:StyleManager::createNodeStyle() factory method.
     */
    explicit EdgeStyle( QString name = "", QString target = "", QObject* parent = 0 );
    virtual ~EdgeStyle( ) { }
    EdgeStyle( const EdgeStyle& ) = delete;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Properties Management *///---------------------------------------
    //@{
signals:
    void            styleModified();

public:
    Q_PROPERTY( QColor lineColor READ getLineColor WRITE setLineColor NOTIFY lineColorChanged FINAL )
    void            setLineColor( const QColor& lineColor ) { _lineColor = lineColor; emit lineColorChanged( ); emit styleModified(); }
    const QColor&   getLineColor( ) const { return _lineColor; }
protected:
    QColor          _lineColor = QColor( 0, 0, 0, 255 );
signals:
    void            lineColorChanged( );

public:
    Q_PROPERTY( qreal lineWidth READ getLineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL )
    void            setLineWidth( qreal lineWidth ) { _lineWidth = lineWidth; emit lineWidthChanged( ); emit styleModified();  }
    qreal           getLineWidth( ) const { return _lineWidth; }
protected:
    qreal           _lineWidth = 2.0;
signals:
    void            lineWidthChanged( );

public:
    Q_PROPERTY( qreal arrowSize READ getArrowSize WRITE setArrowSize NOTIFY arrowSizeChanged FINAL )
    void        setArrowSize( qreal arrowSize ) { _arrowSize = arrowSize; emit arrowSizeChanged( ); emit styleModified();  }
    qreal       getArrowSize( ) const { return _arrowSize; }
protected:
    qreal       _arrowSize = 4.0;
signals:
    void        arrowSizeChanged( );
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::Style )
QML_DECLARE_TYPE( qan::NodeStyle )
QML_DECLARE_TYPE( qan::EdgeStyle )

#endif // qanStyle_h

