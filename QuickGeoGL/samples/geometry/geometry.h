//-----------------------------------------------------------------------------
// This file is a part of the QuickGeoGL software. Copyright (C) 2016 Benoit Autheman.
//
// \file	geometry.h
// \author	benoit@destrat.io
// \date	2016 09 26
//-----------------------------------------------------------------------------

#ifndef geometry_h
#define geometry_h

// Qt headers
#include <QQuickView>

class MainView : public QQuickView
{
    Q_OBJECT
public:
    MainView( );
    virtual ~MainView( ) { }
    MainView( const MainView& ) = delete;
};

#endif // geometry_h

