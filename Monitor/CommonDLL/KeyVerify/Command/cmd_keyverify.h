﻿#ifndef CMD_KEYVERIFY_H
#define CMD_KEYVERIFY_H

#include "CommonDLL_global.h"

#include <QObject>

class COMMONDLL_EXPORT CMD_KeyVerify : public QObject
{
    Q_OBJECT
public:
    enum Command
    {
        CMD_LicenseExpirationDate,
        CMD_LicenseState,
    };
    Q_ENUM(Command)

    explicit CMD_KeyVerify(QObject *parent = nullptr);

signals:

};

#endif // CMD_KEYVERIFY_H
