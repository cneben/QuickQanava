import qbs

Project {
    name: "QuickQanava";
    references: [
        "QuickContainers/quickcontainers.qbs",
        "GTpo/gtpo.qbs"
    ];

    StaticLibrary {
        name: "quickqanava";
        cpp.cxxLanguageVersion: "c++14"
        cpp.defines: ["QUICKQANAVA_STATIC"]

        readonly property stringList qtModules : ["core", "widgets", "gui", "qml", "quick"];

        Export {
            cpp.includePaths: [product.sourceDirectory];

            Depends { name: "cpp"; }
            Depends { name: "gtpo" }
            Depends { name: "quickcontainers" }
            Depends {
                name: "Qt";
                submodules: product.qtModules;
            }
        }
        Depends { name: "cpp"; }
        cpp.warningLevel: "all"
        Depends {
            name: "Qt";
            submodules: product.qtModules;
        }
        Depends { name: "gtpo" }
        Depends { name: "quickcontainers" }

        Group {
            name: "C++ sources";
            prefix: "src/"
            files: [
                "qanGraphView.cpp",
                "qanUtils.cpp",
                "qanEdge.cpp",
                "qanEdgeItem.cpp",
                "qanNode.cpp",
                "qanNodeItem.cpp",
                "qanPortItem.cpp",
                "qanSelectable.cpp",
                "qanDraggable.cpp",
                "qanConnector.cpp",
                "qanBehaviour.cpp",
                "qanGraph.cpp",
                "qanGroup.cpp",
                "qanGroupItem.cpp",
                "qanStyle.cpp",
                "qanStyleManager.cpp",
                "qanNavigable.cpp",
                "qanNavigablePreview.cpp",
                "qanGrid.cpp",
                "qanBottomRightResizer.cpp"
            ]
        }
        Group {
            name: "C++ headers";
            prefix: "src/"
            files: [
                "QuickQanava.h",
                "qanUtils.h",
                "qanGraphConfig.h",
                "qanGraphView.h",
                "qanEdge.h",
                "qanEdgeItem.h",
                "qanNode.h",
                "qanNodeItem.h",
                "qanPortItem.h",
                "qanSelectable.h",
                "qanDraggable.h",
                "qanAbstractDraggableCtrl.h",
                "qanDraggableCtrl.h",
                "qanDraggableCtrl.hpp",
                "qanConnector.h",
                "qanBehaviour.h",
                "qanGroup.h",
                "qanGroupItem.h",
                "qanGraph.h",
                "qanGraph.hpp",
                "qanStyle.h",
                "qanStyleManager.h",
                "qanNavigable.h",
                "qanNavigablePreview.h",
                "qanGrid.h",
                "qanContainerAdapter.h",
                "qanBottomRightResizer.h"
            ]
        }
        Group {
            name: "Qt resources bundle";
            prefix: "src/"
            files: [
                "QuickQanava_static.qrc"
            ]
        }
        Group {
            qbs.install: (product.type === "dynamiclibrary");
            fileTagsFilter: product.type;
        }
    }
}
