//-----------------------------------------------------------------------------
// This file is a part of the GTpo software.
//
// \file	gtpo_tests.cpp
// \author	benoit@qanava.org
// \date	2016 01 26
//-----------------------------------------------------------------------------

// Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Qt headers
#include <QGuiApplication>
#include <QtQml>
#include <QQuickStyle>

#include <QuickQanava>

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    QuickQanava::initialize(engine);

    return RUN_ALL_TESTS();
}



