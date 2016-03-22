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
// \author	benoit@qanava.org
// \date	2016 01 31
//-----------------------------------------------------------------------------

#ifndef gtpoProgressNotifier_h
#define gtpoProgressNotifier_h

// GTpo headers
#include "./gtpoUtils.h"
#include "./gtpoGraph.h"

// STD headers
#include <fstream>
#include <atomic>

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
 * \li When the \c task change phase, the nextPhase() method is called with the estimated weight of the
 * phase in \c task completion. The sum of all weight arguments to nextPhase() must be 1.0.
 * \li The \c task then set the current phase progress with calls to setPhaseProgress(), the overall progress
 * is automatically updated by this notifyer and could be accessed with getProgress().
 * \li When the task ends, it call endProgress().
 *
 * Update of overall or phase progress is protected against values outside of the [0.;1.0] range.
 *
 * IProgressNotifier is the default progress notifier, and it don't has state nor notify anything, any decent compiler
 * should just generate no overhead at runtime when parametrized with such class.
 *
 */
class IProgressNotifier
{
public:
    //! Construct a default notifiyer with 0.0 progress and 0 \c phaseCount.
    IProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        _superProgress( superProgress ) { }
    virtual ~IProgressNotifier() { _notifiers.clear(); }

protected:
    IProgressNotifier*  getSuperProgress( ) { return _superProgress; }
private:
    IProgressNotifier*  _superProgress = nullptr;

public:
    //! Called by a sub progress to force progress update of a super progress.
    virtual void    update( ) {
        if ( getSuperProgress() != nullptr )    // Update any eventual super progress
            getSuperProgress()->update();
    }

    //! Reset the progress notifier (destroy all sub progress).
    virtual void    reset() {
        _notifiers.clear();
    }

    using   IProgressNotifierPtr = std::unique_ptr< IProgressNotifier >;

    /*! Return a new sub progress notifier (this progress keep ownership for the sub progress).
     *
     * Creating a sub progress will introduce a new level in progress reporting. For example, if you are
     * extending GTpo, you might have custom serialization to do with its own progress reporting. In your
     * serialization method:
     * \li Create a progress object.
     * \li Add a new sub progress.
     * \li Call base GTpo serialization methods with the subprogress.
     * \li Then, serialize your data using the root progress notifier.
     *
     * \code
     *    gtpo::ProgressNotifier progress;
     *    // progress.getLevelCount() == 1
     *    gtpo::IProgressNotifier* progress2 = progress.getSubProgress();
     *
     *    // Register progress2 in a gtpo::Serializer
     *    // Call gtpo::Serializer::serialize( )
     *
     *    // Then report the serialization of your own data in progress
     * \endcode
     *
     * \sa getLevelCount()
     */
    virtual IProgressNotifier*  getSubProgress() {
        IProgressNotifier* subProgress = new IProgressNotifier( this );
        IProgressNotifierPtr progressPtr( subProgress );
        _notifiers.emplace_back( std::move( progressPtr ) );
        return subProgress;
    }

    /*!  \brief Return the progress level count (ie the number of sub progress objects registered plus one for this level).
     *
     * \code
     *    gtpo::ProgressNotifier progress;
     *    // progress.getLevelCount() == 1
     *    gtpo::IProgressNotifier* progress2 = progress.getSubProgress();
     *    // progress.getLevelCount() == 2
     * \endcode
     */
    double  getLevelCount( ) const {
        return 1.0 + (double)_notifiers.size();
    }
protected:
    std::vector< IProgressNotifierPtr > _notifiers;

public:
    //! Return the current overall progress, taking into account configured \c phaseCount and current phase progress.
    virtual double  getProgress() const { return 0.; }

    //! Called for user notification purposes when progress change (default implentation is empty).
    virtual void    notifyProgressChanged() { }
    //! Called for user notification purposes when phase progress change (default implentation is empty).
    virtual void    notifyPhaseProgressChanged() { }
    //! Called for user notification purposes when progress phase number change (default implentation is empty).
    virtual void    notifyPhaseChanged() { }
    //! Called for user notification purposes when phase label change (default implentation is empty).
    virtual void    notifyPhaseLabelChanged() { }

    //! Called at the start of the monitored process, indicate to an eventually associed progress display that it must be shown (call reset() automatically).
    virtual void    beginProgress() { reset(); }
    //! Called at the end of the monitored process or if the process has been interrupted, indicate to an eventually associed progress display that it must be shut down.
    virtual void    endProgress() { }
    /*! \brief Set the current overall progress.
     *
     * \note progress value is bound to [0.;1.0].
     */
    virtual void    setProgress( double ) { }

public:
    //! Set the previsible number of phase in the monitored task and specify weight of the first current phase (default to 0, ie overall progress equal phase 0 progress).
    virtual void    setPhaseCount( int phaseCount, double phaseWeight, const std::string& phaseLabel = "" ) { (void)phaseCount; (void)phaseWeight; (void)phaseLabel; }
    /*! \brief Start a new phase weighting \c phaseWeight in the overall progress.
     *
     * \warning calling nextPhase() will automatically close the previous phase and update the overall progress with
     * last phase weight even if previous phase progress was not complete:
     *
     * \code
     *  // Setup a progress notifier with 2 phases, each weighting 50% (0.5) of the global progress
     *  gtpo::ProgressNotifier progress;
     *  progress.setPhaseCount( 2, 0.5 );
     *  progress.setPhaseProgress( 0.5 );
     *  // progress.getProgress() == 0.25   (because phase 1 progress is 0.5 and phase weight is 0.5, global progress is 0.5*0.5=0.25
     *  progress.nextPhase( 0.5 );
     *  // Warning: progress.getProgress() == 0.5, it now equal previous phase total weight even if the phase was not manually complete with setProgressPhase(1.0)
     *  progress.setPhaseProgress( 0.5 );
     *  // progress.getProgress() == 0.75 (one could expect 0.5*0.5 + 0.5*0.5=0.5, but using nextPhase has automatically complete phase 1, so
     *  // progress is (phase1.progress = 1. * 0.5) + (0.5 * 0.5) = 0.75
     * \endcode
     *
     * \note The total number of calls to nextPhase() must equal the value of \c phaseCount set when beginning startup monitoring with setPhaseCount().
     * \note The sum for all \c phaseWeight for the \c phaseCount calls to nextPhase() must equalt 1.0.
     */
    virtual void    nextPhase( double phaseWeight, const std::string& phaseLabel = "" ) { (void)phaseWeight; (void)phaseLabel; }
    //! Return the current phase number (int between 0 and phaseCount - 1).
    virtual int     getPhase() const { return 0; }
    //! Return current phase label.
    virtual const std::string&    getPhaseLabel() const { return _phaseLabel; }
    //! Return current phase progress into the (0.0, 1.0) range.
    virtual double  getPhaseProgress() const { return 0.; }
    //! Set the current phase progress (\c phaseProgress must be in the (0.0, 1.0) range.
    virtual void    setPhaseProgress( double phaseProgress ) { (void)phaseProgress; }
protected:
    //! Current phase label.
    std::string _phaseLabel         = "";
};

//! Multi levels, multi phases progress notifier (basic concrete implementation of IProgressNotifier).
class ProgressNotifier : public IProgressNotifier
{
public:
    ProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        IProgressNotifier( superProgress ) { }

public:
    //! \copydoc IProgressNotifier::update()
    virtual void    update( ) {
        double progress = _progress / getLevelCount();
        for ( auto& subProgress : _notifiers)
            progress += ( subProgress->getProgress() / getLevelCount() );
        _globalProgress = progress;
        if ( getSuperProgress() != nullptr )    // Update any eventual super progress
            getSuperProgress()->update();
    }

    //! \copydoc IProgressNotifier::reset()
    virtual void                reset() override {
        IProgressNotifier::reset( );

        _globalProgress = 0.;
        _progress       = 0.;
        _phaseProgress  = 0.;
        _phase          = 0;
        _phaseWeight    = 0.0;
        _phaseBeginProgress = 0.0;
        _phaseCount     = 1;
        notifyPhaseProgressChanged();
        notifyProgressChanged();
    }

    virtual IProgressNotifier*  getSubProgress() {
        ProgressNotifier* subProgress = new ProgressNotifier( this );
        IProgressNotifierPtr progressPtr( subProgress );
        _notifiers.emplace_back( std::move( progressPtr ) );
        return subProgress;
    }

public:
    /*! \copybrief IProgressNotifier::getProgress()
     *
     * When a sub progress notifier is attached to this progress notifier, getProgress()
     * return a global progress valid for all progress notifier, otherwise, it return the
     * notifier "local" progress.
     */
    virtual double  getProgress() const override {
        return ( getLevelCount() > 1. ? _globalProgress : _progress );
    }
    //! \copydoc IProgressNotifier::setProgress()
    virtual void    setProgress( double progress ) override {
        progress = std::min( progress, 1.0 );
        progress = std::max( progress, 0.0 );
        _progress = progress;
        _globalProgress = _progress / getLevelCount();
        if ( getSuperProgress() != nullptr )
            getSuperProgress()->update();
        notifyProgressChanged();
    }
private:
    //! Used when there is sub progress notifier attached to this progress notifier (otherwise it is the same than _progress).
    double          _globalProgress = 0.;
    double          _progress       = 0.;

public:
    //! \copydoc IProgressNotifier::setPhaseCount()
    virtual void    setPhaseCount( int phaseCount, double phaseWeight, const std::string& phaseLabel = ""  ) override {
        _phaseLabel = phaseLabel;
        _phaseCount = phaseCount;
        _phase = 0;

        _phaseBeginProgress = (double)_progress;
        _phaseWeight = phaseWeight;
        _phaseProgress = 0.;
        notifyPhaseLabelChanged();
        notifyPhaseChanged();
        notifyPhaseProgressChanged();
        notifyProgressChanged();
    }

    //! \copydoc IProgressNotifier::nextPhase()
    virtual void    nextPhase( double phaseWeight, const std::string& phaseLabel = "" ) override {
        setProgress( _phaseBeginProgress + _phaseWeight );  // Overall progress is progress at (last) phase startup plus the last phase weight.
        _phaseLabel = phaseLabel;
        _phase++;
        _phaseBeginProgress = (double)_progress;

        _phaseWeight = phaseWeight;
        _phaseProgress = 0.;
        notifyPhaseLabelChanged();
        notifyPhaseChanged();
        notifyPhaseProgressChanged();
    }

    //! \copydoc IProgressNotifier::getPhase()
    virtual int     getPhase( ) const override { return _phase; }

    //! \copydoc IProgressNotifier::getPhaseLabel()
    virtual const std::string&    getPhaseLabel() const override { return _phaseLabel; }

    //! \copydoc IProgressNotifier::getPhaseProgress()
    virtual double  getPhaseProgress() const override { return _phaseProgress; }

    //! \copydoc IProgressNotifier::setPhaseProgress()
    virtual void    setPhaseProgress( double phaseProgress ) override {
        _phaseProgress = phaseProgress;
        notifyPhaseProgressChanged();
        if ( phaseProgress >= 0. ) {   // phase progress could be -1 if progression can't be evaluated
            double phasePercentage = _phaseWeight;
            if ( phasePercentage <= 0 ) // If no phasePercentage has been specified in beginPhase(), consider
                phasePercentage = 1.0;  // that there is only one phase covering 100% of total progress
            if ( phaseProgress > 1.0 )
                phaseProgress = 1.0;
            setProgress( _phaseBeginProgress + ( ( phaseProgress * phasePercentage ) ) );
        }
    }

private:
    double      _phaseProgress      = 0.;
    //! Current phase index.
    int         _phase              = 0;
    //! Current phase weight.
    double      _phaseWeight        = 0.0;
    //! Overall progress at current phase start (when nextPhase() has been called).
    double      _phaseBeginProgress = 0.0;
    int         _phaseCount         = 1;
};


/*! \brief Report progress in an output stream (default to std::cerr).
 *
 * Example use for debugging purposes:
 * \code
 *    FIXME
 * \endcode
 */
class EchoProgressNotifier : public ProgressNotifier
{
public:
    EchoProgressNotifier( IProgressNotifier* superProgress = nullptr ) :
        ProgressNotifier( superProgress ) { }

public:
    virtual IProgressNotifier*  getSubProgress() {
        EchoProgressNotifier* subProgress = new EchoProgressNotifier( this );
        IProgressNotifierPtr progressPtr( subProgress );
        _notifiers.emplace_back( std::move( progressPtr ) );
        return subProgress;
    }

    virtual void    notifyProgressChanged() override {
        std::cout << "Progress=" << getProgress() << std::endl;
    }
    virtual void    notifyPhaseChanged() override {
        std::cout << "Phase=" << getPhase() << std::endl;
    }
    virtual void    notifyPhaseProgressChanged() override {
        std::cout << "Phase progress=" << getProgress() << std::endl;
    }
};

} // ::gtpo

#endif // gtpoProgressNotifier_h

