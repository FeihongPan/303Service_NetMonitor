#ifndef DEVICERECOGNIZE_H
#define DEVICERECOGNIZE_H

#include "CommonDLL_global.h"

#include <QString>

class COMMONDLL_EXPORT DeviceRecognize
{
public:
    static QString GetUUID();
    static QString GetCpuId();
    static QString GetBaseBoardNum();
    static QString GetBiosNum();
    static QString GetDiskNum();

private:
    static QString getInfo(const QString &cmd);
};

#endif // DEVICERECOGNIZE_H
