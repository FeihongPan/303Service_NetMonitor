/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitormanager.h
* Description:      监控软件主要功能实现包括读取配置文件、打开关闭进程、定期发送部署软件实时状态以及接受控制层传来的命令
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
*       V1.1.0     FeihongPan     2024-04-10     增加注释
*       V1.1.1     FeihongPan     2024-04-17     增加模板函数判断IP
*       V2.0.0     FeihongPan     2024-04-17     1921#2锁定版本
***********************************************************************/

#ifndef NETMONITORMANAGER_H
#define NETMONITORMANAGER_H

#include "Communication/MQTT/imqttclient.h"
#include "FileSystem/ifileoperator.h"
#include "NetworkEquipment/Data/s_netmonitor.h"

class NetMonitorManager : public IMqttClient, public IFileOperator
{
    Q_OBJECT
public:
    ~NetMonitorManager();
    /// <summary>
    /// 单例
    /// </summary>
    static NetMonitorManager *getInstance();

    /// <summary>
    /// 返回所有App名字的列表
    /// </summary>
    inline const QList<QString> &GetApps() const;

    /// <summary>
    /// 设置App连接的检查时间
    /// </summary>
    inline void SetConnectCheckTime(const int &nTime);

    /// <summary>
    /// 设置App本地IP
    /// </summary>
    inline void SetIP(const QString &strIP);

    /// <summary>
    /// 设置AppPathFile的名字
    /// </summary>
    inline void SetAppPathFile(const QString &strAppPathFile);

    /// <summary>
    /// 设置AppProcessNameFile的名字
    /// </summary>
    inline void SetAppProcessNameFile(const QString &strAppProcessNameFile);

    /// <summary>
    /// 周期性发送部署软件的实时状态给控制层对应的Client, Topic为NMS_IP
    /// </summary>
    virtual void SetPeriodPublish() override;

    virtual void StartPublish() override;

    virtual void SaveCommandInfo() override;

private:
    /// <summary>
    /// 构造函数
    /// </summary>
    explicit NetMonitorManager(QString name, QObject *parent = nullptr);

    /// <summary>
    /// 拷贝构造函数和赋值构造函数
    /// </summary>
    NetMonitorManager(const NetMonitorManager &) = delete;
    NetMonitorManager &operator=(const NetMonitorManager &) = delete;

    /// <summary>
    /// 初始化，包括读取配置文件以及判断部署软件是否在线(QTimer)
    /// </summary>
    /// <returns>返回是否成功读取配置文件结果</returns>
    bool initializeConfig();

    /// <summary>
    /// 得到所有部署软件的实时状态
    /// </summary>
    /// <returns>返回所有部署软件的实时状态的QByteArray数据类型</returns>
    QByteArray getAppStateValue(bool &bGetAppState);

    /// <summary>
    /// 通过App名字得到该App的进程名字(FLS->FlsProject.exe)
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    /// <returns>返回该App的进程名字</returns>
    QString getAppProcessName(const QString &strAppName) const;

    /// <summary>
    /// 读取每个部署软件的所在路径
    /// </summary>
    /// <param name="dic_AppPathInfo">每个部署软件的路径合集</param>
    /// <returns>返回是否成功读取配置文件结果</returns>
    bool readPathConfigData(S_Dic_StringString &dic_AppPathInfo);

    /// <summary>
    /// 读取每个部署软件的进程名字
    /// </summary>
    /// <param name="dic_AppProcessNameInfo">每个部署软件的进程名字合集</param>
    /// <returns>返回是否成功读取配置文件结果</returns>
    bool readNameConfigData(S_Dic_StringString &dic_AppProcessNameInfo);

    /// <summary>
    /// 检查App进程是否存在的方法
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    /// <returns>返回进程是否存在</returns>
    bool checkTask(const QString &strAppName);

    /// <summary>
    /// 结束该App进程的方法
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    /// <returns>返回进程是否成功结束</returns>
    bool killTask(const QString &strAppName);

    /// <summary>
    /// 打开进程的方法
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    /// <returns>返回进程是否成功开始</returns>
    bool startTask(const QString &strAppName);

    // App Operator
    /// <summary>
    /// 打开App
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    bool appOpen(const QString &strAppName);

    /// <summary>
    /// 关闭App
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    bool appClose(const QString &strAppName);

    /// <summary>
    /// 重启App
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    bool appRestart(const QString &strAppName);

    /// <summary>
    /// 通过Flag结合QTimer得到软件实时状态
    /// </summary>
    /// <param name="strAppName">app的名字</param>
    void setAppState(const QString &strAppName);

    /// <summary>
    /// 打开App
    /// </summary>
    bool appOpenMC();

    /// <summary>
    /// 接受来自MC_IP的消息
    /// </summary>
    virtual void decoding(const QString &task, bool result, const QByteArray &msg, const QByteArray &errMsg) override;

    template<typename T>
    inline bool checkIp(const T &t)
    {
        return m_strIP == t.IP;
    }

private:
    static NetMonitorManager *self;

    S_Dic_StringBool m_dic_Appstate;
    S_Dic_StringString m_dic_AppProcessNameInfo;
    S_Dic_StringString m_dic_AppPathInfo;

    QList<QString> m_lst_App;
    QMap<QString, bool> m_dic_FunctionCalled;

    QString m_strAppPathFile = "AppPathConfig.json";
    QString m_strAppProcessNameFile = "AppNameConfig.json";
    QString m_strIP = "127.0.0.1";
    int m_nConnectTime = 2000; // 检查两秒内是否连接
    bool m_bInitialized = false;
};

inline const QList<QString> &NetMonitorManager::GetApps() const
{
    return m_lst_App;
}

inline void NetMonitorManager::SetConnectCheckTime(const int &nTime)
{
    m_nConnectTime = nTime;
}

inline void NetMonitorManager::SetIP(const QString &strIP)
{
    m_strIP = strIP;
}

inline void NetMonitorManager::SetAppPathFile(const QString &strAppPathFile)
{
    m_strAppPathFile = strAppPathFile;
}

inline void NetMonitorManager::SetAppProcessNameFile(const QString &strAppProcessNameFile)
{
    m_strAppProcessNameFile = strAppProcessNameFile;
}
#endif // NETMONITORMANAGER_H
