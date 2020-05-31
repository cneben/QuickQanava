//
// Created by alexander on 19/08/18.
//
#pragma once

#include <QQmlExtensionPlugin>

class QuickContainersPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);
};
