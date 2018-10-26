import qbs

Project {
    name: "GTpo";

    Product {
        name: "gtpo";
        targetName: "GTpo";

        readonly property stringList qtModules : ["core"];

        Export {
            cpp.includePaths: "src/";

            Depends { name: "cpp"; }
            Depends {
                name: "Qt";
                submodules: product.qtModules;
            }
        }
        Depends { name: "cpp"; }
        cpp.cxxLanguageVersion: "c++14"
        cpp.warningLevel: "all"
        Depends {
            name: "Qt";
            submodules: product.qtModules;
        }
        Group {
            name: "C++ headers";
            prefix: "src/gtpo/"
            files: [
                "utils.h",
                "config.h",
                "edge.h",
                "edge.hpp",
                "node.h",
                "node.hpp",
                "group.h",
                "group.hpp",
                "graph.h",
                "graph.hpp",
                "graph_property.h",
                "algorithm.h",
                "algorithm.hpp",
                "functional.h",
                "generator.h",
                "generator.hpp",
                "behaviour.h",
                "behaviourable.h",
                "behaviourable.hpp",
                "graph_behaviour.h",
                "graph_behaviour.hpp",
                "node_behaviour.h",
                "node_behaviour.hpp",
                "group_behaviour.h",
                "group_behaviour.hpp",
                "adjacent_behaviour.h",
                "adjacent_behaviour.hpp",
                "container_adapter.h",
                "GTpo.h"
            ]
        }
        Group {
            name: "Include library";
            prefix: "src/gtpo/"
            files: [
                "GTpo"
            ]
        }
        Group {
            qbs.install: (product.type === "dynamiclibrary");
            fileTagsFilter: product.type;
        }
    }
}
