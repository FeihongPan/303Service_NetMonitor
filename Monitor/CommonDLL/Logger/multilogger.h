#ifndef MULTILOGGER_H
#define MULTILOGGER_H

#include "CommonDLL_global.h"
#include "logger.h"

namespace NS_Log
{
    class COMMONDLL_EXPORT MultiLogger : public Logger
    {
    public:
        explicit MultiLogger(const QString &folderPath, qint64 timeSinceEpoch);
    };
}

#endif // MULTILOGGER_H
