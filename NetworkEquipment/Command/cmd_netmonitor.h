/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        cmd_netmonitor.h
* Description:      NETģ��ָ��
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record��
*        V1.0.1     2024-3-14     Feihong     ��ʱָ��
***********************************************************************/

#ifndef CMD_NETMONITOR_H
#define CMD_NETMONITOR_H

#define USEFORDEBUG

#include <QObject>

class CMD_NetMonitor : public QObject
{
    Q_OBJECT

public:
    enum Command
    {
        CMD_HeartBeat, // ��������ָ��
        CMD_AppState, // ����App״̬��ָ��
        CMD_AppOpen, // ��App״̬��ָ��
        CMD_AppClose, // �ر�App״̬��ָ��
        CMD_AppRestart, // ����App״̬��ָ��
        CMD_AppOpenMC, // ��MC��ָ��
    };
    Q_ENUM(Command)
};

#endif // CMD_NETMONITOR_H
