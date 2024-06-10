/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorview.cpp
* Description:      网络设备监控主页面
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
***********************************************************************/

#include "netmonitorview.h"
#include "ui_netmonitorview.h"
#include "netmonitorwidget.h"
#include "NetworkEquipment/Command/cmd_netmonitor.h"
#include "NetworkEquipment/Subscriber/networkequipmentsubscriber.h"
#include "GUIConfig/AppParameter.h"

NetMonitorView::NetMonitorView(QString name, QWidget *parent)
    : AuthorityWidget(name, parent)
    , ui(new Ui::NetMonitorView)
    , m_nLeftHeight(10)
    , m_nMiddleHeight(10)
    , m_nRightHeight(10)
    , m_nStartWidth(20)
    , m_bAllAppState(true)
{
    ui->setupUi(this);
    //    m_Publisher.LinkToClient(MqttClientManager::getInstance()->GetClient(0), PublishTopicPrefix);
}

NetMonitorView::~NetMonitorView()
{
    delete ui;
}

void NetMonitorView::setConnections()
{
    connect(NetworkEquipmentSubscriber::getInstance(), &NetworkEquipmentSubscriber::sig_SendAppState, this, [ = ](S_Dic_AppState & dic_AllAppState)
    {
        m_bAllAppState = true;
        m_dic_AllAppState = dic_AllAppState;
        m_lst_Ip = m_dic_AllAppState.values.keys();
        m_lst_Widget_Ip = m_dic_NetMonitorWidget.keys();

        foreach (auto &strIp, m_lst_Ip)
        {
            if (!m_lst_Widget_Ip.contains(strIp))
            {
                auto widget = new NetMonitorWidget(strIp, ui->scrollAreaWidgetContents);
                initializeNetWidget(strIp, widget);
                setNetWidgetPlace(widget);
                checkNetWidget();
            }
            else
            {
                updateAppState(strIp);
            }
        }
        setViewState();
        ui->scrollAreaWidgetContents->setFixedHeight(qMax(qMax(m_nLeftHeight, m_nMiddleHeight), m_nRightHeight) + 100);
    });
}

void NetMonitorView::initializeNetWidget(const QString &strIp, NetMonitorWidget *pWidget)
{
    pWidget->SetIp(strIp);
    pWidget->UpdateAppState(m_dic_AllAppState.values[strIp]);
    pWidget->ItemInitialize();
    pWidget->PublisherInitialize();

    m_lst_Widget_Ip.append(strIp);
    m_dic_NetMonitorWidget.insert(strIp, pWidget);
}

void NetMonitorView::updateAppState(const QString &strIp)
{
    auto dic_AppState = m_dic_AllAppState.values[strIp];
    auto lst_AppState = dic_AppState.values.values();
    foreach (auto &bState, lst_AppState)
    {
        m_bAllAppState = bState && m_bAllAppState;
        if (!m_bAllAppState)
        {
            break;
        }
    }
    m_dic_NetMonitorWidget[strIp]->UpdateAppState(dic_AppState);
}

void NetMonitorView::setNetWidgetPlace(NetMonitorWidget *pWidget)
{
    auto nAddHeight = pWidget->height() + 20;
    auto nAddWidth = pWidget->width() + 20;

    if (m_nMiddleHeight < m_nLeftHeight && m_nMiddleHeight <= m_nRightHeight)
    {
        pWidget->move(m_nStartWidth + nAddWidth, m_nMiddleHeight);
        m_nMiddleHeight += nAddHeight;
    }
    else if (m_nRightHeight < m_nLeftHeight && m_nRightHeight < m_nMiddleHeight)
    {
        pWidget->move(m_nStartWidth + 2 * nAddWidth, m_nRightHeight);
        m_nRightHeight += nAddHeight;
    }
    else
    {
        pWidget->move(m_nStartWidth, m_nLeftHeight);
        m_nLeftHeight += nAddHeight;
    }
}

void NetMonitorView::checkNetWidget()
{
    foreach (auto &strIp, m_lst_Widget_Ip)
    {
        if (!m_lst_Ip.contains(strIp))
        {
            m_lst_Widget_Ip.removeOne(strIp);
            m_dic_NetMonitorWidget.remove(strIp);
        }
    }
}

void NetMonitorView::setViewState()
{
    if (m_bAllAppState)
    {
        emit ViewStatusChanged(ViewStatus::Idle);
    }
    else
    {
        emit ViewStatusChanged(ViewStatus::Alarm);
    }

}
