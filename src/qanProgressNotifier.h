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
// This file is a part of the Qanava software.
//
// \file	qanProgressNotifier.h
// \author	benoit@qanava.org
// \date	2016 02 15
//-----------------------------------------------------------------------------

#ifndef qanProgressNotifier_h
#define qanProgressNotifier_h

// GTpo headers
#include "gtpoSerializer.h"

// QuickQanava headers
#include "./qanConfig.h"

namespace qan { // ::qan

/*! Standard interface used to monitor progress for long synchronous operation, such as in/out serialization.
 *
 * \note QApplication::processEvents() will be called automatically in setProgress() every time the progress changes from more
 *       than 5% to ensure that the calling UI will stay responsive.
 * \sa qan::Graph::serializeIn(), qan::Graph::serializeOut()
 */
class ProgressNotifier : public QObject,
                         public gtpo::ProgressNotifier
{
    /*! \name ProgressNotyfier Object Management *///--------------------------
    //@{
    Q_OBJECT
public:
    ProgressNotifier( QObject* parent = nullptr );
    virtual ~ProgressNotifier() { }
private:
    Q_DISABLE_COPY( ProgressNotifier )
    //@}
    //-------------------------------------------------------------------------


    /*! \name Progress Monitoring *///-----------------------------------------
    //@{
public:
    /*! Read-only value of the current overall progress between 0.0 (action 0% done) and 1.0 (action 100% done).
     *
     */
    Q_PROPERTY( double progress READ getProgress NOTIFY progressChanged )

    //! Called from task, force show of associed progress dialog.
    void    beginProgress() {
        gtpo::ProgressNotifier::beginProgress();

        //qDebug() << "qan::ProgressNotifier::beginProgress()";
        emit showProgress();
        QCoreApplication::processEvents( );
        QCoreApplication::processEvents( );
        QCoreApplication::processEvents( );
        QCoreApplication::processEvents( );
        QCoreApplication::processEvents( );
    }
    //! Called from task, force progress end and hide associed dialog.
    void    endProgress() {
        emit hideProgress();
        QCoreApplication::processEvents( );
    }
signals:
    //! \sa progress
    void            progressChanged();
    void            showProgress();
    void            hideProgress();

protected:
    //! Called whenever the overall progress change.
    virtual void    notifyProgressChanged() override {
        qDebug() << "notifyProgressChanged(): progress=" << getProgress();
        emit progressChanged();
        QCoreApplication::processEvents( );
    }

public:
    /*! Read-only progress value for current phase between 0. (action 0% done) and 1. (action 100% done), or -1 if progression is unknown.
     *
     * The action currently monitored could eventually be cancelled with a call to cancel() if the receiver supports it.
     */
    Q_PROPERTY( double phaseProgress READ getPhaseProgress NOTIFY phaseProgressChanged )
signals:
    //! \sa phaseProgress
    void            phaseProgressChanged();
protected:
    virtual void    notifyPhaseProgressChanged() override {
        emit phaseProgressChanged(); QCoreApplication::processEvents( );
    }

public:
    /*! Label for the current progress phase, usually set in beginPhase().
     *
     * FIXME: Reactivate phase label support in Qan2...
     */
    Q_PROPERTY( QString phaseLabel READ getQmlPhaseLabel NOTIFY phaseLabelChanged )
    QString         getQmlPhaseLabel() const { return QString::fromStdString( getPhaseLabel() ); }
signals:
    //! \sa phaseLabel
    void            phaseLabelChanged();
protected:
    virtual void    notifyPhaseLabelChanged() override { emit phaseLabelChanged(); QCoreApplication::processEvents( ); }
    //@}
    //-------------------------------------------------------------------------
};

} // ::qan

QML_DECLARE_TYPE( qan::ProgressNotifier )

#endif // qanProgressNotifier_h

