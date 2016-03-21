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
// \file	gtpoSerializer.h
// \author	benoit@qanava.org
// \date	2016 01 31
//-----------------------------------------------------------------------------

#ifndef gtpoSerializer_h
#define gtpoSerializer_h

// GTpo headers
#include "./gtpoUtils.h"
#include "./gtpoGraph.h"

// STD headers
#include <fstream>
#include <atomic>

namespace gtpo { // ::gtpo

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
 * \li When the \c task change phase, the setNextPhase() method is called with the estimated weight of the
 * phase in \c task completion. The sum of all weight arguments to setNextPhase() must be 1.0.
 * \li The \c task then set the current phase progress with calls to setPhaseProgress(), the overall progress
 * is automatically updated by this notifyer and could be accessed with getProgress().
 * \li When the task ends, it call endProgress().
 *
 * Update of overall or phase progress is protected against values outside of the [0.;1.0] range.
 *
 * EmptyProgressNotifier is the default progress notifier, and it don't has state nor notify anything, any decent compiler
 * should just generate no overhead at runtime when parametrized with such class.
 *
 * \sa ProgressNotifier for a default static implementation based on std::atomic.
 * \sa VirtualProgressNotifier for a ProgressNotifier extended with virtual notification calls.
 */
class ProgressNotifier
{
public:
    //! Construct a default notifiyer with 0.0 progress and 0 \c phaseCount.
    EmptyProgressNotifier() = default;

public:
    //! Return the current overall progress, taking into account configured \c phaseCount and current phase progress.
    virtual double  getProgress() const { return 0.; }

    //! Called at the start of the monitored process, indicate to an eventually associed progress display that it must be shown.
    virtual void    beginProgress() { }

    //! Called at the end of the monitored process or if the process has been interrupted, indicate to an eventually associed progress display that it must be shut down.
    virtual void    endProgress() { }

protected:
    //! Set the current overall progress.
    virtual void    setProgress( double ) { }

public:
    //! Set the previsible number of phase in the monitored task (default to 0, ie overall progress equal phase 0 progress).
    void    setPhaseCount( int phaseCount ) { (void)phaseCount; }
    /*! \brief Start a new phase weighting \c phaseWeight in the overall progress.
     *
     * \note The total number of calls to nextPhase() must equal the value of \c phaseCount set when beginning startup monitoring with setPhaseCount().
     * \note The sum for all \c phaseWeight for the \c phaseCount calls to nextPhase() must equalt 1.0.
     */
    virtual void    nextPhase( double phaseWeight ) { (void)phaseWeight; }

    //! Return the current phase number (int between 0 and phaseCount - 1).
    virtual int     getPhase() const { return 0; }

    //! Return current phase progress into the (0.0, 1.0) range.
    virtual double  getPhaseProgress() const { return 0.; }

    //! Set the current phase progress (\c phaseProgress must be in the (0.0, 1.0) range.
    virtual void    setPhaseProgress( double phaseProgress ) { (void)phaseProgress; }
};

//! Simple empty implementation of progress notifier.
class ProgressNotifier : public EmptyProgressNotifier
{
public:
    ProgressNotifier() = default;

public:
    //! \copydoc EmptyProgressNotifier::getProgress()
    double  getProgress() const { return _progress; }
protected:
    //! \copydoc EmptyProgressNotifier::setProgress()
    void    setProgress( double progress ) { _progress = progress; }
private:
    // FIXME: std::atomic<double> invalid on g++ (normal...)
    double  _progress       = 0.;

public:
    //! \copydoc EmptyProgressNotifier::setPhaseCount()
    void    setPhaseCount( int phaseCount ) { _phaseCount = phaseCount; }
    //! \copydoc EmptyProgressNotifier::setNextPhase()
    void    nextPhase( double phaseWeight ) {
        setProgress( _phaseBeginProgress + _phaseWeight );  // Overall progress is progress at (last) phase startup plus the last phase weight.
        _phaseBeginProgress = (double)_progress;

        _phaseWeight = phaseWeight;
        _phaseProgress = 0.;
        _phaseBeginProgress = (double)_progress;
    }

    //! \copydoc EmptyProgressNotifier::getPhase()
    int     getPhase( ) const { return _phase; }
    //! \copydoc EmptyProgressNotifier::getPhaseProgress()
    double  getPhaseProgress() const { return _phaseProgress; }
    //! \copydoc EmptyProgressNotifier::setPhaseProgress()
    void    setPhaseProgress( double phaseProgress ) {
        _phaseProgress = phaseProgress;
        if ( phaseProgress >= 0. ) {   // phase progress could be -1 if progression can't be evaluated
            double phasePercentage = _phaseWeight;
            if ( phasePercentage <= 0 ) // If no phasePercentage has been specified in beginPhase(), consider
                phasePercentage = 1.0;  // that there is only one phase covering 100% of total progress
            if ( phaseProgress > 1.0 )
                phaseProgress = 1.0;
            _progress = _phaseBeginProgress + ( phaseProgress * _phaseWeight );
        }
    }
private:
    double  _phaseProgress          = 0.;
    //! Current phase index.
    int     _phase                  = 0;
    //! Current phase weight.
    double  _phaseWeight            = 0.0;
    //! Overall progress at current phase start (when nextPhase() has been called).
    double  _phaseBeginProgress     = 0.0;
    int     _phaseCount             = 1;
};

/*! \brief Progress monitor with virtual methods to observe progress change.
 *
 * Add "observable" pure virtual methods to ProgressNotifier:
 * \li progressChanged(): \copybrief progressChanged()
 * \li phaseChanged(): \copybrief phaseChanged()
 * \li phaseProgressChanged(): \copybrief phaseProgressChanged()
 */
class VirtualProgressNotifier : public ProgressNotifier
{
public:
    VirtualProgressNotifier() = default;
    virtual ~VirtualProgressNotifier() { }

protected:
    //! \copydoc EmptyProgressNotifier::setProgress()
    void    setProgress( double progress ) {
        std::cout << "gtpo:VirtualProgressNotifier::setProgress(): progress=" << progress << std::endl;
        ProgressNotifier::setProgress( progress );
        progressModified();
    }

    //! Called whenever the overall progress change.
    virtual void    progressModified() = 0;

public:
    //! \copydoc EmptyProgressNotifier::setNextPhase()
    void    nextPhase( double phaseWeight ) { ProgressNotifier::nextPhase( phaseWeight ); phaseModified(); }
    //! \copydoc EmptyProgressNotifier::setPhaseProgress()
    void    setPhaseProgress( double phaseProgress ) { ProgressNotifier::setPhaseProgress( phaseProgress ); phaseProgressModified(); }
protected:
    //! Called whenever the current progress phase is modified.
    virtual void    phaseModified() = 0;
    //! Called whenever the current phase progress is modified.
    virtual void    phaseProgressModified() = 0;
};

// FIXME 20160214: Polish OutSerializer and InSerializer, their serialize methods should be pure virtual and
//  does not match Serializer ones.

/*! Default interface for a GTpo output serializer.
 *
 */
template < class GraphConfig = gtpo::DefaultConfig,
           class Notifier = EmptyProgressNotifier >
class OutSerializer {
public:
    using Graph         = const gtpo::GenGraph< GraphConfig >;
    using SharedNode    = std::shared_ptr< typename GraphConfig::Node >;
    using Node          = typename GraphConfig::Node;
    using WeakNode      = std::weak_ptr< typename GraphConfig::Node >;
    using WeakEdge      = std::weak_ptr< typename GraphConfig::Edge >;
    using SharedEdge    = std::shared_ptr< typename GraphConfig::Edge >;

    OutSerializer() = default;
    virtual ~OutSerializer() = default;
    OutSerializer( const OutSerializer< GraphConfig >& ) = delete;

    /*! Serialize \c graph out.
     *
     * \throw std::exception if an error occurs.
     */
    virtual void    serializeOut( Graph& graph, Notifier* progress = nullptr ) noexcept( false ) { (void)graph; (void)progress; }
};

/*! Default interface for a GTpo output serializer.
 *
 */
template < class GraphConfig = DefaultConfig,
           class Notifier = EmptyProgressNotifier >
class InSerializer {
public:
    using Graph = GenGraph< GraphConfig >;
    using Node  = typename GraphConfig::Node;
    using Edge  = typename GraphConfig::Edge;

    InSerializer() = default;
    virtual ~InSerializer() = default;
    InSerializer( const InSerializer< GraphConfig >& ) = delete;

    /*! Serialize \c graph in.
     *
     * \throw std::exception if an error occurs, graph might be let in an "invalid state" and should no longer be used.
     */
    virtual void    serializeIn( Graph& graph, Notifier* progress = nullptr ) noexcept( false ) { ( void )graph; (void)progress; }
};

/*! Default interface for a GTpo input/output serializer.
 *
 */
template < class GraphConfig = DefaultConfig,
           class Notifier = EmptyProgressNotifier  >
class Serializer : public InSerializer< GraphConfig >,
                     public OutSerializer< GraphConfig >
{
public:
    using Graph         = GenGraph< GraphConfig >;
    using SharedNode    = std::shared_ptr< typename GraphConfig::Node >;
    using Node          = typename GraphConfig::Node;
    using WeakNode      = std::weak_ptr< typename GraphConfig::Node >;
    using WeakEdge      = std::weak_ptr< typename GraphConfig::Edge >;
    using SharedEdge    = std::shared_ptr< typename GraphConfig::Edge >;

    Serializer() = default;
    virtual ~Serializer() = default;
    Serializer( const Serializer< GraphConfig >& ) = delete;

    /*! Serialize \c graph out.
     *
     * \throw std::exception if an error occurs.
     */
    virtual auto    serializeOut( const Graph& graph, std::ostream& os, Notifier* progress = nullptr ) -> void { (void)graph; (void)os; (void)progress; }
    //! Shortcut to serializeOut( const Graph&, std::ostream& ) with a file name instead of an output stream.
    virtual auto    serializeOutTo( const Graph& graph, std::string fileName, Notifier* progress = nullptr ) -> void {
        std::ofstream os( fileName, std::ios::out | std::ios::trunc | std::ios::binary );
        if ( !os ) {
            std::cerr << "gtpo::ProtoSerializer::serializeOut(): Error: Can't open output stream " << fileName << std::endl;
            return;
        }
        serializeOut( graph, os, progress );
        if ( os.is_open() )
            os.close();
    }

    /*! Serialize \c graph in from stream \c is.
     *
     * \throw std::exception if an error occurs, graph might be let in an "invalid state" and should no longer be used.
     */
    virtual auto    serializeIn( std::istream& is, Graph& graph, Notifier* progress = nullptr   ) noexcept( false )  -> void { (void)is; (void)graph; (void)progress; }
    //! Shortcut to serializeIn( std::istream&, Graph& ) with a file name instead of an input stream.
    virtual auto    serializeInFrom( std::string fileName, Graph& graph, Notifier* progress = nullptr   ) noexcept( false ) -> void {
        std::ifstream is( fileName, std::ios::in | std::ios::binary );
        if ( !is ) {
            std::cerr << "gtpo::ProtoSerializer::serializeIn(): Error: Can't open input stream " << fileName << std::endl;
            return;
        }
        serializeIn( is, graph, progress );
        if ( is.is_open() )
            is.close();
    }
};

} // ::gtpo

#endif // gtpoSerializer_h

