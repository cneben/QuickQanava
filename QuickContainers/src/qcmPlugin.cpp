#include "./qcmPlugin.h"
#include "qcmAbstractContainer.h"
#include "qcmContainer.h"

void QuickContainersPlugin::registerTypes(const char *uri)
{
    // @uri QuickContainers
    qmlRegisterType< qcm::AbstractContainer >(uri, 1, 0, "AbstractContainer" );
}
