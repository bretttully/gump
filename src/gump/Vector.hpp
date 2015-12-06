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
#include <string>
#include <boost/lexical_cast.hpp>

namespace gump
{

template <size_t _DIM, typename _FT>
class Vector {
public:
    static const size_t DIM = _DIM;
    using FT = _FT;

    using PtArray = std::array<FT, DIM>;

    // ---------------
    // constructors

    Vector() :
        Vector(FT(0)) {}
        
    explicit Vector(
        const FT& value
        ) :
        mPtArray() 
    {
        mPtArray.fill(value);
    }

    Vector(
        const PtArray& ptArray
        ) :
        mPtArray(ptArray) {}
        
    template <size_t D = DIM, typename = typename std::enable_if<(D == 2)>::type >
    explicit Vector(
        const FT& x,
        const FT& y
        ) :
        mPtArray({x, y}) {}
        
    template <size_t D = DIM, typename = typename std::enable_if<(D == 3)>::type >
    explicit Vector(
        const FT& x,
        const FT& y,
        const FT& z
        ) :
        mPtArray({x, y, z}) {}

    // copy & assignment
    Vector(
        const Vector& rhs
        ) :
        Vector(rhs.mPtArray) {}
    Vector& operator=(
        const Vector& rhs
        )
    {
        mPtArray = rhs.mPtArray;
        return *this;
    }

    // ---------------
    // element access

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 1)>::type >
    inline FT x() const {
        return mPtArray[0];
    }

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 2)>::type >
    inline FT y() const {
        return mPtArray[1];
    }

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 3)>::type >
    inline FT z() const {
        return mPtArray[2];
    }

    /**
     * returns the i'th Cartesian coordinate of the point, starting at 0.
     */
    inline FT operator[](
        size_t i
        ) const;
    inline FT& operator[](
        size_t i
        );

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 1)>::type >
    inline Vector offsetBy(
            const FT& offset
            ) const {
        Vector result(*this);
        for (size_t i = 0; i < DIM; ++i) {
            result[i] += offset;
        }
        return result;
    }

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 2)>::type >
    inline Vector offsetBy(
            const FT& offsetX,
            const FT& offsetY
            ) const {
        Vector result(*this);
        result[0] += offsetX;
        result[1] += offsetY;
        return result;
    }

    template <size_t D = DIM, typename = typename std::enable_if<(D >= 3)>::type >
    inline Vector offsetBy(
            const FT& offsetX,
            const FT& offsetY,
            const FT& offsetZ
            ) const {
        Vector result(*this);
        result[0] += offsetX;
        result[1] += offsetY;
        result[2] += offsetZ;
        return result;
    }

    // ---------------
    // comparators

    inline bool operator==(
        const Vector& w
        ) const;

    inline bool operator!=(
        const Vector& w
        ) const;

    /**
     * returns true iff p is lexicographically smaller than q,
     * i.e. either if p.x() < q.x() or if p.x() == q.x() and p.y() < q.y().
     */
    bool operator<(
        const Vector& w
        ) const;

    /**
     * returns true iff p is lexicographically greater than q
     */
    inline bool operator>(
        const Vector& w
        ) const;

    /**
     * returns true iff p is lexicographically smaller or equal to q
     */
    inline bool operator<=(
        const Vector& w
        ) const;

    /**
     * returns true iff p is lexicographically greater or equal to q
     */
    inline bool operator>=(
        const Vector& w
        ) const;

    /**
     * Write the object to a stream
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const Vector& rhs
        )
    {
        return os << rhs.to_string();
    }
    
private:
    PtArray mPtArray;

    std::string to_string() const;
};

// **********************************************************************************

// ---
// operator[]
template<size_t _DIM, typename _FT>
_FT
Vector<_DIM, _FT>::
operator[](
    size_t i
    ) const
{
    if (i < 0 || i >= DIM) {
        throw std::runtime_error("Incorrect dimension");
    }
    return mPtArray[i];
}
template<size_t _DIM, typename _FT>
_FT&
Vector<_DIM, _FT>::
operator[](
    size_t i
    )
{
    if (i < 0 || i >= DIM) {
        throw std::runtime_error("Incorrect dimension");
    }
    return mPtArray[i];
}

// ---
// operator==
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator==(
    const Vector<_DIM, _FT>& w
    ) const
{
    for (size_t i = 0; i < DIM; ++i) {
        if (mPtArray[i] != w.mPtArray[i]) {
            return false;
        }
    }
    return true;
}

// ---
// operator!=
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator!=(
    const Vector<_DIM, _FT>& w
    ) const
{
    return !(*this == w);
}

// ---
// operator<
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator<(
    const Vector<_DIM, _FT>& w
    ) const
{
    for (size_t i = 0; i < DIM; ++i) {
        bool lessThan = true;
        for (size_t j = 0; lessThan && j < i; ++j) {
            lessThan = lessThan && mPtArray[j] == w.mPtArray[j];
        }
        lessThan = lessThan && mPtArray[i] < w.mPtArray[i];
        if (!lessThan) {
            return false;
        }
    }
    return true;
}

// ---
// operator>
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator>(
    const Vector<_DIM, _FT>& w
    ) const
{
    for (size_t i = 0; i < DIM; ++i) {
        bool greaterThan = true;
        for (size_t j = 0; greaterThan && j < i; ++j) {
            greaterThan = greaterThan && mPtArray[j] == w.mPtArray[j];
        }
        greaterThan = greaterThan && mPtArray[i] > w.mPtArray[i];
        if (!greaterThan) {
            return false;
        }
    }
    return true;
}

// ---
// operator<=
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator<=(
    const Vector<_DIM, _FT>& w
    ) const
{
    return (*this == w) || (*this < w);
}

// ---
// operator>=
template<size_t _DIM, typename _FT>
bool
Vector<_DIM, _FT>::
operator>=(
    const Vector<_DIM, _FT>& w
    ) const
{
    return (*this == w) || (*this > w);
}

// ---
// ostream
template<size_t _DIM, typename _FT>
std::string
Vector<_DIM, _FT>::
to_string() const
{
    std::stringstream ss;
    ss << "Vector("
       << boost::lexical_cast<std::string>(this->operator[](0));
    for (size_t i = 1; i < DIM; ++i) {
        ss << ", "
           << boost::lexical_cast<std::string>(this->operator[](i));
    }
    ss << ")";
    return ss.str();
}

} // namespace gump
