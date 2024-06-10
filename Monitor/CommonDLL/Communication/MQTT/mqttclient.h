#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include "CommonDLL_global.h"
#include "QtMqtt/qmqttclient.h"

class COMMONDLL_EXPORT MqttClient : public QMqttClient
{
public:
    explicit MqttClient(QObject *parent = nullptr);

    void Connect(const QString &ip, quint16 port);

    void Reconnect();

    bool IsConnected();

    void Subscribe();

    void DisConnect();

public:
    QString m_Id;
    QString m_Ip;
    quint16 m_Port;
    QStringList m_lst_SubcribedTopics;
};

#endif // MQTTCLIENT_H
