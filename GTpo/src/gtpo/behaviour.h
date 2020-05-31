/*
 Copyright (c) 2008-2018, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the GTpo software library.
//
// \file	behaviour.h
// \author	benoit@destrat.io
// \date	2016 02 08
//-----------------------------------------------------------------------------

#ifndef gtpo_behaviour_h
#define gtpo_behaviour_h

// STD headers
#include <cstddef>          // std::size_t
#include <vector>
#include <memory>

// GTpo headers
/* Nil */

namespace gtpo { // ::gtpo

/*! \brief Base class for concrete behaviours.
 *
 * \image html behaviour-class.png
 *
 */
template < class target_t >
class behaviour
{
public:
    behaviour() noexcept { }
    ~behaviour() noexcept { }

   behaviour( const behaviour& rhs ) = default;
   behaviour& operator=( const behaviour& rhs ) = default;
   behaviour( behaviour&& rhs ) noexcept = default;
   behaviour& operator=( behaviour&& rhs ) noexcept = default;

public:
    std::weak_ptr<target_t> get_target() noexcept { return _target; }
    void                    set_target(std::weak_ptr<target_t> target) { _target = target; }
protected:
    std::weak_ptr<target_t> _target;

public:
    inline auto getName() const noexcept -> const std::string& { return _name; }
protected:
    inline auto setName( const std::string& name )  noexcept -> void { _name = name; }
private:
    std::string _name = "";

public:
    /*! \brief Enable this behaviour until it is disabled again with a call to disable().
     *
     * \note enabling a disabled behaviour does not apply behaviour on changes made
     *       while it was disabled.
     * \sa isEnabled() for enabled state description.
     */
    inline auto     enable() noexcept -> void { _enabled = true; }
    /*! \brief Disable this behaviour until it is enabled back wit a call to enable().
     *
     * \sa isEnabled() for enabled state description.
     */
    inline auto     disable() noexcept -> void { _enabled = false; }
    /*! \brief Return the actual "enabled" state for this behaviour.
     *
     *  This method could be usefull in very specific use cases, such as serialization or insertion of a large number
     *  of nodes in a graph or group where this behaviour is applied.
     *  A behaviour is enabled by default after its creation.
     *
     *  \return true if enable() has been called, false if disable() has been called.
     */
    inline auto     isEnabled( ) const noexcept -> bool { return _enabled; }

protected:
    bool            _enabled = true;
};

} // ::gtpo

#endif // gtpo_behaviour_h
