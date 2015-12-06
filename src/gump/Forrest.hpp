/**
 * Copyright (c) 2015-2016 Brett Tully
 *
 * All rights reserved. This software is distributed under the
 * Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
 *
 * Redistributions of source code must retain the above copyright
 * and license notice and the following restrictions and disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS' AND CONTRIBUTORS' AGGREGATE
 * LIABILITY FOR ALL CLAIMS REGARDLESS OF THEIR BASIS EXCEED US$250.00.
 */

#pragma once
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <memory>

#include <gump/exceptions.hpp>
#include <gump/TreeNode.hpp>

namespace gump
{
template<size_t _DIM, typename _ValueType>
class Forrest {
private:
    using Node = TreeNode<_DIM, _ValueType>;
    using NodePtr = std::shared_ptr<Node>;
    using Container = std::vector<NodePtr>;
    using LinearContainer = std::map<size_t, Container>;

public:
    static const size_t DIM = _DIM;
    using ValueType = _ValueType;

    Forrest() :
        mNumberOfLevels(0) {}

    void initialise(
            const size_t& numberOfLevels,
            const _ValueType& background
            );

    void linearise();

    template<typename Op>
    void visitLeafs(
            const bool bottomUp,
            const Op& op
            ) const;

    template<typename Op>
    void visitParents(
            const bool bottomUp,
            const Op& op
            ) const;

private:
    size_t mNumberOfLevels;
    Container mChildren;

    LinearContainer mLinearisedLeafNodes;
    LinearContainer mLinearisedParentNodes;

    template<typename IterT, typename Op>
    void visit(
            IterT& iter,
            const IterT& end,
            const Op& op
            ) const
    {
        for (; iter != end; ++iter) {
            for (const auto& node : iter->second) {
                op(*node);
            }
        }
    }
};

// *****************************************************************

template<size_t _DIM, typename _ValueType>
void
Forrest<_DIM, _ValueType>::
initialise(
        const size_t& numberOfLevels,
        const _ValueType& background
        )
{
    mNumberOfLevels = numberOfLevels;
    NodePtr root = std::make_shared<Node>(nullptr, IndexPoint<DIM>(0), numberOfLevels - 1, background);
    mChildren.emplace_back(root);
}

template<size_t _DIM, typename _ValueType>
void
Forrest<_DIM, _ValueType>::
linearise()
{
    // first clear the maps
    mLinearisedLeafNodes.clear();
    mLinearisedParentNodes.clear();

    // process the tree with a queue so that we aren't calling
    // recursively
    std::queue<NodePtr> toProcess;
    for (const auto& node : mChildren) {
        toProcess.emplace(node);
    }
    while(!toProcess.empty()) {
        NodePtr node = toProcess.front();
        toProcess.pop();

        // if there are children, descend and add them to the queue
        if (node->hasChildren()) {
            bool insertedIntoParent = false;
            for (const auto& child : node->getChildren()) {
                toProcess.emplace(child);
                if (!insertedIntoParent && !child->hasChildren()) {
                    mLinearisedParentNodes[node->level()].emplace_back(node);
                    insertedIntoParent = true;
                }
            }
        }

        // if it is a leaf node, then add it to the container
        else {
            mLinearisedLeafNodes[node->level()].emplace_back(node);
        }
    }
}

template<size_t _DIM, typename _ValueType>
template<typename Op>
void
Forrest<_DIM, _ValueType>::
visitLeafs(
        const bool bottomUp,
        const Op& op
        ) const
{
    if (bottomUp) {
        auto iter = mLinearisedLeafNodes.begin();
        auto end = mLinearisedLeafNodes.end();
        visit(iter, end, op);
    }
    else {
        auto iter = mLinearisedLeafNodes.rbegin();
        auto end = mLinearisedLeafNodes.rend();
        visit(iter, end, op);
    }
}

template<size_t _DIM, typename _ValueType>
template<typename Op>
void
Forrest<_DIM, _ValueType>::
visitParents(
        const bool bottomUp,
        const Op& op
        ) const
{
    if (bottomUp) {
        auto iter = mLinearisedParentNodes.begin();
        auto end = mLinearisedParentNodes.end();
        visit(iter, end, op);
    }
    else {
        auto iter = mLinearisedParentNodes.rbegin();
        auto end = mLinearisedParentNodes.rend();
        visit(iter, end, op);
    }
}

} // namespace gump
