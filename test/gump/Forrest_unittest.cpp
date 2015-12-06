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

#include <test/gump/BaseTest.h>

#include <iostream>
#include <random>

#include <gump/Forrest.hpp>
#include <gump/io.hpp>

namespace gump
{
namespace {
struct PrintOp {
    template<typename NodeT>
    void operator()(
        const NodeT& node
        ) const
    {
        WARN(node);
    }
};

struct RefineOp {
    template<typename NodeT>
    void operator()(
        NodeT& node
        ) const
    {
        node.refine();
    }
};
}

template<int _DIM>
class ForrestTest_N :
        public BaseTest {
protected:
    static const size_t DIM = _DIM;
    using ValueType = double;
    using ForrestT = Forrest<DIM, ValueType>;

    void SetUp_Protected() override {}

    void simpleTest(
            int res,
            size_t numberOfLevels
            )
    {
        PrintOp printOp;
        RefineOp refineOp;

        ValueType background(-1);
        size_t rootLevel = numberOfLevels - 1;
        Coord<DIM> coarseRes(res);

        ForrestT forrest;
        forrest.initialise(coarseRes, numberOfLevels, background);
        EXPECT_EQ(std::pow(res, DIM), forrest.numberOfLeafs());

        WARN("");
        forrest.visitLeafs(printOp);

        Coord<DIM> refineCoord(0);
        auto node = forrest.nodeAtCoord(refineCoord);
        EXPECT_EQ(refineCoord, node->coord());
        EXPECT_EQ(rootLevel, node->level());

        forrest.refineToLowestLevelAtCoord(refineCoord, refineOp);
        node = forrest.nodeAtCoord(refineCoord);
        EXPECT_EQ(refineCoord, node->coord());
        EXPECT_EQ(0u, node->level());

        // finalise all of the refinements
        forrest.balance();
        WARN("");
        forrest.visitLeafs(printOp);

//        for (size_t i = 0; i < rootLevel; ++i) {
//            WARN("");
//            forrest.refine(refineOp);
//            forrest.visitLeafs(printOp);
//        }
        for (size_t i = 0; i < rootLevel; ++i) {
            WARN("");
            forrest.coarsen();
            forrest.visitLeafs(printOp);
        }

        // check everything is back to the way it started
        EXPECT_EQ(std::pow(res, DIM), forrest.numberOfLeafs());


    }
};

using ForrestTest1D = ForrestTest_N<1>;
using ForrestTest2D = ForrestTest_N<2>;
using ForrestTest3D = ForrestTest_N<3>;

TEST_F(ForrestTest1D, simple) {
    simpleTest(3, 6);
}
TEST_F(ForrestTest2D, simple) {
    simpleTest(3, 6);
}
TEST_F(ForrestTest3D, simple) {
    simpleTest(3, 6);
}
} // namespace gump
