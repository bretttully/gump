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
#include <algorithm>
#include <memory>

#include <gump/exceptions.hpp>
#include <gump/Coord.hpp>
#include <gump/TreeNode.hpp>

namespace gump
{
enum class TraversalDirection {
    BOTTOM_UP,
    TOP_DOWN,
    MORTON
};

template<size_t _DIM, typename _ValueType>
class Forrest {
private:
    using Node = TreeNode<_DIM, _ValueType>;
    using NodePtr = std::shared_ptr<Node>;
    using RootContainer = std::map<Coord<_DIM>, NodePtr>;
    using FlatContainer = std::vector<Node>;
    using LinearContainer = std::map<size_t, std::vector<NodePtr> >;

public:
    static constexpr size_t DIM = _DIM;
    using ValueType = _ValueType;

    Forrest() :
        mNumberOfLevels(0) {}

    // ---
    // properties
    size_t numberOfLeafs() const { return mNumberOfLeafNodes; }

    // ---
    // initialisation

    /**
     * Clear the tree and insert a set of coarse-level octtrees based on the
     * resolution specified
     *
     * @param coarseResolution
     * @param numberOfLevels
     * @param background
     */
    void initialise(
            const Coord<_DIM>& coarseResolution,
            const size_t& numberOfLevels,
            const _ValueType& background
            );

    /**
     * Ensure that the branching factor is respected by all nodes
     * in the forrest
     */
    void balance();

    // ---
    // use the tree to accelerate point and box queries

    const NodePtr nodeAtCoord(
            const Coord<DIM>& coord
            ) const;

    // ---
    // visit the leafs and leaf-parents in a linearised fashion

    template<typename Op>
    void visitLeafNodes(
            const Op& op,
            const TraversalDirection& direction = TraversalDirection::MORTON
            );

    /**
     * Refine to the lowest level at the specified coordinate.
     *
     * NB: following this call, the tree will need to be balanced
     * before any calls to visit(), refine() or coarsen() can be called.
     */
    template<typename Op>
    void refineToLowestLevelAtCoord(
            const Coord<DIM>& coord,
            const Op& refineOp
            );

    template<typename Op>
    void refine(
            const Op& refineOp
            );

    /**
     * Any nodes that are marked for coarsening should coarsen here.
     * The new value that is assigned will be derived from a volume
     * average of its children.
     *
     * NB: this implications this has for non-floating point types
     *
     * This implies that @tparam _ValueType must provide
     *   - operator+=(const _ValueType& other)
     *   - operator/=(const _int& divisor)
     */
    void coarsen();

private:
    size_t mNumberOfLevels;
    RootContainer mChildren;

    size_t mNumberOfLeafNodes;
    LinearContainer mLinearisedLeafNodes;
    LinearContainer mLinearisedParentNodes;
    FlatContainer mMortonLeafNodes;

    /**
     * Convert the tree into a set of linear containers of values
     * and the parents of leaves so that the visitors can operate
     * in a more performant manner
     */
    void linearise();

    /**
     * This can be used to iterate bottom-up using .begin() and .end()
     * iterators -- and top-down using .rbegin() and .rend() iterators.
     */
    template<typename IterT, typename Op>
    void visit(
            IterT& iter,
            const IterT& end,
            const Op& op
            )
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
        const Coord<_DIM>& coarseResolution,
        const size_t& numberOfLevels,
        const _ValueType& background
        )
{
    mChildren.clear();

    mNumberOfLevels = numberOfLevels;
    size_t rootLevel = numberOfLevels - 1;
    size_t rootWidth = 1 << rootLevel;

    size_t loopI = (DIM > 0) ? coarseResolution[0] : 1;
    size_t loopJ = (DIM > 1) ? coarseResolution[1] : 1;
    size_t loopK = (DIM > 2) ? coarseResolution[2] : 1;

    Coord<DIM> coord(0);
    for (size_t k = 0; k < loopK; ++k) {
        if (DIM > 2) {
            coord[2] = k * rootWidth;
        }

        for (size_t j = 0; j < loopJ; ++j) {
            if (DIM > 1) {
                coord[1] = j * rootWidth;
            }

            for (size_t i = 0; i < loopI; ++i) {
                coord[0] = i * rootWidth;
                NodePtr root = std::make_shared<Node>(nullptr, coord, rootLevel, background);
                auto pair = std::make_pair(coord, root);
                auto success = mChildren.insert(pair).second;
                if (!success) {
                    std::stringstream ss;
                    ss << "Failed to insert root node: "
                       << coord
                       << ", "
                       << *root;
                    throw std::runtime_error(ss.str().c_str());
                }
            }
        }
    }
    linearise();
}

template<size_t _DIM, typename _ValueType>
void
Forrest<_DIM, _ValueType>::
balance()
{
    linearise();
}

template<size_t _DIM, typename _ValueType>
void
Forrest<_DIM, _ValueType>::
linearise()
{
    // first clear the maps
    if (!mLinearisedLeafNodes.empty()) {
        mLinearisedLeafNodes.clear();
    }
    if (!mLinearisedParentNodes.empty()) {
        mLinearisedParentNodes.clear();
    }
    if (!mMortonLeafNodes.empty()) {
        mMortonLeafNodes.clear();
    }
    mNumberOfLeafNodes = 0;

    // process the tree with a queue so that we aren't calling
    // recursively
    std::queue<NodePtr> toProcess;
    for (const auto& pair : mChildren) {
        toProcess.emplace(pair.second);
    }
    while(!toProcess.empty()) {
        NodePtr node = toProcess.front();
        toProcess.pop();

        // if there are children, descend and add them to the queue
        if (node->hasChildren()) {
            bool insertedIntoParentVector = false;
            for (const auto& child : node->children()) {
                toProcess.emplace(child);
                if (!insertedIntoParentVector && !child->hasChildren()) {
                    mLinearisedParentNodes[node->level()].emplace_back(node);
                    insertedIntoParentVector = true;
                }
            }
        }

        // if it is a leaf node, then add it to the container
        else {
            ++mNumberOfLeafNodes;
            mLinearisedLeafNodes[node->level()].emplace_back(node);
            mMortonLeafNodes.emplace_back(*node);
        }
    }

    auto cmp = [](const Node& a, const Node& b) {
        return a.id() < b.id();
    };
    std::sort(mMortonLeafNodes.begin(), mMortonLeafNodes.end(), cmp);
}

template<size_t _DIM, typename _ValueType>
const typename Forrest<_DIM, _ValueType>::NodePtr
Forrest<_DIM, _ValueType>::
nodeAtCoord(
        const Coord<DIM>& coord
        ) const
{
    NodePtr resultNode;

    // find the root node that contains this coord
    for (const auto& pair : mChildren) {
        if (pair.second->bbox().contains(coord)) {
            resultNode = pair.second;
            break;
        }
    }

    if (resultNode) {
        while(resultNode->hasChildren()) {
            for (const auto& child : resultNode->children()) {
                if (child->bbox().contains(coord)) {
                    resultNode = child;
                    break;
                }
            }
        }
    }

    return resultNode;
}

template<size_t _DIM, typename _ValueType>
template<typename Op>
void
Forrest<_DIM, _ValueType>::
visitLeafNodes(
        const Op& op,
        const TraversalDirection& direction
        )
{
    if (direction == TraversalDirection::BOTTOM_UP) {
        ASSERT(!mLinearisedLeafNodes.empty());
        auto iter = mLinearisedLeafNodes.begin();
        auto end = mLinearisedLeafNodes.end();
        visit(iter, end, op);
    }
    else if (direction == TraversalDirection::TOP_DOWN) {
        ASSERT(!mLinearisedLeafNodes.empty());
        auto iter = mLinearisedLeafNodes.rbegin();
        auto end = mLinearisedLeafNodes.rend();
        visit(iter, end, op);
    }
    else {
        ASSERT(!mMortonLeafNodes.empty());
        for (auto& node : mMortonLeafNodes) {
            op(node);
        }
    }
}

template<size_t _DIM, typename _ValueType>
template<typename Op>
void
Forrest<_DIM, _ValueType>::
refineToLowestLevelAtCoord(
        const Coord<DIM>& coord,
        const Op& refineOp
        )
{
    auto node = nodeAtCoord(coord);
    while (node->level() != 0) {
        refineOp(*node);
        for (const auto& child : node->children()) {
            if (child->bbox().contains(coord)) {
                node = child;
                break;
            }
        }
    }

    mLinearisedLeafNodes.clear();
    mLinearisedParentNodes.clear();
    mMortonLeafNodes.clear();
    mNumberOfLeafNodes = 0;
}

template<size_t _DIM, typename _ValueType>
template<typename Op>
void
Forrest<_DIM, _ValueType>::
refine(
        const Op& refineOp
        )
{
    visitLeafNodes(refineOp, TraversalDirection::BOTTOM_UP);
    balance();
}

namespace {
struct CoarsenOp {
    template<typename NodeT>
    void operator()(
        NodeT& node
        ) const
    {
        node.coarsen();
    }
};
}

template<size_t _DIM, typename _ValueType>
void
Forrest<_DIM, _ValueType>::
coarsen()
{
    if (mLinearisedParentNodes.empty()) {
        return;
    }
    auto iter = mLinearisedParentNodes.begin();
    auto end = mLinearisedParentNodes.end();
    visit(iter, end, CoarsenOp());
    balance();
}

} // namespace gump
