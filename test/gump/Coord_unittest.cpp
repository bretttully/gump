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
 
#include <random>
#include <gtest/gtest.h>
#include <gump/Coord.hpp>

namespace gump
{

TEST(Coord1DTest, constructorAndAccessor) {
    using Pt = Coord<1>;

    ASSERT_NO_THROW(Pt());
    ASSERT_NO_THROW(Pt(0));
    EXPECT_EQ(Pt(), Pt(0));
    
    std::mt19937_64 rng(0);
    std::uniform_int_distribution<int> randValue(-100, 100);
    for (int i = 0; i < 1e4; ++i) {
        int x = randValue(rng);
        Pt v(x);
        EXPECT_EQ(x, v.x());
        EXPECT_EQ(v.x(), v[0]);
        EXPECT_ANY_THROW(v[1]);
        EXPECT_ANY_THROW(v[2]);

        int xOffset = randValue(rng);
        Pt v2 = v.offsetBy(xOffset);
        EXPECT_EQ(v.x() + xOffset, v2.x());
    }
}

TEST(Coord2DTest, constructorAndAccessor) {
    using Pt = Coord<2>;

    ASSERT_NO_THROW(Pt());
    ASSERT_NO_THROW(Pt(0));
    EXPECT_EQ(Pt(), Pt(0));

    ASSERT_NO_THROW(Pt(1, 1));
    
    std::mt19937_64 rng(0);
    std::uniform_int_distribution<int> randValue(-100, 100);
    for (int i = 0; i < 1e4; ++i) {
        int x = randValue(rng);
        Pt v1(x);
        EXPECT_EQ(x, v1.x());
        EXPECT_EQ(x, v1.y());
        
        int y = randValue(rng);
        Pt v2(x, y);
        EXPECT_EQ(x, v2.x());
        EXPECT_EQ(y, v2.y());

        EXPECT_EQ(v2.x(), v2[0]);
        EXPECT_EQ(v2.y(), v2[1]);

        EXPECT_ANY_THROW(v2[2]);

        int xOffset = randValue(rng);
        Pt v3 = v2.offsetBy(xOffset);
        EXPECT_EQ(v2.x() + xOffset, v3.x());
        EXPECT_EQ(v2.y() + xOffset, v3.y());

        int yOffset = randValue(rng);
        Pt v4 = v2.offsetBy(xOffset, yOffset);
        EXPECT_EQ(v2.x() + xOffset, v4.x());
        EXPECT_EQ(v2.y() + yOffset, v4.y());
    }
}

TEST(Coord3DTest, constructorAndAccessor) {
    using Pt = Coord<3>;

    ASSERT_NO_THROW(Pt());
    ASSERT_NO_THROW(Pt(0));
    EXPECT_EQ(Pt(), Pt(0));

    ASSERT_NO_THROW(Pt(1, 1, 1));
    
    std::mt19937_64 rng(0);
    std::uniform_int_distribution<int> randValue(-100, 100);
    for (int i = 0; i < 1e4; ++i) {
        int x = randValue(rng);
        Pt v1(x);
        EXPECT_EQ(x, v1.x());
        EXPECT_EQ(x, v1.y());
        EXPECT_EQ(x, v1.z());
        
        int y = randValue(rng);
        int z = randValue(rng);
        Pt v2(x, y, z);
        EXPECT_EQ(x, v2.x());
        EXPECT_EQ(y, v2.y());
        EXPECT_EQ(z, v2.z());

        EXPECT_EQ(v2.x(), v2[0]);
        EXPECT_EQ(v2.y(), v2[1]);
        EXPECT_EQ(v2.z(), v2[2]);

        int xOffset = randValue(rng);
        Pt v3 = v2.offsetBy(xOffset);
        EXPECT_EQ(v2.x() + xOffset, v3.x());
        EXPECT_EQ(v2.y() + xOffset, v3.y());
        EXPECT_EQ(v2.z() + xOffset, v3.z());

        int yOffset = randValue(rng);
        int zOffset = randValue(rng);
        Pt v4 = v2.offsetBy(xOffset, yOffset, zOffset);
        EXPECT_EQ(v2.x() + xOffset, v4.x());
        EXPECT_EQ(v2.y() + yOffset, v4.y());
        EXPECT_EQ(v2.z() + zOffset, v4.z());
    }
}

} // namespace gump
