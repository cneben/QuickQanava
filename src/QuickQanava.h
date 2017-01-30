/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

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
// \file	QuickQanava.h
// \author	benoit@destrat.io
// \date	2016 02 04
//-----------------------------------------------------------------------------

#ifndef QuickQanava_h
#define QuickQanava_h

// GTpo headers
#include "GTpo.h"

// QuickGeoGL headers
#include "../QuickGeoGL/src/QuickGeoGL.h"

// QuickContainers headers
#include "../QuickContainers/src/QuickContainers.h"

// QuickQanava headers
#include "./qanConfig.h"
#include "./qanEdge.h"
#include "./qanNode.h"
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanPointGrid.h"
#include "./qanGraphView.h"
#include "./qanStyle.h"
#include "./qanStyleManager.h"
#include "./qanProgressNotifier.h"
#include "./fqlBottomRightResizer.h"

struct QuickQanava {
    static void initialize() {
        QuickGeoGL::initialize();
        QuickContainers::initialize();

        qmlRegisterType< qan::Node >( "QuickQanava", 2, 0, "AbstractNode");
        qmlRegisterType< qan::Edge >( "QuickQanava", 2, 0, "Edge");
        qmlRegisterType< qan::Group >( "QuickQanava", 2, 0, "AbstractGroup");
        qmlRegisterType< qan::Graph >( "QuickQanava", 2, 0, "AbstractGraph");
        qmlRegisterType< qan::GraphView >( "QuickQanava", 2, 0, "AbstractGraphView");
        qmlRegisterType< qan::Navigable >( "QuickQanava", 2, 0, "Navigable");
        qmlRegisterType< qan::Grid >( "QuickQanava", 2, 0, "Grid");
        qmlRegisterType< qan::PointGrid >( "QuickQanava", 2, 0, "PointGrid");
        qmlRegisterType< qan::Style >( "QuickQanava", 2, 0, "Style");
        qmlRegisterType< qan::NodeStyle >( "QuickQanava", 2, 0, "NodeStyle");
        qmlRegisterType< qan::EdgeStyle >( "QuickQanava", 2, 0, "EdgeStyle");
        qmlRegisterType< qan::StyleManager >( "QuickQanava", 2, 0, "StyleManager");
        qmlRegisterType< qan::ProgressNotifier >( "QuickQanava", 2, 0, "ProgressNotifier");
        qmlRegisterType< fql::BottomRightResizer >( "QuickQanava", 2, 0, "BottomRightResizer" );
    }
};

namespace qan { // ::qan

} // ::qan

#endif // QuickQanava_h

