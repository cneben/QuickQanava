//
// Created by alexander on 19/08/18.
//

#include "./qanPlugin.h"

// QuickQanava headers
#include "./qanGraphConfig.h"
#include "./qanEdge.h"
#include "./qanEdgeItem.h"
#include "./qanNode.h"
#include "./qanNodeItem.h"
#include "./qanPortItem.h"
#include "./qanConnector.h"
#include "./qanGroup.h"
#include "./qanGroupItem.h"
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanGrid.h"
#include "./qanGraphView.h"
#include "./qanStyle.h"
#include "./qanStyleManager.h"
#include "./qanBottomRightResizer.h"
#include "./qanNavigablePreview.h"

static void initResources()
{
    Q_INIT_RESOURCE(QuickQanava);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "LineGrid", 2, 0 },
    { "PointGrid", 2, 0 },
    { "Edge", 2, 0 },
    { "EdgeTemplate", 2, 0 },
    { "Node", 2, 0 },
    { "GraphView", 2, 0 },
    { "Group", 2, 0 },
    { "RectNodeTemplate", 2, 0 },
    { "RectSolidBackground",  2, 0 },
    { "RectGroupTemplate",  2, 0 },
    { "CanvasNodeTemplate",  2, 0 },
    { "VisualConnector",  2, 0 },
    { "LabelEditor",  2, 0 },
    { "SelectionItem",  2, 0 },
    { "StyleListView",  2, 0 },
    { "StyleEditor",  2, 0 },
    { "HorizontalDock",  2, 0 },
    { "VerticalDock",  2, 0 },
    { "Port",  2, 0 },
};

void QuickQanavaPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri QuickQanava
    qmlRegisterType< qan::Node >( uri, 2, 0, "AbstractNode");
    qmlRegisterType< qan::NodeItem >( uri, 2, 0, "NodeItem");
    qmlRegisterType< qan::PortItem >( uri, 2, 0, "PortItem");
    qmlRegisterType< qan::Edge >( uri, 2, 0, "AbstractEdge");
    qmlRegisterType< qan::EdgeItem >( uri, 2, 0, "EdgeItem");
    qmlRegisterType< qan::Group >( uri, 2, 0, "AbstractGroup");
    qmlRegisterType< qan::GroupItem >( uri, 2, 0, "GroupItem");
    qmlRegisterType< qan::Connector >( uri, 2, 0, "Connector");

    qmlRegisterType< qan::Graph >( uri, 2, 0, "Graph");
    qmlRegisterType< qan::GraphView >( uri, 2, 0, "AbstractGraphView");
    qmlRegisterType< qan::Navigable >( uri, 2, 0, "Navigable");
    qmlRegisterType< qan::NavigablePreview >( uri, 2, 0, "AbstractNavigablePreview");
    qmlRegisterType< qan::OrthoGrid >( uri, 2, 0, "OrthoGrid");
    qmlRegisterType< qan::PointGrid >( uri, 2, 0, "AbstractPointGrid");
    qmlRegisterType< qan::LineGrid >( uri, 2, 0, "AbstractLineGrid");
    qmlRegisterType< qan::Style >( uri, 2, 0, "Style");
    qmlRegisterType< qan::NodeStyle >( uri, 2, 0, "NodeStyle");
    qmlRegisterType< qan::EdgeStyle >( uri, 2, 0, "EdgeStyle");
    qmlRegisterType< qan::StyleManager >( uri, 2, 0, "StyleManager");
    qmlRegisterType< qan::BottomRightResizer >( uri, 2, 0, "BottomRightResizer" );

    const QString filesLocation = fileLocation();
    for (auto i : qmldir) {
        qmlRegisterType(QUrl(filesLocation + "/" + i.type + ".qml"), uri, i.major, i.minor, i.type);
    }
}

void QuickQanavaPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));

    engine->rootContext()->setContextProperty( "defaultNodeStyle", QVariant::fromValue(qan::Node::style()) );
    engine->rootContext()->setContextProperty( "defaultEdgeStyle", QVariant::fromValue(qan::Edge::style()) );
    engine->rootContext()->setContextProperty( "defaultGroupStyle", QVariant::fromValue(qan::Group::style()) );
}

QString QuickQanavaPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return QStringLiteral("qrc:/QuickQanava");
    return baseUrl().toString();
}

bool QuickQanavaPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + qmldir[0].type + ".qml");
    return !file.exists();
}