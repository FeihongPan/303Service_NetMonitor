/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        cmd_netmonitor.h
* Description:      NET模块指令
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
*        V1.0.1     2024-3-14     Feihong     临时指令
*        V2.0.0     FeihongPan     2024-04-17     1921#2锁定版本
***********************************************************************/

#ifndef CMD_NETMONITOR_H
#define CMD_NETMONITOR_H

#include <QObject>

class CMD_NetMonitor : public QObject
{
    Q_OBJECT

public:
    enum Command
    {
        CMD_HeartBeat, // 心跳检测的指令
        CMD_AppState, // 接收App状态的指令
        CMD_AppOpen, // 打开App状态的指令
        CMD_AppClose, // 关闭App状态的指令
        CMD_AppRestart, // 重启App状态的指令
        CMD_AppOpenMC, // 打开MC的指令
    };
    Q_ENUM(Command)
};

#endif // CMD_NETMONITOR_H
