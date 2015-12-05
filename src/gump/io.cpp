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

#include "io.hpp"

#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/layout.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>

namespace gump {

void
IOControl::
intialiseFileLogging(
    std::string fileName,
    log4cxx::LevelPtr level
    )
{
    log4cxx::LayoutPtr layout(new log4cxx::PatternLayout("\%-5p [\%l]: \%m\%n"));
    log4cxx::AppenderPtr appender(new log4cxx::FileAppender(layout, fileName, false));
    log4cxx::BasicConfigurator::configure(appender);

    IOControl::logger()->setLevel(level);
}

void
IOControl::
intialiseBasicLogging(
    log4cxx::LevelPtr level
    )
{
    log4cxx::LayoutPtr layout(new log4cxx::PatternLayout("\%-5p [\%l]: \%m\%n"));
    log4cxx::AppenderPtr appender(new log4cxx::ConsoleAppender(layout));
    log4cxx::BasicConfigurator::configure(appender);

    IOControl::logger()->setLevel(level);
}

log4cxx::LoggerPtr
IOControl::
logger()
{
    return log4cxx::Logger::getRootLogger();
}

void
IOControl::
shutdownLogging()
{
    log4cxx::LogManager::shutdown();
}

log4cxx::LevelPtr
IOControl::
getDebugLevel()
{
    return log4cxx::Level::getDebug();
}

log4cxx::LevelPtr
IOControl::
getInfoLevel()
{
    return log4cxx::Level::getInfo();
}

log4cxx::LevelPtr
IOControl::
getWarnLevel()
{
    return log4cxx::Level::getWarn();
}

void
IOControl::
setLoggerLevel(
    log4cxx::LevelPtr level
    )
{
    IOControl::logger()->setLevel(level);
}

} // namespace gump
