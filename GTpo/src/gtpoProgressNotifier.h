/*
    The MIT License (MIT)

    Copyright (c) 2016 Benoit AUTHEMAN

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

//-----------------------------------------------------------------------------
// This file is a part of the GTpo software library.
//
// \file	gtpoProgressNotifier.h
// \author	benoit@destrat.io
// \date	2016 01 31
//-----------------------------------------------------------------------------

#ifndef gtpoProgressNotifier_h
#define gtpoProgressNotifier_h

// GTpo headers
#include "./gtpoUtils.h"
#include "./gtpoGraph.h"

// STD headers
#include <deque>
#include <iostream>

namespace gtpo { // ::gtpo

class ProgressNotifier;

/*! \brief Standard interface used to monitor progress long operations, either synchronous or asynchronous, such as in/out serialization.
 *
 * GTpo support two related kind of progress concepts:
 * \li overall progress: global task completion between 0.0 and 1.0 with setProgress() and getProgress() methods
 * \li phase progress: a long operation could be split in different phases (for example node serialization phase, then
 * edge serialization and so on), where each phase has a progress between 0.0 and 1.0.
 *
 * Here is a summary of the interaction between a long \c task and a progress \c notifier:
 * \li Progress \c notifier is passed as an argument of the long \c task method or functor.
 * \li First, the task initialize the number of phases that are usually required to complete the task with a call to setPhaseCount().
 * \li When the \c task change phase, the nextPhase() method is called.
 * \li The \c task then set the current phase progress with calls to setPhaseProgress(), the overall progress
 * is automatically updated by this notifyer and could be accessed with getProgress().
 * \li When the task ends, it call endProgress().
 *
 * Example code with a notifier and a sub notifier for sub tasks:
 * \code
 * void    Consumer::consume( gtpo::IProgressNotifier& notifier )
 * {
 *   notifier.reset();
 *   notifier.reserveSubProgress( 2 );
 *   try {
 *     gtpo::IProgressNotifier& subNotifier1 = notifier.takeSubProgress();
 *     subNotifier1.reserveSubProgress( 2 );
 *
 *     gtpo::IProgressNotifier& subNotifier2 = notifier.takeSubProgress();
 *     subNotifier2.reserveSubProgress( 3 );
 *
 *     notifier->beginProgress();
 *       gtpo::IProgressNotifier& subNotifier11 = subNotifier1.takeSubProgress( "Sub progress 1 - phase1" );
 *       consumePhase( subNotifier11 );  // Consume phase just call setProgress() mutliple times from 0. to 1.
 *
 *       gtpo::IProgressNotifier& subNotifier12 = subNotifier1.takeSubProgress( "Sub Progress 1 - phase2" );
 *       consumePhase( subNotifier12 );
 *
 *       gtpo::IProgressNotifier& subNotifier21 = subNotifier2.takeSubProgress( "Sub progress 2 - phase1");
 *       consumePhase( subNotifier21 );
 *
 *       gtpo::IProgressNotifier& subNotifier22 = subNotifier2.takeSubProgress( "Sub progress 2 - phase2" );
 *       consumePhase( subNotifier22 );
 *
 *       gtpo::IProgressNotifier& subNotifier23 = subNotifier2.takeSubProgress( "Sub progress 3 - phase3" );
 *       consumePhase( subNotifier23 );
 *    } catch ( std::exception e ) { std::cerr << "Consumer::concume(): Error while reporting progress: " << e.what() << std::endl; }
 *    notifier.endProgress();    // Calling enProgress() is not mandatory on intermediate sub progress.
 *  }
 * \endcode
 *
 * \image html IProgressNotifier.gif
 *
 * Update of overall or phase progress is protected against values outside of the [0.;1.0] range.
 *
 * IProgressNotifier is the default progress notifier, and it don't has state nor notify anything, any decent compiler
 * should just generate no overhead at runtime when parametrized with such class.
 *
 * \nosubgrouping
 */
class IProgressNotifier
{
    /*! \name IProgressNotifier Object Management *///-------------------------
    //@{
public:
    //! Construct a default notifiyer with 0.0 progress and 0 \c phaseCount.
    IProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        _superProgress( superProgress ) { }
    virtual ~IProgressNotifier() { _subProgress.clear(); }
    //@}
    //-------------------------------------------------------------------------

    /*! \name Sub/Sup Progress Management *///---------------------------------
    //@{
protected:
    IProgressNotifier*  getSuperProgress( ) { return _superProgress; }
private:
    IProgressNotifier*  _superProgress = nullptr;

public:
    //! Reset the progress notifier (destroy all sub progress).
    virtual void    reset() {
        _subProgressQueue.clear( );
        _subProgress.clear();
        notifyModified();
    }

    using   IProgressNotifierPtr = std::unique_ptr< IProgressNotifier >;

    /*! Return a sub progress notifier previously reserved using reserveSubProgress(), or throw std::exception if no reservation was made.
     *
     * Creating a sub progress will introduce a new level in progress reporting. For example, if you are
     * extending GTpo, you might have custom serialization to process with its own progress reporting.
     * In your specialized serialization method, use the following code:
     * \li Get the super progress notifier given by the user as an argument.
     * \li Reserve the number of subprogress necessary to model all you serialization "phases".
     * \li Call setProgress() on sub progress notifiers returned by takeSubProgress()
     *
     * \code
     * auto mySerializationMethod( gtpo::ProgressNotifier& progress ) -> void {
     *   progress.reserveSubProgress( 2 );  // Reserve sub progress before serializing
     *           // to allow GTpo compute a global progress in "super progress notifiers"
     *
     *   progress.reserveSubProgress( 2 );

     *   // Phase 1
     *   gtpo::IProgressNotifier& progress1 = progress.takeSubProgress( "Phase1" );
     *   // Serialize something with progress 1:
     *   progress1.setProgress( 0.25 ); // 25%
     *   // ...
     *   progress1.setProgress( 0.75 ); // 75%
     *   progress1.endProgress()
     *
     *   // Phase 2
     *   gtpo::IProgressNotifier* progress2 = progress.takeSubProgress( "Phase2" );
     *   // ...
     * }
     * \endcode
     *
     * \note Seting \c beginImmediately automatically call beginProgress() with \c label.
     * \throw std::exception when sub progress is nullptr or sub notifier has not been reserved with a call to reserveSubProgress().
     */
    virtual IProgressNotifier&  takeSubProgress( const std::string& label = "", bool beginImmediately = true ) {
        if ( _subProgressQueue.empty() )
            throw std::runtime_error( "gtpo::ProgressNotifier::takeSubProgress(): Error: trying to access a sub progress notifier without a reservation." );
        IProgressNotifier* subProgress = _subProgressQueue.front( );
        _subProgressQueue.pop_front( );
        if ( subProgress == nullptr )
            throw std::runtime_error( "gtpo::ProgressNotifier::takeSubProgress(): Error: trying to access a nullptr sub progress notifier." );
        if ( beginImmediately )
            beginProgress( label );
        return *subProgress;
    }

    //! Reserve a given \c subProgressCount number of sub prgress in this notifier.
    virtual void                reserveSubProgress( int subProgressCount ) {
        for ( int p = 0; p < subProgressCount; ++p ) {
            IProgressNotifier* subProgress = createSubProgress( );
            IProgressNotifierPtr progressPtr( subProgress );
            _subProgressQueue.push_back( subProgress );
            _subProgress.emplace_back( std::move( progressPtr ) );
        }
    }

protected:
    virtual IProgressNotifier*  createSubProgress() {
        return nullptr;
    }

    const std::vector< IProgressNotifierPtr >&  takeSubProgress() const { return _subProgress; }
private:
    std::deque< IProgressNotifier* >            _subProgressQueue;
    std::vector< IProgressNotifierPtr >         _subProgress;
    //@}
    //-------------------------------------------------------------------------

    /*! \name Progress Reporting *///------------------------------------------
    //@{
public:
    //! Called for user notification purposes when notifier change (default implementation notify super notifier).
    virtual void    notifyModified() {
        if ( getSuperProgress() != nullptr ) {
            getSuperProgress()->setLabel( _label );         // Push phase label and progress upstream
            getSuperProgress()->setPhaseProgress( getPhaseProgress() );
            getSuperProgress()->notifyModified( );
        }
        onModified();
    }

    /*! \brief Called at the start of the monitored process, indicate to an eventually associed progress display that it must be shown (call reset() automatically).
     *
     * \warning User should call base method at the begining of their implementation.
     */
    virtual void    beginProgress( const std::string& label = "" ) { setLabel( label ); }

    /*! \brief Called at the end of the monitored process or if the process has been interrupted, indicate to an eventually associed progress display that it must be shut down.
     *
     * \warning User should call base method at the end of their implementation.
     */
    virtual void    endProgress() { }

protected:
    //! Virtual notifier method that should be overriden in user code to be notified of phase/label or global progress changes.
    virtual void    onModified() = 0;

public:
    //! Set the current global progress, might equal global progress for leaf progress notifiers (\c progress must be in the (0.0, 1.0) range).
    virtual void    setProgress( double progress ) {
        _phaseProgress = progress;
        if ( getSuperProgress() != nullptr ) {
            getSuperProgress()->setPhaseProgress( getPhaseProgress() );
            getSuperProgress()->notifyModified( );
        }
        onModified();
    }

    /*! Return the current overall progress, taking into account configured \c phaseCount and current phase progress.
     *
     * When a sub progress notifier is attached to this progress notifier, getProgress()
     * return a global progress valid for all progress notifier, otherwise, it return the
     * notifier "local" progress.
     */
    virtual double  getProgress() const {
        double progress = 0.;
        if ( takeSubProgress().size() <= 0 )   // When there is no sub notifiers, return local progress
            progress = _phaseProgress;
        else {   // Otherwise, average all sub notifiers progress
            for ( auto& subProgress : takeSubProgress() )
                progress += subProgress->getProgress();
            progress /= static_cast< double >( takeSubProgress().size() );
        }
        return progress;
    }

    double          getPhaseProgress() const {
        return ( _subProgress.size() <= 0 ? getProgress() : _phaseProgress );
    }

protected:
    //! Set the current phase progress (\c phaseProgress must be in the (0.0, 1.0) range.
    void            setPhaseProgress( double phaseProgress ) {
        _phaseProgress = phaseProgress;
        if ( getSuperProgress() != nullptr ) {
            getSuperProgress()->setPhaseProgress( getPhaseProgress() );
            getSuperProgress()->notifyModified( );
        }
        onModified();
    }

    //! Current phase progress.
    double          _phaseProgress = 0.;

public:
    //! Return current phase label.
    virtual const std::string&  getLabel() const { return _label; }

    //! Used to set the current phase label from a sub notifier, from a super progress notifier, use beginPhase().
    virtual void                setLabel( const std::string& label ) {
        _label = label;
        if ( getSuperProgress() != nullptr ) {
            getSuperProgress()->setLabel( _label );         // Push phase label and progress upstream
            getSuperProgress()->notifyModified( );
        }
        onModified();
    }

protected:
    //! Current progress label.
    std::string _label  = "";
    //@}
    //-------------------------------------------------------------------------
};


/*! \brief Multi levels, multi phases progress notifier (basic concrete implementation of IProgressNotifier).
 *
 * \nosubgrouping
 */
class ProgressNotifier : public IProgressNotifier
{
public:
    ProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        IProgressNotifier( superProgress ) { }

protected:
    virtual IProgressNotifier*  createSubProgress() { return new ProgressNotifier( this ); }

protected:
    virtual void    onModified() { }
};


/*! \brief Report progress in an output stream (default to std::cerr).
 *
 * Minimal demonstration for a concrete progress notifier.
 */
class EchoProgressNotifier : public ProgressNotifier
{
public:
    EchoProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        ProgressNotifier( superProgress ) { }

public:
    virtual void    onModified() override {
        std::cout << "Progress=" << getProgress() << std::endl;
        std::cout << "Phase progress=" << getPhaseProgress() << std::endl;
    }
protected:
    virtual IProgressNotifier*  createSubProgress() { return new EchoProgressNotifier( this ); }
};

} // ::gtpo

#endif // gtpoProgressNotifier_h

