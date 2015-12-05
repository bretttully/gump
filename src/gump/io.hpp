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

// include log4cxx header files.
#include <log4cxx/logger.h>

namespace gump {
/**
 * A class to simplify the setup of logging
 */
class IOControl
{
public:
    /**
     * Create a root logger and set it up to log to file with the
     * appropriate level.
     *
     * @param fileName
     * @param level
     */
    static void intialiseFileLogging(
            std::string fileName,
            log4cxx::LevelPtr level
            );

    /**
     * Create a very simple logging system configured based on printing
     * to the console.
     *
     * @param level
     */
    static void intialiseBasicLogging(
            log4cxx::LevelPtr level
            );

    /**
     * Return the root logger
     * @return
     */
    static log4cxx::LoggerPtr logger();

    /**
     * At the end of the application, we can close down the logging and remove
     * any appenders that have been added.
     */
    static void shutdownLogging();

    /**
     * Control the level of the logger
     * @param logger
     * @param level
     */
    static void setLoggerLevel(
        log4cxx::LevelPtr level
        );

    /**
     * Get the logging level that we associate with DEBUG messages
     * @return a level relevant to the \c Logging class
     */
    static log4cxx::LevelPtr getDebugLevel();

    /**
     * Get the logging level that we associate with INFO messages
     * @return a level relevant to the \c Logging class
     */
    static log4cxx::LevelPtr getInfoLevel();

    /**
     * Get the logging level that we associate with WARN messages
     * @return a level relevant to the \c Logging class
     */
    static log4cxx::LevelPtr getWarnLevel();
};

} // namespace gump

#define DEBUG(msg)                                         \
    {                                                      \
        if (IOControl::logger()->isDebugEnabled()) {       \
            std::stringstream ss__;                        \
            ss__ << msg;                                   \
            LOG4CXX_DEBUG(IOControl::logger(), ss__.str()) \
        }                                                  \
    }
#define INFO(msg)                                          \
    {                                                      \
        if (IOControl::logger()->isInfoEnabled()) {        \
            std::stringstream ss__;                        \
            ss__ << msg;                                   \
            LOG4CXX_INFO(IOControl::logger(), ss__.str())  \
        }                                                  \
    }
#define WARN(msg)                                          \
    {                                                      \
        if (IOControl::logger()->isWarnEnabled()) {        \
            std::stringstream ss__;                        \
            ss__ << msg;                                   \
            LOG4CXX_WARN(IOControl::logger(), ss__.str())  \
        }                                                  \
    }
