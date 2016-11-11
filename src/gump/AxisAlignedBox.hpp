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
#include <ostream>
#include <array>
#include <stdexcept>
#include <type_traits>
#include <gump/Vector.hpp>

namespace gump {

template<size_t _DIM, typename _FT>
class AxisAlignedBox {
private:
    using VectorT = Vector<_DIM, _FT>;

public:
    static constexpr size_t DIM = _DIM;
    using FT = _FT;

    AxisAlignedBox() {}
    AxisAlignedBox(
            const VectorT& lowLeft,
            const VectorT& upRight
            )
        : mLowLeft(lowLeft)
        , mUpRight(upRight) {}

    inline bool contains(
            const VectorT& pt
            ) const
    {
        return !(pt < mLowLeft || pt > mUpRight);
    }

    /**
     * Write the object to a stream
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const AxisAlignedBox& rhs
        )
    {
        return os << rhs.to_string();
    }

private:
    VectorT mLowLeft;
    VectorT mUpRight;

    std::string to_string() const;
};

// **********************************************************************************

// ---
// ostream
template<size_t _DIM, typename _FT>
std::string
AxisAlignedBox<_DIM, _FT>::
to_string() const
{
    std::stringstream ss;
    ss << "AxisAlignedBox("
       << mLowLeft
       << " -> "
       << mUpRight
       << ")";
    return ss.str();
}

} // namespace gump
