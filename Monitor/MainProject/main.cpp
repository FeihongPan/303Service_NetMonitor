/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        main.cpp
* Description:      监控软件主函数，主要包括文件读写初始化、Mqtt通讯初始化、以及发布和订阅的主题
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
*       V2.0.0     FeihongPan     2024-04-17     1921#2锁定版本
***********************************************************************/

#include "Logger/loggermanager.h"
#include "FileSystem/fileoperatormanager.h"
#include "AppConfig/Manager/appconfigmanager.h"
#include "Communication/MQTT/mqttclientmanager.h"
#include "NetworkEquipmentMonitor/Manage/heartbeat.h"
#include "NetworkEquipment/Manage/netmonitormanager.h"

#include <iostream>
#include <QSharedMemory>
#include <QCoreApplication>

#ifdef Q_OS_WIN
    #include <WinUser.h>
    #include <shellapi.h>
    #include <Windows.h>
    #include <DbgHelp.h>
#endif

using namespace NS_Log;

#ifdef Q_OS_WIN

bool bComfirmResult = false;

BOOL WINAPI ConsoleHandler(DWORD ctrlType)
{
    int nComfirmResult = 0;
    if (ctrlType == CTRL_CLOSE_EVENT /* || ctrlType == CTRL_C_EVENT*/)
    {
        nComfirmResult = MessageBoxW(NULL, L"Are you sure you want to close the software?", L"MessageBox", MB_OKCANCEL | MB_ICONQUESTION);
        if (!bComfirmResult)
        {
            if (nComfirmResult == IDOK)
            {
                bComfirmResult = true;
                return 0;
            }
            else if (nComfirmResult == IDCANCEL)
            {
                return 1;
            }
        }
    }
    std::cout << "Unexpected ctrl type triggered: " << ctrlType << std::endl;
    return 1;
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    // 创建 Dump 文件
    HANDLE hDumpFile = CreateFile(reinterpret_cast<const wchar_t *>(("crash_" + Time()->getInstance()->TimeToString("yyyy-MM-dd_hh-mm-ss") + ".dmp").utf16()),
                                  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != hDumpFile)
    {
        // Dump 信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;

        // 写入 Dump 文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // check only 1 program can be constructed
    QSharedMemory mem("iStar_NetworkEquipmentMonitor");
    bool b = mem.create(1);
    if (!b)
    {
        std::cout << "An instance has already been running." << std::endl;
        return -1;
    }

#ifdef Q_OS_WIN
    HWND consolewindow = GetConsoleWindow();
    if (NULL != consolewindow)
    {
        ShowWindow(consolewindow, SW_SHOW);
    }
#endif

#ifdef Q_OS_WIN
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode &= ~ENABLE_INSERT_MODE;
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, mode);

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif

    // MqttClient
    AppConfigManager::getInstance()->SetPath(m_AppConfig.appDataPath + "NetMonitorConfig", m_AppConfig.appDataPath_Backup + "NetMonitorConfig",
            "appConfig.json", m_AppConfig.appDataPath + "CommunicationCommand");
    AppConfigManager::getInstance()->Initialize(); // Read Config Data
    MqttClientManager::getInstance()->AddClient(0, "NetMonitor");

    // AppConfig
    AppConfig config;
    if (AppConfigManager::getInstance()->IsInitialized())
    {
        config = AppConfigManager::getInstance()->GetAppConfig();
        MqttClientManager::getInstance()->Connect(0, config.serverAdd.IP, config.serverAdd.port);
    }
    LOG()->Initialize_DebugLogger("D:/");
    LOG()->Log_Debug("Server.IP = " + config.serverAdd.IP.toStdString());
    LOG()->Log_Debug("Server.Port = " + std::to_string(config.serverAdd.port));
    LOG()->Log_Debug("Local IP = " + config.LocalIP.toStdString());

    // File
    NetMonitorManager::getInstance()->SetPath(config.appDataPath + "NetMonitorConfig", config.appDataPath_Backup
            + "NetMonitorConfig", "", config.appDataPath + "CommunicationCommand");
    FileOperatorManager::getInstance()->AddFileOperator(NetMonitorManager::getInstance());

    // Setup SetIP
    NetMonitorManager::getInstance()->SetIP(config.LocalIP);

    // Setup ConnectCheckTime
    NetMonitorManager::getInstance()->SetConnectCheckTime(2000);

    // Setup PathFileName
    NetMonitorManager::getInstance()->SetAppPathFile("AppPathConfig.json");
    NetMonitorManager::getInstance()->SetAppProcessNameFile("AppNameConfig.json");

    // Publish IP
    NetMonitorManager::getInstance()->LinkToPublishClient(MqttClientManager::getInstance()->GetClient(0), "NMS", config.LocalIP);
    NetMonitorManager::getInstance()->StartPublish();
    NetMonitorManager::getInstance()->SetPeriodPublish();// to MC_IP

    // HeartBeat
    HeartBeat::getInstance()->LinkHeartBeatToPublishClient(MqttClientManager::getInstance()->GetClient(0), config.LocalIP);
    HeartBeat::getInstance()->SetPeriodPublish();

    // Subscribe
    NetMonitorManager::getInstance()->LinkToSubscribeClient(MqttClientManager::getInstance()->GetClient(0));
    //    NetMonitorManager::getInstance()->Subscribe("MC_" + config.LocalIP); // subscribe MC_IP
    NetMonitorManager::getInstance()->Subscribe("GUI_NetworkEquipment"); // subscribe GUI
    foreach (auto &strAppname, NetMonitorManager::getInstance()->GetApps())
    {
        NetMonitorManager::getInstance()->Subscribe(strAppname + "_HeartBeat"); // subscribe Apps
    }

    return a.exec();
}
