/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitormanager.cpp
* Description:      监控软件主要功能实现包括读取配置文件、打开关闭进程、定期发送部署软件实时状态以及接受控制层传来的命令
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
*       V1.1.0     FeihongPan     2024-04-10     调整了接收指令的方式，改为直接从GUI层获取指令
*       V1.1.1     FeihongPan     2024-04-17     增加模板函数判断IP
*       V2.0.0     FeihongPan     2024-04-17     1921#2锁定版本
***********************************************************************/

#include "Converter/enumconverter.h"
#include "FileSystem/fileoperatormanager.h"
#include "Logger/loggermanager.h"
#include "netmonitormanager.h"
#include "NetworkEquipment/Command/cmd_netmonitor.h"

#include <QProcess>

using namespace NS_Log;

NetMonitorManager *NetMonitorManager::self = nullptr;
NetMonitorManager::NetMonitorManager(QString name, QObject *parent)
    : IMqttClient(name, parent)
    , IFileOperator(name)
{
    LOG()->Initialize_DebugLogger("D:/");
}

NetMonitorManager::~NetMonitorManager()
{

}

NetMonitorManager *NetMonitorManager::getInstance()
{
    if(nullptr == self)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(nullptr == self)
        {
            self = new NetMonitorManager("NetMonitorManager");
        }
    }
    return self;
}

QString NetMonitorManager::getAppProcessName(const QString &strAppName) const
{
    return m_dic_AppProcessNameInfo.values[strAppName];
}

bool NetMonitorManager::initializeConfig()
{
    bool bReadPathConfig = readPathConfigData(m_dic_AppPathInfo);
    readNameConfigData(m_dic_AppProcessNameInfo);
    m_lst_App = m_dic_AppPathInfo.values.keys();
    foreach (auto &strAppName, m_lst_App)
    {
        m_dic_Appstate.values.insert(strAppName, false);
        m_dic_FunctionCalled[strAppName] = false;
    }

    foreach (auto &strAppName, m_lst_App)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [ = ]
        {
            if (m_dic_FunctionCalled[strAppName])
            {
                m_dic_FunctionCalled[strAppName] = false;
                m_dic_Appstate.values[strAppName] = true;
                timer->stop();
                timer->start(m_nConnectTime);
            }
            else
            {
                m_dic_Appstate.values[strAppName] = false;
            }
        });
        timer->setInterval(m_nConnectTime);
        timer->start();
    }
    return bReadPathConfig;
}

QByteArray NetMonitorManager::getAppStateValue(bool &bGetAppState)
{
    if (!m_bInitialized)
    {
        m_bInitialized = initializeConfig();
    }
    bGetAppState = m_bInitialized;
    auto dic_AppState = m_dic_Appstate.toRawJson();
    return dic_AppState;
}

bool NetMonitorManager::readPathConfigData(S_Dic_StringString &dic_AppPathInfo)
{
    SetFileName(m_strAppPathFile);
    QByteArray configData;
    bool bReadPathConfig = ReadFromFile(configData);
    dic_AppPathInfo.fromJson(configData);
    if (!bReadPathConfig)
    {
        LOG()->Log_Debug("Failed to read apppath config file");
    }
    return bReadPathConfig;
}

bool NetMonitorManager::readNameConfigData(S_Dic_StringString &dic_AppProcessNameInfo)
{
    SetFileName(m_strAppProcessNameFile);
    QByteArray AppNameconfigData;
    bool bReadNameConfig = ReadFromFile(AppNameconfigData);
    dic_AppProcessNameInfo.fromJson(AppNameconfigData);
    if (!bReadNameConfig)
    {
        LOG()->Log_Debug("Failed to read appprocessname config file");
    }
    return bReadNameConfig;
}

bool NetMonitorManager::checkTask(const QString &strAppName)
{
    auto strAppProcessName = getAppProcessName(strAppName);
    QProcess process;
    QString strProgram = "tasklist";
    //    QStringList arguments = QStringList() << "/NH" << "/FO" << "CSV" << "/FI" << QString("IMAGENAME eq %1").arg(strAppProcessName);
    QStringList strArguments = QStringList() << "/NH" << "/FO" << "CSV" << "/FI" << QString("IMAGENAME eq " + strAppProcessName);
    process.start(strProgram, strArguments);
    process.waitForFinished();
    QString strProcessInfo = process.readAllStandardOutput();
    //    bool res = ProcessInfo.startsWith(QString("\"%1").arg(appProcessName));
    bool bProcessOpened = strProcessInfo.contains(strAppProcessName, Qt::CaseInsensitive); // true: process opened
    return bProcessOpened;
}

bool NetMonitorManager::killTask(const QString &strAppName)
{
    auto strAppProcessName = getAppProcessName(strAppName);
    QString strCmdCommand = "cmd /c taskkill /F /IM " + strAppProcessName;
    bool bTaskClosed = QProcess::execute(strCmdCommand); // true: task executed success
    if (!bTaskClosed)
    {
        LOG()->Log_Debug("Failed to kill the process");
    }
    return bTaskClosed;
}

bool NetMonitorManager::startTask(const QString &strAppName)
{
    auto strAppPath = m_dic_AppPathInfo.values[strAppName];
    QString strProgram = "cmd.exe";
    QStringList strArguments = QStringList() << "/c" << "start" << strAppPath;
    bool bTaskOpened = QProcess::startDetached(strProgram, strArguments); // true: task executed success
    if (!bTaskOpened)
    {
        LOG()->Log_Debug("Failed to start the process");
    }
    return bTaskOpened;
}

bool NetMonitorManager::appOpen(const QString &strAppName)
{
    auto strAppProcessName = getAppProcessName(strAppName);
    bool strAppState = checkTask(strAppName); // true: app is opened
    if(!strAppState)
    {
        startTask(strAppName);
        LOG()->Log_Debug("open " + strAppProcessName.toStdString());
    }
    else
    {
        LOG()->Log_Debug(strAppProcessName.toStdString() + " has been opened!");
    }
    return !strAppState;
}

bool NetMonitorManager::appClose(const QString &strAppName)
{
    auto strAppProcessName = getAppProcessName(strAppName);
    bool strAppState = checkTask(strAppName); // res true: app is opened
    if (strAppState)
    {
        killTask(strAppName);
        LOG()->Log_Debug("close " + strAppProcessName.toStdString());
    }
    else
    {
        strAppState = false;
        LOG()->Log_Debug(strAppProcessName.toStdString() + " has been closed!");
    }
    return strAppState;
}

bool NetMonitorManager::appRestart(const QString &strAppName)
{
    auto strAppProcessName = getAppProcessName(strAppName);
    bool strAppState = checkTask(strAppName); // true: app is opened
    if (strAppState)
    {
        killTask(strAppName);
        startTask(strAppName);
    }
    else
    {
        LOG()->Log_Debug(strAppProcessName.toStdString() + " has been closed!");
        startTask(strAppName);
    }
    LOG()->Log_Debug("restart" + strAppProcessName.toStdString());
    return strAppState;
}

void NetMonitorManager::setAppState(const QString &strAppName)
{
    if (m_dic_Appstate.values.contains(strAppName))
    {
        m_dic_FunctionCalled[strAppName] = true;
    }
}

bool NetMonitorManager::appOpenMC()
{
    auto strAppName = "MC";
    auto bHasMC = m_lst_App.contains(strAppName);

    if (bHasMC)
    {
        auto strAppProcessName = getAppProcessName(strAppName);
        bool strAppState = checkTask(strAppName); // true: app is opened
        if(!strAppState)
        {
            startTask(strAppName);
            LOG()->Log_Debug("open " + strAppProcessName.toStdString());
        }
        else
        {
            LOG()->Log_Debug(strAppProcessName.toStdString() + " has been opened!");
        }
        return !strAppState;
    }
    return !bHasMC;
}

void NetMonitorManager::decoding(const QString &task, bool result, const QByteArray &msg, const QByteArray &errMsg) // from NETC_IP / from APP_NetMonitor
{
    CMD_NetMonitor::Command cmd = EnumConverter::ConvertStringToEnum<CMD_NetMonitor::Command>(task);

    S_AppOperator dic_AppNameInfo;
    dic_AppNameInfo.fromJson(msg);

    switch (cmd)
    {
        case CMD_NetMonitor::Command::CMD_AppOpen:
        {
            if (checkIp(dic_AppNameInfo))
            {
                auto strAppName = dic_AppNameInfo.appName;
                appOpen(strAppName);
            }
        }
        break;

        case CMD_NetMonitor::Command::CMD_AppClose:
        {
            if (checkIp(dic_AppNameInfo))
            {
                auto strAppName = dic_AppNameInfo.appName;
                appClose(strAppName);
            }
        }
        break;

        case CMD_NetMonitor::Command::CMD_AppRestart:
        {
            if (checkIp(dic_AppNameInfo))
            {
                auto strAppName = dic_AppNameInfo.appName;
                appRestart(strAppName);
            }
        }
        break;

        case CMD_NetMonitor::Command::CMD_HeartBeat:
        {
            auto strAppName = QString(msg);
            setAppState(strAppName);
        }
        break;

        case CMD_NetMonitor::Command::CMD_AppState:
        {
        }
        break;

        case CMD_NetMonitor::Command::CMD_AppOpenMC:
        {
            appOpenMC();
        }
        break;

        default:
            break;
    }
}

void NetMonitorManager::SetPeriodPublish()
{
    bool r = false;
    m_Publisher.PublishPeriodically(this->m_Topic, EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppState), r, std::bind(&NetMonitorManager::getAppStateValue, this, std::placeholders::_1), 500);
}

void NetMonitorManager::StartPublish()
{
    bool r = false;
    QByteArray msg;
    msg = getAppStateValue(r);
    Publish(EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppState), r, msg);
}

void NetMonitorManager::SaveCommandInfo()
{
    bool bGetAppState;
    QByteArray msg = getAppStateValue(bGetAppState);
    QByteArray resultMsg = m_Publisher.GetPublishData(this->m_Topic, EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppState), true, msg);
    writeCmdSampleToFile(GetTopic(), resultMsg);
}
