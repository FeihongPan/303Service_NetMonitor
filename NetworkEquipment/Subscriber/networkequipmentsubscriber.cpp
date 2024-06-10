/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        networkequipmentsubscriber.cpp
* Description:      接受来自控制层的所有IP部署软件的实时状态和监控软件自身的心跳检测信号，并且传递给NetmonitorView
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
***********************************************************************/

#include <QMutex>
#include "Converter/enumconverter.h"
#include "FileSystem/filesystem.h"
#include "Logger/loggermanager.h"
#include "networkequipmentsubscriber.h"
#include "NetworkEquipment/Command/cmd_netmonitor.h"
#include "NetworkEquipment/Data/s_netmonitor.h"

using namespace NS_Log;

NetworkEquipmentSubscriber *NetworkEquipmentSubscriber::self = nullptr;

NetworkEquipmentSubscriber::NetworkEquipmentSubscriber(QString name, QObject *parent)
    : ISubscriber(name, parent)
{
    qRegisterMetaType<S_Dic_AppState>("S_Dic_AppState");
    qRegisterMetaType<S_Dic_AppState>("S_Dic_AppState&");
}

NetworkEquipmentSubscriber::~NetworkEquipmentSubscriber()
{

}

NetworkEquipmentSubscriber *NetworkEquipmentSubscriber::getInstance()
{
    if(nullptr == self)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(nullptr == self)
        {
            self = new NetworkEquipmentSubscriber("NetworkEquipmentSubscriber");
        }
    }
    return self;
}

bool NetworkEquipmentSubscriber::Initialize()
{
    auto bReadIPConfigData = ReadIPConfigData();

    if ((!m_bInitialized) && bReadIPConfigData)
    {
        foreach (auto &strIp, m_lst_App)
        {
            Subscribe(strIp + "_HeartBeat");
        }
        m_bInitialized = true;
    }
    return m_bInitialized;
}

bool NetworkEquipmentSubscriber::ReadIPConfigData()
{
    QByteArray ConfigData;
    S_List_String lst_IpInfo;
    auto bReadIPConfigData = FileSystem::Read(m_strFilePath, ConfigData);
    lst_IpInfo.fromJson(ConfigData);
    m_lst_App = lst_IpInfo.values;
    return bReadIPConfigData;
}

void NetworkEquipmentSubscriber::decoding(const QString &topic, const QByteArray &message) // from MC_NetworkEquipment
{
    TaskInfo task_Recived;
    task_Recived.fromJson(message);
    CMD_NetMonitor::Command cmd = EnumConverter::ConvertStringToEnum<CMD_NetMonitor::Command>(task_Recived.task);

    switch (cmd)
    {
        case CMD_NetMonitor::CMD_AppOpen:
            break;
        case CMD_NetMonitor::CMD_AppClose:
            break;
        case CMD_NetMonitor::CMD_AppRestart:
            break;
        case CMD_NetMonitor::CMD_HeartBeat:
            emit sig_SendNetMonitorState(QString(task_Recived.context)); // IP
            break;
        case CMD_NetMonitor::CMD_AppState:
            {
                S_Dic_AppState dic_AllAppState;
                dic_AllAppState.fromJson(task_Recived.context);
                emit sig_SendAppState(dic_AllAppState);
            }
        default:
            break;
    }
}
