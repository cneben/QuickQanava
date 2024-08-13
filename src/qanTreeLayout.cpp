/*
 Copyright (c) 2008-2024, Benoit AUTHEMAN All rights reserved.

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
// \file	qanOrgTreeLayout.h
// \author	benoit@destrat.io
// \date	2024 08 13
//-----------------------------------------------------------------------------

// Std headers
#include <queue>
#include <unordered_set>

// Qt headers
#include <QQmlProperty>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlComponent>

// QuickQanava headers
#include "./qanTreeLayout.h"


namespace qan { // ::qan

/* NaiveTreeLayout Object Management *///--------------------------------------
NaiveTreeLayout::NaiveTreeLayout(QObject* parent) noexcept :
    QObject{parent}
{
}
NaiveTreeLayout::~NaiveTreeLayout() { }

void    NaiveTreeLayout::layout(qan::Node& root) noexcept
{
    // Pre-condition: root must be a tree subgraph, this is not enforced in this methodod.

    // Algorithm:
    // 1. Use BFS to generate an array of nodes "level by level".
    // 2. Layout the tree bottom up; for every level, bottom up:
    // 2.1 Layout the node at y(level) position.
    // 2.2 For mid level, shift nodes x position to align right subgraph on it's previous
    //     node subgraph (magic happen here: use a shifting heuristic !)
    // 2.3 Align node on it's sub graph (according to input configuration align left or center)
    // 3. Shift the tree to align root to it's original position.

    const auto collectBFS = [](qan::Node* root) -> std::vector<std::vector<qan::Node*>> {
        std::vector<std::vector<qan::Node*>> r;
        if (root == nullptr)
            return r;

        // <-- hand tuned ChatGPT code
        std::queue<std::pair<Node*, int>> nodeLevelqueue;
        std::unordered_set<Node*> visited;

        nodeLevelqueue.push({root, 0});
        visited.insert(root);

        while (!nodeLevelqueue.empty()) {
            auto [current, level] = nodeLevelqueue.front();
            nodeLevelqueue.pop();

            if (r.size() <= level)
                r.resize(level + 1);    // Resize() initialize new items
            r[level].push_back(current);
            for (auto child : current->get_out_nodes()) {             // Enqueue unvisited children with their level
                if (visited.find(child) == visited.end()) {
                    nodeLevelqueue.push({child, level + 1});
                    visited.insert(child);
                }
            }
        }
        // <-- ChatGPT code
        return r;
    };

    // 1. BFS
    const auto levels = collectBFS(&root);

    // Debug
    int l = 0;
    for (const auto& level: levels) {
        std::cerr << l++ << ": ";
        for (const auto node: level)
            std::cerr << node->getLabel().toStdString() << "\t";
        std::cerr << std::endl;
    }

    // 2.
    if (levels.size() <= 1)   // Can't layout a tree with less than 2 levels
        return;
    const double xSpacing = 25.;
    const double ySpacing = 125.;
    for (int level = levels.size() - 1; level >= 0; level--) {
        auto nodes = levels[level];

        // 2.1
        const double y = level * ySpacing; // FIXME, be smarter on shift here...

        // 2.2
        double x = 0.;
        for (const auto node: nodes) {
            node->getItem()->setX(x);
            node->getItem()->setY(y);
            x += node->getItem()->getBoundingShape().boundingRect().width() + xSpacing;
        }
    }

    // FIXME centering in another pass...
}

void    NaiveTreeLayout::layout(qan::Node* root) noexcept
{
    qWarning() << "qan::NaiveTreeLayout::layout(): root=" << root;
    if (root != nullptr)
        layout(*root);
}
//-----------------------------------------------------------------------------


/* OrgTreeLayout Object Management *///----------------------------------------
OrgTreeLayout::OrgTreeLayout(QObject* parent) noexcept :
    QObject{parent}
{
}
OrgTreeLayout::~OrgTreeLayout() { }

void    OrgTreeLayout::layout(qan::Node& root, qreal xSpacing, qreal ySpacing) noexcept
{
    // FIXME #228: Variant / naive Reingold-Tilford algorithm

    // Pre-condition: root must be a tree subgraph, this is not enforced in this algorithm,
    // any circuit will lead to intinite recursion...

    // Algorithm:
        // Traverse graph DFS aligning child nodes vertically
        // At a given level: `shift` next node according to previous node sub-tree BR
    auto layout_rec = [xSpacing, ySpacing](auto&& self, auto& childNodes, QRectF br) -> QRectF {
        //qWarning() << "layout_rec(): br=" << br;
        const auto x = br.right() + xSpacing;
        for (auto child: childNodes) {
            //qWarning() << "layout_rec(): child.label=" << child->getLabel() << "  br=" << br;
            child->getItem()->setX(x);
            child->getItem()->setY(br.bottom() + ySpacing);
            br = br.united(child->getItem()->boundingRect().translated(child->getItem()->position()));
            const auto prevBr = self(self, child->get_out_nodes(), br);
            br = br.united(prevBr);
        }
        return br;
    };
    //qWarning() << "root.bottomRight=" << root.getItem()->boundingRect().bottomRight();
    // Note: QQuickItem boundingRect is in item local CS, translate to scene CS.
    layout_rec(layout_rec, root.get_out_nodes(),
               root.getItem()->boundingRect().translated(root.getItem()->position()));
}

void    OrgTreeLayout::layout(qan::Node* root, qreal xSpacing, qreal ySpacing) noexcept
{
    //qWarning() << "qan::OrgTreeLayout::layout(): root=" << root;
    if (root != nullptr)
        layout(*root, xSpacing, ySpacing);
}
//-----------------------------------------------------------------------------

} // ::qan
