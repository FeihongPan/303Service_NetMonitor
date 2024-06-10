#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "CommonDLL_global.h"

#include <QString>

class COMMONDLL_EXPORT Encryption
{
public:
    static QString DecodedText(const QString &data, const QString &key);
};

#endif // ENCRYPTION_H
