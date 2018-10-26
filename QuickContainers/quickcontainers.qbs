import qbs

StaticLibrary {
    name: "quickcontainers"
    readonly property stringList qtModules : ["core", "widgets", "gui", "qml"]

    Depends { name: "cpp"; }
    cpp.cxxLanguageVersion: "c++14"
    cpp.warningLevel: "all"
    Depends {
        name: "Qt";
        submodules: product.qtModules
    }
    Export {
        cpp.includePaths: ["include/"]

        Depends { name: "cpp"; }
        Depends {
            name: "Qt";
            submodules: product.qtModules;
        }
    }
    Group {
        name: "C++ headers";
        prefix: "include/"
        files: [
            "qcmContainerModel.h",
            "qcmAbstractContainer.h",
            "qcmAdapter.h",
            "qcmContainer.h",
            "QuickContainers.h"
        ]
    }
    Group {
        name: "Include library";
        prefix: "include/"
        files: [
            "QuickContainers"
        ]
    }
}
