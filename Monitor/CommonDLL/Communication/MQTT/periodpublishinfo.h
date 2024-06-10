#ifndef PERIODPUBLISHINFO_H
#define PERIODPUBLISHINFO_H

#include "CommonDLL_global.h"
#include "Task.h"

#include <functional>

struct COMMONDLL_EXPORT PeriodPublishInfo
{
    QString topic;
    TaskInfo taskInfo;
    std::function<QByteArray(bool &)> deleObj;
    int elapsed;
    int period;
};

#endif // PERIODPUBLISHINFO_H
