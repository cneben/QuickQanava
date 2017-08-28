/*
 Copyright (c) 2008-2017, Benoit AUTHEMAN All rights reserved.

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
// This file is a part of the QuickQanava software library.
//
// \file	qanUtils.h
// \author	benoit@destrat.io
// \date	2017 03 17
//-----------------------------------------------------------------------------

#ifndef qanUtils_h
#define qanUtils_h

// Std headers
#include <sstream>
#include <random>
#include <random>
#include <exception>

// GTpo headers
#include <GTpo>

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanGraph.h"

//! Main QuickQanava namespace
namespace qan { // ::qan

/*! \brief Default QuickQanava exception, use what() and msg().
 *
 */
class Error : public std::runtime_error {
public:
    explicit Error( const std::string& what_arg ) :
        std::runtime_error{what_arg}, _msg{ QString::fromStdString(what_arg) } { }
    explicit Error( const char* what_arg ) :
        std::runtime_error{what_arg}, _msg{ QString(what_arg) } { }
    explicit Error( const QString& msg_arg ) : std::runtime_error{ "" }, _msg{msg_arg} { }

    const QString&  getMsg() const noexcept { return _msg; }
private:
    QString _msg{""};
};

} // ::qan

#endif // qanUtils_h

