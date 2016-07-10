//-----------------------------------------------------------------------------
// This file is a part of the FastQMLlibrary. Copyright 2016 Benoit AUTHEMAN.
//
// \file	fqlBottomRightResizer.h
// \author	benoit@qanava.org
// \date	2016 07 08
//-----------------------------------------------------------------------------

#ifndef fqlBottomRightResizer_h
#define fqlBottomRightResizer_h

// QT headers
#include <QtQml>
#include <QQuickItem>

namespace fql {  // ::fql

/*! \brief Add a resize handler ont the bottom right of a target QML Item.
 *
 * \image html sample-resizer.png
 *
 * \image html sample-resizer.gif
 *
 * Don't forget to register the component during application initialization:
 * \code
 *   // From c++:
 *   qmlRegisterType< fql::BottomRightResizer >( "FastQml", 1, 0, "BottomRightResizer" );
 *
 *   // From QML:
 *   import FastQml 1.0 as Fql
 *   import "qrc:/FastQml" 1.0 as Fql
 *
 *   Item {
 *     id: targetItem
 *     Fql.BottomRightResizer { target: targetItem }
 *   }
 * \endcode
 *
 * \note Resizer not necessarilly has to be in \c target sibling, you can perfectly define
 * the Fql.BottomRightResizer outside of target item hierarchy, for example to avoid corrupting
 * the target childrenRect property. It is however more efficient to use the resizer as a target
 * child (it is the most common case).
 *
 * \nosubgrouping
 */
class BottomRightResizer : public QQuickItem
{
    /*! \name BottomRightResizer Object Management *///------------------------
    //@{
    Q_OBJECT
public:
    //! .
    explicit BottomRightResizer( QQuickItem* parent = nullptr );
    virtual ~BottomRightResizer( );
private:
    Q_DISABLE_COPY( BottomRightResizer )
    //@}
    //-------------------------------------------------------------------------

    /*! \name Resizer Management *///------------------------------------------
    //@{
public:
    Q_PROPERTY( QString handlerComponent READ getHandlerComponent WRITE setHandlerComponent NOTIFY handlerComponentChanged )
    void        setHandlerComponent( QString handlerComponentString );
    QString     getHandlerComponent( ) const { return _handlerComponentString; }
signals:
    void        handlerComponentChanged();
private:
    QString     _handlerComponentString{ "" };
    QScopedPointer< QQmlComponent > _handlerComponent{ nullptr };

public:
    //! Item that should be resized by this resizer (set to nullptr to disable the resizer).
    Q_PROPERTY( QQuickItem* target READ getTarget WRITE setTarget NOTIFY targetChanged )
    void        setTarget( QQuickItem* target );
    QQuickItem* getTarget( ) const { return _target.data(); }
signals:
    void        targetChanged();
private:
    QPointer< QQuickItem >  _target{ nullptr };
private slots:
    void        onTargetXChanged();
    void        onTargetYChanged();
    void        onTargetWidthChanged();
    void        onTargetHeightChanged();

public:
    /*! Size of the bottom right handler component (default to \c 9x9).
     *
     * \note Setting an empty or invalid size has not effect, use \c autoHideHandler property to control
     * handler visibility (or set it's opacity to a given value).
     */
    Q_PROPERTY( QSizeF handlerSize READ getHandlerSize WRITE setHandlerSize NOTIFY handlerSizeChanged )
    void        setHandlerSize( QSizeF handlerSize );
    QSizeF      getHandlerSize( ) const { return _handlerSize; }
signals:
    void        handlerSizeChanged();
private:
    QSizeF      _handlerSize{ 9.0, 9.0 };

public:
    /*! Color of the bottom right handler component (default to \c Qt::black).
     *
     * \note For a default rectangular handler, color is border color, background is
     * set to transparent, use getHandler()->setProperty() to change this default behaviour.
     */
    Q_PROPERTY( QColor handlerColor READ getHandlerColor WRITE setHandlerColor NOTIFY handlerColorChanged )
    void        setHandlerColor( QColor handlerColor );
    QColor      getHandlerColor( ) const { return _handlerColor; }
signals:
    void        handlerColorChanged();
private:
    QColor      _handlerColor{ Qt::black };

public:
    //! Target could not be resized below \c minimumTargetSize (default to \c 50x50).
    Q_PROPERTY( QSizeF minimumTargetSize READ getMinimumTargetSize WRITE setMinimumTargetSize NOTIFY minimumTargetSizeChanged )
    //! \sa minimumTargetSize
    void        setMinimumTargetSize( QSizeF minimumTargetSize );
    //! \sa minimumTargetSize
    QSizeF      getMinimumTargetSize( ) const { return _minimumTargetSize; }
signals:
    //! \sa minimumTargetSize
    void        minimumTargetSizeChanged();
private:
    //! \sa minimumTargetSize
    QSizeF      _minimumTargetSize{ 50., 50. };

public:
    //! Set to true to hide the resize handler component when the resize area is not hovered (default to \c false).
    Q_PROPERTY( bool autoHideHandler READ getAutoHideHandler WRITE setAutoHideHandler NOTIFY autoHideHandlerChanged )
    void        setAutoHideHandler( bool autoHideHandler );
    bool        getAutoHideHandler( ) const { return _autoHideHandler; }
signals:
    void        autoHideHandlerChanged();
private:
    bool        _autoHideHandler{ false };

private:
    QPointer< QQuickItem >  _handler{ nullptr };
    //@}
    //-------------------------------------------------------------------------

    /*! \name Resizer Management *///------------------------------------------
    //@{
protected:

    virtual bool    childMouseEventFilter(QQuickItem *item, QEvent *event) override;
private:
    //! Initial global mouse position at the beginning of a resizing handler drag.
    QPointF         _dragInitialPos{ 0., 0. };
    //! Target item size at the beginning of a resizing handler drag.
    QSizeF          _targetInitialSize{ 0., 0. };
    //@}
    //-------------------------------------------------------------------------
};

}  // ::fql

QML_DECLARE_TYPE( fql::BottomRightResizer );

struct FastQml {
    static void initialize() {
        qmlRegisterType< fql::BottomRightResizer >( "FastQml", 1, 0, "BottomRightResizer" );
    }
};

#endif // fqlBottomRightResizer_h


