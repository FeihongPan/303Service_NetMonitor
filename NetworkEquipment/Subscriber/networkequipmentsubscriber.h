/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        networkequipmentsubscriber.h
* Description:      接受来自控制层的所有IP部署软件的实时状态和监控软件自身的心跳检测信号，并且传递给NetmonitorView
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
***********************************************************************/

#ifndef NETWORKEQUIPMENTSUBSCRIBER_H
#define NETWORKEQUIPMENTSUBSCRIBER_H

#include "Communication/MQTT/isubscriber.h"
#include "GUIConfig/AppParameter.h"

class S_Dic_AppState;

class NetworkEquipmentSubscriber : public ISubscriber
{
    Q_OBJECT
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    ~NetworkEquipmentSubscriber();

    /// <summary>
    /// 单例
    /// </summary>
    static NetworkEquipmentSubscriber *getInstance();

    /// <summary>
    /// 初始化，判断订阅监控软件的Topic
    /// </summary>
    bool Initialize();

    /// <summary>
    /// 读取IP配置文件
    /// </summary>
    bool ReadIPConfigData();

signals:
    /// <summary>
    /// 发送所有IP部署软件的实时状态给NetmonitorView
    /// </summary>
    /// <param name="dic_AllAppState">所有IP部署软件的实时状态</param>
    void sig_SendAppState(S_Dic_AppState &dic_AllAppState);

    /// <summary>
    /// 发送所有IP监控软件的实时状态给UI
    /// </summary>
    /// <param name="strIp">监控软件的Ip</param>
    void sig_SendNetMonitorState(QString strIp);

private:
    /// <summary>
    /// 构造函数
    /// </summary>
    explicit NetworkEquipmentSubscriber(QString name, QObject *parent = nullptr);

    /// <summary>
    /// 饿汉式单例模式删除拷贝构造函数和赋值构造函数
    /// </summary>
    NetworkEquipmentSubscriber(const NetworkEquipmentSubscriber &) = delete;
    NetworkEquipmentSubscriber &operator =(const NetworkEquipmentSubscriber &) = delete;

    /// <summary>
    /// 接受来自控制层的部署软件的实时状态和监控软件的心跳检测信号
    /// </summary>
    virtual void decoding(const QString &topic, const QByteArray &message) override;

private:
    static NetworkEquipmentSubscriber *self;

    QList<QString> m_lst_App;

    QString m_strFolderPath = DataPath + "NetMonitorConfig";
    QString m_strFileName = "IPConfig.json";
    QString m_strFilePath = m_strFolderPath + "/" + m_strFileName;

    bool m_bInitialized = false;
};

#endif // NETWORKEQUIPMENTSUBSCRIBER_H
