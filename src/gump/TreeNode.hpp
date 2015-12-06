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
#include <array>
#include <memory>

#include <gump/exceptions.hpp>
#include <gump/io.hpp>
#include <gump/Coord.hpp>
#include <gump/CoordAABB.hpp>

namespace gump
{
template<size_t _DIM, typename _ValueType>
class TreeNode {
private:
    static const size_t NUM_CHILDREN = 1 << _DIM;
    using Self = TreeNode<_DIM, _ValueType>;
    using SelfPtr = std::shared_ptr<Self>;
    using ParentPtr = Self*;
    using ChildrenArray = std::array<SelfPtr, NUM_CHILDREN>;

public:
    static const size_t DIM = _DIM;
    using ValueType = _ValueType;

    TreeNode() = default;
    TreeNode(
            const TreeNode& other
            );
    TreeNode& operator=(
            const TreeNode& other
            );

    TreeNode(
            const ParentPtr& parent,
            const Coord<DIM>& coord,
            const size_t level,
            const _ValueType& value
            );

    ~TreeNode();

    // ---
    // node properties
    inline const Coord<DIM>& coord() const { return mCoord; }
    inline size_t level() const { return mLevel; }
    inline size_t width() const { return mWidth; }
    inline const CoordAABB<DIM>& bbox() const { return mBBox; }

    // ---
    // deal with values

    inline const ValueType& value() const {  ASSERT(!mHasChildren); return *mValue; }
    inline ValueType& value() {  ASSERT(!mHasChildren); return *mValue; }
    void setValue(
            const ValueType& value
            );

    // ---
    // deal with children
    inline bool hasChildren() const { return mHasChildren; }
    inline ChildrenArray children() const { return mHasChildren ? mChildren : ChildrenArray(); }

    // ---
    // refine and coarsen
    void coarsen();
    void refine();

    // ---
    // operators

    /**
     * Write the object to a stream
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const TreeNode& rhs
        )
    {
        return os << rhs.to_string();
    }

private:
    ParentPtr mParent;
    Coord<DIM> mCoord;
    size_t mLevel;
    size_t mWidth;
    CoordAABB<DIM> mBBox;

    ChildrenArray mChildren;
    // if this is true, the array must not have any nullptr's
    bool mHasChildren;
    std::shared_ptr<ValueType> mValue;

    SelfPtr getChild(
            size_t index
            ) const;
    void setChildren(
            const ChildrenArray& children
            );

    std::string to_string() const;
};

// **********************************************************************************

// ---
// ostream
template<size_t _DIM, typename _FT>
std::string
TreeNode<_DIM, _FT>::
to_string() const
{
    std::stringstream ss;
    ss << "TreeNode("
       << mLevel
       << ", "
       << mBBox
       << ")";
    return ss.str();
}

template<size_t _DIM, typename _ValueType>
TreeNode<_DIM, _ValueType>::
TreeNode(
        const TreeNode<_DIM, _ValueType>& other
        )
{
    // node properties
    mParent = other.mParent;
    mCoord = other.mCoord;
    mLevel = other.mLevel;
    mWidth = other.mWidth;
    mBBox = other.mBBox;

    // children and values
    if (other.mHasChildren) {
        setChildren(other.children());
    }
    else {
        setValue(other.value());
    }
}

template<size_t _DIM, typename _ValueType>
TreeNode<_DIM, _ValueType>&
TreeNode<_DIM, _ValueType>::
operator=(
        const TreeNode& other
        )
{
    // node properties
    mParent = other.mParent;
    mCoord = other.mCoord;
    mLevel = other.mLevel;
    mWidth = other.mWidth;
    mBBox = other.mBBox;

    // children and values
    if (other.mHasChildren) {
        setChildren(other.children());
    }
    else {
        setValue(other.value());
    }
    return *this;
}

template<size_t _DIM, typename _ValueType>
TreeNode<_DIM, _ValueType>::
TreeNode(
        const ParentPtr& parent,
        const Coord<DIM>& coord,
        const size_t level,
        const _ValueType& value
        ) :
    mParent(parent),
    mCoord(coord),
    mLevel(level),
    mWidth(1 << level),
    mBBox(coord, coord.offsetBy(mWidth - 1)),
    mChildren(),
    mHasChildren(false),
    mValue()
{
    setValue(value);
}

template<size_t _DIM, typename _ValueType>
TreeNode<_DIM, _ValueType>::
~TreeNode()
{
    setChildren(ChildrenArray());
}

template<size_t _DIM, typename _ValueType>
typename TreeNode<_DIM, _ValueType>::SelfPtr
TreeNode<_DIM, _ValueType>::
getChild(
        size_t index
        ) const
{
    return mHasChildren ? mChildren.at(index) : SelfPtr();
}

template<size_t _DIM, typename _ValueType>
void
TreeNode<_DIM, _ValueType>::
setChildren(
        const ChildrenArray& children
        )
{
    if (!mHasChildren) {
        mValue.reset();
    }
    mChildren = children;
    mHasChildren = true;
}

template<size_t _DIM, typename _ValueType>
void
TreeNode<_DIM, _ValueType>::
setValue(
        const ValueType& value
        )
{
    mValue = std::make_shared<ValueType>(value);
    mChildren = ChildrenArray();
    mHasChildren = false;
}

template<size_t _DIM, typename _ValueType>
void
TreeNode<_DIM, _ValueType>::
coarsen()
{
    if (!mHasChildren) {
        return;
    }

    ValueType value(0);
    static const double weight = 1.0 / static_cast<double>(NUM_CHILDREN);
    for (const auto& node : mChildren) {
        if (node->hasChildren()) {
            return;
        }
        value += weight * node->value();
    }
    setValue(value);
}

template<size_t _DIM, typename _ValueType>
void
TreeNode<_DIM, _ValueType>::
refine()
{
    if (mLevel == 0) {
        return;
    }
    ASSERT(!mHasChildren);

    ChildrenArray children;
    for (size_t i = 0; i < NUM_CHILDREN; ++i) {
        Coord<DIM> newCoord(mCoord);
        for (size_t j = 0; j < DIM; ++j) {
            // In 1D:    x
            //  - i = 0: 0
            //  - i = 1: 1
            //
            // In 2D:    x y
            //  - i = 0: 0 0
            //  - i = 1: 1 0
            //  - i = 2: 0 1
            //  - i = 3: 1 1
            //
            // In 3D:    x y z
            //  - i = 0: 0 0 0
            //  - i = 1: 1 0 0
            //  - i = 2: 0 1 0
            //  - i = 3: 1 1 0
            //  - i = 4: 0 0 1
            //  - i = 5: 1 0 1
            //  - i = 6: 0 1 1
            //  - i = 7: 1 1 1
            if ((i >> j) & 0x001) {
                newCoord[j] += mWidth / 2;
            }
        }
        children[i] = std::make_shared<Self>(this, newCoord, mLevel - 1, *mValue);
    }
    setChildren(children);
}


} // namespace gump
