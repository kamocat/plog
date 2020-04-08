#pragma once
#include "Logger.h"
#include "Formatters/CsvFormatter.h"
#include "Formatters/TxtFormatter.h"
#include "Appenders/RollingFileAppender.h"
#include <cstring>

namespace plog
{
    namespace
    {
        bool isCsv(const util::nchar* fileName)
        {
            const util::nchar* dot = util::findExtensionDot(fileName);
            return dot && 0 == std::strcmp(dot, ".csv");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Empty initializer / one appender

    template<int instanceId>
    inline Logger<instanceId>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        static Logger<instanceId> logger(maxSeverity);
        return appender ? logger.addAppender(appender) : logger;
    }

    inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
    }

    //////////////////////////////////////////////////////////////////////////
    // RollingFileAppender with any Formatter

    template<class Formatter, int instanceId>
    inline Logger<instanceId>& init(Severity maxSeverity, const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxFileSize, maxFiles);
        return init<instanceId>(maxSeverity, &rollingFileAppender);
    }

    template<class Formatter>
    inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<Formatter, PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // RollingFileAppender with TXT/CSV chosen by file extension

    template<int instanceId>
    inline Logger<instanceId>& init(Severity maxSeverity, const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return isCsv(fileName) ? init<CsvFormatter, instanceId>(maxSeverity, fileName, maxFileSize, maxFiles) : init<TxtFormatter, instanceId>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // CHAR variants for Windows

}
