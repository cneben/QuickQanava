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
// \file	qanNavigable.h
// \author	benoit@destrat.io
// \date	2015 07 19
//-----------------------------------------------------------------------------

#ifndef canNavigable_h
#define canNavigable_h

// QT headers
#include <QQuickItem>

namespace qan { // ::qan

/*! \brief Provide a pannable/zoomable container for quick items.
 *
 * Child items must manually set their parent property to this area \c containerItem:
 * Exemple code for navigating an image:
 * \code
 * // Don't forget to register the component:
 * // C++: qmlRegisterType< qan::Navigable >( "Qanava", 1, 0, "Navigable");
 * // QML: import QuickQanava 2.0 as Qan
 * Qan.Navigable {
 *   anchors.fill: parent
 *   clip: true     // Don't set clipping if Navigable is anchored directly to your main window
 *   Image {
 *       parent: imageView.containerItem    // Any direct child must manually set its parent item
 *       id: image
 *       source: "qrc:/myimage.png"
 *   }
 * }
 * \endcode
 *
 * A "navigable area" could easily be controlled with a slider control:
 * \code
 * // QML: import QuickQanava 2.0 as Qan
 *
 * Qan.Navigable {
 *   id: navigable
 *   clip: true
 *   Slider {
 *     anchors.top: navigable.top
 *     anchors.horizontalCenter: navigable.horizontalCenter
 *     width: navigable.width / 2
 *     maximumValue: navigable.zoomMax > 0. ? navigable.zoomMax : 10.
 *     minimumValue: navigable.zoomMin
 *     stepSize: 0.0   // Non 0 stepSize will create binding loops until you protect the onValueChanged with a fuzzyCompare against zoom+-stepSize
 *     value: navigable.zoom
 *     onValueChanged: { navigable.zoom = value }
 *   }
 * }
 * \endcode
 *
 * Setting the \c autoFitMode to \c Navigable::AutoFit has the following effects:
 * \copydetails qan::Navigable::AutoFit
 *
 * \note For the math behind "zoom on a point", see:
 * http://stackoverflow.com/questions/2916081/zoom-in-on-a-point-using-scale-and-translate
 * and
 * http://stackoverflow.com/questions/16657397/scale-qgraphicsitem-about-an-arbitrary-point
 */
class Navigable : public QQuickItem
{
Q_OBJECT
public:
    explicit Navigable( QQuickItem* parent = 0 );
    virtual ~Navigable( ) { }
    Navigable( const Navigable& ) = delete;

public:
    /*! \brief Enable or disable navigation (navigation is enabled by default).
     */
    Q_PROPERTY( bool navigable READ getNavigable WRITE setNavigable NOTIFY navigableChanged FINAL )
    //! \sa navigable
    inline qreal    getNavigable( ) const { return _navigable; }
    //! \sa navigable
    void            setNavigable( bool navigable ) { _navigable = navigable; emit navigableChanged(); }
private:
    //! \copydoc navigable
    bool            _navigable{ true };
signals:
    //! \sa navigable
    void            navigableChanged( );

public:
    /*! \brief Parent container for area child items.
     *
     * Items added as child of the area must manually update their parents property to \c containerItem
     *
     * Exemple code for navigating an image:
     * \code
     * // Don't forget to register the component:
     * // C++: qmlRegisterType< qan::Navigable >( "Qanava", 1, 0, "Navigable");
     * // QML: import QuickQanava 2.0
     * Qan.Navigable {
     *   anchors.fill: parent
     *   clip: true     // Don't set clipping if Navigable is anchored directly to your main window
     *   Image {
     *       parent: imageView.containerItem    // Any direct child must manually set its parent item
     *       id: image
     *       source: "qrc:/myimage.png"
     *       Component.onCompleted: {
     *         // Eventually, fit the image in view: navigable.fitInView(), where navigable is Navigable id
     *       }
     *   }
     * }
     * \endcode
     *
     */
    Q_PROPERTY( QQuickItem* containerItem READ getContainerItem NOTIFY containerItemChanged FINAL )
    //! \sa containerItem
    QQuickItem*     getContainerItem( ) { return _containerItem; }
signals:
    //! Used internally, never change containerItem.
    void    containerItemChanged( ); // Never used, avoid a QML warning

public:
    //! Center the view on a given child item (zoom level is not modified).
    Q_INVOKABLE void    centerOn( QQuickItem* item );

    /*! Fit the area content (\c containerItem childs) in view and update current zoom level.
     *
     * Area content will be fitted in view even if current AutoFitMode is NoAutoFit.
     * \sa autoFitMode
     */
    Q_INVOKABLE void    fitInView( );

public:
    //! \brief Auto fitting mode.
    enum AutoFitMode
    {
        //! No auto-fitting (default).
        NoAutoFit,
        /*! Auto fit area content when the area is resized without user modified zoom or pan, and eventually auto center content.
         *
         * Setting \c autoFitMode to \c AutoFit has the following effects:
         * \li Content will be fitted automatically when the view is resized if user has not already applied a custom zoom
         * or pan.
         * \li If the user has applied a custom zoom or pan, the content will be automatically centered if the transformed
         * container item is smaller than navigable area.
         * \li Content will be anchored to the left or the right view borders when its width is larger than the navigable area, but there
         * is a space "in panning" between content and view border.
         */
        AutoFit
    };
    Q_ENUM( AutoFitMode )

    /*! \brief Current auto-fit mode (default to NoAutoFit).    */
    Q_PROPERTY( AutoFitMode autoFitMode READ getAutoFitMode WRITE setAutoFitMode NOTIFY autoFitModeChanged FINAL )
    //! \sa autoFitMode
    AutoFitMode getAutoFitMode( ) const { return _autoFitMode; }
    //! \sa autoFitMode
    void        setAutoFitMode( AutoFitMode autoFitMode );
private:
    //! \copydoc autoFitMode
    AutoFitMode _autoFitMode{ NoAutoFit };
signals:
    //! \sa autoFitMode
    void        autoFitModeChanged( );
private:
    //! Flag set to true if area panning has been modified since the last fitInView() call.
    bool        _panModified{ false };
    //! Flag set to true if area zoom has been modified since the last fitInView() call.
    bool        _zoomModified{ false };

public:
    /*! \brief Zoom incrementation delta (default to 0.05).
     */
    Q_PROPERTY( qreal zoomIncrement READ getZoomIncrement WRITE setZoomIncrement NOTIFY zoomIncrementChanged FINAL )
    //! \sa zoomIncrement
    qreal       getZoomIncrement( ) const { return _zoomIncrement; }
    //! \sa zoomIncrement
    void        setZoomIncrement( qreal zoomIncrement ) { _zoomIncrement = zoomIncrement; emit zoomIncrementChanged(); }
private:
    //! \copydoc zoomIncrement
    qreal       _zoomIncrement{0.05 };
signals:
    //! \sa zoomIncrement
    void        zoomIncrementChanged( );

public:
    /*! \brief Area current zoom level (default to 1.0).
     *
     * Setting a zoom value outside the (minZoom, maxZoom) range has no effect.
     *
     * Accessing zoom directly from QML is safe, since setZoom() is protected against binding loops.
     */
    Q_PROPERTY( qreal zoom READ getZoom WRITE setZoom NOTIFY zoomChanged FINAL )
    //! \sa zoom
    qreal       getZoom( ) const { return _zoom; }
    /*! \brief Set navigable area current zoom (zoom is applied on current \c zoomOrigin).
     *
     * \note To avoid QML binding loops, this setter is protected against setting the same value multiple times.
     * \sa zoom
     */
    void        setZoom( qreal zoom );
    //! Set area current zoom centered on a given \c center point.
    void        zoomOn( QPointF center, qreal zoom );
    //! Return true if zoom is valid (ie it is different from the actual zoom and in the (minZoom, maxZoom) range.
    bool        isValidZoom( qreal zoom ) const;
private:
    //! \copydoc zoom
    qreal       _zoom{ 1.0 };
signals:
    //! \sa zoom
    void        zoomChanged( );

public:
    /*! \brief Origin point where any zoom set view setZoom or \c zoom will be applied (default to QQuickItem::Center).
     */
    Q_PROPERTY( QQuickItem::TransformOrigin zoomOrigin READ getZoomOrigin WRITE setZoomOrigin NOTIFY zoomOriginChanged FINAL )
    //! \sa zoomOrigin
    QQuickItem::TransformOrigin       getZoomOrigin( ) const { return _zoomOrigin; }
    /*! \brief Set navigable area current zoom origin (either QQuickItem::TopLeft or QQuickItem::Center).
     *
     * \note Zooming initiated via mouse wheel is always applied at current mouse position.
     * \sa zoom
     */
    void        setZoomOrigin( QQuickItem::TransformOrigin zoomOrigin );
private:
    //! \copydoc zoomOrigin
    QQuickItem::TransformOrigin _zoomOrigin{ QQuickItem::Center };
signals:
    //! \sa zoomOrigin
    void        zoomOriginChanged( );

public:
    //! Area maximum zoom level (-1 = no maximum zoom limitation, 1.0 = no zoom allowed, >1.0 = zoomMax*100% maximum zoom).
    Q_PROPERTY( qreal zoomMax READ getZoomMax WRITE setZoomMax NOTIFY zoomMaxChanged FINAL )
    //! \sa zoomMax
    qreal       getZoomMax( ) const { return _zoomMax; }
    //! \sa zoomMax
    void        setZoomMax( qreal zoomMax );
private:
    //! \copydoc zoomMax
    qreal       _zoomMax{ -1.0 };
signals:
    //! \sa zoomMax
    void        zoomMaxChanged( );

public:
    //! Area minimum zoom level, default to 0.1 (10% zoom), zoomMin can't be <= 0.
    Q_PROPERTY( qreal zoomMin READ getZoomMin WRITE setZoomMin NOTIFY zoomMinChanged FINAL )
    //! \sa zoomMin
    qreal       getZoomMin( ) const { return _zoomMin; }
    //! \sa zoomMin
    void        setZoomMin( qreal zoomMin );
private:
    //! \copydoc zoomMin
    qreal       _zoomMin{ 0.1 };
signals:
    //! \sa zoomMin
    void        zoomMinChanged( );

signals:
    //! Emitted whenever the mouse is right clicked in the containe.
    void    rightClicked( QVariant pos );

    //! Emitted whenever the container item is scaled (zoomed) or panned.
    void    containerItemModified( );

protected:
    virtual void    geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry ) override;
    virtual void    mouseMoveEvent( QMouseEvent* event ) override;
    virtual void    mousePressEvent( QMouseEvent* event ) override;
    virtual void    mouseReleaseEvent( QMouseEvent* event ) override;
    virtual void    wheelEvent( QWheelEvent* event ) override;

protected:
    QQuickItem* _containerItem{ nullptr };
    bool        _leftButtonPressed{ false };
    QPointF     _lastPan{};
};

} // ::qan

QML_DECLARE_TYPE( qan::Navigable )

#endif // qanNavigable_h

