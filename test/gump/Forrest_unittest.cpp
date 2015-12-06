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

#include <iostream>
#include <random>
#include <gtest/gtest.h>
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
        WARN(node.id() << ", " << node.level());
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

TEST(Forrest1DTest, constructorAndAccessor) {
    using ValueType = int;
    static const int DIM = 1;
    using ForrestT = Forrest<DIM, ValueType>;

    ValueType background(-1);
    size_t numberOfLevels = 6;

    ForrestT forrest;
    forrest.initialise(numberOfLevels, background);

    PrintOp printOp;
    RefineOp refineOp;
    CoarsenOp coarsenOp;

    forrest.linearise();
    WARN("");
    forrest.visitLeafs(/*bottomUp=*/true, printOp);

    int numLoops = numberOfLevels - 1;
    for (int i = 0; i < numLoops; ++i) {
        WARN("");
        forrest.visitLeafs(/*bottomUp=*/true, refineOp);
        forrest.linearise();
        forrest.visitLeafs(/*bottomUp=*/true, printOp);
    }
    for (int i = 0; i < numLoops; ++i) {
        WARN("");
        forrest.visitParents(/*bottomUp=*/true, coarsenOp);
        forrest.linearise();
        forrest.visitLeafs(/*bottomUp=*/true, printOp);
    }

}
} // namespace gump
